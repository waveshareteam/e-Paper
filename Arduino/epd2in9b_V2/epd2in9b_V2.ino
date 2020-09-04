/**
 *  @filename   :   epd2in9b_V2-demo.ino
 *  @brief      :   2.9inch e-paper display demo
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     July 3 2020
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
#include "epd2in9b_V2.h"
#include "imagedata.h"

Epd epd;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("e-Paper init \r\n ");
  if (epd.Init() != 0) {
      Serial.print("e-Paper init failed\r\n ");
      return;
  }
  
  Serial.print("e-Paper Clear\r\n ");
  epd.Clear();
  
  Serial.print("draw image\r\n ");
  epd.DisplayFrame(IMAGE_DATA, IMAGE_DATA);
  
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
}
