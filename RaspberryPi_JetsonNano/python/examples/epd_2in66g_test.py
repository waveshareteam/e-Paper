#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in66g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in9g Demo")

    epd = epd2in66g.EPD()  
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
    draw.text((5, 20), '2.66inch G', font = font18, fill = epd.YELLOW)
    draw.text((5, 50), u'微雪电子', font = font40, fill = epd.BLACK)
    draw.text((5, 100), u'微雪电子', font = font40, fill = epd.YELLOW)
    draw.text((5, 150), u'微雪电子', font = font40, fill = epd.RED)

    draw.rectangle((5, 200, 75, 270), outline = epd.BLACK)
    draw.line((5, 200, 75, 270), fill = epd.RED)
    draw.line((75, 200, 5, 270), fill = epd.YELLOW)
    draw.rectangle((90, 200, 160, 270), fill = epd.BLACK)

    draw.arc((5, 300, 45, 340), 0, 360, fill = epd.BLACK)
    draw.chord((45, 300, 85, 340), 0, 360, fill = epd.BLACK)
    draw.chord((85, 300, 125, 340), 0, 360, fill = epd.YELLOW)
    draw.chord((125, 300, 165, 340), 0, 360, fill = epd.RED)
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    logging.info("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '2in66g.bmp'))
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
    epd2in66g.epdconfig.module_exit(cleanup=True)
    exit()
