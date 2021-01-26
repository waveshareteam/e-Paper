#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in7b_V2
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in7b_V2 Demo")
    
    epd = epd2in7b_V2.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    logging.info("Drawing")
    blackimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    redimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    
    # Drawing on the Horizontal image
    logging.info("1.Drawing on the Horizontal image...")
    HBlackimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126
    HRedimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126    
    drawblack = ImageDraw.Draw(HBlackimage)
    drawred = ImageDraw.Draw(HRedimage)
    drawblack.text((10, 0), 'hello world', font = font24, fill = 0)
    drawblack.text((10, 20), '2.7inch e-Paper', font = font24, fill = 0)
    drawblack.text((150, 0), u'微雪电子', font = font24, fill = 0)    
    drawblack.line((20, 50, 70, 100), fill = 0)
    drawblack.line((70, 50, 20, 100), fill = 0)
    drawblack.rectangle((20, 50, 70, 100), outline = 0)    
    drawred.line((165, 50, 165, 100), fill = 0)
    drawred.line((140, 75, 190, 75), fill = 0)
    drawred.arc((140, 50, 190, 100), 0, 360, fill = 0)
    drawred.rectangle((80, 50, 130, 100), fill = 0)
    drawred.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(20)
    
    # Drawing on the Vertical image
    logging.info("2.Drawing on the Vertical image...")
    LBlackimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    LRedimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    drawblack = ImageDraw.Draw(LBlackimage)
    drawred = ImageDraw.Draw(LRedimage)
    
    drawblack.text((2, 0), 'hello world', font = font18, fill = 0)
    drawblack.text((2, 20), '2.9inch epd', font = font18, fill = 0)
    drawblack.text((20, 50), u'微雪电子', font = font18, fill = 0)
    drawblack.line((10, 90, 60, 140), fill = 0)
    drawblack.line((60, 90, 10, 140), fill = 0)
    drawblack.rectangle((10, 90, 60, 140), outline = 0)
    drawred.line((95, 90, 95, 140), fill = 0)
    drawred.line((70, 115, 120, 115), fill = 0)
    drawred.arc((70, 90, 120, 140), 0, 360, fill = 0)
    drawred.rectangle((10, 150, 60, 200), fill = 0)
    drawred.chord((70, 150, 120, 200), 0, 360, fill = 0)
    epd.display(epd.getbuffer(LBlackimage), epd.getbuffer(LRedimage))
    time.sleep(2)
    
    logging.info("3.read bmp file")
    HBlackimage = Image.open(os.path.join(picdir, '2in7b-b.bmp'))
    HRedimage = Image.open(os.path.join(picdir, '2in7b-r.bmp'))
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(2)
    
    logging.info("4.read bmp file on window")
    blackimage1 = Image.new('1', (epd.height, epd.width), 255)  # 298*126
    redimage1 = Image.new('1', (epd.height, epd.width), 255)  # 298*126    
    newimage = Image.open(os.path.join(picdir, '100x100.bmp'))
    blackimage1.paste(newimage, (50,10))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage1))
    
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
    epd2in7b_V2.epdconfig.module_exit()
    exit()
