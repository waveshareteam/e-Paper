/*****************************************************************************
* | File      	:  	EPD_4in37b.c
* | Author      :   Waveshare team
* | Function    :   4.37inch e-paper b
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-01-07
* | Info        :
* -----------------------------------------------------------------------------
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
#ifndef __EPD_4IN37B_H_
#define __EPD_4IN37B_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_4IN37B_WIDTH       176
#define EPD_4IN37B_HEIGHT      480


void EPD_4IN37B_Init(void);
void EPD_4IN37B_Display(UBYTE *ImageBlack, UBYTE*ImageRed);
void EPD_4IN37B_Display_Part(UBYTE *ImageBlack, UBYTE*ImageRed, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void EPD_4IN37B_Clear(void);
void EPD_4IN37B_Sleep(void);
#endif
