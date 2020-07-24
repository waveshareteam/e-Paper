import logging
import sys
import os
import time
import traceback

from waveshare_epd import epd7in5b_HD
from PIL import Image, ImageDraw, ImageFont

logging.basicConfig(level=logging.DEBUG)

picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')

if os.path.exists(libdir):
    sys.path.append(libdir)

try:

    logging.info("epd7in5b_HD Demo")

    epd = epd7in5b_HD.EPD()

    logging.info("init and Clear")

    epd.init()
    epd.Clear()
    
    logging.info("3.read bmp file...")

    blackimage = Image.open(os.path.join(picdir, 'fucking_retarded.bmp'))
    redimage = Image.open(os.path.join(picdir, 'fucking_retarded.bmp'))

    epd.display(epd.getbuffer(blackimage), epd.getbuffer(redimage))

    time.sleep(1)

    logging.info("Goto Sleep...")

    epd.sleep()

except IOError as e:
    logging.info(e)

except KeyboardInterrupt:
    logging.info("ctrl + c:")
    epd7in5b_HD.epdconfig.module_exit()
    exit()
