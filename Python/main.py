import os
import time
import serial
from utils.sensors import (
    get_distance, get_ir_value, is_on_ring,
    forward, backward, left, right,
    rotate_cw, rotate_ccw, stop
)

# === LECTURE AUDIO ===
import os

def play_john_cena():
    print("🔊 Lecture de l’intro : JOHN CENA !")
    os.system("file/and-his-name-is-john-cena-sound-effect-(hd)-made-with-Voicemod.mp3")


# === INITIALISATION ===
arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)

def main_loop():
    play_john_cena()
    print("⏳ Attente de 5 secondes (règlement)...")
    time.sleep(5)

    state = "SCAN"

    while True:
        try:
            ir = get_ir_value(arduino)
            dist = get_distance(arduino)

            if not is_on_ring(ir):
                print("⚠️ Bord détecté : ESCAPE")
                backward(arduino)
                time.sleep(0.4)
                rotate_ccw(arduino)
                time.sleep(0.3)
                stop(arduino)
                state = "SCAN"

            elif dist < 30:
                print(f"🎯 Ennemi à {dist} cm : ATTACK")
                forward(arduino)
                time.sleep(0.2)
                stop(arduino)
                state = "SCAN"

            else:
                print("🔎 SCAN (rotation)")
                rotate_cw(arduino)
                time.sleep(0.3)
                stop(arduino)

            time.sleep(0.1)

        except KeyboardInterrupt:
            stop(arduino)
            print("🛑 Arrêt manuel")
            break

if __name__ == "__main__":
    main_loop()
