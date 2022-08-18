#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd7in3g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd7in3g Demo")

    epd = epd7in3g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)
    
    # Drawing on the image
    logging.info("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.width, epd.height), epd.WHITE)  # 255: clear the frame
    draw = ImageDraw.Draw(Himage)
    draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    draw.text((5, 20), '7.3inch e-Paper', font = font24, fill = epd.YELLOW)
    draw.text((5, 45), u'微雪电子', font = font40, fill = epd.BLACK)
    draw.text((5, 85), u'微雪电子', font = font40, fill = epd.YELLOW)
    draw.text((5, 125), u'微雪电子', font = font40, fill = epd.RED)

    draw.line((5, 170, 80, 245), fill = epd.RED)
    draw.line((80, 170, 5, 245), fill = epd.YELLOW)
    draw.rectangle((5, 170, 80, 245), outline = epd.BLACK)
    draw.rectangle((90, 170, 165, 245), fill = epd.YELLOW)
    draw.arc((5, 250, 80, 325), 0, 360, fill = epd.BLACK)
    draw.chord((90, 250, 165, 325), 0, 360, fill = epd.RED)
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    logging.info("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '7.3inch-1.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    logging.info("3.read bmp file")
    Himage = Image.open(os.path.join(picdir, '7.3inch-2.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    logging.info("4.read bmp file")
    Himage = Image.open(os.path.join(picdir, '7.3inch-3.bmp'))
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
    epd7in3g.epdconfig.module_exit()
    exit()
