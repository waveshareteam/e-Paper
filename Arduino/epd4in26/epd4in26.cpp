/**
 *  @filename   :   epd4in26.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     23-12-20
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
#include "epd4in26.h"

const unsigned char LUT_DATA_4Gray[112] =    //112bytes
{											
0x80,	0x48,	0x4A,	0x22,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x0A,	0x48,	0x68,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x88,	0x48,	0x60,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0xA8,	0x48,	0x45,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	0x00,	
0x07,	0x1E,	0x1C,	0x02,	0x00,						
0x05,	0x01,	0x05,	0x01,	0x02,						
0x08,	0x01,	0x01,	0x04,	0x04,						
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
    WIDTH = EPD_WIDTH;
    HEIGHT = EPD_HEIGHT;
};

int Epd::Init(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();
    DelayMs(100);

    ReadBusy();   
    SendCommand(0x12);  //SWRESET
    ReadBusy();   
    
    SendCommand(0x18); // use the internal temperature sensor
    SendData(0x80);

    SendCommand(0x0C); //set soft start     
    SendData(0xAE);
    SendData(0xC7);
    SendData(0xC3);
    SendData(0xC0);
    SendData(0x80);

    SendCommand(0x01);   //      drive output control    
    SendData((HEIGHT-1)%256); //  Y  
    SendData((HEIGHT-1)/256); //  Y 
    SendData(0x02);

    SendCommand(0x3C);        // Border       Border setting 
    SendData(0x01);

    SendCommand(0x11);        //    data  entry  mode
    SendData(0x01);           //       X-mode  x+ y-    

    SetWindows(0, HEIGHT-1, WIDTH-1, 0);

    SetCursor(0, 0);

    ReadBusy();

    return 0;
}

int Epd::Init_Fast(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();
	DelayMs(100);

	ReadBusy();   
	SendCommand(0x12);  //SWRESET
	ReadBusy();   
	
	SendCommand(0x18); // use the internal temperature sensor
	SendData(0x80);

	SendCommand(0x0C); //set soft start     
	SendData(0xAE);
	SendData(0xC7);
	SendData(0xC3);
	SendData(0xC0);
	SendData(0x80);

	SendCommand(0x01);   //      drive output control    
	SendData((HEIGHT-1)%256); //  Y  
	SendData((HEIGHT-1)/256); //  Y 
	SendData(0x02);

	SendCommand(0x3C);        // Border       Border setting 
	SendData(0x01);

	SendCommand(0x11);        //    data  entry  mode
	SendData(0x01);           //       X-mode  x+ y-    

	SetWindows(0, HEIGHT-1, WIDTH-1, 0);

	SetCursor(0, 0);

	ReadBusy();

	//TEMP (1.5s)
	SendCommand(0x1A);  
    SendData(0x5A); 

    SendCommand(0x22);  
    SendData(0x91); 
    SendCommand(0x20); 
	
	ReadBusy();

    return 0;
}

int Epd::Init_4GRAY(void) {
    unsigned int count;

    if (IfInit() != 0) {
        return -1;
    }
    Reset();
	DelayMs(100);

	ReadBusy();   
	SendCommand(0x12);  //SWRESET
	ReadBusy();   
	
	SendCommand(0x18); // use the internal temperature sensor
	SendData(0x80);

	SendCommand(0x0C); //set soft start     
	SendData(0xAE);
	SendData(0xC7);
	SendData(0xC3);
	SendData(0xC0);
	SendData(0x80);

	SendCommand(0x01);   //      drive output control    
	SendData((WIDTH-1)%256); //  Y  
	SendData((WIDTH-1)/256); //  Y 
	SendData(0x02);

	SendCommand(0x3C);        // Border       Border setting 
	SendData(0x01);

	SendCommand(0x11);        //    data  entry  mode
	SendData(0x01);           //       X-mode  x+ y-    

	SetWindows(0, HEIGHT-1, WIDTH-1, 0);

	SetCursor(0, 0);

	ReadBusy();

    
    SendCommand(0x32); //vcom
    for(count = 0; count < 105 ; count++) {
        SendData(LUT_DATA_4Gray[count]);
    }

    SendCommand(0x03); //VGH      
	SendData(LUT_DATA_4Gray[105]);

	SendCommand(0x04); //      
	SendData(LUT_DATA_4Gray[106]); //VSH1   
	SendData(LUT_DATA_4Gray[107]); //VSH2   
	SendData(LUT_DATA_4Gray[108]); //VSL   

	SendCommand(0x2C);     //VCOM Voltage
	SendData(LUT_DATA_4Gray[109]);    //0x1C

    return 0;
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
void Epd::SetWindows(unsigned long Xstart, unsigned long Ystart, unsigned long Xend, unsigned long Yend)
{
    SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    SendData(Xstart & 0xFF);
    SendData((Xstart>>8) & 0x03);
    SendData(Xend & 0xFF);
    SendData((Xend>>8) & 0x03);
	
    SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    SendData(Ystart & 0xFF);
    SendData((Ystart>>8) & 0x03);
    SendData(Yend & 0xFF);
    SendData((Yend>>8) & 0x03);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
void Epd::SetCursor(unsigned long Xstart, unsigned long Ystart)
{
    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);
    SendData((Xstart>>8) & 0x03);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart>>8) & 0x03);
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
    while(1)
    {	 //=1 BUSY
      if(DigitalRead(busy_pin)==0) 
        break;
      DelayMs(20);
    }
    Serial.print("e-Paper Busy Release\r\n ");
    DelayMs(20);
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

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void Epd::TurnOnDisplay(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xF7);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_Fast(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xC7);
	SendCommand(0x20); //Activate Display Update Sequence
	ReadBusy();
}

void Epd::TurnOnDisplay_Part(void)
{
	SendCommand(0x22); //Display Update Control
	SendData(0xFF);
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

void Epd::Display(const unsigned char* frame_buffer) {
    
    SendCommand(0x24);
    for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH/8; i++) {
            SendData(frame_buffer[i + j * WIDTH/8]);
        }
    }

    TurnOnDisplay();
}

void Epd::Displaypart(const unsigned char* pbuffer, unsigned long xStart, unsigned long yStart,unsigned long Picture_Width,unsigned long Picture_Height) {
    SendCommand(0x24);
    // xStart = xStart/8;
    // xStart = xStart*8;
    for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH/8; i++) {
            if( (j>=yStart) && (j<yStart+Picture_Height) && (i*8>=xStart) && (i*8<xStart+Picture_Width)){
                SendData(pgm_read_byte(&(pbuffer[i-xStart/8 + (Picture_Width)/8*(j-yStart)])) );
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
    for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH/8; i++) {
            SendData(frame_buffer[i + j * WIDTH/8]);
        }
    }

    SendCommand(0x26);
    for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH/8; i++) {
            SendData(frame_buffer[i + j * WIDTH/8]);
        }
    }

    TurnOnDisplay();
}

void Epd::Display_Fast(const unsigned char* frame_buffer) {
    
    SendCommand(0x24);
    for (unsigned long j = 0; j < HEIGHT; j++) {
        for (unsigned long i = 0; i < WIDTH/8; i++) {
            SendData(frame_buffer[i + j * WIDTH/8]);
        }
    }

    TurnOnDisplay_Fast();
}

void Epd::Display_Part(unsigned char *Image, unsigned long x, unsigned long y, unsigned long w, unsigned long l)
{
	unsigned long i;
	unsigned long height = l;
	unsigned long width =  (w % 8 == 0)? (w / 8 ): (w / 8 + 1);

    Reset();

	SendCommand(0x18); // use the internal temperature sensor
	SendData(0x80);

	SendCommand(0x3C);        // Border       Border setting 
	SendData(0x80);

	SetWindows(x, y, x+w-1, y+l-1);

	SetCursor(x, y);

	SendCommand(0x24);   //write RAM for black(0)/white (1)
	for (unsigned long j = 0; j < height; j++) {
        for (unsigned long i = 0; i < width; i++) {
            SendData(Image[i + j * width]);
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
    for(unsigned long i=0; i<HEIGHT*WIDTH; i++) {
        SendData(0xff);
    }
    // SendCommand(0x26);
    // for(unsigned long i=0; i<HEIGHT*WIDTH; i++)	{
    //     SendData(0xff);
    // }
    TurnOnDisplay();
}


/* END OF FILE */


