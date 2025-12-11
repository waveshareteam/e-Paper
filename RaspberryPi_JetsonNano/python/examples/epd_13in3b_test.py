#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd13in3b
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd13in3k Demo")
    epd = epd13in3b.EPD()
    
    logging.info("init and Clear")
    epd.init()
    epd.Clear()

    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font35 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 35)

    logging.info("2.read bmp file")
    HBlackimage = Image.open(os.path.join(picdir, '13in3b_r.bmp')) 
    HRedimage = Image.open(os.path.join(picdir, '13in3b_b.bmp'))
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(2)

    # Drawing on the Horizontal image
    logging.info("3.Drawing on the Horizontal image...")
    HBlackimage = Image.new('1', (epd.width, epd.height), 255)  
    HRedimage = Image.new('1', (epd.width, epd.height), 255) 
    drawblack = ImageDraw.Draw(HBlackimage)
    drawred = ImageDraw.Draw(HRedimage)
    drawblack.text((10, 0), 'hello world', font = font24, fill = 0)
    drawred.text((10, 20), '13.3inch e-Paper (B)', font = font24, fill = 0)
    drawblack.text((150, 0), u'微雪电子', font = font24, fill = 0)
    drawred.line((20, 50, 70, 100), fill = 0)
    drawblack.line((70, 50, 20, 100), fill = 0)
    drawred.rectangle((20, 50, 70, 100), outline = 0)
    drawblack.line((165, 50, 165, 100), fill = 0)
    drawred.line((140, 75, 190, 75), fill = 0)
    drawblack.arc((140, 50, 190, 100), 0, 360, fill = 0)
    drawred.rectangle((80, 50, 130, 100), fill = 0)
    drawblack.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display_Base(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(2)

    # partial update
    logging.info("4.show time")
    '''
    # If you didn't use the epd.display_Base() function to refresh the image before,
    # use the epd.Clear_Base() function to refresh the background color, 
    # otherwise the background color will be garbled 
    '''
    # epd.Clear_Base()
    # Himage = Image.new('1', (epd.height ,epd.width), 0xff)
    # draw = ImageDraw.Draw(time_image)
    num = 0
    while (True):
        drawblack.rectangle((0, 110, 120, 150), fill = 255)
        drawblack.text((10, 120), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        epd.display_Partial(epd.getbuffer(HBlackimage),0, 110, 120, 160)
        num = num + 1
        if(num == 10):
            break

    logging.info("Clear...")
    epd.init()
    epd.Clear()

    logging.info("Goto Sleep...")
    epd.sleep()
    
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd13in3b.epdconfig.module_exit(cleanup=True)
    exit()
