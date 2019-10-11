/*****************************************************************************
* | File      	:	EPD_1in02.h
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-03-12
* | Info        :
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
#ifndef _EPD_1IN02_H_
#define _EPD_1IN02_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_WIDTH   80
#define EPD_HEIGHT  128

UBYTE EPD_Init(void);
void EPD_Clear(void);
void EPD_Display(UBYTE *Image);
void EPD_Display_Image(UBYTE *Image);
void EPD_DisplayPartial(UBYTE *Image1, UBYTE *Image2);
void EPD_Sleep(void);
void EPD_Part_Init(void);
void EPD_DisplayPartia_Clear(void);
#endif


