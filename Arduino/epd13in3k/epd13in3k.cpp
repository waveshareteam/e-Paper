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

const unsigned char Lut_Partial[]={										
0x15,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x2A,	0x88,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x15,	0x44,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x08,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x01,	0x01,	0x01,	0x00,						
0x0A,	0x00,	0x05,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x01,	0x01,						
0x22,	0x22,	0x22,	0x22,	0x22,						
0x17,	0x41,	0xA8,	0x32,	0x18,						
0x00,	0x00,									
};	

const unsigned char LUT_DATA_4Gray[112] =    //112bytes
{											
0x80,	0x48,	0x4A,	0x22,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x0A,	0x48,	0x68,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x88,	0x48,	0x60,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0xA8,	0x48,	0x45,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x07,	0x23,	0x17,	0x02,	0x00,						
0x05,	0x01,	0x05,	0x01,	0x02,						
0x08,	0x02,	0x01,	0x04,	0x04,						
0x00,	0x02,	0x00,	0x02,	0x01,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x00,						
0x00,	0x00,	0x00,	0x00,	0x01,						
0x22,	0x22,	0x22,	0x22,	0x22,						
0x17,	0x41,	0xA8,	0x32,	0x30,						
0x00,	0x00,	
};

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
    SendData(0x00);

    SendCommand(0x4F);
    SendData(0x00);
    SendData(0x00);

    ReadBusy();

    return 0;
}

int Epd::Init_Part(void) {
    unsigned int count;

    if (IfInit() != 0) {
        return -1;
    }
    Reset();
    ReadBusy();

    SendCommand(0x3C);  
	SendData(0x80);

    SendCommand(0x32);
    for(count = 0; count < 105 ; count++) {
        SendData(Lut_Partial[count]);
    }

    SendCommand(0x03);
	SendData(Lut_Partial[105]);

	SendCommand(0x04);  
	SendData(Lut_Partial[106]);
	SendData(Lut_Partial[107]);
	SendData(Lut_Partial[108]); 

	SendCommand(0x2C); 
	SendData(Lut_Partial[109]);

    SendCommand(0x37); 
    SendData(0x00);  
    SendData(0x00);  
    SendData(0x00);  
    SendData(0x00);  
    SendData(0x00); 
    SendData(0x40);  
    SendData(0x00);  
    SendData(0x00);   
    SendData(0x00);  
    SendData(0x00);  

    SendCommand(0x3C); 
    SendData(0x80);   

    SendCommand(0x22); 
    SendData(0xC0);   
    SendCommand(0x20); 

    ReadBusy();

    return 0;
}

int Epd::Init_4GRAY(void) {
    unsigned int count;

    if (IfInit() != 0) {
        return -1;
    }
    ReadBusy();   
	SendCommand(0x12);  //SWRESET
	ReadBusy();   

	SendCommand(0x0C); //set soft start     
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

	SendCommand(0x3C);        // Border       Border setting 
	SendData(0x00);

	SendCommand(0x18); // use the internal temperature sensor
	SendData(0x80);
	
	SendCommand(0x4E);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x4F);
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x32);
    for(count = 0; count < 105 ; count++) {
        SendData(LUT_DATA_4Gray[count]);
    }

    SendCommand(0x03);
	SendData(LUT_DATA_4Gray[105]);

	SendCommand(0x04);  
	SendData(LUT_DATA_4Gray[106]);
	SendData(LUT_DATA_4Gray[107]);
	SendData(LUT_DATA_4Gray[108]); 

	SendCommand(0x2C); 
	SendData(LUT_DATA_4Gray[109]);
	
	ReadBusy();

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

void Epd::TurnOnDisplay_Part(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xCF);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_4GRAY(void)
{
    SendCommand(0x22);
    SendData(0xC7);
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
    DelayMs(2);
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

void Epd::Display_Base(const unsigned char* frame_buffer) {
    
    SendCommand(0x24);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(frame_buffer[i + j * width/8]);
        }
    }

    SendCommand(0x26);
    for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width/8; i++) {
            SendData(frame_buffer[i + j * width/8]);
        }
    }

    TurnOnDisplay();
}

void Epd::Display_Part(unsigned char *Image, unsigned long x, unsigned long y, unsigned long w, unsigned long l)
{
	unsigned long i;
	unsigned long HEIGHT = l;
	unsigned long WIDTH =  (w % 8 == 0)? (w / 8 ): (w / 8 + 1);

    Reset();

	SendCommand(0x18); // use the internal temperature sensor
	SendData(0x80);

	SendCommand(0x3C);        // Border       Border setting 
	SendData(0x80);

    SendCommand(0x44); 
    SendData(x & 0xFF);
    SendData((x>>8) & 0x03);  
    SendData((x+w-1) & 0xFF);
    SendData(((x+w-1)>>8) & 0x03);
    
    SendCommand(0x45); 
    SendData(y & 0xFF);
    SendData((y>>8) & 0x03);
    SendData((y+l-1) & 0xFF);
    SendData(((y+l-1)>>8) & 0x03);

	SendCommand(0x4E);
    SendData(x & 0xFF);
    SendData((x>>8) & 0x03);  

    SendCommand(0x4F);
    SendData(y & 0xFF);
    SendData((y>>8) & 0x03);

	SendCommand(0x24);   //write RAM for black(0)/white (1)
	for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH; i++) {
            SendData(Image[i + j * WIDTH]);
        }
    }
	TurnOnDisplay_Part();	
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
    SendData(0x03);
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


