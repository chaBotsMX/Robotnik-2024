import sensor
import time
import pyb
from pyb import UART

# Color Tracking Thresholds
amarillo = [(45, 58, -22, 33, 15, 100),]
azul = [(30, 55, -18, 26, -52, -19),]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_contrast(3)
sensor.set_brightness(-3)
sensor.set_saturation(3)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)
sensor.skip_frames(time=2000)

uart = UART(1, 9600, timeout_char=1000)

roi = (60, 15, 220, 240)

# Establece el ROI
sensor.set_windowing(roi)


while True:
    img = sensor.snapshot()

    # Inicializar variables para almacenar el blob más grande
    max_area_amarillo = 0
    max_blob_amarillo = None
    max_area_azul = 0
    max_blob_azul = None

    # Buscar el blob más grande de color amarillo
    for blob in img.find_blobs(amarillo, pixels_threshold=20, area_threshold=20, merge=True):
        if blob.area() > max_area_amarillo:
            max_area_amarillo = blob.area()
            max_blob_amarillo = blob

    # Buscar el blob más grande de color azul
    for blob in img.find_blobs(azul, pixels_threshold=100, area_threshold=100, merge=True):
        if blob.area() > max_area_azul:
            max_area_azul = blob.area()
            max_blob_azul = blob

    # Calcular el vector hacia el blob más grande de cada color
    if max_blob_amarillo:
        vectorAmarilloX = max_blob_amarillo.cx()
        vectorAmarilloY = max_blob_amarillo.cy()
        img.draw_cross(max_blob_amarillo.cx(), max_blob_amarillo.cy())
    else:
        vectorAmarilloX = 0
        vectorAmarilloY = 0

    if max_blob_azul:
        vectorAzulX = max_blob_azul.cx()
        vectorAzulY = max_blob_azul.cy()
        img.draw_cross(max_blob_azul.cx(), max_blob_azul.cy())
    else:
        vectorAzulX = 0
        vectorAzulY = 0

    # Sumar los vectores
    vectorFinalX = vectorAmarilloX + vectorAzulX
    vectorFinalY = vectorAmarilloY + vectorAzulY

    # Dibujar línea hacia el vector final
    img.draw_line(112, 120, int(vectorFinalX/2), int(vectorFinalY/2))

    # Enviar datos por UART
    uart.write("VX:{} VY:{}\n".format(vectorFinalX/2, vectorFinalY/2))
    print("X",vectorFinalX/2)
    print("Y",vectorFinalY/2)
