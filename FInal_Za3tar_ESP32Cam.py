import cv2
import urllib.request
import numpy as np
from ultralytics import YOLO
import requests
import time
import socket

# ===============================
# Load YOLO model
# ===============================
model = YOLO(r"C:\Users\narko\OneDrive\Desktop\Files\Programming\Python_Projects\best_n.pt")

# ESP32-CAM URL
url = "http://192.168.1.108/cam-lo.jpg"

# Other ESP32
ESP32_IP = "10.90.3.2"
CMD_URL = f"http://{ESP32_IP}/command"
ESP32_PORT = 1234                       # Port Number written on the ESP32 code

command = 0                             # Variable used to store the data needed to be sent to the Other ESP32

# ===============================
# Calibration
# ===============================
REAL_BALL_DIAMETER = 20.0               # Real Ball diameter to the ball used 
CALIBRATION_DISTANCE = 20.0             # Measured distance from the ESP32_cam to the ball
FOCAL_LENGTH = None                     # Focal length which will be calculated later after calibration

last_command = None                     # Prevent sending repeated data to the other esp32

#-----------------------------------------------------------------------------------
# The following function is used to calculate the focal length to get back the depth
#-----------------------------------------------------------------------------------

def calculate_depth(pixel_height, focal_length):
    if pixel_height <= 0 or focal_length is None:
        return -1
    return (focal_length * REAL_BALL_DIAMETER) / pixel_height


while True:
    try:
        img_resp = urllib.request.urlopen(url, timeout=2)
        img_np = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(img_np, cv2.IMREAD_COLOR)

        if frame is None:
            continue

    except Exception as e:
        print("Camera error:", e)
        time.sleep(1)
        continue

    results = model(frame, imgsz=320, conf=0.5, verbose=False)
    annotated = results[0].plot()

    h, w, _ = annotated.shape
    left_boundary = w * 0.35          # Left Part of the frame     
    right_boundary = w * 0.65         # Right Part of the frame
    
    # The rest is the Center region of the frame

    boxes = results[0].boxes

    for box in boxes:
        cls_id = int(box.cls[0])
        if model.names[cls_id] != "ball":
            continue

        x1, y1, x2, y2 = box.xyxy[0]
        cx = int((x1 + x2) / 2)
        cy = int((y1 + y2) / 2)
        pixel_height = int(y2 - y1)

        if cx < left_boundary:
            region, command = "LEFT", 1
        elif cx > right_boundary:
            region, command = "RIGHT", 2
        else:
            region, command = "CENTER", 3

        # Sending Command only in case the state changed
        
        if command != last_command:
            try:
                requests.post(CMD_URL, data=command, timeout=0.3)
                last_command = command
            except:
                pass

        cv2.circle(annotated, (cx, cy), 5, (0, 0, 255), -1)
        cv2.putText(annotated, region, (cx - 40, cy - 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 2)  

        if FOCAL_LENGTH:
            depth = calculate_depth(pixel_height, FOCAL_LENGTH)
            cv2.putText(annotated, f"{depth:.1f} cm",
                        (cx - 40, cy - 15),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)
        else:
            cv2.putText(annotated, "Press 'c' to Calibrate",
                        (20, 40),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

    cv2.imshow("ESP32-CAM Auto Calibration + Depth", annotated)

    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect((ESP32_IP, ESP32_PORT))

    s.sendall(bytes([command]))
    
    key = cv2.waitKey(1) & 0xFF
    if key == ord('c') and boxes:
        FOCAL_LENGTH = (pixel_height * CALIBRATION_DISTANCE) / REAL_BALL_DIAMETER
        print(f"[CALIBRATED] FOCAL_LENGTH = {FOCAL_LENGTH:.2f}")

    if key == ord('q'):
        break

    time.sleep(0.08)  # Pause System for 0.08 sec (80 ms)

cv2.destroyAllWindows()