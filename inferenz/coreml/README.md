# Core ML Umsetzung

## Setup

Unter `macOS` muss `Xcode` installiert sein.

**In Xcode**

`Detector.xcodeproj` in Xcode öffnen und `⌘B` drücken.

**Im Terminal**
Projekt im Release-Modus inkl. aller `.xcconfig` Abhängigkeiten über `xcodebuild` kompilieren:

```bash
xcodebuild -project Detect.xcodeproj -scheme Detector -configuration Release clean build
```

## Ausführen

Es kann in `Xcode` ausgeführt werden oder als Command im Terminal aufgerufen werden.

**In Xcode**

Als Standard für die Übergabe Argumente ist im `Detector` Scheme folgendes voreingestellt:

Als maximale Frame Anzahl bsi zu der im Vidoe Stream gearbeitet wird, ist 3000 eingestellt, also es werden die ersten 3000 Frames untersucht. (1. Scheme Command Line Argument: `3000`)

Als Input ist der Pfad zur Datei [`clip640x424.mp4`](./resources/clip640x424.mp4) eingestellt. (2. Scheme Command Line Argument: `${SRCROOT}/resources/clip640x424.mp4`)

Als Output ist der Pfad `./resources/clip640x424_annotated_coreml.mp4` eingestellt. (3. Scheme Command Line Argument: `${SRCROOT}/resources/clip640x424_annotated_coreml.mp4`)

Im Xcode-Projekt zum Starten `⌘R` drücken.

Die Objekterkennung startet und es werden die ersten 3000 Frames der Input Film-Datei verarbeitet und als `clip640x424_annotated_coreml.mp4` ins Verzeichnis [`./resources`](./resources)` ausgegeben.

**Im Terminal**

Starten des Tools aus dem lokalen Build-Verzeichnis unter `coreml/build/Debug`:

(oder anderer Pfad, falls im Release-Modus gebaut wurde)

```bash
cd coreml/build/Debug
```

Beispiel:

```bash
./Detector 3000 <pfad zum Clip in clip640x424.mp4> out.mp4
```