# Drowsiness Detection using Eye Aspect Ratio (EAR)
# importing necessary libraries
import cv2
import mediapipe as mp
import numpy as np
import pygame
# initialize Pygame mixer for sound playback
pygame.mixer.init()
alert_sound = pygame.mixer.Sound("alert1.wav")
# initializing the flag variable to count consecutive frames where EAR is below the threshold
FLAG = 0
# initializing MediaPipe Face Mesh for facial landmark detection
mp_face_mesh = mp.solutions.face_mesh.FaceMesh()
# function to calculate the Eye Aspect Ratio (EAR) given the eye landmarks (look at the README file for more information)
def ear_calc(mark):
    A = np.linalg.norm(mark[1] - mark[5]) # 2-6
    B = np.linalg.norm(mark[2] - mark[4]) # 3-5
    C = np.linalg.norm(mark[0] - mark[3]) # 1-4
    ear = (A + B) / (2.0 * C)
    return ear
# initialize video capture from the webcam
vid = cv2.VideoCapture(0)
# loop to read frames from the webcam and process them
while True:
    # ret is a boolean indicating if the frame was read successfully, and frame is the actual image frame
    ret, frame = vid.read()
    # if the frame was not read successfully, break the loop
    if not ret:
        break
    # convert the frame from BGR color space (used by OpenCV) to RGB color space (used by MediaPipe)
    rgb_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    # get the height and width of the frame to scale the landmark points correctly
    height, width, _ = frame.shape
    # using MediaPipe to detect facial landmarks in the frame
    result = mp_face_mesh.process(rgb_frame)
    if result.multi_face_landmarks:
        for facial_landmarks in result.multi_face_landmarks:
            np_points = np.array([(int(pt.x * width), int(pt.y * height)) for pt in facial_landmarks.landmark])
            # there are 468 facial landmarks detected by MediaPipe, the following are the ones needed to calculate the EAR for both eyes
            left_eye_marks = np_points[[33, 160, 158, 133, 153, 144]]
            right_eye_marks = np_points[[362, 385, 387, 263, 373, 380]]
            # applying the EAR calculation function to both eyes and taking the average EAR
            left_ear = ear_calc(left_eye_marks)
            right_ear = ear_calc(right_eye_marks)
            ear = (left_ear + right_ear) / 2.0
            EAR_THRESHOLD = 0.19
            if ear < EAR_THRESHOLD:
                FLAG += 1
                if FLAG >= 15:
                    # if more than 15 consecutive frames have EAR below the threshold, trigger the alert
                    if not pygame.mixer.get_busy():
                        alert_sound.play()
                    # Uncomment the following line to print the flag value (for debugging purposes)
                        # print(f"ALERT{FLAG}")
            else:            
                FLAG = 0
        # Uncomment the following lines to visualize the eye landmarks on the frame (for debugging purposes)
        #     for pt in left_eye_marks:
        #         cv2.circle(frame, pt, 2, (0, 255, 0), -1)
        #     for pt in right_eye_marks:
        #         cv2.circle(frame, pt, 2, (0, 255, 0), -1)
        # cv2.imshow('landmarks', frame)
        # if cv2.waitKey(1) & 0xFF == ord('q'):
        #     break
    else:
        print("No face detected, shutting down.")
        break

vid.release()
cv2.destroyAllWindows()
pygame.mixer.quit()
