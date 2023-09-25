/**
 *  @filename   :   epd13in3k.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 25 2023
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
#include "epd13in3k.h"

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
    ReadBusy();

    SendCommand(0x12);
    ReadBusy();

    SendCommand(0x0C);
    SendData(0xAE);
    SendData(0xC7);
    SendData(0xC3);
    SendData(0xC0);
    SendData(0x80);

    SendCommand(0x01); 
    SendData(0xA7);
    SendData(0x02);  
    SendData(0x00);

    SendCommand(0x11); 
    SendData(0x03);

    SendCommand(0x44); 
    SendData(0x00);
    SendData(0x00);  
    SendData(0xBF);
    SendData(0x03); 
    
    SendCommand(0x45); 
    SendData(0x00);
    SendData(0x00); 
    SendData(0xA7);
    SendData(0x02);

    SendCommand(0x3C);
    SendData(0x05);

    SendCommand(0x18);
    SendData(0x80);

    SendCommand(0x4E);
    SendData(0x00);

    SendCommand(0x4F);
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
void Epd::ReadBusy(void) {
    unsigned char busy;
    Serial.print("e-Paper Busy\r\n ");
    do{
        DelayMs(20);
        busy = DigitalRead(busy_pin);
    }while(busy == 1);
    Serial.print("e-Paper Busy Release\r\n ");
    DelayMs(20);
}

/**
 *  @brief: Refresh function
 */
void Epd::TurnOnDisplay(void) {
    SendCommand(0x22);
    SendData(0xF7);
    SendCommand(0x20);  
    ReadBusy();
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20); 
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(4);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
}

void Epd::DisplayFrame(const unsigned char* frame_buffer) {
    
    SendCommand(0x24);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(frame_buffer[i + j * width/8]);
        }
    }
    TurnOnDisplay();
}

void Epd::Displaypart(const unsigned char* pbuffer, unsigned long xStart, unsigned long yStart,unsigned long Picture_Width,unsigned long Picture_Height) {
    SendCommand(0x24);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
                SendData((pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)]))) );
                // SendData(0xff);
            }else {
                SendData(0xFF);
            }
        }
    }
    TurnOnDisplay();
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
    SendCommand(0x10);
    SendData(0x01);
    DelayMs(100);
    ReadBusy();
}

void Epd::Clear(void) {
    
    SendCommand(0x24);
    for(unsigned long i=0; i<height*width/8; i++)	{
        SendData(0xFF);
    }
    TurnOnDisplay();
}


/* END OF FILE */


