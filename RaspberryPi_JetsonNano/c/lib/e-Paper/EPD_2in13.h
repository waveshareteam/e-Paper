/*****************************************************************************
* | File      	:   EPD_2IN13.h
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2019-06-12
* | Info        :
* -----------------------------------------------------------------------------
* V3.0(2019-06-12):
* 1.Change:
*    EPD_Reset() => EPD_2IN13_Reset()
*    EPD_SendCommand() => EPD_2IN13_SendCommand()
*    EPD_SendData() => EPD_2IN13_SendData()
*    EPD_WaitUntilIdle() => EPD_2IN13_ReadBusy()
*    EPD_Init() => EPD_2IN13_Init()
*    EPD_Clear() => EPD_2IN13_Clear()
*    EPD_Display() => EPD_2IN13_Display()
*    EPD_Sleep() => EPD_2IN13_Sleep()
* -----------------------------------------------------------------------------
* V2.0(2019-01-03):
* 1.Remove:ImageBuff[EPD_2IN13_HEIGHT * EPD_2IN13_WIDTH / 8]
* 2.Change:EPD_Display(UBYTE *Image)
*   Need to pass parameters: pointer to cached data
* 3.Change:
*   EPD_RST -> EPD_RST_PIN
*   EPD_DC -> EPD_DC_PIN
*   EPD_CS -> EPD_CS_PIN
*   EPD_BUSY -> EPD_BUSY_PIN
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
#ifndef _EPD2IN13_H
#define _EPD2IN13_H

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN13_WIDTH       122
#define EPD_2IN13_HEIGHT      250

#define EPD_2IN13_FULL			0
#define EPD_2IN13_PART			1

void EPD_2IN13_Init(UBYTE Mode);
void EPD_2IN13_Clear(void);
void EPD_2IN13_Display(UBYTE *Image);
void EPD_2IN13_Sleep(void);

#endif
