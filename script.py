# imports
from picamera2 import Picamera2
import RPi.GPIO as GPIO 
import time
import numpy as np
import tflite_runtime.interpreter as tflite
import cv2
import threading
from pydbus import SessionBus
from gi.repository import GLib
import logging
import os
import sys
from pathlib import Path

# Configure logging
log_file = Path.home() / "drowsiness_detection.log"
logging.basicConfig(
    level=logging.INFO,
    format='%(asctime)s - %(levelname)s - %(message)s',
    handlers=[
        logging.FileHandler(log_file),
        logging.StreamHandler()
    ]
)
logger = logging.getLogger(__name__)

# D-BUS setup
try:
    bus = SessionBus()
    auth_event = threading.Event() # event to signal when authentication is received via D-Bus
    loop = GLib.MainLoop() # create a GLib main loop to handle D-Bus events in a separate thread
    ALERT_XML = """
<node>
    <interface name="com.drowsiness.monitor.Control">
        <signal name="DrowsinessAlert">
            <arg type="s" name="alert_level"/>
        </signal>
    </interface>
</node>
"""
    class DrowsinessPublisher:
        dbus = ALERT_XML
        def emit_alert(self, alert_level):
            self.DrowsinessAlert(alert_level)
    
    publisher = DrowsinessPublisher() 
    bus.publish("com.drowsiness.monitor", publisher)
    logger.info("D-BUS publisher initialized successfully")
    
except Exception as e:
    logger.error(f"Failed to initialize D-BUS: {e}")
    publisher = None
    bus = None
# call-back function to handle incoming D-Bus messages 
def on_start_ai(sender, object_path, interface_name, signal_name, params):
    try:
        authenticated = params[0] # extract the authentication token from the D-Bus message parameters
        if authenticated: 
            logger.info("Authentication successful. Starting AI processing...")
            auth_event.set() # signal the main thread that authentication is successful and the AI processing can start
        else:
            logger.warning("Authentication failed.")
    except Exception as e:
        logger.error(f"Error in D-Bus callback: {e}")

# Listener thread
def run_dbus_listener():
    try:
        bus.subscribe(
            iface="com.automotive.system",  # listen for signals on this interface
            signal="StartAI",               # listen for this specific signal
            signal_fired=on_start_ai        # call the on_start_ai function when the signal is received 
        )
        loop.run() # start the GLib main loop to listen for D-Bus events
    except Exception as e:
        logger.error(f"D-BUS listener error: {e}")

if bus is not None:
    listener_thread = threading.Thread(target=run_dbus_listener, daemon=True)
    listener_thread.start()
    logger.info("D-BUS listener thread started")
else:
    auth_event = threading.Event()
    auth_event.set()  # Auto-start if D-BUS not available
    logger.warning("D-BUS unavailable, starting AI processing immediately")

# TFLite model setup
MODEL_PATH = "model.tflite"

# Check if model file exists
if not os.path.exists(MODEL_PATH):
    logger.error(f"Model file not found: {MODEL_PATH}")
    sys.exit(1)

try:
    interpreter = tflite.Interpreter(model_path=MODEL_PATH)
    interpreter.allocate_tensors()
    input_details = interpreter.get_input_details()
    output_details = interpreter.get_output_details()
    logger.info(f"TFLite model loaded successfully. Input shape: {input_details[0]['shape']}")
except Exception as e:
    logger.error(f"Failed to load TFLite model: {e}")
    sys.exit(1)

# Load face cascade classifier for face detection
try:
    face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    if face_cascade.empty():
        logger.error("Failed to load Haar Cascade classifier")
        sys.exit(1)
    logger.info("Face cascade classifier loaded successfully")
except Exception as e:
    logger.error(f"Error loading face cascade: {e}")
    sys.exit(1)
try:
    eye_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_eye.xml')
    if eye_cascade.empty():
        logger.error("Failed to load Haar Cascade eye classifier")
        sys.exit(1)
    logger.info("Eye cascade classifier loaded successfully")
except Exception as e:
    logger.error(f"Error loading eye cascade: {e}")
    sys.exit(1)
# eye state detection
def is_eye_closed(eye_img):
    if(eye_img is None or eye_img.size == 0 or eye_img.shape[0] < 5 or eye_img.shape[1] < 5):
        logger.warning("Corrupted image received for classification")
        return False
    eye_img_resized = cv2.resize(eye_img, (32, 32)) # resize the eye image to match the model's expected input size
    eye_img_rgb = cv2.cvtColor(eye_img_resized, cv2.COLOR_GRAY2RGB) # convert the grayscale eye image to RGB format (the model expects 3 channels)
    eye_img_normalized = eye_img_rgb.astype(np.float32) / 255.0 # normalize pixel values to the range [0, 1]
    eye_img_reshaped = eye_img_normalized.reshape(1, 32, 32, 3) # reshape to match the model's input shape (1, height, width, channels)
    interpreter.set_tensor(input_details[0]['index'], eye_img_reshaped) # set the preprocessed eye image as the input tensor for the model
    interpreter.invoke()
    output = interpreter.get_tensor(output_details[0]['index']) # Get output: [[P(awake), P(sleepy)]]
    output_prob = output[0][1] # probability of the eye being closed (sleepy)
    return output_prob > 0.7 # return True if the probability of the eye being closed is greater than 0.7, otherwise return False

# hardware setup
buzzer_pin = 17
buzzer_pwm = None

try:
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(buzzer_pin, GPIO.OUT) 
    buzzer_pwm = GPIO.PWM(buzzer_pin, 1000) # set frequency to 1kHz
    buzzer_pwm.start(0) # start PWM with 0% duty cycle (off)
    logger.info("GPIO buzzer initialized successfully")
except RuntimeError as e:
    logger.error(f"GPIO initialization error (may need sudo): {e}")
    buzzer_pwm = None
except Exception as e:
    logger.error(f"Unexpected GPIO error: {e}")
    buzzer_pwm = None
# function that sends alerts to both the D-Bus and hardware
def send_alert(level):
    try:
        if publisher is not None:
            publisher.emit_alert(level)
        
        if buzzer_pwm is not None:
            if level == "warning":
                buzzer_pwm.ChangeFrequency(1000) 
                buzzer_pwm.ChangeDutyCycle(50)
            elif level == "critical":
                buzzer_pwm.ChangeFrequency(2000) 
                buzzer_pwm.ChangeDutyCycle(80)
        
        logger.info(f"Alert sent: {level}")
    except Exception as e:
        logger.error(f"Error sending alert '{level}': {e}")

def buzzer_off():
    try:
        if buzzer_pwm is not None:
            buzzer_pwm.ChangeDutyCycle(0)
    except Exception as e:
        logger.error(f"Error turning off buzzer: {e}") 

# Main thread
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8))

# FPS limiter
TARGET_FPS = 15
frame_time = 1.0 / TARGET_FPS

if __name__ == "__main__":
    picam2 = None
    try:
        logger.info("Waiting for authentication to start AI processing...") 
        auth_event.wait()
        
        logger.info("Authentication successful. Initializing camera...")
        # camera setup
        try:
            picam2 = Picamera2()
            config = picam2.create_preview_configuration(main={"format": "RGB888", "size": (640, 480)})
            picam2.configure(config)
            picam2.start()
            time.sleep(1) # sensor warm-up time
            logger.info("Camera initialized successfully")
        except Exception as e:
            logger.error(f"Camera initialization failed: {e}")
            sys.exit(1)
        
        eye_closed_start = None
        no_face_start = None
        alert_sent = {"warning": False, "critical": False, "SOS": False}
        frame_count = 0
        last_time = time.time()
        
        while True:
            frame_start = time.time()
            
            try:
                frame = picam2.capture_array()
                if frame is None:
                    logger.warning("Failed to capture frame")
                    continue
                
                gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY)
                gray = clahe.apply(gray)
                faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=7, minSize=(80, 80))
                
                if len(faces) == 0:
                    if no_face_start is None:
                        no_face_start = time.time()
                    elif time.time() - no_face_start > 3 and not alert_sent["SOS"]:
                        send_alert("SOS")
                        alert_sent["SOS"] = True
                        logger.warning("SOS alert triggered: No face detected for 3+ seconds")
                else:
                    no_face_start = None
                    x, y, w, h = faces[0]
                    roi_gray = gray[y:y+h, x:x+w]
                    eyes = eye_cascade.detectMultiScale(roi_gray, scaleFactor=1.1, minNeighbors=10, minSize=(20, 20))
                    if len(eyes) >= 2:
                        flags = [is_eye_closed(roi_gray[ey:ey+eh, ex:ex+ew]) for (ex, ey, ew, eh) in eyes[:2]]
                        both_eyes_closed = all(flags)
                    else:
                        both_eyes_closed = False
                    if both_eyes_closed:
                        if eye_closed_start is None:
                            eye_closed_start = time.time()
                        else:
                            closure_duration = time.time() - eye_closed_start
                            if closure_duration >= 4 and not alert_sent["SOS"]:
                                send_alert("SOS")
                                alert_sent["SOS"] = True
                                logger.warning("SOS alert triggered: Eyes closed for 4+ seconds")
                            elif closure_duration >= 3 and not alert_sent["critical"]:
                                send_alert("critical")
                                alert_sent["critical"] = True
                                logger.warning("Critical alert triggered: Eyes closed for 3+ seconds")
                            elif closure_duration >= 1.5 and not alert_sent["warning"]:
                                send_alert("warning")
                                alert_sent["warning"] = True
                                logger.info("Warning alert triggered: Eyes closed for 1.5+ seconds")
                    else:
                        eye_closed_start = None
                        if any(alert_sent.values()):
                            buzzer_off()
                            alert_sent = {"warning": False, "critical": False, "SOS": False}
                
                frame_count += 1
                current_time = time.time()
                if current_time - last_time >= 5:  # Log FPS every 5 seconds
                    fps = frame_count / (current_time - last_time)
                    logger.debug(f"Current FPS: {fps:.1f}")
                    frame_count = 0
                    last_time = current_time
                
                # FPS limiting
                elapsed = time.time() - frame_start
                if elapsed < frame_time:
                    time.sleep(frame_time - elapsed)
            
            except ValueError as e:
                logger.error(f"Image processing error: {e}")
                continue
            except Exception as e:
                logger.error(f"Unexpected error in main loop: {e}")
                continue
    
    except KeyboardInterrupt:
        logger.info("Program interrupted by user")
    except Exception as e:
        logger.error(f"Fatal error: {e}")
    finally:
        logger.info("Cleaning up resources...")
        try:
            buzzer_off()
        except:
            pass
        try:
            GPIO.cleanup()
        except:
            pass
        if picam2 is not None:
            try:
                picam2.stop()
            except:
                pass
        logger.info("All resources released. Exiting.")