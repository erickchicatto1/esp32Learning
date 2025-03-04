import cv2

# Función para detectar bordes (pared)
def detectar_pared(frame):
    # Convertir a escala de grises
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Aplicar un filtro Gaussiano para reducir el ruido
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    
    # Detectar bordes usando Canny
    edges = cv2.Canny(blurred, 50, 150)
    
    # Contar los píxeles de bordes detectados
    edge_pixels = cv2.countNonZero(edges)
    
    # Si hay muchos bordes, es probable que sea una pared
    return edge_pixels > 5000  # Ajusta este umbral según tu caso

# Función para detectar personas (usando HOG + SVM)
def detectar_persona(frame):
    # Inicializar el detector de personas HOG
    hog = cv2.HOGDescriptor()
    hog.setSVMDetector(cv2.HOGDescriptor_getDefaultPeopleDetector())
    
    # Detectar personas en el frame
    (rects, _) = hog.detectMultiScale(frame, winStride=(4, 4), padding=(8, 8), scale=1.05)
    
    # Si se detecta al menos una persona, retornar True
    return len(rects) > 0

# Función principal
def main():
    # Abrir la cámara o un video
    cap = cv2.VideoCapture(0)  # Usar 0 para la cámara web, o la ruta a un archivo de video
    
    while True:
        # Leer un frame del video
        ret, frame = cap.read()
        if not ret:
            print("No se pudo capturar el frame.")
            break
        
        # Redimensionar el frame para que sea más rápido el procesamiento
        frame = cv2.resize(frame, (640, 480))
        
        # Detectar si hay una pared
        if detectar_pared(frame):
            print("¡Se detectó una pared!")
            cv2.putText(frame, "Pared Detectada", (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 255, 0), 2)
        
        # Detectar si hay una persona
        if detectar_persona(frame):
            print("¡Se detectó una persona!")
            cv2.putText(frame, "Persona Detectada", (10, 60), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 0, 255), 2)
        
        # Mostrar el frame
        cv2.imshow("Detección en Tiempo Real", frame)
        
        # Salir si se presiona la tecla 'q'
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    # Liberar recursos
    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()