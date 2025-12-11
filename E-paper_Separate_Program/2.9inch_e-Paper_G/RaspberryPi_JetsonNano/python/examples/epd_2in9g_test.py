#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in9g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in9g Demo")

    epd = epd2in9g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    
    font12 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 12)
    font16 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 16)
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)

    # read bmp file 
    epd.init_fast()
    logging.info("read bmp file")
    Himage = Image.open(os.path.join(picdir, '2in9g.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    # Drawing on the image
    logging.info("Drawing on the image...")
    epd.init()
    Himage = Image.new('RGB', (epd.height, epd.width), epd.WHITE)  
    draw = ImageDraw.Draw(Himage)
    draw.point((10, 80), fill=epd.RED)
    draw.rectangle([(10, 90), (13, 93)], fill=epd.YELLOW)
    draw.rectangle([(10, 100), (15, 105)], fill=epd.BLACK)
    draw.line([(20, 70), (70, 120)], fill=epd.RED, width=1)
    draw.line([(70, 70), (20, 120)], fill=epd.RED, width=1)
    draw.rectangle([(20, 70), (70, 120)], outline=epd.YELLOW, width=1)
    draw.rectangle([(80, 70), (130, 120)], fill=epd.YELLOW)
    draw.ellipse([(45-20, 95-20), (45+20, 95+20)], outline=epd.BLACK, width=1)
    draw.ellipse([(105-20, 95-20), (105+20, 95+20)], fill=epd.BLACK) 
    draw.line([(85, 95), (125, 95)], fill=epd.RED, width=1)
    draw.line([(105, 75), (105, 115)], fill=epd.YELLOW, width=1)
    draw.text((10, 0), 'Red,yellow,white and black', font=font16, fill=epd.RED)
    draw.text((10, 20), 'Four color e-Paper', font=font12, fill=epd.YELLOW)
    draw.text((150, 20), u'微雪电子', font=font16, fill=epd.BLACK)
    draw.text((10, 35), '123456', font=font12, fill=epd.RED)

    for x in range(5, 250, 10):  
        draw.point((x, 50), fill=epd.RED)
    draw.rounded_rectangle([(5, 55), (220, 65)], radius=5, fill=epd.RED, outline=epd.YELLOW, width=1)
    for i in range(8):
        draw.ellipse([(150+i*8, 80), (155+i*8, 85)], fill=epd.YELLOW)
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
    epd2in9g.epdconfig.module_exit(cleanup=True)
    exit()
