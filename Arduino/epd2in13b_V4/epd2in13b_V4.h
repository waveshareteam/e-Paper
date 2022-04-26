/**
 *  @filename   :   epd2in13b_V4.h
 *  @brief      :   Header file for Three-color e-paper library epd2in13b_V4.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     April 25 2022
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

#ifndef EPD2IN13B_V4_H
#define EPD2IN13B_V4_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       122
#define EPD_HEIGHT      250

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

class Epd : EpdIf {
public:
    int width;
    int bufwidth;
    int height;
    int bufheight;
    int count;
    
    Epd();
    ~Epd();
    int  Init(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
    void SetCursor(UWORD Xstart, UWORD Ystart);
    void DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red);
    void DisplayFrame(void);
    void Display(const unsigned char* frame_buffer) ;
    void ClearFrame(void);
    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif

/* END OF FILE */
