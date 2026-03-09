#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd3in7g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd3in7g Demo")

    epd = epd3in7g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font15 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 15)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)

    # Drawing on the image
    logging.info("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.height, epd.width), epd.WHITE)  
    draw = ImageDraw.Draw(Himage)
    draw.rectangle([(0,0),(50,50)],outline = epd.BLACK)
    draw.rectangle([(55,0),(100,50)],fill = epd.RED)
    draw.line([(0,0),(50,50)], fill = epd.YELLOW,width = 1)
    draw.line([(0,50),(50,0)], fill = epd.YELLOW,width = 1)
    draw.pieslice((55, 60, 95, 100), 90, 180, outline = epd.RED)
    draw.pieslice((55, 60, 95, 100), 270, 360, fill = epd.BLACK)
    draw.chord((10, 60, 50, 100), 0, 360, fill = epd.YELLOW)
    draw.ellipse((55, 60, 95, 100), outline = epd.RED)
    draw.polygon([(110,0),(110,50),(150,25)],outline = epd.BLACK)
    draw.polygon([(190,0),(190,50),(150,25)],fill = epd.BLACK)
    draw.text((120, 60), 'e-Paper demo', font = font15, fill = epd.YELLOW)
    draw.text((110, 90), u'微雪电子', font = font24, fill = epd.RED)

    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    logging.info("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '3.7inch-G.bmp'))
    epd.init_Fast()
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd3in7g.epdconfig.module_exit(cleanup=True)
    exit()
