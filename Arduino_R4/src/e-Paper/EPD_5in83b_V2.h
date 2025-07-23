/*****************************************************************************
* | File      	:	EPD_5in83b_V2.h
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-07-04
* | Info        :   
******************************************************************************
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
#ifndef _EPD_5IN83B_V2_h_
#define _EPD_5IN83B_V2_h_

#include "../Config/Debug.h"
#include "../Config/DEV_Config.h"


// Display resolution
#define EPD_5IN83B_V2_WIDTH       648
#define EPD_5IN83B_V2_HEIGHT      480

UBYTE EPD_5IN83B_V2_Init(void);
void EPD_5IN83B_V2_Clear(void);
void EPD_5IN83B_V2_Display(const UBYTE *blackimage, const UBYTE *ryimage);
void EPD_5IN83B_V2_Sleep(void);
void EPD_5IN83B_V2_DisplayPicture(const UBYTE *blackimage,UBYTE Block);
void EPD_5IN83B_V2_Clear_C(UBYTE color);	
#endif
