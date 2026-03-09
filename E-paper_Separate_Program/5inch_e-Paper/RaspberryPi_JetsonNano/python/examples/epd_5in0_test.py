#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd5in0
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd5in0e Demo")

    epd = epd5in0.EPD()   
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)

    # read bmp file 
    logging.info("read bmp file")
    Himage = Image.open(os.path.join(picdir, '1.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    Himage = Image.open(os.path.join(picdir, '100x100.bmp'))
    screen_image = Image.new('1', (epd.width, epd.height), 255)
    # offset_x = (epd.width - Himage.width) // 2
    # offset_y = (epd.height - Himage.height) // 2
    # screen_image.paste(Himage, (offset_x, offset_y)) #center
    screen_image.paste(Himage, (100, 100))
    epd.display(epd.getbuffer(screen_image))
    time.sleep(3)

    # Drawing on the Horizontal image
    logging.info("Drawing on the Horizontal image...")
    Himage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(Himage)
    draw.text((10, 0), 'hello world', font = font24, fill = 0)
    draw.text((10, 20), '5inch e-Paper', font = font24, fill = 0)
    draw.text((150, 0), u'微雪电子', font = font24, fill = 0)    
    draw.line((20, 50, 70, 100), fill = 0)
    draw.line((70, 50, 20, 100), fill = 0)
    draw.rectangle((20, 50, 70, 100), outline = 0)
    draw.line((165, 50, 165, 100), fill = 0)
    draw.line((140, 75, 190, 75), fill = 0)
    draw.arc((140, 50, 190, 100), 0, 360, fill = 0)
    draw.rectangle((80, 50, 130, 100), fill = 0)
    draw.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display(epd.getbuffer(Himage))
    time.sleep(2)

    
    logging.info("show time")
    num = 0
    while (True):
        draw.rectangle((10, 120, 130, 170), fill = 255)
        draw.text((10, 120), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        epd.display_Partial(epd.getbuffer(Himage))
        num = num + 1
        if(num == 10):
            break
    time.sleep(2)
    
    logging.info("Clear...")
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd5in0.epdconfig.module_exit(cleanup=True)
    exit()
