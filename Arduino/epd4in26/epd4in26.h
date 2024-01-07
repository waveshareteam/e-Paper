/**
 *  @filename   :   epd4in26.h
 *  @brief      :   Header file for e-paper library epd4in26.cpp
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     23-12-20
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

#ifndef EPD4IN26_H
#define EPD4IN26_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       800
#define EPD_HEIGHT      480

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int  Init(void);
    int Init_Fast(void);
    int Init_4GRAY(void);
    void SetWindows(unsigned long Xstart, unsigned long Ystart, unsigned long Xend, unsigned long Yend);
    void SetCursor(unsigned long Xstart, unsigned long Ystart);
    void ReadBusy(void);
    void Reset(void);
    void SetLut(void);
    void TurnOnDisplay(void);
    void TurnOnDisplay_Fast(void);
    void TurnOnDisplay_Part(void);
    void TurnOnDisplay_4GRAY(void);
    void Display(const unsigned char* frame_buffer);
    void Display_Base(const unsigned char* frame_buffer);
    void Display_Fast(const unsigned char* frame_buffer);
    void Display_Part(unsigned char *Image, unsigned long x, unsigned long y, unsigned long w, unsigned long l);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Sleep(void);
    void Clear(void);
    void Displaypart(const unsigned char* pbuffer, unsigned long Start_X, unsigned long Start_Y,unsigned long END_X,unsigned long END_Y);
 
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long WIDTH;
    unsigned long HEIGHT;

	void SetLut(unsigned char *lut);
    void SetLut_by_host(unsigned char *lut_vcom, unsigned char *lut_ww, unsigned char *lut_bw, unsigned char *lut_wb, unsigned char *lut_bb);
};

#endif /* EPD7IN5_H */

/* END OF FILE */
