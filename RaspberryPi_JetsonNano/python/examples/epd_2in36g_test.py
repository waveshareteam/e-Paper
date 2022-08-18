#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in36g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in36g Demo")

    epd = epd2in36g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)

    # Drawing on the image
    logging.info("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.width, epd.height), epd.WHITE)  
    draw = ImageDraw.Draw(Himage)
    draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    draw.text((5, 20), '2.36inch e-Paper', font = font18, fill = epd.YELLOW)
    draw.text((5, 40), u'微雪电子', font = font40, fill = epd.BLACK)
    draw.text((5, 85), u'微雪电子', font = font40, fill = epd.YELLOW)
    draw.text((5, 125), u'微雪电子', font = font40, fill = epd.RED)

    draw.line((5, 175, 45, 245), fill = epd.RED)
    draw.line((45, 175, 5, 245), fill = epd.YELLOW)
    draw.rectangle((5, 175, 45, 245), outline = epd.BLACK)
    draw.rectangle((55, 175, 95, 245), fill = epd.BLACK)
    draw.arc((115, 175, 150, 210), 0, 360, fill = epd.BLACK)
    draw.chord((115, 215, 150, 250), 0, 360, fill = epd.BLACK)
    draw.chord((10, 252, 50, 295), 0, 360, fill = epd.YELLOW)
    draw.chord((110, 252, 150, 295), 0, 360, fill = epd.RED)
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    logging.info("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '2.36inch-1.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    logging.info("3.read bmp file")
    Himage = Image.open(os.path.join(picdir, '2.36inch-2.bmp'))
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
    epd2in36g.epdconfig.module_exit()
    exit()
