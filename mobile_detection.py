import cv2
from ultralytics import YOLO

model = YOLO(r"D:\measurement\Humanoid\best (1).pt")
url = "http://10.14.19.62:8080/video"  
cap = cv2.VideoCapture(url)

frame_id = 0

while True:
    ret, frame = cap.read()
    if not ret:
        break

    frame_id += 1
    #processing each 4 frames
    if frame_id % 8!= 0:
        continue

    # detection
    results = model(frame, conf=0.3, imgsz=320)

    # fiteration of the non ball object
    annotated_frame = frame.copy()
    for r in results:
        for box in r.boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])
            area = (x2 - x1) * (y2 - y1)
            if area < 500:  
                continue
            cv2.rectangle(annotated_frame, (x1, y1), (x2, y2), (0,255,0),2)
            cv2.putText(annotated_frame,
                        f"ball {float(box.conf[0]):.2f}",
                        (x1, y1-10),
                        cv2.FONT_HERSHEY_SIMPLEX,
                        0.6,(0,255,0),2)

    cv2.imshow("Ball Detection - Mobile Camera", annotated_frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()
