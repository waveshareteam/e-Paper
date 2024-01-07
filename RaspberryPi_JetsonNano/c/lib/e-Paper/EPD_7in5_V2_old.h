/*****************************************************************************
* | File      	:	EPD_7in5_V2.h
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-12-18
* | Info        :   
* 1.Remove:ImageBuff[EPD_HEIGHT * EPD_WIDTH / 8]
* 2.Change:EPD_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
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
#ifndef _EPD_7IN5_V2_H_
#define _EPD_7IN5_V2_H_

#include "DEV_Config.h"


// Display resolution
#define EPD_7IN5_V2_WIDTH       800
#define EPD_7IN5_V2_HEIGHT      480

UBYTE EPD_7IN5_V2_Init(void);
UBYTE EPD_7IN5_V2_Init_Fast(void);
UBYTE EPD_7IN5_V2_Init_Partial(void);
void EPD_7IN5_V2_Clear(void);
void EPD_7IN5_V2_ClearBlack(void);
void EPD_7IN5_V2_Display(UBYTE *blackimage);
void EPD_7IN5_V2_Display_Partial(UBYTE *blackimage,UDOUBLE x_start, UDOUBLE y_start, UDOUBLE x_end, UDOUBLE y_end);
void EPD_7IN5_V2_Sleep(void);

#endif
