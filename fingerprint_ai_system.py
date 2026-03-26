import serial
import cv2
import time

# 1. Initialize Serial communication with ATmega
# Adjust '/dev/ttyS0' based on Pi 5 connection
mega_serial = serial.Serial('/dev/ttyS0', 9600, timeout=1)
from ai_processor import process_ai_security

#2. Initialize Camera for AI monitoring
camera = cv2.VideoCapture(0)

def start_ai_monitor():
    print("AI System: Waiting for signals from ATmega...")
    
    while True:
        # Check if there is data waiting in the serial buffer
        if mega_serial.in_waiting > 0:
          
            message = mega_serial.readline().decode('utf-8').strip()   # Read the line and decode from bytes to string
            
            # CASE 1: successful fingerprint match
            if "AUTH_SUCCESS" in message:
                user_id = message.split(":")[1]
                print(f"Access Granted! Welcome User ID: {user_id}") #user_id can be used for personalized greetings or logging
                
            # CASE 2: Fingerprint Match Failed 
            elif "AUTH_FAIL" in message:
                print("Access Denied! Triggering Camera...")
                capture_intruder() 

def capture_intruder():
    """Capture photo and save it with a timestamp"""
    ret, frame = camera.read()
    if ret:
        timestamp = time.strftime("%Y%m%d-%H%M%S") #%Y%m%d for date and %H%M%S for time
        intruder_photo = f"intruder_alert_{timestamp}.jpg"
        cv2.imwrite(intruder_photo, frame)
        print(f"Intruder photo saved as: {intruder_photo}")
        # Next step: Run Face Detection on this frame

if __name__ == "__main__": 
    start_ai_monitor()