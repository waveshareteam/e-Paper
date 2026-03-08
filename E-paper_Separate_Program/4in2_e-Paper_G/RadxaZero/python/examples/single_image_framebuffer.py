import PIL.Image

# Values from your fbset output
WIDTH = 1024
HEIGHT = 768
DEPTH = 32 # 4 bytes per pixel

fb_device = "/dev/fb0"

try:
    with open(fb_device, "rb") as f:
        # Read the full buffer: 1024 * 768 * 4 = 3,145,728 bytes
        raw_data = f.read(WIDTH * HEIGHT * 4)

    # 'RGB' is our target. 'BGRX' matches your rgba 8/16,8/8,8/0 output.
    img = PIL.Image.frombytes('RGB', (WIDTH, HEIGHT), raw_data, 'raw', 'BGRX')

    # POCKET COMPUTER EFFECT:
    # 1. Convert to Grayscale ('L')
    # 2. Convert to 1-bit Black and White ('1')
    eink_style = img.convert('L').convert('1')

    # If you want the 400x300 "Pocket" look from the video, 
    # we crop the top-left corner where the terminal text usually is.
    pocket_view = eink_style.crop((0, 0, 400, 300))
    
    # Save files
    img.save("full_color.png")
    eink_style.save("full_eink.png")
    pocket_view.save("pocket_computer.png")

    print("Success! Created full_color.png and pocket_computer.png")

except Exception as e:
    print(f"Capture failed: {e}")
