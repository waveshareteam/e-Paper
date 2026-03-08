#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd4in2g
import time
from PIL import Image, ImageDraw
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd4in2g Demo")
    epd = epd4in2g.EPD()
    logging.info("Initializing display")
    epd.init()
    time.sleep(1)

    # Create an image where the left half is black and the right half is white
    
    # ToDo: appearing top row as black and bottom row as white in (landscape mode) ? is this expected? 
    logging.info("Rendering 50 percent black test pattern")
    img_width = epd.height
    img_height = epd.width
    Himage = Image.new('RGB', (img_width, img_height), epd.WHITE)
    draw = ImageDraw.Draw(Himage)
    draw.rectangle((0, 0, img_width // 2, img_height - 1), fill=epd.BLACK)

    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    traceback.print_exc()
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd4in2g.epdconfig.module_exit(cleanup=True)
    exit()
