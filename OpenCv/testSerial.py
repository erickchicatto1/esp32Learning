import serial
import time

#Configuracion del puerto serial 
port='COM5'
baudrate = 115200

#Abrir la conexion serial
try:
    ser = serial.Serial(port,baudrate,timeout=1)
    print(f"Conexión establecida con {port} a {baudrate} baudios.")
except serial.SerialException as e:
    print(f"Error al abrir el puerto serial : {e}")
    exit()


#Funcion para enviar datos al esp32
def enviar_mensaje(mensaje):
    ser.write(mensaje.encode('utf-8'))
    print(f"Enviado : {mensaje}")

#Funcion para recibir datos 
def recibir_mensajes():
    if ser.in_waiting>0:
        bytes_recibidos = ser.readline()
        mensaje = bytes_recibidos.decode('utf-8').strip() #convertir bytes a string
        print(f"Recibido : {mensaje}")
        return mensaje
    return None
#Bucle principal 
try:
    while True:
        #Enviar un mensaje al esp32
        time.sleep(1)
        recibir_mensajes()
        time.sleep(2)
except KeyboardInterrupt:
    print("Programa terminado por el usuario")
finally:
    ser.close()
    print("Conexion serial cerrada")
        