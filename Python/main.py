import serial
import time
from utils.sensors import (
    get_distance,
    get_ir_value,
    is_on_ring,
    forward,
    backward,
    left,
    right,
    stop,
    rotate_cw,
    rotate_ccw
)

# === ÉTATS POSSIBLES ===
STATE_SCAN = "SCAN"
STATE_ATTACK = "ATTACK"
STATE_ESCAPE = "ESCAPE"

# === INIT ===
arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)

# === PARAMÈTRES ===
DISTANCE_THRESHOLD = 30  # cm
ESCAPE_DURATION = 0.4
ATTACK_DURATION = 0.2
SCAN_ROTATION_TIME = 0.3

def main_loop():
    state = STATE_SCAN
    print("⏳ Attente règlementaire de 5 secondes...")
    time.sleep(5)

    while True:
        try:
            ir = get_ir_value(arduino)
            dist = get_distance(arduino)

            # 🔁 Sécurité : sortir si on touche la ligne blanche
            if not is_on_ring(ir):
                print("⚠️ Proximité bord détectée : ESCAPE")
                state = STATE_ESCAPE

            if state == STATE_ESCAPE:
                backward(arduino)
                time.sleep(ESCAPE_DURATION)
                rotate_ccw(arduino)
                time.sleep(0.3)
                stop(arduino)
                state = STATE_SCAN

            elif dist < DISTANCE_THRESHOLD:
                print("🎯 Ennemie détecté à", dist, "cm : ATTACK")
                forward(arduino)
                time.sleep(ATTACK_DURATION)
                stop(arduino)
                state = STATE_SCAN  # ou rester en ATTACK si tu veux enchainer

            else:
                print("🔎 Aucun ennemi : SCAN (rotation)")
                rotate_cw(arduino)
                time.sleep(SCAN_ROTATION_TIME)
                stop(arduino)
                state = STATE_SCAN

            time.sleep(0.1)

        except KeyboardInterrupt:
            stop(arduino)
            print("🛑 Interruption manuelle")
            break

if __name__ == "__main__":
    main_loop()
