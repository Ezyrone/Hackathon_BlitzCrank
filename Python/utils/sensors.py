import time

def send_command(ser, cmd):
    ser.write(cmd.encode())
    time.sleep(0.1)
    response = ser.readline().decode().strip()
    return response

def get_distance(ser):
    """Demande la distance à l'Arduino (en cm)"""
    try:
        response = send_command(ser, 'D')
        return int(response)
    except:
        return 999  

def get_ir_value(ser):
    """Demande la valeur brute du capteur IR"""
    try:
        response = send_command(ser, 'I')
        return int(response)
    except:
        return 0  
