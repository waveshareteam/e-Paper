/*****************************************************************************
* | File        :   EPD_4in0e.h
* | Author      :   Waveshare team
* | Function    :   4inch e-Paper (E) Driver
* | Info        :
*----------------
* | This version:   V1.0
* | Date        :   2024-08-20
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
#ifndef __EPD_4IN0E_H_
#define __EPD_4IN0E_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_4IN0E_WIDTH       400
#define EPD_4IN0E_HEIGHT      600

/**********************************
Color Index
**********************************/
#define EPD_4IN0E_BLACK   0x0   /// 000
#define EPD_4IN0E_WHITE   0x1   /// 001
#define EPD_4IN0E_YELLOW  0x2   /// 010
#define EPD_4IN0E_RED     0x3   /// 011
#define EPD_4IN0E_BLUE    0x5   /// 101
#define EPD_4IN0E_GREEN   0x6   /// 110

void EPD_4IN0E_Init(void);
void EPD_4IN0E_Clear(UBYTE color);
void EPD_4IN0E_Show7Block(void);
void EPD_4IN0E_Show(void);
void EPD_4IN0E_Display(UBYTE *Image);
void EPD_4IN0E_Sleep(void);

#endif
