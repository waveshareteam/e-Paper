#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
sys.path.append(r'../lib')

import epd1in54b
import epdconfig
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    print("epd1in54b Demo")
    
    epd = epd1in54b.EPD()
    print("init and Clear")
    epd.init()
    epd.Clear()
    time.sleep(1)
    
    # Drawing on the image
    print("1.Drawing on the image...")
    blackimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    redimage = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    
    font = ImageFont.truetype('../lib/Font.ttc', 24)
    font18 = ImageFont.truetype('../lib/Font.ttc', 18)
    
    drawblack = ImageDraw.Draw(blackimage)
    drawred = ImageDraw.Draw(redimage)
    drawblack.rectangle((0, 10, 200, 34), fill = 0)
    drawblack.text((8, 12), 'hello world', font = font, fill = 255)
    drawblack.text((8, 36), u'微雪电子', font = font18, fill = 0)
    drawblack.line((16, 60, 56, 60), fill = 0)
    drawblack.line((56, 60, 56, 110), fill = 0)
    drawblack.line((16, 110, 56, 110), fill = 0)
    drawred.line((16, 110, 16, 60), fill = 0)
    drawred.line((16, 60, 56, 110), fill = 0)
    drawred.line((56, 60, 16, 110), fill = 0)
    drawred.arc((90, 60, 150, 120), 0, 360, fill = 0)
    drawred.rectangle((16, 130, 56, 180), fill = 0)
    drawred.chord((90, 130, 150, 190), 0, 360, fill = 0)
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)
    
    # read bmp file 
    print("2.read bmp file...")
    blackimage = Image.open('../pic/1in54b-b.bmp')
    redimage = Image.open('../pic/1in54b-r.bmp')    
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)
    
    # read bmp file on window
    print("3.read bmp file on window...")
    blackimage1 = Image.new('1', (epd.width, epd.height), 255)  # 255: clear the frame
    redimage2 = Image.new('1', (epd.width, epd.height), 255)
    
    newimage = Image.open('../pic/100x100.bmp')
    blackimage1.paste(newimage, (50,50))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage2))
    
    print("Clear...")
    epd.init()
    epd.Clear()
    
    print("Goto Sleep...")
    epd.sleep()
        
except IOError as e:
    print(e)
    
except KeyboardInterrupt:    
    print("ctrl + c:")
    epdconfig.module_exit()
    exit()
