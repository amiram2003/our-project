# from library import class
from picamera2 import Picamera2
import RPi.GPIO as GPIO
import time
import mediapipe as mp
import numpy as np
import serial
import cv2
# UART communication setup
uart = serial.Serial('/dev/ttyUSB0', 9600, timeout=1) # port, baudrate, timeout
# initializing MediaPipe Face Mesh for facial landmark detection
mp_face_mesh = mp.solutions.face_mesh.FaceMesh()
# function to calculate the Eye Aspect Ratio (EAR) given the eye landmarks (look at the README file for more information)
def ear_calc(mark):
    A = np.linalg.norm(mark[1] - mark[5]) # 2-6
    B = np.linalg.norm(mark[2] - mark[4]) # 3-5
    C = np.linalg.norm(mark[0] - mark[3]) # 1-4
    ear = (A + B) / (2.0 * C)
    return ear
# pin control functions
buzzer_pin = 17
GPIO.setmode(GPIO.BCM)
GPIO.setup(buzzer_pin, GPIO.OUT) # buzzer
buzzer_pwm = GPIO.PWM(buzzer_pin, 1000) # set frequency to 1kHz
def warning_alert():
    buzzer_pwm.ChangeFrequency(1000) # reset frequency to 1kHz for warning alert
    buzzer_pwm.start(50) # start PWM with 50% duty cycle
def critical_alert():
    buzzer_pwm.ChangeFrequency(2000) # change frequency to 2kHz for critical alert
    buzzer_pwm.start(80) # start PWM with 80% duty cycle

def buzzer_off():
    buzzer_pwm.stop()
# creating an object from Picamera2 class
picam2 = Picamera2()
# configuration
config = picam2.create_preview_configuration(main = {"format":"RGB888", "size": (640, 480)})
# preview_configuration means Real-time processing (not taking photos "still_configuration()" or videos "video_configuration()")
picam2.configure(config)
# start the camera *add a condition later*
picam2.start()
time.sleep(1) # sensor warm-up time
picam2.set_controls({"AeEnable": False, "ExposureTime": 8000, "AnalogueGain": 4.0}) # manual exposure settings to maintain consistent brightness for better landmark detection
time.sleep(0.2)
clahe = cv2.createCLAHE(clipLimit=2.0, tileGridSize=(8, 8)) # contrast limited adaptive histogram equalization object (for enhancing contrast in low-light conditions and improving landmark detection accuracy)
height, width = 480, 640 # dimensions of the captured frame (used for scaling landmark points)
EAR_THRESHOLD = 0.19 # threshold for determining if eyes are closed (this value may need to be calibrated based on testing)
eye_closed_start = None # variable to track when the eyes were first detected as closed
sos_sent = False # flag to track if SOS signal has been sent
no_face_start = None # variable to track when no face was first detected
try:
    while True:
        # grabbing frames and returning it as NumPy array
        frame = picam2.capture_array()
        # convert to grayscale to optimize performance (grayscale images are sufficient for facial landmark detection)
        gray = cv2.cvtColor(frame, cv2.COLOR_RGB2GRAY) 
        # apply CLAHE for contrast enhancement in low-light conditions
        enhanced = clahe.apply(gray) 
        rgb_enhanced = cv2.cvtColor(enhanced, cv2.COLOR_GRAY2RGB) # convert back to RGB for MediaPipe processing
        # using MediaPipe to detect facial landmarks in the frame
        result = mp_face_mesh.process(rgb_enhanced)
        vehicle_speed = 50 # placeholder for vehicle speed
        if vehicle_speed > 0: # only perform drowsiness detection when the vehicle is in motion
            if result.multi_face_landmarks:
                for facial_landmarks in result.multi_face_landmarks:
                    no_face_start = None # reset no face timer when a face is detected
                    np_points = np.array([(int(pt.x * width), int(pt.y * height)) for pt in facial_landmarks.landmark])
                    # there are 468 facial landmarks detected by MediaPipe, the following are the ones needed to calculate the EAR for both eyes
                    left_eye_marks = np_points[[33, 160, 158, 133, 153, 144]]
                    right_eye_marks = np_points[[362, 385, 387, 263, 373, 380]]
                    # applying the EAR calculation function to both eyes and taking the average EAR
                    left_ear = ear_calc(left_eye_marks)
                    right_ear = ear_calc(right_eye_marks)
                    ear = (left_ear + right_ear) / 2.0
                    if ear < EAR_THRESHOLD:
                        if eye_closed_start is None:
                            eye_closed_start = time.time()
                        closure_duration = time.time() - eye_closed_start
                        if closure_duration >= 4.0 and not sos_sent: # if eyes are closed for more than 4 seconds and SOS signal has not been sent yet
                            uart.write(b'SOS') # send SOS signal via UART
                            sos_sent = True # set SOS sent flag to True to prevent multiple SOS signals from being sent
                            # The AVR will take it from here and stop the vehicle and reset the program.
                        elif closure_duration >= 3: # if eyes are closed for more than 3 seconds
                            # print("Critical drowsiness detected! Alerting driver.")
                            critical_alert() # turn on buzzer for critical alert
                        elif closure_duration >= 1.5: # if eyes are closed for more than 1.5 seconds
                            # print("Drowsiness detected! Alerting driver.")
                            warning_alert() # turn on buzzer for warning alert
                    else:            
                        eye_closed_start = None # reset eye closed timer when eyes are open
                        buzzer_off() # turn off buzzer if eyes are open

            else:
                if no_face_start is None:
                    no_face_start = time.time()
                no_face_duration = time.time() - no_face_start
                if no_face_duration >= 3.0 and not sos_sent: # if no face is detected for more than 3 seconds and SOS signal has not been sent yet
                    uart.write(b'SOS') # send SOS signal via UART
                    sos_sent = True # set SOS sent flag to True to prevent multiple SOS signals from being sent
                    # The AVR will take it from here and stop the vehicle and reset the program.
except KeyboardInterrupt:
    print("Exiting program.")
finally:
    picam2.stop()
    mp_face_mesh.close()
    GPIO.cleanup()
    uart.close()
    print("All resources released.")