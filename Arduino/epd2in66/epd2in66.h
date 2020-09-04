/**
 *  @filename   :   epd2in66.h
 *  @brief      :   Header file for e-paper library epd2in66.cpp
 *  @author     :   Waveshare
 *  
 *  Copyright (C) Waveshare     July 29 2020
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

#ifndef EPD2IN66_H
#define EPD2IN66_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       152
#define EPD_HEIGHT      296

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE unsigned long

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int  Init(void);
    int  Init_Partial(void);
    void WaitUntilIdle(void);
    void DisplayFrame(const UBYTE *Image);
    void DisplayFrame_part(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD iwidth, UWORD iheight);
    void Sleep(void);
    void Clear(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long height;
    void Load_LUT(void);
    void TurnOnDisplay(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Reset(void);
};

#endif /* EPD2IN66_H */

/* END OF FILE */
