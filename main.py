import cv2
import os

def list_cameras():
    index = 0
    cameras = []
    while True:
        cap = cv2.VideoCapture(index)
        if not cap.read()[0]:  # Check if camera is available
            break
        cameras.append(f"Camera {index}")
        cap.release()
        index += 1
    return cameras

def preview_camera(index):
    cap = cv2.VideoCapture(index)
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        cv2.imshow(f"Preview - Camera {index}", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):  # Quit on 'q'
            break
    cap.release()
    cv2.destroyAllWindows()

cameras = list_cameras()
for i in range(len(cameras)):
    print(cameras[i])
    preview_camera(i)
