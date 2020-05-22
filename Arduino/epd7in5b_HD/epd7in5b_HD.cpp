/**
 *  @filename   :   epd7in5b.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
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
#include "epd7in5b_HD.h"

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
    
    SendCommand(0x12); 		  //SWRESET
    WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    SendCommand(0x46);  // Auto Write RAM
    SendData(0xF7);
    WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    SendCommand(0x47);  // Auto Write RAM
    SendData(0xF7);
    WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    SendCommand(0x0C);  // Soft start setting
    SendData(0xAE);
    SendData(0xC7);
    SendData(0xC3);
    SendData(0xC0);
    SendData(0x40);   

    SendCommand(0x01);  // Set MUX as 527
    SendData(0xAF);
    SendData(0x02);
    SendData(0x01);

    SendCommand(0x11);  // Data entry mode
    SendData(0x01);

    SendCommand(0x44);
    SendData(0x00); // RAM x address start at 0
    SendData(0x00);
    SendData(0x6F); // RAM x address end at 36Fh -> 879
    SendData(0x03);
    SendCommand(0x45);
    SendData(0xAF); // RAM y address start at 20Fh;
    SendData(0x02);
    SendData(0x00); // RAM y address end at 00h;
    SendData(0x00);

    SendCommand(0x3C); // VBD
    SendData(0x01); // LUT1, for white

    SendCommand(0x18);
    SendData(0X80);
    SendCommand(0x22);
    SendData(0XB1);	//Load Temperature and waveform setting.
    SendCommand(0x20);
    WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    SendCommand(0x4E); 
    SendData(0x00);
    SendData(0x00);
    SendCommand(0x4F); 
    SendData(0xAF);
    SendData(0x02);

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
    while(DigitalRead(busy_pin)){
        // DelayMs(10);   
    }
    DelayMs(200);      
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);
}

void Epd::Displaypart(const unsigned char* pbuffer, unsigned long xStart,         unsigned long yStart,\
                      unsigned long Picture_Width,  unsigned long Picture_Height, unsigned char Block) {
    SendCommand(0x4F); 
    SendData(0xAf);
    SendData(0x02);
    if(Block == 0){
        SendCommand(0x24);
    }else if(Block == 1){
        SendCommand(0x26);
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
        SendCommand(0x22);
        SendData(0xC7);
        SendCommand(0x20);
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
    SendCommand(0x10);  	//deep sleep
    SendData(0x01);
}

/* END OF FILE */


