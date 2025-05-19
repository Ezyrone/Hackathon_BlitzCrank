import time
import logging

# === CONFIGURATION === #
IR_THRESHOLD = 900     
DISTANCE_MAX = 999     
TIMEOUT = 0.2          

# === LOGGING === #
logging.basicConfig(level=logging.INFO, format='[%(levelname)s] %(message)s')

# === FONCTIONS UTILITAIRES === #

def send_command(ser, cmd):
    """Envoie une commande série à l'Arduino et lit la réponse"""
    try:
        ser.reset_input_buffer()
        ser.write(cmd.encode())
        time.sleep(0.05)  # petite pause pour l'envoi
        t_start = time.time()
        while ser.in_waiting == 0:
            if time.time() - t_start > TIMEOUT:
                raise TimeoutError("Aucune réponse série")
        response = ser.readline().decode().strip()
        logging.debug(f"Commande '{cmd}' -> Réponse : {response}")
        return response
    except Exception as e:
        logging.warning(f"[SERIAL] Erreur sur commande '{cmd}' : {e}")
        return None

# === CAPTEURS === #

def get_distance(ser):
    """Lit la distance depuis le capteur ultrason (Arduino)"""
    response = send_command(ser, 'D')
    try:
        dist = int(response)
        return dist
    except:
        return DISTANCE_MAX

def get_ir_value(ser):
    """Lit la valeur du capteur infrarouge"""
    response = send_command(ser, 'I')
    try:
        val = int(response)
        return val
    except:
        return 0

def is_on_ring(ir_value):
    """Retourne True si le robot est encore dans le ring"""
    return ir_value < IR_THRESHOLD

# === MOTEURS === #

def stop(ser): send_command(ser, 'S')
def forward(ser): send_command(ser, 'F')
def backward(ser): send_command(ser, 'B')
def left(ser): send_command(ser, 'L')
def right(ser): send_command(ser, 'R')
