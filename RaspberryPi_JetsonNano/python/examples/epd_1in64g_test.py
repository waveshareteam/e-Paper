#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd1in64g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd1in64g Demo")

    epd = epd1in64g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)
    
    # # The Sunrise X3 PI is a bit problematic to use
    # # Drawing on the image
    # logging.info("1.Drawing on the image...")
    # Himage = Image.new('RGB', (epd.width, epd.height), epd.WHITE)  
    # draw = ImageDraw.Draw(Himage)
    # draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    # draw.text((5, 20), '1.64inch e-Paper', font = font18, fill = epd.YELLOW)
    # draw.text((5, 40), u'微雪电子', font = font40, fill = epd.BLACK)

    # draw.line((5, 90, 45, 160), fill = epd.RED)
    # draw.line((45, 90, 5, 160), fill = epd.YELLOW)
    # draw.rectangle((5, 90, 45, 160), outline = epd.BLACK)
    # draw.rectangle((55, 90, 95, 160), fill = epd.BLACK)
    # draw.arc((115, 90, 150, 125), 0, 360, fill = epd.BLACK)
    # draw.chord((115, 130, 150, 165), 0, 360, fill = epd.BLACK)
    # epd.display(epd.getbuffer(Himage))
    # time.sleep(3)
    
    # # read bmp file 
    # logging.info("2.read bmp file")
    # Himage = Image.open(os.path.join(picdir, '1.64inch-1.bmp'))
    # epd.display(epd.getbuffer(Himage))
    # time.sleep(3)

    logging.info("3.read bmp file")
    Himage = Image.open(os.path.join(picdir, '1.64inch-2.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    logging.info("Clear...")
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd1in64g.epdconfig.module_exit()
    exit()
