import sensor
import time
import pyb
from pyb import UART

# Color Tracking Thresholds
thresholds = [(43, 84, 12, 58, 39, 89),
             (30, 55, -18, 26, -52, -19)]

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

roi = (60, 120  , 220, 240)
red_led = pyb.LED(2)

# Establece el ROI
sensor.set_windowing(roi)

while True:
    img = sensor.snapshot()

    # Inicializar variables para almacenar el blob más grande
    max_area = 0
    max_blob = None

    # Buscar el blob más grande entre todos los colores
    for blob in img.find_blobs(thresholds, pixels_threshold=20, area_threshold=20, merge=True):
        if blob.area() > max_area:
            max_area = blob.area()
            max_blob = blob

    # Actuar en base al blob más grande encontrado
    if max_blob:
        img.draw_cross(max_blob.cx(),max_blob.cy())
        # Convierte las coordenadas a string
        data_str = "{},{}".format(max_blob.cx(), max_blob.cy())
        uart.write(data_str + '\n')  # Añade un salto de línea para delimitar los mensajes
        #red_led.on()

