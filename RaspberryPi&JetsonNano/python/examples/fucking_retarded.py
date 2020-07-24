import logging
import sys
import os
import time
import traceback
import numpy as np

from waveshare_epd import epd7in5b_HD
from PIL import Image, ImageDraw, ImageFont

logging.basicConfig(level=logging.DEBUG)

picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')

image_full = Image.open(os.path.join(picdir, 'fucking_retarded.bmp'))

buffer = np.array(image_full.convert('RGB'))

r, g, b = buffer[:, :, 0], buffer[:, :, 1], buffer[:, :, 2]

buffer_black = np.full(buffer.shape, 255, 'uint8')

buffer_black[np.logical_and(r == 0, g == 0, b == 0)] = [0, 0, 0]

buffer_red = np.full(buffer.shape, 255, 'uint8')

buffer_red[np.logical_and(r == 255, g == 0, b == 0)] = [0, 0, 0]

image_black = Image.fromarray(buffer_black)
image_red = Image.fromarray(buffer_red)

if os.path.exists(libdir):
    sys.path.append(libdir)

try:

    logging.info("epd7in5b_HD Demo")

    epd = epd7in5b_HD.EPD()

    logging.info("init and Clear")

    epd.init()
    epd.Clear()
    
    logging.info("3.read bmp file...")

    epd.display(epd.getbuffer(image_black), epd.getbuffer(image_red))

    time.sleep(1)

    logging.info("Goto Sleep...")

    epd.sleep()

except IOError as e:
    logging.info(e)

except KeyboardInterrupt:
    logging.info("ctrl + c:")
    epd7in5b_HD.epdconfig.module_exit()
    exit()
