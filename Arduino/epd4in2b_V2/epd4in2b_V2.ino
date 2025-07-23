/**
 *  @filename   :   epd4in2b_V2.ino
 *  @brief      :   4.2inch e-paper display (B_V2) demo 
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     Nov 25 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "epd4in2b_V2.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Epd epd;

  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.Clear();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
    unsigned char image[1600];
    Paint paint(image, 128, 100);    //width should be the multiple of 8 
    paint.SetWidth(128);
    paint.SetHeight(100);

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 0, "ePaper Demo", &Font16, COLORED);
    epd.Display_Window_Black(paint.GetImage(), 0);

    paint.Clear(UNCOLORED);
    paint.DrawRectangle(0, 0, 40, 50, COLORED);
    paint.DrawLine(0, 0, 40, 50, COLORED);
    paint.DrawLine(40, 0, 0, 50, COLORED);
    epd.Display_Window_Black(paint.GetImage(), 1);    

    paint.Clear(UNCOLORED);
    paint.DrawCircle(32, 32, 25, COLORED);
    epd.Display_Window_Black(paint.GetImage(), 1);    

    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 20, "Hello world", &Font16, COLORED);
    epd.Display_Window_Red(paint.GetImage(), 0);

    paint.Clear(UNCOLORED);
    paint.DrawFilledRectangle(60, 0, 100, 50, COLORED);
    epd.Display_Window_Red(paint.GetImage(), 1);

    paint.Clear(UNCOLORED);
    paint.DrawFilledCircle(92, 32, 25, COLORED);
    epd.Display_Window_Red(paint.GetImage(), 1);
    epd.DisplayFrame();
    delay(1000);
    /* This displays an image */
    // epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  /* Deep sleep */
  epd.Clear();
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
