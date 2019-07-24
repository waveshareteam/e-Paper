#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in13bc
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in13bc Demo")
    
    epd = epd2in13bc.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    logging.info("Drawing")    
    font20 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 20)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    
    # Drawing on the Horizontal image
    logging.info("1.Drawing on the Horizontal image...") 
    HBlackimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126
    HRYimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126  ryimage: red or yellow image  
    drawblack = ImageDraw.Draw(HBlackimage)
    drawry = ImageDraw.Draw(HRYimage)
    drawblack.text((10, 0), 'hello world', font = font20, fill = 0)
    drawblack.text((10, 20), '2.13inch e-Paper bc', font = font20, fill = 0)
    drawblack.text((120, 0), u'微雪电子', font = font20, fill = 0)    
    drawblack.line((20, 50, 70, 100), fill = 0)
    drawblack.line((70, 50, 20, 100), fill = 0)
    drawblack.rectangle((20, 50, 70, 100), outline = 0)    
    drawry.line((165, 50, 165, 100), fill = 0)
    drawry.line((140, 75, 190, 75), fill = 0)
    drawry.arc((140, 50, 190, 100), 0, 360, fill = 0)
    drawry.rectangle((80, 50, 130, 100), fill = 0)
    drawry.chord((85, 55, 125, 95), 0, 360, fill =1)
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRYimage))
    time.sleep(2)
    
    # Drawing on the Vertical image
    logging.info("2.Drawing on the Vertical image...")
    LBlackimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    LRYimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    drawblack = ImageDraw.Draw(LBlackimage)
    drawry = ImageDraw.Draw(LRYimage)
    
    drawblack.text((2, 0), 'hello world', font = font18, fill = 0)
    drawblack.text((2, 20), '2.13 epd b', font = font18, fill = 0)
    drawblack.text((20, 50), u'微雪电子', font = font18, fill = 0)
    drawblack.line((10, 90, 60, 140), fill = 0)
    drawblack.line((60, 90, 10, 140), fill = 0)
    drawblack.rectangle((10, 90, 60, 140), outline = 0)
    drawry.rectangle((10, 150, 60, 200), fill = 0)
    drawry.arc((15, 95, 55, 135), 0, 360, fill = 0)
    drawry.chord((15, 155, 55, 195), 0, 360, fill =1)
    epd.display(epd.getbuffer(LBlackimage), epd.getbuffer(LRYimage))
    time.sleep(2)
    
    logging.info("3.read bmp file")
    HBlackimage = Image.open(os.path.join(picdir, '2in13bc-b.bmp'))
    HRYimage = Image.open(os.path.join(picdir, '2in13bc-ry.bmp'))
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRYimage))
    time.sleep(2)
    
    logging.info("4.read bmp file on window")
    blackimage1 = Image.new('1', (epd.height, epd.width), 255)  # 298*126
    redimage1 = Image.new('1', (epd.height, epd.width), 255)  # 298*126    
    newimage = Image.open(os.path.join(picdir, '100x100.bmp'))
    blackimage1.paste(newimage, (10,10))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage1))
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd2in13bc.epdconfig.module_exit()
    exit()
