import cv2
from ultralytics import YOLO
import requests
import time
import socket

# ===============================
# Load YOLO model
# ===============================
model = YOLO(r"C:\Users\narko\OneDrive\Desktop\Files\Programming\Python_Projects\best_n.pt")

# ===============================
# Laptop Camera
# ===============================
cap = cv2.VideoCapture(0)  # 0 = Laptop Camera

# ===============================
# Other ESP32
# ===============================
ESP32_IP = "10.90.3.2"

CMD_URL = f"http://{ESP32_IP}/command"
ESP32_PORT = 1234

command = 0
last_command = None

# ===============================
# Calibration
# ===============================
REAL_BALL_DIAMETER = 20.0      # cm
CALIBRATION_DISTANCE = 20.0    # cm
FOCAL_LENGTH = None

# ===============================
# Depth calculation
# ===============================
def calculate_depth(pixel_height, focal_length):
    if pixel_height <= 0 or focal_length is None:
        return -1
    return (focal_length * REAL_BALL_DIAMETER) / pixel_height


# ===============================
# Main loop
# ===============================
while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to read from laptop camera")
        break

    results = model(frame, imgsz=320, conf=0.5, verbose=False)
    annotated = results[0].plot()

    h, w, _ = annotated.shape
    left_boundary = w * 0.35
    right_boundary = w * 0.65

    boxes = results[0].boxes

    for box in boxes:
        cls_id = int(box.cls[0])
        if model.names[cls_id] != "ball":
            continue

        x1, y1, x2, y2 = box.xyxy[0]
        cx = int((x1 + x2) / 2)
        cy = int((y1 + y2) / 2)
        pixel_height = int(y2 - y1)

        # ===============================
        # Region decision
        # ===============================
        if cx < left_boundary:
            region, command = "RIGHT", 1
        elif cx > right_boundary:
            region, command = "LEFT", 2
        else:
            region, command = "CENTER", 3

        # ===============================
        # Send HTTP command (only if changed)
        # ===============================
        if command != last_command:
            try:
                requests.post(CMD_URL, data=command, timeout=0.3)
                last_command = command
            except:
                pass

        # ===============================
        # Send socket command
        # ===============================
        try:
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect((ESP32_IP, ESP32_PORT))
            s.sendall(bytes([command]))
            s.close()
        except:
            pass

        # ===============================
        # Drawing
        # ===============================
        cv2.circle(annotated, (cx, cy), 5, (0, 0, 255), -1)
        cv2.putText(annotated, region, (cx - 40, cy - 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 0, 255), 2)

        if FOCAL_LENGTH:
            depth = calculate_depth(pixel_height, FOCAL_LENGTH)
            cv2.putText(annotated, f"{depth:.1f} cm",
                        (cx - 40, cy - 15),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.8, (255, 0, 0), 2)

    if not FOCAL_LENGTH:
        cv2.putText(annotated, "Press 'c' to Calibrate",
                    (20, 40),
                    cv2.FONT_HERSHEY_SIMPLEX, 0.8, (0, 255, 255), 2)

    cv2.imshow("Laptop Camera + YOLO + Depth", annotated)

    key = cv2.waitKey(1) & 0xFF

    # ===============================
    # Calibration key
    # ===============================
    if key == ord('c') and boxes:
        FOCAL_LENGTH = (pixel_height * CALIBRATION_DISTANCE) / REAL_BALL_DIAMETER
        print(f"[CALIBRATED] FOCAL_LENGTH = {FOCAL_LENGTH:.2f}")

    if key == ord('q'):
        break

    time.sleep(0.05)

cap.release()
cv2.destroyAllWindows()
