/**
 *  @filename   :   epd2in13b-demo.ino
 *  @brief      :   2.13inch e-paper display (B) V3 demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2017
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
#include "epd2in13b_V3.h"
#include "imagedata.h"
#include "epdpaint.h"

#define COLORED     0
#define UNCOLORED   1
unsigned char image[2756/4];
//unsigned char image_r[2756];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Epd epd;

  if (epd.Init() != 0) {
    Serial.print("e-Paper init failed");
    return;
  }

  /* This clears the SRAM of the e-paper display */
  epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  Paint paint(image, 104, 106);    //width should be the multiple of 8 

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(8, 2, "e-Paper Demo", &Font12, COLORED);
  
  epd.Display(image);//1
  
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(2,2,50,50,COLORED);
  paint.DrawLine(2,2,50,50,COLORED);
  paint.DrawLine(2,50,50,2,COLORED);

  epd.Display(image);//2
  
  paint.Clear(UNCOLORED);
  paint.DrawCircle(25,25,20,COLORED);
  epd.Display(image);//3
  
  paint.Clear(UNCOLORED);
  epd.Display(image);//4
  
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(8, 20, "Hello world", &Font12, COLORED);
  epd.Display(image);//5
  
  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(52,2,100,50,COLORED);
  paint.DrawLine(52,2,100,50,UNCOLORED);
  paint.DrawLine(100,2,52,50,UNCOLORED);
  epd.Display(image);//6
  
  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(75,25,20,COLORED);
  epd.Display(image);//7
  
  paint.Clear(UNCOLORED);
  epd.Display(image);//8

  delay(1000);
  /* This displays an image */
  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  /* Deep sleep */
  epd.Sleep();
}

void loop() {
  // put your main code here, to run repeatedly:

}
