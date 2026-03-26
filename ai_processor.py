import serial
import cv2
import time
import mediapipe as mp 

# 1. Setup Serial to listen to ATmega (Embedded Team)
mega_serial = serial.Serial('/dev/ttyS0', 9600, timeout=1)

# 2. Setup AI Face Detection Model
mp_face_detection = mp.solutions.face_detection
face_detector = mp_face_detection.FaceDetection(model_selection=0, min_detection_confidence=0.5)

# 3. Setup Camera
camera = cv2.VideoCapture(0)

def listen_to_embedded():
    """Main loop to listen for ATmega signals [cite: 124]"""
    print("AI System Online: Monitoring Serial Port...")
    while True:
        if mega_serial.in_waiting > 0:
            # Read and clean the incoming message [cite: 127]
            raw_data = mega_serial.readline().decode('utf-8').strip()
            
            if "AUTH_FAIL" in raw_data:
                print("Security Alert: Fingerprint mismatch! Analyzing intruder...")
                process_ai_security()
            
            elif "AUTH_SUCCESS" in raw_data:
                user_id = raw_data.split(":")[1]
                print(f"Driver Recognized: ID #{user_id} [cite: 127]")

def process_ai_security():
    """AI Logic: Capture frame and detect human face"""
    ret, frame = camera.read()
    if not ret: return

    # Convert to RGB for processing
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = face_detector.process(rgb_frame)

    # If the AI finds a face in the image
    if results.detections:
        print("AI Detection: Human face confirmed!")
        timestamp = time.strftime("%Y%m%d-%H%M%S")
        cv2.imwrite(f"intruder_confirmed_{timestamp}.jpg", frame)
        # This photo can now be sent to a mobile app or dashboard
    else:
        print("AI Detection: No clear face found, but photo saved for safety.")
        cv2.imwrite(f"suspicious_activity_{time.time()}.jpg", frame)

if __name__ == "__main__":
    listen_to_embedded()