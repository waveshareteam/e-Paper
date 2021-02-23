#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in66
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in66 Demo")
    
    epd = epd2in66.EPD()
    logging.info("init and Clear")
    epd.init(0)
    epd.Clear()
    
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    
    # Drawing on the Horizontal image
    logging.info("1.Drawing on the Horizontal image...")
    Himage = Image.new('1', (epd.height, epd.width), 0xFF)  # 0xFF: clear the frame
    draw = ImageDraw.Draw(Himage)
    draw.text((10, 0), 'hello world', font = font24, fill = 0)
    draw.text((10, 20), '2.66inch e-Paper', font = font24, fill = 0)
    draw.text((10, 100), u'微雪电子', font = font24, fill = 0)
    draw.line((20, 50, 70, 100), fill = 0)
    draw.line((70, 50, 20, 100), fill = 0)
    draw.rectangle((20, 50, 70, 100), outline = 0)
    draw.line((165, 50, 165, 100), fill = 0)
    draw.line((140, 75, 190, 75), fill = 0)
    draw.arc((140, 50, 190, 100), 0, 360, fill = 0)
    draw.rectangle((80, 50, 130, 100), fill = 0)
    draw.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display(epd.getbuffer(Himage))
    time.sleep(5)
    
    logging.info("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '2.66inch-9.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(5)
    
    logging.info("3.read bmp file on window")
    Himage2 = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    bmp = Image.open(os.path.join(picdir, '100x100.bmp'))
    Himage2.paste(bmp, (50,20))
    epd.display(epd.getbuffer(Himage2))
    time.sleep(5)
    
    # Drawing on the Vertical image
    logging.info("4.Drawing on the Vertical image...")
    Limage = Image.new('1', (epd.width, epd.height), 0xFF)  # 0xFF: clear the frame
    draw = ImageDraw.Draw(Limage)
    draw.text((2, 0), 'hello world', font = font18, fill = 0)
    draw.text((2, 20), '2.66inch epd', font = font18, fill = 0)
    draw.text((10, 40), u'微雪电子', font = font24, fill = 0)
    draw.line((10, 90, 60, 140), fill = 0)
    draw.line((60, 90, 10, 140), fill = 0)
    draw.rectangle((10, 90, 60, 140), outline = 0)
    draw.line((95, 90, 95, 140), fill = 0)
    draw.line((70, 115, 120, 115), fill = 0)
    draw.arc((70, 90, 120, 140), 0, 360, fill = 0)
    draw.rectangle((10, 150, 60, 200), fill = 0)
    draw.chord((70, 150, 120, 200), 0, 360, fill = 0)
    epd.display(epd.getbuffer(Limage))
    time.sleep(5)

    # partial update, mode 1
    logging.info("5.show time, partial update, just mode 1")
    epd.init(1)         # partial mode
    epd.Clear()
    time_draw = ImageDraw.Draw(Limage)
    num = 0
    while (True):
        time_draw.rectangle((10, 210, 120, 250), fill = 255)
        time_draw.text((10, 210), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        epd.display(epd.getbuffer(Limage))
        
        num = num + 1
        if(num == 10):
            break
            
    logging.info("Clear...")
    epd.init(0)
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
    
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd2in66.epdconfig.module_exit()
    exit()
