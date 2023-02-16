/*****************************************************************************
* | File      	:   epd2in13_V3.h
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V3
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2021-11-01
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

#ifndef epd2in13_V3
#define epd2in13_V3

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       122
#define EPD_HEIGHT      250

#define FULL			0
#define PART			1

class Epd : EpdIf {
public:
    unsigned long width;
    unsigned long height;
    int bufwidth;
    int bufheight;
    int count;

    Epd();
    ~Epd();
    int  Init(char Mode);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
	void SetWindows(unsigned char Xstart, unsigned char Ystart, unsigned char Xend, unsigned char Yend);
	void SetCursor(unsigned char Xstart, unsigned char Ystart);
	void Lut(unsigned char *lut);
    void Reset(void);
    void Clear(void);
    void Display(const unsigned char* frame_buffer);
    void Display1(const unsigned char* frame_buffer);
    void DisplayPartBaseImage(const unsigned char* frame_buffer);
    void DisplayPart(const unsigned char* frame_buffer);
    void ClearPart(void);
    
    void Sleep(void);
private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD2IN13_V3_H */

/* END OF FILE */
