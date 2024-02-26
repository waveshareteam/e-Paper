#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in9bc
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in9bc Demo")
    
    epd = epd2in9bc.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    logging.info("3.read bmp file")
    HBlackimage = Image.open(os.path.join(picdir, 'blackhole.bmp'))
    epd.display(epd.getbuffer(HBlackimage))

    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd2in9bc.epdconfig.module_exit(cleanup=True)
    exit()
