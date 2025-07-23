#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd2in9b_V4
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd2in9b V4 Demo")
    
    epd = epd2in9b_V4.EPD()
    logging.info("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    logging.info("Drawing")    
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)

    logging.info("read bmp file")
    HBlackimage = Image.open(os.path.join(picdir, '2in9bc-b.bmp'))
    HRYimage = Image.open(os.path.join(picdir, '2in9bc-ry.bmp'))
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRYimage))
    time.sleep(2)
    
    # Drawing on the Horizontal image
    logging.info("Drawing on the Horizontal image...") 
    epd.init_Fast()
    HBlackimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126
    HRYimage = Image.new('1', (epd.height, epd.width), 255)  # 298*126  ryimage: red or yellow image  
    drawblack = ImageDraw.Draw(HBlackimage)
    drawry = ImageDraw.Draw(HRYimage)
    drawblack.text((10, 0), 'hello world', font = font24, fill = 0)
    drawblack.text((10, 20), '2.9inch e-Paper b V4', font = font24, fill = 0)
    drawblack.text((150, 0), u'微雪电子', font = font24, fill = 0)    
    drawblack.line((20, 50, 70, 100), fill = 0)
    drawblack.line((70, 50, 20, 100), fill = 0)
    drawblack.rectangle((20, 50, 70, 100), outline = 0)    
    drawry.line((165, 50, 165, 100), fill = 0)
    drawry.line((140, 75, 190, 75), fill = 0)
    drawry.arc((140, 50, 190, 100), 0, 360, fill = 0)
    drawry.rectangle((80, 50, 130, 100), fill = 0)
    drawry.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display_Fast(epd.getbuffer(HBlackimage), epd.getbuffer(HRYimage))
    time.sleep(2)

    '''
    # If you didn't use the display_Base() function to refresh the image before,
    # use the display_Base_color() function to refresh the background color, 
    # otherwise the background color will be garbled 
    # epd.init()
    # epd.display_Base(epd.getbuffer(HBlackimage), epd.getbuffer(HRYimage))
    # epd.display_Base_color(0xFF)
    '''
    # partial update
    logging.info("5.show time")
    epd.init()
    epd.display_Base_color(0xFF)
    HBlackimage = Image.new('1', (epd.height, epd.width), 255)
    drawblack = ImageDraw.Draw(HBlackimage)
    num = 0
    while (True):
        drawblack.rectangle((10, 10, 120, 50), fill = 255)
        drawblack.text((10, 10), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        newimage = HBlackimage.crop([10, 10, 120, 50])
        HBlackimage.paste(newimage, (10,10))  
        epd.display_Partial(epd.getbuffer(HBlackimage),10, epd.height - 120, 50, epd.height - 10)
        num = num + 1
        if(num == 10):
            break
    
    # # Drawing on the Vertical image
    # logging.info("Drawing on the Vertical image...")
    # LBlackimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    # LRYimage = Image.new('1', (epd.width, epd.height), 255)  # 126*298
    # drawblack = ImageDraw.Draw(LBlackimage)
    # drawry = ImageDraw.Draw(LRYimage)
    
    # drawblack.text((2, 0), 'hello world', font = font18, fill = 0)
    # drawblack.text((2, 20), '2.9inch epd b V4', font = font18, fill = 0)
    # drawblack.text((20, 50), u'微雪电子', font = font18, fill = 0)
    # drawblack.line((10, 90, 60, 140), fill = 0)
    # drawblack.line((60, 90, 10, 140), fill = 0)
    # drawblack.rectangle((10, 90, 60, 140), outline = 0)
    # drawry.line((95, 90, 95, 140), fill = 0)
    # drawry.line((70, 115, 120, 115), fill = 0)
    # drawry.arc((70, 90, 120, 140), 0, 360, fill = 0)
    # drawry.rectangle((10, 150, 60, 200), fill = 0)
    # drawry.chord((70, 150, 120, 200), 0, 360, fill = 0)
    # epd.display(epd.getbuffer(LBlackimage), epd.getbuffer(LRYimage))
    # time.sleep(2)
    
    logging.info("Clear...")
    epd.init()
    epd.Clear()
    
    logging.info("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd2in9b_V4.epdconfig.module_exit(cleanup=True)
    exit()
