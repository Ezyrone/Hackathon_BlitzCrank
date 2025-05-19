import serial
import time
import random

# Configuration
BORDER_THRESHOLD = 900      # Seuil de détection des lignes
ATTACK_DISTANCE = 30        # Distance en cm pour lancer une attaque
SEARCH_DISTANCE = 60        # Distance max de détection
INITIAL_DELAY = 5           # Délai initial (règle standard)

# Modes de combat
MODE_SEARCH = 0
MODE_ATTACK = 1
MODE_DEFENSIVE = 2

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

def move_forward(duration=0.3, speed='H'):
    """Avancer avec une vitesse optionnelle (H=haute, M=moyenne, L=basse)"""
    send(f'F{speed}')
    if duration > 0:
        time.sleep(duration)
        send('S')

def move_backward(duration=0.5):
    """Reculer"""
    send('B')
    if duration > 0:
        time.sleep(duration)
        send('S')

def turn_left(duration=0.3):
    """Tourner à gauche"""
    send('L')
    if duration > 0:
        time.sleep(duration)
        send('S')

def turn_right(duration=0.3):
    """Tourner à droite"""
    send('R')
    if duration > 0:
        time.sleep(duration)
        send('S')

def quick_spin(direction='L'):
    """Faire un tour rapide sur soi-même"""
    send(f'{direction}')
    time.sleep(0.8)
    send('S')

def scan_for_opponent():
    """Scan à 360° pour trouver l'adversaire, retourne la meilleure distance et angle"""
    best_distance = 999
    best_angle = 0
    
    # Scan complet
    for angle in range(0, 12):  # 12 positions = 30° chacune
        turn_right(0.15)  # ~30 degrés
        dist = get_distance()
        if dist < best_distance and dist < SEARCH_DISTANCE:
            best_distance = dist
            best_angle = angle
    
    # Si on a trouvé quelque chose, on y retourne
    if best_distance < SEARCH_DISTANCE:
        for _ in range(12 - best_angle):
            turn_right(0.15)
    
    return best_distance

def escape_border():
    """Stratégie d'échappement quand on détecte une ligne"""
    move_backward(0.7)
    
    # On tourne dans une direction aléatoire pour éviter les comportements prévisibles
    if random.choice([True, False]):
        turn_left(0.5)
    else:
        turn_right(0.5)

def main_loop():
    print("Démarrage dans", INITIAL_DELAY, "secondes...")
    time.sleep(INITIAL_DELAY)
    
    current_mode = MODE_SEARCH
    last_seen_time = 0
    
    while True:
        # Vérification de la bordure (prioritaire)
        ir = get_ir()
        if ir > BORDER_THRESHOLD:  
            print("Bordure détectée! Mode défensif.")
            current_mode = MODE_DEFENSIVE
            escape_border()
        else:
            # Mesure de la distance
            dist = get_distance()
            print(f"Distance: {dist} cm | Mode: {current_mode}")
            
            # Décision selon le mode et la distance
            if dist < ATTACK_DISTANCE:
                # L'adversaire est proche, on attaque!
                current_mode = MODE_ATTACK
                last_seen_time = time.time()
                print("Adversaire détecté! Mode attaque!")
                move_forward(0.5, 'H')  # Attaque à pleine puissance
            
            elif current_mode == MODE_ATTACK and time.time() - last_seen_time < 2.0:
                # On vient de perdre l'adversaire mais il doit être proche
                # On continue d'avancer un peu avant de chercher
                move_forward(0.2, 'M')
                turn_left(0.1)  # Légère correction pour chercher
            
            else:
                # Pas d'adversaire en vue, on passe en mode recherche
                current_mode = MODE_SEARCH
                
                # Stratégie de recherche intelligente
                if time.time() % 10 < 5:  # Toutes les 5 secondes
                    # Scan complet à 360°
                    print("Scan complet...")
                    if scan_for_opponent() < SEARCH_DISTANCE:
                        move_forward(0.4, 'H')
                else:
                    # Mouvement en spirale
                    move_forward(0.2, 'L')
                    turn_left(0.1)
        
        time.sleep(0.05)  # Petit délai pour éviter de surcharger l'Arduino

if __name__ == "__main__":
    try:
        main_loop()
    except KeyboardInterrupt:
        send('S')
        print("Arrêt du robot.")
    finally:
        arduino.close()
