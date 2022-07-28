/*****************************************************************************
* | File      	:   EPD_3IN52.h
* | Author      :   Waveshare team
* | Function    :   3.52inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-05-07
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
#ifndef __EPD_3IN52_H_
#define __EPD_3IN52_H_

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       240
#define EPD_HEIGHT      360 

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE unsigned long

#define EPD_3IN52_WHITE                         0xFF  // 
#define EPD_3IN52_BLACK                         0x00  //
#define EPD_3IN52_Source_Line                   0xAA  //
#define EPD_3IN52_Gate_Line                     0x55  //
#define EPD_3IN52_UP_BLACK_DOWN_WHITE           0xF0  //
#define EPD_3IN52_LEFT_BLACK_RIGHT_WHITE        0x0F  //
#define EPD_3IN52_Frame                         0x01  // 
#define EPD_3IN52_Crosstalk                     0x02  //
#define EPD_3IN52_Chessboard                    0x03  //
#define EPD_3IN52_Image                         0x04  //


extern unsigned char EPD_3IN52_Flag;

class Epd : EpdIf {
public:
    Epd();
    ~Epd();
    int Init(void);
    void Reset(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void ReadBusy(void);
    void lut(void);
    void refresh(void);
    void lut_GC(void);
    void lut_DU(void);
    void display(UBYTE* picData);
    void display_part(UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
    void display_NUM(UBYTE NUM);
    void Clear(void);
    void sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    unsigned long width;
    unsigned long height;
    unsigned char EPD_3IN52_Flag;
};



#endif
