import serial
import time
from utils.sensors import get_distance, get_ir_value, is_on_ring, forward, backward, left, right, stop

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  

def send(cmd):
    arduino.write(cmd.encode())

def read_response():
    return arduino.readline().decode().strip()

def get_distance():
    send('D')
    time.sleep(0.1)
    return int(read_response() or 999)

def get_ir():
    send('I')
    time.sleep(0.1)
    return int(read_response() or 0)


def main_loop():
    print("Démarrage dans 5 secondes...")
    time.sleep(5)

    while True:
        ir = get_ir_value(arduino)
        if not is_on_ring(ir):
            print("⚠️ Hors du ring détecté !")
            backward(arduino)
            time.sleep(0.4)
            left(arduino)
            time.sleep(0.3)
            stop(arduino)
        else:
            dist = get_distance(arduino)
            if dist < 30:
                forward(arduino)
                time.sleep(0.2)
            else:
                left(arduino)
                time.sleep(0.2)
            stop(arduino)

        time.sleep(0.1)



if __name__ == "__main__":
    try:
        main_loop()
    except KeyboardInterrupt:
        send('S')
        print("Arrêt du robot.")
