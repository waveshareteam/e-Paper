#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd1in54c
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd1in54c Demo")
    
    epd = epd1in54c.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    logging.info("1.Drawing on the image...")
    blackimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    yellowimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    
    font = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    
    drawblack = ImageDraw.Draw(blackimage)
    drawyellow = ImageDraw.Draw(yellowimage)
    drawblack.rectangle((0, 10, 200, 34), fill = 0)
    drawblack.text((8, 12), 'hello world', font = font, fill = 255)
    drawblack.text((8, 36), u'微雪电子', font = font, fill = 0)
    drawblack.line((10, 100, 70, 100), fill = 0)
    drawblack.line((40, 70, 40, 130), fill = 0)
    
    drawyellow.rectangle((10, 70, 70, 130), outline = 0)    
    drawyellow.arc((80, 70, 140, 130), 0, 360, fill = 0)
    drawyellow.chord((90, 80, 130, 120), 0, 360, fill = 0)
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(yellowimage))
    time.sleep(1)
    
    # read bmp file 
    logging.info("2.read bmp file...")
    blackimage = Image.open(os.path.join(picdir, '1in54c-b.bmp'))
    yellowimage = Image.open(os.path.join(picdir, '1in54c-y.bmp'))    
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(yellowimage))
    time.sleep(1)
    
    # read bmp file on window
    logging.info("3.read bmp file on window...")
    blackimage1 = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    yellowimage2 = Image.new('1', (epd.width, epd.height), 255)
    
    newimage = Image.open(os.path.join(picdir, '100x100.bmp'))
    blackimage1.paste(newimage, (50,50))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(yellowimage2))
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd1in54c.epdconfig.module_exit()
    exit()
