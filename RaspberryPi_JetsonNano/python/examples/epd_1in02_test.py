#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd1in02
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd1in02 Demo")
    
    epd = epd1in02.EPD()
    logging.info("init and Clear")
    epd.Init()
    epd.Clear()
    
    # Drawing on the image
    logging.info("1.Drawing on the image...")
    image = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    
    draw.text((5, 0), 'hello world', font = font, fill = 0)
    draw.text((15, 40), u'微雪电子', font = font, fill = 0)
    epd.Display(epd.getbuffer(image))
    time.sleep(2)
    
    image = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(image)
    draw.rectangle((20, 0, 100, 80), fill = 0)
    draw.rectangle((22, 2, 98, 78), fill = 255)
    draw.chord((22, 2, 98, 78), 0, 360, fill = 0)
    draw.chord((24, 4, 96, 76), 0, 360, fill = 255)
    draw.line((20, 0, 100, 80), fill = 0)
    draw.line((20, 80, 100, 0), fill = 0)
    epd.display(epd.getbuffer(image))
    time.sleep(2)

    # read bmp file 
    logging.info("2.read bmp file...")
    image = Image.open(os.path.join(picdir, '1in02.bmp'))
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file on window
    logging.info("3.read bmp file on window...")
    image1 = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    bmp = Image.open(os.path.join(picdir, '100x100.bmp'))
    image1.paste(bmp, (0,0))    
    epd.display(epd.getbuffer(image1))
    time.sleep(2)
    
    # # partial update
    logging.info("4.show time...")
    epd.Clear()
    epd.Partial_Init()    
    
    
    time_image = Image.new('1', (epd.height, epd.width), 255)
    time_draw = ImageDraw.Draw(time_image)
    
    image_old = epd.getbuffer(time_image)
    num = 0
    while (True):
        time_draw.rectangle((10, 10, 120, 50), fill = 255)
        time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font, fill = 0)
        newimage = time_image.crop([10, 10, 120, 50])
        time_image.paste(newimage, (10,10))
        epd.DisplayPartial(image_old, epd.getbuffer(time_image))
        image_old = epd.getbuffer(time_image)
        num = num + 1
        if(num == 10):
            break
    
    logging.info("Clear...")
    epd.Init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.Sleep()
        
except IOError as e:
    logging.info(e)
    
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd1in54.epdconfig.module_exit()
    exit()
