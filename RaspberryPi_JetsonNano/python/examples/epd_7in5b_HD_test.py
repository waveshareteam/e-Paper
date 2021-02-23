#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd7in5b_HD
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

logging.basicConfig(level=logging.DEBUG)

try:
    logging.info("epd7in5b_HD Demo")

    epd = epd7in5b_HD.EPD()

    logging.info("init and Clear")
    epd.init()
    epd.Clear()

    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)

    # Drawing on the Horizontal image
    logging.info("1.Drawing on the Horizontal image...")
    Himage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    Other = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    draw_Himage = ImageDraw.Draw(Himage)
    draw_other = ImageDraw.Draw(Other)
    draw_Himage.text((10, 0), 'hello world', font = font24, fill = 0)
    draw_Himage.text((10, 20), '7.5inch e-Paper', font = font24, fill = 0)
    draw_Himage.text((150, 0), u'微雪电子', font = font24, fill = 0)    
    draw_other.line((20, 50, 70, 100), fill = 0)
    draw_other.line((70, 50, 20, 100), fill = 0)
    draw_other.rectangle((20, 50, 70, 100), outline = 0)
    draw_other.line((165, 50, 165, 100), fill = 0)
    draw_Himage.line((140, 75, 190, 75), fill = 0)
    draw_Himage.arc((140, 50, 190, 100), 0, 360, fill = 0)
    draw_Himage.rectangle((80, 50, 130, 100), fill = 0)
    draw_Himage.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display(epd.getbuffer(Himage),epd.getbuffer(Other))
    time.sleep(2)

    # Drawing on the Vertical image
    logging.info("2.Drawing on the Vertical image...")
    Limage = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    Limage_Other = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    draw_Himage = ImageDraw.Draw(Limage)
    draw_Himage_Other = ImageDraw.Draw(Limage_Other)
    draw_Himage.text((2, 0), 'hello world', font = font18, fill = 0)
    draw_Himage.text((2, 20), '7.5inch epd', font = font18, fill = 0)
    draw_Himage_Other.text((20, 50), u'微雪电子', font = font18, fill = 0)
    draw_Himage_Other.line((10, 90, 60, 140), fill = 0)
    draw_Himage_Other.line((60, 90, 10, 140), fill = 0)
    draw_Himage_Other.rectangle((10, 90, 60, 140), outline = 0)
    draw_Himage_Other.line((95, 90, 95, 140), fill = 0)
    draw_Himage.line((70, 115, 120, 115), fill = 0)
    draw_Himage.arc((70, 90, 120, 140), 0, 360, fill = 0)
    draw_Himage.rectangle((10, 150, 60, 200), fill = 0)
    draw_Himage.chord((70, 150, 120, 200), 0, 360, fill = 0)
    epd.display(epd.getbuffer(Limage), epd.getbuffer(Limage_Other))
    time.sleep(2)
    
    logging.info("3.read bmp file...")
    blackimage = Image.open(os.path.join(picdir, '7in5_HD_b.bmp'))
    redimage = Image.open(os.path.join(picdir, '7in5_HD_r.bmp'))    
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)

    logging.info("4.read bmp file on window")
    Himage2 = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    Himage2_Other = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    bmp = Image.open(os.path.join(picdir, '100x100.bmp'))
    Himage2.paste(bmp, (50,10))
    Himage2_Other.paste(bmp, (50,300))
    epd.display(epd.getbuffer(Himage2), epd.getbuffer(Himage2_Other))
    time.sleep(2)

    logging.info("Clear...")
    epd.init()
    epd.Clear()

    logging.info("Goto Sleep...")
    epd.sleep()
    
except IOError as e:
    logging.info(e)
    
except KeyboardInterrupt:    
    logging.info("ctrl + c:")
    epd7in5b_HD.epdconfig.module_exit()
    exit()
