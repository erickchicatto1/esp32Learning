import cv2
import requests

# URL del stream del ESP32-CAM
URL = "http://172.20.10.4:81/stream"

# Cargar el clasificador en cascada para detección de rostros
face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")

# Verificar si el clasificador se cargó correctamente
if face_cascade.empty():
    print("Error: No se pudo cargar el clasificador de rostros.")
    exit()

# Abrir la conexión con el stream del ESP32-CAM
cap = cv2.VideoCapture(URL)

# Verificar si la conexión se abrió correctamente
if not cap.isOpened():
    print("Error: No se pudo abrir la conexión con la cámara.")
    exit()

print("Conexión exitosa con la cámara. Presiona 'ESC' para salir.")

while True:
    # Leer un frame del stream
    ret, frame = cap.read()

    # Verificar si se recibió un frame válido
    if not ret:
        print("Error: No se pudo recibir un frame del stream.")
        break

    # Convertir el frame a escala de grises
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detectar rostros en el frame
    faces = face_cascade.detectMultiScale(gray, scaleFactor=1.1, minNeighbors=5, minSize=(30, 30))

    # Dibujar rectángulos alrededor de los rostros detectados
    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

    # Mostrar el frame con los rostros detectados
    cv2.imshow("ESP32-CAM Stream", frame)

    # Esperar una tecla (1 ms) y verificar si se presionó 'ESC' (27)
    key = cv2.waitKey(1)
    if key == 27:  # Tecla ESC
        break

# Liberar recursos
cap.release()
cv2.destroyAllWindows()
print("Programa terminado.")