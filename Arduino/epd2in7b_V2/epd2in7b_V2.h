/**
 *  @filename   :   epd2in7b_V2.h
 *  @brief      :   Header file for Dual-color e-paper library epd2in7b_V2.cpp
 *  @author     :   
 *  
 *  Copyright (C) Waveshare     Feb 19 2021
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

#ifndef __EPD2IN7B_V2_H_
#define __EPD2IN7B_V2_H_

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       176
#define EPD_HEIGHT      264

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t

class Epd : EpdIf {
public:
    unsigned int width;
    unsigned int height;

    Epd();
    ~Epd();
    int  Init(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void TransmitPartialBlack(const unsigned char* buffer_black, int x, int y, int w, int l);
	void TransmitPartialRed(const unsigned char* buffer_red, int x, int y, int w, int l);
    void DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red);
    void DisplayFrame(void);
    void ClearFrame(void);
    void Sleep(void);
	void SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
	void SetCursor(UWORD Xstart, UWORD Ystart);
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD2IN7B_V2_H */

/* END OF FILE */
