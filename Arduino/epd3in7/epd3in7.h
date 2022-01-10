/**
 *  @filename   :   epd3in7.h
 *  @brief      :   Header file for e-paper library epd3in7.cpp
 *  @author     :   Waveshare
 *  
 *  Copyright (C) Waveshare     July 17 2020
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

#ifndef EPD3IN7_H
#define EPD3IN7_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       480
#define EPD_HEIGHT      280

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE unsigned long

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int  Init(void);
    void WaitUntilIdle(void);
    void Reset(void);
    void DisplayFrame(const UBYTE *Image, bool isBase);
    void SendCommand(unsigned char command);
    void DisplayFrame_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD iwidth, UWORD iheight);
    void DisplayFrame_Part(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend, bool isGC);
    void TurnOnDisplay(void);
    void SendData(unsigned char data);
    void Sleep(void);
    void Clear(UBYTE mode);
    void Load_LUT(UBYTE mode);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long height;
};

#endif /* EPD3IN7_H */

/* END OF FILE */
