/*****************************************************************************
* | File        :   EPD_7in3f.h
* | Author      :   Waveshare team
* | Function    :   7.3inch e-Paper (F) Driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2022-10-20
* | Info        :
* -----------------------------------------------------------------------------
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/
#ifndef __EPD_7IN3F_H_
#define __EPD_7IN3F_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_7IN3F_WIDTH       800
#define EPD_7IN3F_HEIGHT      480

/**********************************
Color Index
**********************************/
#define EPD_7IN3F_BLACK   0x0   /// 000
#define EPD_7IN3F_WHITE   0x1   /// 001
#define EPD_7IN3F_GREEN   0x2   /// 010
#define EPD_7IN3F_BLUE    0x3   /// 011
#define EPD_7IN3F_RED     0x4   /// 100
#define EPD_7IN3F_YELLOW  0x5   /// 101
#define EPD_7IN3F_ORANGE  0x6   /// 110
#define EPD_7IN3F_CLEAN   0x7   /// 111   unavailable  Afterimage

void EPD_7IN3F_Init(void);
void EPD_7IN3F_Clear(UBYTE color);
void EPD_7IN3F_Show7Block(void);
void EPD_7IN3F_Display(const UBYTE *Image);
void EPD_7IN3F_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
void EPD_7IN3F_Sleep(void);

#endif
