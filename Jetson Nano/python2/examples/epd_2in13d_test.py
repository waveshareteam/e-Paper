#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
sys.path.append(r'../lib')

import epd2in13d
import epdconfig
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    print("epd2in13d Demo")
    
    epd = epd2in13d.EPD()
    print("init and Clear")
    epd.init()
    epd.Clear(0xFF)
    
    font15 = ImageFont.truetype('../lib/Font.ttc', 15)
    
    # Drawing on the Horizontal image
    print("1.Drawing on the Horizontal image...")
    Himage = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    draw = ImageDraw.Draw(Himage)
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
    draw.text((110, 60), 'e-Paper demo', font = font15, fill = 0)
    draw.text((110, 80), u'微雪电子', font = font15, fill = 0)
    epd.display(epd.getbuffer(Himage))
    time.sleep(2)
        
    print("3.read bmp file")
    Himage = Image.open('../pic/2in13d.bmp')
    epd.display(epd.getbuffer(Himage))
    time.sleep(2)
    
    print("4.read bmp file on window")
    Himage2 = Image.new('1', (epd.height, epd.width), 255)  # 255: clear the frame
    bmp = Image.open('../pic/100x100.bmp')
    Himage2.paste(bmp, (20,20))
    epd.display(epd.getbuffer(Himage2))
    time.sleep(2)
    
    # # partial update
    print("5.show time...")
    epd.init()    
    epd.Clear(0xFF)
    
    time_image = Image.new('1', (epd.width, epd.height), 255)
    time_draw = ImageDraw.Draw(time_image)
    num = 0
    while (True):
        time_draw.rectangle((10, 10, 120, 50), fill = 255)
        time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        newimage = time_image.crop([10, 10, 120, 50])
        time_image.paste(newimage, (10,10))  
        epd.DisplayPartial(epd.getbuffer(time_image))
        num = num + 1
        if(num == 10):
            break
    
    print("Clear...")
    epd.Clear(0xFF)
    
    print("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    print(e)
    
except KeyboardInterrupt:    
    print("ctrl + c:")
    epdconfig.module_exit()
    exit()
