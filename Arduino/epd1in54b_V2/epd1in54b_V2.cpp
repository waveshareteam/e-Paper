/**
 *  @filename   :   epd1in54b.cpp
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
#include "epd1in54b_V2.h"

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
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    /* EPD hardware init start */
    Reset();
   
    WaitUntilIdle();   
    SendCommand(0x12);  //SWRESET
    WaitUntilIdle();   

    SendCommand(0x01); //Driver output control      
    SendData(0xC7);
    SendData(0x00);
    SendData(0x01);

    SendCommand(0x11); //data entry mode       
    SendData(0x01);

    SendCommand(0x44); //set Ram-X address start/end position   
    SendData(0x00);
    SendData(0x18);    //0x18-->(24+1)*8=200

    SendCommand(0x45); //set Ram-Y address start/end position          
    SendData(0xC7);    //0xC7-->(199+1)=200
    SendData(0x00);
    SendData(0x00);
    SendData(0x00); 

    SendCommand(0x3C); //BorderWavefrom
    SendData(0x05);

    SendCommand(0x18); //Read built-in temperature sensor
    SendData(0x80);

    SendCommand(0x4E);   // set RAM x address count to 0;
    SendData(0x00);
    SendCommand(0x4F);   // set RAM y address count to 0X199;    
    SendData(0xC7);
    SendData(0x00);
    WaitUntilIdle();
   
    /* EPD hardware init end */

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
    while(1) {
        if(DigitalRead(busy_pin) == 0)
            break;
        DelayMs(100);
    }      
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
    DelayMs(10);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);    
}

/**
 *  @brief: set the look-up tables
 */
void Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    unsigned char temp;
    unsigned int i;
    
    SendCommand(0x24);
    for(i=0;i<this->width * this->height / 8;i++)
    {               
        SendData(pgm_read_byte(&frame_buffer_black[i]));
    }
    SendCommand(0x26);
    for(i=0;i<this->width * this->height / 8;i++)
    {               
        SendData(~pgm_read_byte(&frame_buffer_red[i]));
    }
    
    SendCommand(0x22);
    SendData(0xF7);
    SendCommand(0x20);
    WaitUntilIdle();
}

void Epd::DisplayClear() {
    unsigned char temp;
    unsigned int i;
    
    SendCommand(0x24);
    for(i=0;i<this->width * this->height / 8;i++)
    {               
        SendData(0xff);
    }
    SendCommand(0x26);
    for(i=0;i<this->width * this->height / 8;i++)
    {               
        SendData(0x00);
    }
    
    SendCommand(0x22);
    SendData(0xF7);
    SendCommand(0x20);
    WaitUntilIdle();
}
/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use Epd::Init() to awaken
 */
void Epd::Sleep() {
    SendCommand(0x10);         //power setting
    SendData(0x01);        //gate switch to external
    DelayMs(100);
}



/* END OF FILE */


