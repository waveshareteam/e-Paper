/**
    @filename   :   epd2in7-demo.ino
    @brief      :   2.7inch e-paper display demo
    @author     :   Yehui from Waveshare

    Copyright (C) Waveshare     August 22 2017

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documnetation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to  whom the Software is
   furished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
   THE SOFTWARE.
*/

#include <SPI.h>
#include "epd2in7.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Epd epd;

  Serial.print("e-Paper init\r\n");
  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed\r\n");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
      Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
      In this case, a smaller image buffer is allocated and you have to
      update a partial display several times.
      1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
#if 1
  unsigned char image[1024];
  Paint paint(image, 176, 24);    //width should be the multiple of 8

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "e-Paper Demo", &Font16, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 16, 32, paint.GetWidth(), paint.GetHeight());

  paint.Clear(COLORED);
  paint.DrawStringAt(20, 5, "Hello world!", &Font16, UNCOLORED);
  epd.TransmitPartialData(paint.GetImage(), 0, 64, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);

  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 10, 130, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.TransmitPartialData(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  Serial.print("show draw image\r\n");
  epd.DisplayFrame();
  delay(1000);
#endif
  
#if 1
  /* This displays an image */
  Serial.print("show 2-gray image\r\n");
  epd.DisplayFrame(IMAGE_DATA);
  delay(1000);
#endif

#if 1
  Serial.print("show 4-gray image\r\n");
  epd.Init_4Gray();
  epd.Display4Gray(IMAGE_DATA_4Gray);
#endif

  /* Deep sleep */
  Serial.print("sleep...");
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
