import cv2
import sys
import argparse
from ultralytics import YOLO

# --- Kommandozeilen-Parameter einrichten ---
parser = argparse.ArgumentParser(description="Live-Ansicht für Ameisen-Erkennung mit YOLO")
parser.add_argument("input_video", help="Pfad zur Eingabe-Videodatei (z.B. ./resources/clip640x424.mp4)")
args = parser.parse_args()

model_path = "./resources/bestYv11_03.pt"
video_path = args.input_video

model = YOLO(model_path)
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    sys.exit(f"Fehler: Konnte Video {video_path} nicht öffnen.")

fps = cap.get(cv2.CAP_PROP_FPS)
delay = int(1000 / fps) if fps > 0 else 30

while cap.isOpened():
    success, frame = cap.read()
    if not success:
        break

    results = model(frame, verbose=False)

    for box in results[0].boxes:
        x1, y1, x2, y2 = map(int, box.xyxy[0])
        cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 1)

    cv2.imshow("Ameisen Erkennung Live", frame)

    if cv2.waitKey(delay) & 0xFF == ord("q"):
        break

cap.release()
cv2.destroyAllWindows()