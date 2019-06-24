#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
sys.path.append(r'../lib')

import epd1in54_V2
import epdconfig
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    print("epd1in54_V2 Demo")
    
    epd = epd1in54_V2.EPD()
    print("init and Clear")
    epd.init()
    epd.Clear(0xFF)
    time.sleep(1)
    
    # Drawing on the image
    print("1.Drawing on the image...")
    image = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype('../lib/Font.ttc', 24)
    draw.rectangle((0, 10, 200, 34), fill = 0)
    draw.text((8, 12), 'hello world', font = font, fill = 255)
    draw.text((8, 36), u'微雪电子', font = font, fill = 0)
    draw.line((16, 60, 56, 60), fill = 0)
    draw.line((56, 60, 56, 110), fill = 0)
    draw.line((16, 110, 56, 110), fill = 0)
    draw.line((16, 110, 16, 60), fill = 0)
    draw.line((16, 60, 56, 110), fill = 0)
    draw.line((56, 60, 16, 110), fill = 0)
    draw.arc((90, 60, 150, 120), 0, 360, fill = 0)
    draw.rectangle((16, 130, 56, 180), fill = 0)
    draw.chord((90, 130, 150, 190), 0, 360, fill = 0)
    epd.display(epd.getbuffer(image.rotate(90)))
    time.sleep(2)
    
    # read bmp file 
    print("2.read bmp file...")
    image = Image.open('../pic/1in54.bmp')
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file on window
    print("3.read bmp file on window...")
    epd.Clear(0xFF)
    image1 = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    bmp = Image.open('../pic/100x100.bmp')
    image1.paste(bmp, (50,50))    
    epd.display(epd.getbuffer(image1))
    time.sleep(2)
    
    # partial update
    print("4.show time...")    
    time_image = Image.new('1', (epd.width, epd.height), 255)
    epd.displayPartBaseImage(epd.getbuffer(time_image))
    
    time_draw = ImageDraw.Draw(time_image)
    num = 0
    while (True):
        time_draw.rectangle((10, 10, 120, 50), fill = 255)
        time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font, fill = 0)
        newimage = time_image.crop([10, 10, 120, 50])
        time_image.paste(newimage, (10,10))  
        epd.displayPart(epd.getbuffer(time_image))
        num = num + 1
        if(num == 10):
            break
    
    print("Clear...")
    epd.init()
    epd.Clear(0xFF)
    
    print("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    print(e)
    
except KeyboardInterrupt:    
    print("ctrl + c:")
    epdconfig.module_exit()
    exit()