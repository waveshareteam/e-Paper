#!/usr/bin/python
# -*- coding:utf-8 -*-

import epd1in54
import time
from PIL import Image,ImageDraw,ImageFont
import traceback

try:
    epd = epd1in54.EPD()
    epd.init(epd.lut_full_update)
    epd.Clear(0xFF)
    
    # Drawing on the image
    image = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)  # 255: clear the frame
    
    draw = ImageDraw.Draw(image)
    font = ImageFont.truetype('/usr/share/fonts/truetype/wqy/wqy-microhei.ttc', 24)
    draw.rectangle((0, 10, 200, 34), fill = 0)
    draw.text((8, 12), 'hello world', font = font, fill = 255)
    draw.text((8, 36), 'e-Paper Demo', font = font, fill = 0)
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
    # epd.Clear(0xFF)
    image = Image.open('1in54.bmp')
    epd.display(epd.getbuffer(image))
    time.sleep(2)
    
    # read bmp file on window
    epd.Clear(0xFF)
    image1 = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)  # 255: clear the frame
    bmp = Image.open('100x100.bmp')
    image1.paste(bmp, (50,50))    
    epd.display(epd.getbuffer(image1))
    time.sleep(2)
    
    # # partial update
    epd.init(epd.lut_partial_update)
    
    epd.Clear(0xFF)
    time_image = Image.new('1', (epd1in54.EPD_WIDTH, epd1in54.EPD_HEIGHT), 255)
    time_draw = ImageDraw.Draw(time_image)
    while (True):
        time_draw.rectangle((10, 10, 120, 50), fill = 255)
        time_draw.text((10, 10), time.strftime('%H:%M:%S'), font = font, fill = 0)
        newimage = time_image.crop([10, 10, 120, 50])
        time_image.paste(newimage, (10,10))  
        epd.display(epd.getbuffer(time_image))
        
    epd.sleep()
        
        
except:
    print("traceback.format_exc():\n%s", traceback.format_exc()) 
    exit()


