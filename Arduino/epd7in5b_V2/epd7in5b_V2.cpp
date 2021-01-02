/**
 *  @filename   :   epd7in5b_V2.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     Nov 30 2020
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdlib.h>
#include "epd7in5b_V2.h"

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
};

int Epd::Init(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    SendCommand(0x01);			//POWER SETTING
    SendData(0x07);
    SendData(0x07);    //VGH=20V,VGL=-20V
    SendData(0x3f);		//VDH=15V
    SendData(0x3f);		//VDL=-15V

    SendCommand(0x04); //POWER ON
    DelayMs(100);
    WaitUntilIdle();

    SendCommand(0X00);			//PANNEL SETTING
    SendData(0x0F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    SendCommand(0x61);        	//tres
    SendData(0x03);		//source 800
    SendData(0x20);
    SendData(0x01);		//gate 480
    SendData(0xE0);

    SendCommand(0X15);
    SendData(0x00);

    SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    SendData(0x11);
    SendData(0x07);

    SendCommand(0X60);			//TCON SETTING
    SendData(0x22);

    SendCommand(0x65);  // Resolution setting
    SendData(0x00);
    SendData(0x00);//800*480
    SendData(0x00);
    SendData(0x00);
	
    return 0;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command) {
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd::SendData(unsigned char data) {
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 
 */
void Epd::WaitUntilIdle(void) {
	unsigned char busy;
	do	{
		SendCommand(0x71);
		busy = DigitalRead(busy_pin);
		busy =!(busy & 0x01);        
	}while(busy);
	DelayMs(200);      
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
}

void Epd::Displaypart(const unsigned char* pbuffer, unsigned long xStart,         unsigned long yStart,\
                      unsigned long Picture_Width,  unsigned long Picture_Height, unsigned char Block) {
    if(Block == 0){
        SendCommand(0x10);
    }else if(Block == 1){
        SendCommand(0x13);
    }
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
                if(Block == 0){
                    SendData((pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]))) );
                }else{
                    SendData(~(pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]))) );
                }
            }else {
              if(Block == 0){
                SendData(0xff);
              }else {
                SendData(0x00);
              }
                
            }
        }
    }
    if(Block == 1){
        SendCommand(0x12);
        DelayMs(100);
        WaitUntilIdle();
    }

}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use EPD_Reset() to awaken
 */
void Epd::Sleep(void) {
    SendCommand(0X02);
    WaitUntilIdle();
    SendCommand(0X07);
    SendData(0xa5);
}

/* END OF FILE */


