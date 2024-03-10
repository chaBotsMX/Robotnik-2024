# This work is licensed under the MIT license.
# Copyright (c) 2013-2023 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# Multi Color Blob Tracking Example
#
# This example shows off multi color blob tracking using the OpenMV Cam.

import sensor
import time
import math
from pyb import UART


# Color Tracking Thresholds (L Min, L Max, A Min, A Max, B Min, B Max)
# The below thresholds track in general red/green things. You may wish to tune them...
amarillo = [
    (52, 67, -8, 33, 25, 69),
]
azul = [
    (0, 100, -15, 26, -50, -17),
]


sensor.reset()
#sensor.set_auto_exposure(False)
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time=2000)
sensor.set_contrast(3)
sensor.set_brightness(-2)
sensor.set_saturation(0)

sensor.set_auto_gain(False)  # must be turned off for color tracking
sensor.set_auto_whitebal(False)  # must be turned off for color tracking
clock = time.clock()

# Only blobs that with more pixels than "pixel_threshold" and more area than "area_threshold" are
# returned by "find_blobs" below. Change "pixels_threshold" and "area_threshold" if you change the
# camera resolution. Don't set "merge=True" because that will merge blobs which we don't want here.

uart = UART(1, 115200, timeout_char=1000)                         # init with given baudrate

while True:
    img = sensor.snapshot()

    vectorAmarilloX = 0
    vectorAmarilloY = 0
    vectorAzulX = 0
    vectorAzulY = 0
    cont = 0
    for blob in img.find_blobs(amarillo, pixels_threshold=20, area_threshold=20):
        cont =+ 1
        vectorAmarilloX = vectorAmarilloX + blob.cx() - 160
        vectorAmarilloY = vectorAmarilloY + blob.cy() - 120
        img.draw_cross(blob.cx(), blob.cy())

    if cont > 0 :
        vectorAmarilloY = vectorAmarilloY/2


    cont = 0
    for blob in img.find_blobs(azul, pixels_threshold=100, area_threshold=100):
        cont =+ 1
        vectorAzulX = vectorAzulX + blob.cx() - 160
        vectorAzulY = vectorAzulY + blob.cy() - 120
        img.draw_cross(blob.cx(), blob.cy())
    if cont > 0 :
        vectorAzulY = vectorAzulY/2
    vectorAzulX = vectorAzulX + vectorAmarilloX
    vectorAzulY = vectorAzulY + vectorAmarilloY
    vectorAzulX = int(vectorAzulX)
    vectorAzulY = int(vectorAzulY)
    img.draw_line(160,120,vectorAzulX + 160, vectorAzulY + 120)


