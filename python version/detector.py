from imutils.video import VideoStream
from imutils import face_utils

import numpy as np

import dlib
import cv2

class detector:
    EYE_AR_THRESH = 0.28
    frontalFace_Detector = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")
    predictor = dlib.shape_predictor("shape_predictor_68_face_landmarks.dat")
    (lStart, lEnd) = face_utils.FACIAL_LANDMARKS_IDXS["left_eye"]
    (rStart, rEnd) = face_utils.FACIAL_LANDMARKS_IDXS["right_eye"]

    def __init__(self):
        print("detector created")
        
    def euclidean_dist(self, A, B):
        
        return np.linalg.norm(A - B)

    def eye_aspect_ratio(self,eye):
        A = self.euclidean_dist(eye[1], eye[5])
        B = self.euclidean_dist(eye[2], eye[4])
        C = self.euclidean_dist(eye[0], eye[3])
        ear = (A + B) / (2.0 * C)
        
        return ear
    
    def detectFaceFeatures(self, frame):
        gray=cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)        
        rects = self.frontalFace_Detector.detectMultiScale(gray, scaleFactor=1.1,
            minNeighbors=5, minSize=(30, 30),
	    flags=cv2.CASCADE_SCALE_IMAGE)
        for (x, y, w, h) in rects:
            rect = dlib.rectangle(int(x), int(y), int(x + w),
			int(y + h))
            shape = self.predictor(gray, rect)
            shape = face_utils.shape_to_np(shape)

            leftEye = shape[self.lStart:self.lEnd]
            rightEye = shape[self.rStart:self.rEnd]
            leftEAR = self.eye_aspect_ratio(leftEye)
            rightEAR = self.eye_aspect_ratio(rightEye)

            ear = (leftEAR + rightEAR) / 2.0

            leftEyeHull = cv2.convexHull(leftEye)
            rightEyeHull = cv2.convexHull(rightEye)
            cv2.drawContours(frame, [leftEyeHull], -1, (0, 255, 0), 1)
            cv2.drawContours(frame, [rightEyeHull], -1, (0, 255, 0), 1)

            cv2.putText(frame, "EAR: {:.3f}".format(ear), (300, 30),
			cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)
 
            if ear < self.EYE_AR_THRESH:
                return 1
            else:
                return 0
        return -1
