#!/usr/bin/python
# -*- coding:utf-8 -*-

import epd1in54b
import time
import Image
import ImageDraw
import ImageFont
import traceback

try:
    epd = epd1in54b.EPD()
    epd.init()
    epd.Clear()
    
    # Drawing on the image
    blackimage = Image.new('1', (epd1in54b.EPD_WIDTH, epd1in54b.EPD_HEIGHT), 255)  # 255: clear the frame
    redimage = Image.new('1', (epd1in54b.EPD_WIDTH, epd1in54b.EPD_HEIGHT), 255)  # 255: clear the frame
    print "drawing"
    drawblack = ImageDraw.Draw(blackimage)
    drawred = ImageDraw.Draw(redimage)
    font = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 24)
    drawblack.rectangle((0, 10, 200, 34), fill = 0)
    drawblack.text((8, 12), 'hello world', font = font, fill = 255)
    drawblack.text((8, 36), 'e-Paper Demo', font = font, fill = 0)
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
 
    print "open pic"
    blackimage = Image.open('1in54b-b.bmp')
    redimage = Image.open('1in54b-r.bmp')    
    epd.display(epd.getbuffer(blackimage),epd.getbuffer(redimage))
    time.sleep(1)
    
    print "show windows"
    blackimage1 = Image.new('1', (epd1in54b.EPD_WIDTH, epd1in54b.EPD_HEIGHT), 255)  # 255: clear the frame
    redimage2 = Image.new('1', (epd1in54b.EPD_WIDTH, epd1in54b.EPD_HEIGHT), 255)
    
    newimage = Image.open('100x100.bmp')
    blackimage1.paste(newimage, (50,50))    
    epd.display(epd.getbuffer(blackimage1), epd.getbuffer(redimage2))
    
    print "sleep"    
    epd.sleep()
        
except Exception, e:
    print 'traceback.format_exc():\n%s' % traceback.format_exc()
    exit()

