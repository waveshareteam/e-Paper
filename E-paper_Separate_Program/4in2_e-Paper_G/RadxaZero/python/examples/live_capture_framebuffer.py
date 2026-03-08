#!/usr/bin/python
# -*- coding:utf-8 -*-
import sys
import os
import logging
import time
import hashlib
from PIL import Image, ImageChops

# --- Setup Paths ---
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

from waveshare_epd import epd4in2g

# --- Configuration ---
FB_W, FB_H = 1024, 768  # Your detected Radxa resolution
EPD_W, EPD_H = 400, 300 # Your physical screen size

logging.basicConfig(level=logging.INFO)

def get_frame_hash(data):
    return hashlib.md5(data).hexdigest()

try:
    epd = epd4in2g.EPD()
    logging.info("Initializing E-ink...")
    epd.init()
    epd.init_fast(epd.Seconds_1_5S) # Your preferred fast mode
    
    last_hash = None
    last_image = None
    
    while True:
        # 1. Grab pixels from the system's hidden screen
        with open("/dev/fb0", "rb") as f:
            raw_data = f.read(FB_W * FB_H * 4)
        
        # Convert raw bytes to Image and crop to your pocket screen size immediately
        full_img = Image.frombytes('RGB', (FB_W, FB_H), raw_data, 'raw', 'BGRX')
        terminal_view = full_img.crop((0, 0, EPD_W, EPD_H)).convert('1')

        # Calculate hash based ONLY on the visible cropped area
        curr_hash = get_frame_hash(terminal_view.tobytes())

        # 2. Only update the e-ink if the visible area changed
        if curr_hash != last_hash:
            if last_image is not None:
                diff = ImageChops.difference(last_image, terminal_view)
                bbox = diff.getbbox()
                if bbox:
                    logging.info(f"Change detected in region: {bbox}")
                else:
                    logging.info("Hash changed but pixels identical? (Rare)")
            
            # 3. Push to physical screen
            epd.display_Fast(epd.getbuffer(terminal_view))
            last_hash = curr_hash
            last_image = terminal_view
            logging.info("E-ink Updated.")

        time.sleep(0.2)
        
except KeyboardInterrupt:
    epd4in2g.epdconfig.module_exit()
    exit()