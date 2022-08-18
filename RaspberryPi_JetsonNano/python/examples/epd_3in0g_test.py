#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd3in0g
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd3in0g Demo")

    epd = epd3in0g.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)
    
    # Drawing on the image
    epd.init()
    logging.info("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.width, epd.height), 0xffffff)  
    draw = ImageDraw.Draw(Himage)
    draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    draw.text((5, 20), '3inch e-Paper', font = font24, fill = epd.YELLOW)
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

    # Switch width and height for landscape display
    epd.init()
    logging.info("2.Drawing on the image...")
    Himage = Image.new('RGB', (epd.height, epd.width), epd.WHITE)      
    draw = ImageDraw.Draw(Himage)
    draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    draw.text((5, 20), '3inch e-Paper', font = font24, fill = epd.YELLOW)
    draw.text((5, 45), u'微雪电子', font = font40, fill = epd.BLACK)
    draw.text((5, 85), u'微雪电子', font = font40, fill = epd.YELLOW)
    draw.text((5, 125), u'微雪电子', font = font40, fill = epd.RED)
    draw.line((205, 5, 295, 65), fill = epd.RED)
    draw.line((295, 5, 205, 65), fill = epd.YELLOW)
    draw.rectangle((205, 5, 295, 65), outline = epd.BLACK)
    draw.rectangle((305, 5, 395, 65), fill = epd.RED)
    draw.arc((205, 75, 295, 165), 0, 360, fill = epd.BLACK)
    draw.chord((305, 75, 395, 165), 0, 360, fill = epd.YELLOW)
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    epd.init()
    logging.info("3.read bmp file")
    Himage = Image.open(os.path.join(picdir, '3inch-1.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    epd.init()
    logging.info("4.read bmp file")
    Himage = Image.open(os.path.join(picdir, '3inch-2.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    epd.init()
    logging.info("5.read bmp file")
    Himage = Image.open(os.path.join(picdir, '3inch-3.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    epd.init()
    logging.info("Clear...")
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd3in0g.epdconfig.module_exit()
    exit()
