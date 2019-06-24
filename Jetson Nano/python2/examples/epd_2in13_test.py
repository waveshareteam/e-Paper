#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
sys.path.append(r'../lib')

import epd2in13
import epdconfig
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    print("epd2in13 Demo")
    
    epd = epd2in13.EPD()
    print("init and Clear")
    epd.init(epd.lut_full_update)
    epd.Clear(0xFF)
    
    # Drawing on the image
    font15 = ImageFont.truetype('../lib/Font.ttc', 15)
    font24 = ImageFont.truetype('../lib/Font.ttc', 24)
    
    print("1.Drawing on the image...")
    image = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame    
    draw = ImageDraw.Draw(image)
    
    draw.rectangle([(0,0),(50,50)],outline = 0)
    draw.rectangle([(55,0),(100,50)],fill = 0)
    draw.line([(0,0),(50,50)], fill = 0,width = 1)
    draw.line([(0,50),(50,0)], fill = 0,width = 1)
    draw.chord((10, 60, 50, 100), 0, 360, fill = 0)
    draw.ellipse((55, 60, 95, 100), outline = 0)
    draw.pieslice((55, 60, 95, 100), 90, 180, outline = 0)
    draw.pieslice((55, 60, 95, 100), 270, 360, fill = 0)
    draw.polygon([(110,0),(110,50),(150,25)],outline = 0)
    draw.polygon([(190,0),(190,50),(150,25)],fill = 0)
    draw.text((120, 60), 'e-Paper demo', font = font15, fill = 0)
    draw.text((110, 90), u'微雪电子', font = font24, fill = 0)
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file 
    print("2.read bmp file...")
    image = Image.open('../pic/2in13.bmp')
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file on window
    print("3.read bmp file on window...")
    # epd.Clear(0xFF)
    image1 = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    bmp = Image.open('../pic/100x100.bmp')
    image1.paste(bmp, (2,2))    
    epd.display(epd.getbuffer(image1))
    time.sleep(2)
    
    # # partial update
    print("4.show time...")
    epd.init(epd.lut_partial_update)    
    epd.Clear(0xFF)
    
    time_image = Image.new('1', (epd.height, epd.width), 255)
    time_draw = ImageDraw.Draw(time_image)
    num = 0
    while (True):
        time_draw.rectangle((120, 80, 220, 105), fill = 255)
        time_draw.text((120, 80), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        epd.display(epd.getbuffer(time_image))
        num = num + 1
        if(num == 10):
            break
    
    print("Clear...")
    epd.init(epd.lut_full_update)
    epd.Clear(0xFF)
    
    print("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    print(e)
    
except KeyboardInterrupt:    
    print("ctrl + c:")
    epdconfig.module_exit()
    exit()
