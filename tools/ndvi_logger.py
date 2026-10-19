"""
NDVI logger for the Crop-Stress Hat (Tech Canopies, Group 4).

Talks to the Arduino Uno running firmware/hatvest_ndvi/hatvest_ndvi.ino and saves every reading taken with 'r'
(and every white reference taken with 'w') to a CSV file, with a timestamp and a label.

Setup (once):   pip install pyserial
Run:            python ndvi_logger.py
                (close the Arduino IDE Serial Monitor first: only one program can use the port)

Commands while running:
  w              store white reference (white paper under the sensor)
  r              take one reading and save it
  label <text>   set the label saved with the next readings, e.g.  label green leaf 1
  q              quit
"""
import csv
import glob
import os
import re
import sys
import threading
import time
from datetime import datetime

import serial  # pip install pyserial

BAUD = 115200
CSV_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "ndvi_log.csv")
FIELDS = ["timestamp", "type", "label", "red_F7", "edge_F8", "nir", "ndvi", "ndre"]

PATTERN = re.compile(
    r"red\(F7\)=(?P<red>[\d.]+)\s+edge\(F8\)=(?P<edge>[\d.]+)\s+nir=(?P<nir>[\d.]+)"
    r"(?:.*?NDVI=(?P<ndvi>-?[\d.]+)\s+NDRE=(?P<ndre>-?[\d.]+))?"
)


def find_port():
    ports = sorted(glob.glob("/dev/cu.usbmodem*") + glob.glob("/dev/ttyACM*") + glob.glob("COM*"))
    if not ports:
        sys.exit("No Arduino port found. Is the Arduino plugged in?")
    if len(ports) > 1:
        print("Several ports found:", ports, "- using", ports[0])
    return ports[0]


def main():
    port = sys.argv[1] if len(sys.argv) > 1 else find_port()
    ser = serial.Serial(port, BAUD, timeout=1)
    time.sleep(2)  # board resets when the port opens
    print(f"Connected to {port}. Saving to {CSV_PATH}")
    print("Commands: w, r, label <text>, q")

    new_file = not os.path.exists(CSV_PATH)
    f = open(CSV_PATH, "a", newline="")
    writer = csv.DictWriter(f, fieldnames=FIELDS)
    if new_file:
        writer.writeheader()

    state = {"label": "unlabelled", "running": True}
    ser.write(b"s\n")  # pause the live stream so only deliberate readings appear

    def reader():
        while state["running"]:
            line = ser.readline().decode(errors="ignore").strip()
            if not line:
                continue
            kind = "sample" if line.startswith("SAMPLE") else "white" if line.startswith("WHITE") else None
            if kind is None:
                if line.startswith("raw") or "stream" in line.lower() or "received" in line:
                    continue
                print("  arduino:", line)
                continue
            m = PATTERN.search(line)
            if not m:
                print("  (could not parse)", line)
                continue
            row = {
                "timestamp": datetime.now().isoformat(timespec="seconds"),
                "type": kind,
                "label": state["label"],
                "red_F7": m["red"], "edge_F8": m["edge"], "nir": m["nir"],
                "ndvi": m["ndvi"] or "", "ndre": m["ndre"] or "",
            }
            writer.writerow(row)
            f.flush()
            print(f"  saved {kind:6} [{row['label']}]  NDVI={row['ndvi'] or '-'}  NDRE={row['ndre'] or '-'}")

    threading.Thread(target=reader, daemon=True).start()

    try:
        while True:
            cmd = input("> ").strip()
            if cmd == "q":
                break
            if cmd.startswith("label "):
                state["label"] = cmd[6:].strip()
                print(f"  label set to: {state['label']}")
            elif cmd in ("w", "r"):
                ser.write((cmd + "\n").encode())
            elif cmd:
                print("  use w, r, label <text> or q")
    finally:
        state["running"] = False
        time.sleep(0.5)
        ser.close()
        f.close()
        print("Saved to", CSV_PATH)


if __name__ == "__main__":
    main()
