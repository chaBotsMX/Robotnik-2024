import sensor
import time
from pyb import UART

# Color Tracking Thresholds
amarillo = [(43, 84, -25, 49, 13, 61)]
azul = [(0, 100, -72, 48, -83, -28)]

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_contrast(2)
sensor.set_brightness(-3)
sensor.set_saturation(3)
sensor.skip_frames(time=1000)
sensor.set_auto_gain(False)
sensor.set_auto_whitebal(False)

uart = UART(1, 9600, timeout_char=1000)

roi = (55, 0, 230, 240)
sensor.set_windowing(roi)
sensor.skip_frames(time=1000)
while True:
    img = sensor.snapshot()
    datos_envio = ""

    max_area_amarillo, max_blob_amarillo = 0, None
    max_area_azul, max_blob_azul = 0, None

    for blob in img.find_blobs(amarillo, pixels_threshold=20, area_threshold=20, merge=True):
        if blob.area() > max_area_amarillo:
            max_area_amarillo, max_blob_amarillo = blob.area(), blob

    for blob in img.find_blobs(azul, pixels_threshold=100, area_threshold=100, merge=True):
        if blob.area() > max_area_azul:
            max_area_azul, max_blob_azul = blob.area(), blob

    # Agregar datos de blob amarillo al string de envío
    if max_blob_amarillo:
        datos_envio += "YA:{} {}, ".format(max_blob_amarillo.cx(), max_blob_amarillo.cy())
        img.draw_cross(max_blob_amarillo.cx(), max_blob_amarillo.cy())
    else:
        datos_envio += "YA:0 0, "

    # Agregar datos de blob azul al string de envío
    if max_blob_azul:
        datos_envio += "BA:{} {}\n".format(max_blob_azul.cx(), max_blob_azul.cy())
        img.draw_cross(max_blob_azul.cx(), max_blob_azul.cy())
        print(max_blob_azul.cx(), max_blob_azul.cy())
    else:
        datos_envio += "BA:0 0\n"

    # Enviar toda la información en un solo string


    uart.write(datos_envio.encode())
