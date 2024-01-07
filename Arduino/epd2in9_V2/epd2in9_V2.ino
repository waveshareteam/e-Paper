  /**
 *  @filename   :   epd2in9_V2-demo.ino
 *  @brief      :   2.9inch e-paper V2 display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     Nov 09 2020
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
#include "epd2in9_V2.h"
#include "epdpaint.h"
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to 
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8 
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;
char time_string[] = {'0', '0', ':', '0', '0', '\0'};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

#if 1
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  
  paint.SetRotate(ROTATE_0);
  paint.SetWidth(128);
  paint.SetHeight(24);

  /* For simplicity, the arguments are explicit numerical coordinates */
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Hello world!", &Font12, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, "e-Paper Demo", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());

  paint.SetWidth(64);
  paint.SetHeight(64);
  
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 8, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 56, 60, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 8, 130, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 56, 130, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();

  delay(2000);
#endif

#if 1
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed ");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
  epd.SetFrameMemory_Base(IMAGE_DATA);
  epd.DisplayFrame();
#endif

#if 0
  // Quick brush demo
  if (epd.Init_Fast() != 0) {
      Serial.print("e-Paper init failed ");
      return;
  }

  /** 
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
  epd.SetFrameMemory_Base(IMAGE_DATA);
  epd.DisplayFrame();
#endif

#if 0
  Serial.print("show 4-gray image\r\n");
  if (epd.Init_4Gray() != 0) {
      Serial.print("e-Paper init failed ");
      return;
  }
  epd.Display4Gray(IMAGE_DATA_4Gray);
#endif

#if 0
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed");
      return;
  }

  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  
  time_start_ms = millis();
  
  // put your main code here, to run repeatedly:

  for(;;){
    time_now_s = (millis() - time_start_ms) / 1000;
    time_string[0] = time_now_s / 60 / 10 + '0';
    time_string[1] = time_now_s / 60 % 10 + '0';
    time_string[3] = time_now_s % 60 / 10 + '0';
    time_string[4] = time_now_s % 60 % 10 + '0';

    paint.SetWidth(32);
    paint.SetHeight(96);
    paint.SetRotate(ROTATE_90);
  
    paint.Clear(UNCOLORED);
    paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);
    epd.SetFrameMemory_Partial(paint.GetImage(), 80, 72, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame_Partial();
  }
#endif

  /* Deep sleep */
  Serial.print("sleep...");
  epd.Sleep();
}

void loop() {


  // delay(300);
}
