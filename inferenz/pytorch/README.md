# PyTorch Umsetzung

## Setup

Unter Linux kann man ein Setup für die beiden `.py` Skripte in einer virtuellen Python-Umgebung (`.venv`) machen.

### Installation

Erstellen einer virtuellen Umgebung im Verzeichnis `pytorch`:

```bash
cd pytorch
```

`.venv` anlegen:

```bash
pyenv install 3.11.8
pyenv local 3.11.8
python -m venv .venv
source .venv/bin/activate
```

`ultralytics` installieren:

```bash
pip install ultralytics
```

## Ausführen

Starten von Python-Skripten im Terminal. Die virtuelle Umgebung (`.venv`) muss dafür aktiviert sein.

Als Input dient zum Beispiel das Test-Video [`clip640x424.mp4`](./resources/clip640x424.mp4).

**Export-Skript (`ameisen_export.py`)**

```bash
python ameisen_export.py ./resources/clip640x424.mp4 out.mp4

```

**Live-Skript (`ameisen_live.py`)**

Echtzeit-Anzeige der Objekterkennung (ohne Datei-Export):

```bash
python ameisen_live.py ./resources/clip640x424.mp4
```