#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in13d
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

#Set output log level
logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in13d Demo")
    
    epd = epd2in13d.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    
    font15 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 15)
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    # Drawing on the Horizontal image
    logging.info("1.Drawing on the Horizontal image...")
    Himage = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(Himage)
    draw.rectangle([(0,0),(50,50)],outline = 0)
    draw.rectangle([(55,0),(100,50)],fill = 0)
    draw.line([(0,0),(50,50)], fill = 0,width = 1)
    draw.line([(0,50),(50,0)], fill = 0,width = 1)
    draw.chord((10, 60, 50, 100), 0, 360, fill = 0)
    draw.ellipse((55, 60, 95, 100), outline = 0)
    draw.pieslice((55, 60, 95, 100), 90, 180, outline = 0)
    draw.pieslice((55, 60, 95, 100), 270, 360, fill = 0)
    draw.polygon([(110,0),(110,50),(150,25)],outline = 0)
    draw.polygon([(190,0),(190,50),(150,25)],fill = 0)
    draw.text((110, 60), 'e-Paper demo', font = font15, fill = 0)
    draw.text((110, 80), u'微雪电子', font = font15, fill = 0)
    epd.display(epd.getbuffer(Himage))
    time.sleep(2)
        
    logging.info("3.read bmp file")
    Himage = Image.open(os.path.join(picdir, '2in13d.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(2)
    
    logging.info("4.read bmp file on window")
    Himage2 = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    bmp = Image.open(os.path.join(picdir, '100x100.bmp'))
    Himage2.paste(bmp, (20,0))
    epd.display(epd.getbuffer(Himage2))
    time.sleep(2)
    
    # partial update
    logging.info("5.show time...")
    # epd.init()    
    # epd.Clear()
    # time_image = Image.new('1', (epd.width, epd.height), 255)
    # time_draw = ImageDraw.Draw(time_image)
    # num = 0
    # while (True):
        # time_draw.rectangle((10, 10, 120, 50), fill = 255)
        # time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        # newimage = time_image.crop([10, 10, 120, 50])
        # time_image.paste(newimage, (10,10))  
        # epd.DisplayPartial(epd.getbuffer(time_image))
        # num = num + 1
        # if(num == 10):
            # break
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd2in13d.epdconfig.module_exit()
    exit()
