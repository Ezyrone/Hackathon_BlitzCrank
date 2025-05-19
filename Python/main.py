import serial
import time

arduino = serial.Serial('/dev/ttyUSB0', 9600, timeout=1)
time.sleep(2)  # Laisser le temps à l'Arduino de redémarrer

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
        ir = get_ir()
        if ir > 900:  
            print("Trop proche de la ligne, on recule !")
            send('B')
            time.sleep(0.5)
            send('L')
            time.sleep(0.3)
            send('S')
        else:
            dist = get_distance()
            print(f"Distance: {dist} cm")
            if dist < 30:
                send('F')
                time.sleep(0.3)
            else:
                send('L')
                time.sleep(0.2)
            send('S')
        time.sleep(0.1)

if __name__ == "__main__":
    try:
        main_loop()
    except KeyboardInterrupt:
        send('S')
        print("Arrêt du robot.")
