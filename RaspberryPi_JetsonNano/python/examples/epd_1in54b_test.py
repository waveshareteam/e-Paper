#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd1in54b
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd1in54b Demo")
    
    epd = epd1in54b.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    logging.info("1.Drawing on the image...")
    blackimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    redimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    
    font = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    
    drawblack = ImageDraw.Draw(blackimage)
    drawred = ImageDraw.Draw(redimage)
    drawblack.rectangle((0, 10, 200, 34), fill = 0)
    drawblack.text((8, 12), 'hello world', font = font, fill = 255)
    drawblack.text((8, 36), u'微雪电子', font = font18, fill = 0)
    drawblack.line((16, 60, 56, 60), fill = 0)
    drawblack.line((56, 60, 56, 110), fill = 0)
    drawblack.line((16, 110, 56, 110), fill = 0)
    drawred.line((16, 110, 16, 60), fill = 0)
    drawred.line((16, 60, 56, 110), fill = 0)
    drawred.line((56, 60, 16, 110), fill = 0)
    drawred.arc((90, 60, 150, 120), 0, 360, fill = 0)
    drawred.rectangle((16, 130, 56, 180), fill = 0)
    drawred.chord((90, 130, 150, 190), 0, 360, fill = 0)
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)
    
    # read bmp file 
    logging.info("2.read bmp file...")
    blackimage = Image.open(os.path.join(picdir, '1in54b-b.bmp'))
    redimage = Image.open(os.path.join(picdir, '1in54b-r.bmp'))    
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)
    
    # read bmp file on window
    logging.info("3.read bmp file on window...")
    blackimage1 = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    redimage2 = Image.new('1', (epd.width, epd.height), 255)
    
    newimage = Image.open(os.path.join(picdir, '100x100.bmp'))
    blackimage1.paste(newimage, (50,50))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage2))
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
    time.sleep(3)
    
    epd.Dev_exit()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd1in54b.epdconfig.module_exit()
    exit()
