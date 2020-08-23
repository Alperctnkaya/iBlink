from imutils.video import VideoStream
from imutils import face_utils
import imutils
import cv2
import time

import detector
import messageDecoder

print(14*"\n")

detector=detector.detector()
decoder= messageDecoder.messageDecoder()
decoder.displayDictionary()

vs = VideoStream(usePiCamera=False,resolution=(320, 240)).start()
time.sleep(1.0)

while(True):
    frame =vs.read()
    frame = imutils.resize(frame,width=450)
    state = detector.detectFaceFeatures(frame)    
    decoder.getState(state)
    #cv2.imshow("frame",frame)
    #key = cv2.waitKey(1) & 0xFF
    #if key == ord("q"):
       # break


cv2.destroyAllWindows()
vs.stop()
