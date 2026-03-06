/*****************************************************************************
* | File      	:   EPD_3IN97.h
* | Author      :   Waveshare team
* | Function    :   1.54inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2019-06-11
* | Info        :   
#
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
#ifndef __EPD_3IN97_H_
#define __EPD_3IN97_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN97_WIDTH       800
#define EPD_3IN97_HEIGHT      480

void EPD_3IN97_Init(void);
void EPD_3IN97_Init_Fast(void);
void EPD_3IN97_Init_Partial(void);
void EPD_3IN97_Init_4GRAY(void);
void EPD_3IN97_Clear(void);
void EPD_3IN97_Clear_Black(void);
void EPD_3IN97_Display(const UBYTE *Image);
void EPD_3IN97_Display_Base(const UBYTE *Image);
void EPD_3IN97_Display_Fast(const UBYTE *Image);
void EPD_3IN97_Display_Fast_Base(const UBYTE *Image);
void EPD_3IN97_V2_Display_Window(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
void EPD_3IN97_V2_Display_Window_Base(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
void EPD_3IN97_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend);
void EPD_3IN97_Display_4Gray(const UBYTE *Image);
void EPD_3IN97_WritePicture_4Gray(const UBYTE *Image);
void EPD_3IN97_Sleep(void);

#endif
