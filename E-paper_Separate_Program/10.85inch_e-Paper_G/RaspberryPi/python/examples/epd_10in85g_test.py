#!/usr/bin/python
# -*- coding:utf-8 -*-

import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)


import epd10in85g
import time

from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
from PIL import ImageColor

from PIL import Image

import json

print("10.85inch e-paper (G) Demo...")

epd = epd10in85g.EPD()
try:
    print("clearing...")
    epd.Init()
    epd.Clear()

    image1 = Image.new("1", (epd10in85g.EPD_WIDTH*2, epd10in85g.EPD_HEIGHT), 255)
    draw = ImageDraw.Draw(image1)
    font24 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)
    font18 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font15 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 15)
    font40 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 40)
    
    # Drawing on the image
    print("1.Drawing on the image...")
    Himage = Image.new('RGB', (epd.height, epd.width*2), epd.WHITE)  
    draw = ImageDraw.Draw(Himage)
    draw.rectangle([(0,0),(50,50)],outline = epd.BLACK)
    draw.rectangle([(55,0),(100,50)],fill = epd.RED)
    draw.line([(0,0),(50,50)], fill = epd.YELLOW,width = 1)
    draw.line([(0,50),(50,0)], fill = epd.YELLOW,width = 1)
    draw.pieslice((55, 60, 95, 100), 90, 180, outline = epd.RED)
    draw.pieslice((55, 60, 95, 100), 270, 360, fill = epd.BLACK)
    draw.chord((10, 60, 50, 100), 0, 360, fill = epd.YELLOW)
    draw.ellipse((55, 60, 95, 100), outline = epd.RED)
    draw.polygon([(110,0),(110,50),(150,25)],outline = epd.BLACK)
    draw.polygon([(190,0),(190,50),(150,25)],fill = epd.BLACK)
    draw.text((120, 60), 'e-Paper demo', font = font15, fill = epd.YELLOW)
    draw.text((110, 90), u'微雪电子', font = font24, fill = epd.RED)
    
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)
    
    # read bmp file 
    print("2.read bmp file")
    Himage = Image.open(os.path.join(picdir, '10in85G.bmp'))
    epd.display(epd.getbuffer(Himage))
    time.sleep(3)

    print("clearing...")
    epd.Clear()

    print("goto sleep...")
    epd.sleep()
except:
    print("goto sleep...")
    epd.sleep()


