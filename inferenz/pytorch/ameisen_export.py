import cv2
import time
import sys
import argparse
from ultralytics import YOLO

# --- Kommandozeilen-Parameter einrichten ---
parser = argparse.ArgumentParser(description="Batch-Export für Ameisen-Erkennung mit YOLO")
parser.add_argument("input_video", help="Pfad zur Eingabe-Videodatei (z.B. ./resources/clip640x424.mp4)")
parser.add_argument("output_video", help="Pfad zur Ausgabe-Videodatei (z.B. ./resources/out.mp4)")
args = parser.parse_args()

model_path = "./resources/bestYv11_03.pt"
video_path = args.input_video
output_path = args.output_video

# --- NEU: Wie viele Bilder sollen gleichzeitig zur Grafikkarte? ---
batch_size = 8 
# ----------------------------------------------------------------

model = YOLO(model_path)
cap = cv2.VideoCapture(video_path)

if not cap.isOpened():
    sys.exit(f"Fehler: Konnte Video {video_path} nicht öffnen.")

fps_in = cap.get(cv2.CAP_PROP_FPS)
width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))

fourcc = cv2.VideoWriter_fourcc(*'mp4v')
out = cv2.VideoWriter(output_path, fourcc, fps_in, (width, height))

print(f"Starte Batch-Export (Paketgröße: {batch_size})... Bitte warten.\n")

last_print_time = time.time()
frames_since_last_print = 0
frame_index = 0

# Hier sammeln wir die Bilder, bis der Karton voll ist
frames_batch = []

while cap.isOpened():
    success, frame = cap.read()
    
    if success:
        frames_batch.append(frame)
    
    # Wenn unser Paket voll ist ODER das Video zu Ende ist (und noch Reste im Paket sind)
    if len(frames_batch) == batch_size or (not success and len(frames_batch) > 0):
        
        # Wir übergeben jetzt kein einzelnes Bild mehr, sondern eine ganze Liste!
        results = model(frames_batch, verbose=False, half=True)
       
        # Jetzt packen wir das Paket wieder aus und zeichnen die Kästen
        for i, result in enumerate(results):
            current_frame = frames_batch[i]
            
            for box in result.boxes:
                x1, y1, x2, y2 = map(int, box.xyxy[0])
                cv2.rectangle(current_frame, (x1, y1), (x2, y2), (0, 255, 0), 1)
            
            out.write(current_frame)
            frames_since_last_print += 1
            frame_index += 1
        
        # Paket für die nächste Runde leeren
        frames_batch = []
        
        # Terminal-Ausgabe
        now = time.time()
        elapsed_time = now - last_print_time
        
        if elapsed_time >= 1.0:
            # Echte FPS berechnen: Bilder geteilt durch vergangene Zeit
            actual_fps = frames_since_last_print / elapsed_time
            
            sys.stdout.write(
                f"\r⏱ Frame {frame_index:4d} | "
                f"Pipeline: {actual_fps:5.1f} FPS )"
            )
            sys.stdout.flush()
            
            last_print_time = now
            frames_since_last_print = 0

    # Wenn wir das Ende des Videos erreicht haben, Schleife abbrechen
    if not success:
        break

cap.release()
out.release()

sys.stdout.write(f"\n\n✅ Verarbeitung abgeschlossen: {frame_index} Frames verarbeitet.\n")
print(f"Export abgeschlossen: {output_path}")