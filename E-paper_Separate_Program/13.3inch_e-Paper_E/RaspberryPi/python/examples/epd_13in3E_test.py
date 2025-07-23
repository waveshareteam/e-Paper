#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)


import epd13in3E
import time

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
from PIL import ImageColor

from PIL import Image

import json

print("13.3inch e-paper (E) Demo...")

epd = epd13in3E.EPD()
try:
    epd.Init()
    print("clearing...")
    epd.Clear()

    image1 = Image.new("1", (epd13in3E.EPD_WIDTH, epd13in3E.EPD_HEIGHT), 255)
    draw = ImageDraw.Draw(image1)
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)
    
    # Drawing on the image
    print("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.width, epd.height), epd.WHITE)  # 255: clear the frame
    draw = ImageDraw.Draw(Himage)
    draw.text((5, 0), 'hello world', font = font18, fill = epd.RED)
    draw.text((5, 20), '13.3    inch e-Paper (e)', font = font24, fill = epd.YELLOW)
    draw.text((5, 45), u'微雪电子', font = font40, fill = epd.GREEN)
    draw.text((5, 85), u'微雪电子', font = font40, fill = epd.BLUE)
    draw.text((5, 125), u'微雪电子', font = font40, fill = epd.BLACK)

    draw.line((5, 170, 80, 245), fill = epd.BLUE)
    draw.line((80, 170, 5, 245), fill = epd.YELLOW)
    draw.rectangle((5, 170, 80, 245), outline = epd.BLACK)
    draw.rectangle((90, 170, 165, 245), fill = epd.GREEN)
    draw.arc((5, 250, 80, 325), 0, 360, fill = epd.RED)
    draw.chord((90, 250, 165, 325), 0, 360, fill = epd.YELLOW)
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    # read bmp file 
    print("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '13in3E.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    print("clearing...")
    epd.Clear()

    print("goto sleep...")
    epd.sleep()
except:
    print("goto sleep...")
    epd.sleep()


