#!/usr/bin/python
# -*- coding:utf-8 -*-

import epd7in5b
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    epd = epd7in5b.EPD()
    epd.init()
    print("Clear...")
    epd.Clear(0xFF)
    
    # Drawing on the Horizontal image
    HBlackimage = Image.new('1', (epd7in5b.EPD_WIDTH, epd7in5b.EPD_HEIGHT), 255)  # 298*126
    HRedimage = Image.new('1', (epd7in5b.EPD_WIDTH, epd7in5b.EPD_HEIGHT), 255)  # 298*126    
    
    # Horizontal
    print("Drawing")
    drawblack = ImageDraw.Draw(HBlackimage)
    drawred = ImageDraw.Draw(HRedimage)
    font24 = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 24)
    drawblack.text((10, 0), 'hello world', font = font24, fill = 0)
    drawblack.text((10, 20), '7.5inch e-Paper B', font = font24, fill = 0)
    drawblack.text((150, 0), u'微雪电子', font = font24, fill = 0)    
    drawblack.line((20, 50, 70, 100), fill = 0)
    drawblack.line((70, 50, 20, 100), fill = 0)
    drawblack.rectangle((20, 50, 70, 100), outline = 0)    
    drawred.line((165, 50, 165, 100), fill = 0)
    drawred.line((140, 75, 190, 75), fill = 0)
    drawred.arc((140, 50, 190, 100), 0, 360, fill = 0)
    drawred.rectangle((80, 50, 130, 100), fill = 0)
    drawred.chord((200, 50, 250, 100), 0, 360, fill = 0)
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(2)
    
    # Drawing on the Vertical image
    LBlackimage = Image.new('1', (epd7in5b.EPD_HEIGHT, epd7in5b.EPD_WIDTH), 255)  # 126*298
    LRedimage = Image.new('1', (epd7in5b.EPD_HEIGHT, epd7in5b.EPD_WIDTH), 255)  # 126*298
    # Vertical
    drawblack = ImageDraw.Draw(LBlackimage)
    drawred = ImageDraw.Draw(LRedimage)
    font18 = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 18)
    drawblack.text((2, 0), 'hello world', font = font18, fill = 0)
    drawblack.text((2, 20), '7.5inch e-Paper B', font = font18, fill = 0)
    drawblack.text((20, 50), u'微雪电子', font = font18, fill = 0)
    drawblack.line((10, 90, 60, 140), fill = 0)
    drawblack.line((60, 90, 10, 140), fill = 0)
    drawblack.rectangle((10, 90, 60, 140), outline = 0)
    drawred.line((95, 90, 95, 140), fill = 0)
    drawred.line((70, 115, 120, 115), fill = 0)
    drawred.arc((70, 90, 120, 140), 0, 360, fill = 0)
    drawred.rectangle((10, 150, 60, 200), fill = 0)
    drawred.chord((70, 150, 120, 200), 0, 360, fill = 0)
    epd.display(epd.getbuffer(LBlackimage), epd.getbuffer(LRedimage))
    time.sleep(2)
    
    print("read bmp file")
    HBlackimage = Image.open('7in5b-b.bmp')
    HRedimage = Image.open('7in5b-r.bmp')
    epd.display(epd.getbuffer(HBlackimage), epd.getbuffer(HRedimage))
    time.sleep(2)
    
    print("read bmp file on window")
    blackimage1 = Image.new('1', (epd7in5b.EPD_HEIGHT, epd7in5b.EPD_WIDTH), 255)  # 298*126
    redimage1 = Image.new('1', (epd7in5b.EPD_HEIGHT, epd7in5b.EPD_WIDTH), 255)  # 298*126    
    newimage = Image.open('100x100.bmp')
    blackimage1.paste(newimage, (50,10))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage1))
    
    epd.sleep()
        
except:
    print('traceback.format_exc():\n%s',traceback.format_exc())
    exit()
