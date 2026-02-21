from http.server import BaseHTTPRequestHandler, HTTPServer
from urllib.parse import parse_qs
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'pic')
libdir = os.path.join(os.path.dirname(os.path.dirname(os.path.realpath(__file__))), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)

import logging
from waveshare_epd import epd4in01f
import cgi
from PIL import Image, ImageDraw, ImageFont

def display_data_centered(data, font_size=40):

    logging.info("E-Paper Centered Data Display")
    epd = epd4in01f.EPD()
    logging.info("Init and Clear")
    epd.init()
    epd.Clear()

    # Define the font path.
    font_path = os.path.join(picdir, 'Font.ttc')
    
    # Create a new white image with dimensions matching the display.
    image = Image.new('RGB', (epd.width, epd.height), 0xffffff)
    draw = ImageDraw.Draw(image)
    
    # Load the specified font.
    font = ImageFont.truetype(font_path, font_size)
    
    # Measure the text dimensions.
    text_width, text_height = draw.textsize(data, font=font)
    
    # Compute coordinates to center the text.
    x = (epd.width - text_width) // 2
    y = (epd.height - text_height) // 2
    
    # Draw the text onto the image.
    draw.text((x, y), data, font=font, fill=epd.BLACK)
    
    # Update the display with the new image.
    epd.display(epd.getbuffer(image))

# --- Custom image conversion function inspired by the provided code ---
def epaper_convert_image(input_image, direction=None, mode='scale', dither=Image.FLOYDSTEINBERG):
    """
    Convert a PIL image for a 7-color E-Ink display.
    - direction: 'landscape' or 'portrait'; if None, auto-detect based on image dimensions.
    - mode: 'scale' (resize and letterbox) or 'cut' (crop to fit).
    - dither: dithering algorithm (default: Image.FLOYDSTEINBERG).

    Returns a quantized RGB image.
    """
    width, height = input_image.size

    # Determine target size based on orientation
    if direction:
        if direction == 'landscape':
            target_width, target_height = 640, 400
        else:  # portrait
            target_width, target_height = 400, 640
    else:
        # Auto-detect: if width > height assume landscape; otherwise portrait.
        if width > height:
            target_width, target_height = 640, 400
        else:
            target_width, target_height = 400, 640

    # Resize or crop the image based on the selected mode.
    if mode == 'scale':
        # Scale up so that the image covers the target dimensions.
        scale_ratio = max(target_width / width, target_height / height)
        resized_width = int(width * scale_ratio)
        resized_height = int(height * scale_ratio)
        output_image = input_image.resize((resized_width, resized_height), Image.LANCZOS)
        # Create a new white background and paste the scaled image centered.
        resized_image = Image.new('RGB', (target_width, target_height), (255, 255, 255))
        left = (target_width - resized_width) // 2
        top = (target_height - resized_height) // 2
        resized_image.paste(output_image, (left, top))
    elif mode == 'cut':
        # Use ImageOps.fit to crop the image to the target size.
        resized_image = ImageOps.fit(input_image, (target_width, target_height), method=Image.LANCZOS, centering=(0.5, 0.5))
    else:
        # Default to scale mode if an unknown mode is provided.
        resized_image = input_image.resize((target_width, target_height), Image.LANCZOS)

    # Create a custom 7-color palette:
    # Colors: black, white, green, blue, red, yellow, orange
    palette_data = (
        0, 0, 0,         # black
        255, 255, 255,   # white
        0, 255, 0,       # green
        0, 0, 255,       # blue
        255, 0, 0,       # red
        255, 255, 0,     # yellow
        255, 128, 0      # orange
    ) + (0, 0, 0) * (256 - 7)  # pad the rest of the 256*3 palette entries with zeros

    pal_image = Image.new("P", (1, 1))
    pal_image.putpalette(palette_data)

    # Quantize the resized image using the provided dithering algorithm
    quantized_image = resized_image.quantize(dither=dither, palette=pal_image).convert('RGB')
    return quantized_image

# --- End of custom conversion function ---

def display_on_eink(bmp_image_path):
    """
    Open the BMP image and display it on the Waveshare E-Ink display.
    """
    epd = epd4in01f.EPD()
    epd.init()
    epd.Clear()
    
    print("Image path : "+ bmp_image_path)
    
    image = Image.open(bmp_image_path)
    epd.display(epd.getbuffer(image))

    print("Displayed BMP image on E-Ink.")

class MyRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Serve a simple HTML page with text and image upload forms.
        if self.path == '/':
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            html = """
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>E ink - Server  - Send Text and Upload Image</title>
                <style>
                    body {
                        font-family: Arial, sans-serif;
                        padding: 20px;
                        margin: 0;
                    }
                    h1, h2 {
                        text-align: center;
                    }
                    form {
                        max-width: 600px;
                        margin: auto;
                        padding: 10px;
                    }
                    input, button {
                        width: 100%;
                        padding: 10px;
                        margin: 5px 0;
                        box-sizing: border-box;
                    }
                    hr {
                        margin: 20px 0;
                    }
                </style>
            </head>
            <body>
                <h1>E ink - Server </h1>
                <h2>Set text on display</h2>
                <form action="/submit" method="post">
                    <input type="text" name="user_text" placeholder="Enter text here" required>
                    <button type="submit">Send</button>
                </form>
                <hr>
                <h2>Upload an Image</h2>
                <form action="/upload" method="post" enctype="multipart/form-data">
                    <input type="file" name="image_file" accept="image/*" required>
                    <button type="submit">Upload</button>
                </form>
            </body>
            </html>
            """
            self.wfile.write(html.encode('utf-8'))
        else:
            self.send_error(404, "File Not Found")
    
    def do_POST(self):
        if self.path == '/submit':
            # Handle text submission.
            content_length = int(self.headers.get('Content-Length', 0))
            post_data = self.rfile.read(content_length).decode('utf-8')
            parsed_data = parse_qs(post_data)
            user_text = parsed_data.get('user_text', [''])[0]
            print("Received text:", user_text)
            display_data_centered(user_text, font_size=40)
            self.send_response(200)
            self.send_header('Content-type', 'text/html')
            self.end_headers()
            response = f"""
            <!DOCTYPE html>
            <html lang="en">
            <head>
                <meta charset="UTF-8">
                <meta name="viewport" content="width=device-width, initial-scale=1.0">
                <title>Text Received - E ink - Server </title>
                <style>
                    body {{
                        font-family: Arial, sans-serif;
                        padding: 20px;
                        margin: 0;
                    }}
                    h2 {{
                        text-align: center;
                    }}
                    a {{
                        display: block;
                        text-align: center;
                        margin-top: 20px;
                    }}
                </style>
            </head>
            <body>
                <h2>Received: {user_text}</h2>
                <a href="/">Go Back</a>
            </body>
            </html>
            """
            self.wfile.write(response.encode('utf-8'))
        elif self.path == '/upload':
            # Handle image upload.
            ctype, pdict = cgi.parse_header(self.headers.get('Content-Type'))
            if ctype == 'multipart/form-data':
                pdict['boundary'] = bytes(pdict['boundary'], "utf-8")
                pdict['CONTENT-LENGTH'] = int(self.headers.get('Content-Length', 0))
                fields = cgi.parse_multipart(self.rfile, pdict)
                if 'image_file' in fields:
                    file_data = fields['image_file'][0]
                    jpg_path = os.path.join(os.getcwd(), "uploaded_image.jpg")
                    with open(jpg_path, 'wb') as f:
                        f.write(file_data)
                    print("Image saved as JPG to", jpg_path)
                    
                    # Open the uploaded image using Pillow.
                    try:
                        input_img = Image.open(jpg_path).convert("RGB")
                        # Convert the image for e-paper using our custom function.
                        converted_img = epaper_convert_image(input_img, direction=None, mode='scale', dither=Image.FLOYDSTEINBERG)
                        bmp_path = os.path.join(os.getcwd(), "processed_image.bmp")
                        converted_img.save(bmp_path, "BMP")
                        print("Converted image saved as BMP to", bmp_path)
                        
                        # Display the BMP on the E-Ink display.
                        display_on_eink(bmp_path)
                        display_message = "Image uploaded, converted, and displayed on E-Ink successfully."
                    except Exception as e:
                        print("Error processing image:", e)
                        display_message = f"Image uploaded but error processing: {e}"
                    
                    self.send_response(200)
                    self.send_header('Content-type', 'text/html')
                    self.end_headers()
                    response = f"""
                    <!DOCTYPE html>
                    <html lang="en">
                    <head>
                        <meta charset="UTF-8">
                        <meta name="viewport" content="width=device-width, initial-scale=1.0">
                        <title>Image Processed - E ink - Server </title>
                        <style>
                            body {{
                                font-family: Arial, sans-serif;
                                padding: 20px;
                                margin: 0;
                            }}
                            h2 {{
                                text-align: center;
                            }}
                            a {{
                                display: block;
                                text-align: center;
                                margin-top: 20px;
                            }}
                        </style>
                    </head>
                    <body>
                        <h2>{display_message}</h2>
                        <a href="/">Go Back</a>
                    </body>
                    </html>
                    """
                    self.wfile.write(response.encode('utf-8'))
                else:
                    self.send_error(400, "No image file uploaded")
            else:
                self.send_error(400, "Invalid form encoding")
        else:
            self.send_error(404, "Path Not Found")

def run(server_class=HTTPServer, handler_class=MyRequestHandler, port=8080):
    server_address = ('', port)  # Listen on all interfaces.
    httpd = server_class(server_address, handler_class)
    print(f"Starting server on port {port}...")
    httpd.serve_forever()

if __name__ == '__main__':
    run()
