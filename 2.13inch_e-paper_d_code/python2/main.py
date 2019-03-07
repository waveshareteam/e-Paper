#!/usr/bin/python
# -*- coding:utf-8 -*-

import epd2in13d
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    epd = epd2in13d.EPD()
    epd.init()
    epd.Clear(0xFF)
    
    # Drawing on the image
    image = Image.new('1', (epd2in13d.EPD_HEIGHT, epd2in13d.EPD_WIDTH), 255)  # 255: clear the frame
    
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
    font15 = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 15)
    draw.text((110, 60), 'e-Paper demo', font = font15, fill = 0)
    draw.text((110, 80), 'Hello world', font = font15, fill = 0)
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file 
    # epd.Clear(0xFF)
    image = Image.open('2in13d.bmp')
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file on window
    epd.Clear(0xFF)
    image1 = Image.new('1', (epd2in13d.EPD_WIDTH, epd2in13d.EPD_HEIGHT), 255)  # 255: clear the frame
    bmp = Image.open('100x100.bmp')
    image1.paste(bmp, (0,10))    
    epd.display(epd.getbuffer(image1))
    time.sleep(2)
    
    # # partial update
    epd.Clear(0xFF)
    font24 = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 24)
    time_image = Image.new('1', (epd2in13d.EPD_HEIGHT, epd2in13d.EPD_WIDTH), 255)
    time_draw = ImageDraw.Draw(time_image)
    while (True):
        time_draw.rectangle((10, 10, 120, 50), fill = 255)
        time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font24, fill = 0)
        newimage = time_image.crop([10, 10, 120, 50])
        time_image.paste(newimage, (10,10))  
        epd.DisplayPartial(epd.getbuffer(time_image))
        
    epd.sleep()
        
except Exception, e:
    print 'traceback.format_exc():\n%s' % traceback.format_exc()
    exit()

