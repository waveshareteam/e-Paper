/*****************************************************************************
* | File      	:   epd2in13_V4.cpp
* | Author      :   Waveshare team
* | Function    :   2.13inch e-paper V4
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-6-25
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
#include <stdlib.h>
#include "epd2in13_V4.h"

Epd::~Epd()
{
};

/******************************************************************************
function :	Pin definition
parameter:
******************************************************************************/
Epd::Epd()
{
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    bufwidth = 128/8;  //16
    bufheight = 63;
};

/******************************************************************************
function :	send command
parameter:
     command : Command register
******************************************************************************/
void Epd::SendCommand(unsigned char command)
{
    DigitalWrite(dc_pin, LOW);
    SpiTransfer(command);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void Epd::SendData(unsigned char data)
{
    DigitalWrite(dc_pin, HIGH);
    SpiTransfer(data);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void Epd::WaitUntilIdle(void)
{
    while(1) {      //LOW: idle, HIGH: busy
        if(DigitalRead(busy_pin) == 0)
            break;
        DelayMs(10);
    }
}

/******************************************************************************
function :	Setting the display window
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
	Xend : End position of X-axis
	Yend : End position of Y-axis
******************************************************************************/
void Epd::SetWindows(unsigned char Xstart, unsigned char Ystart, unsigned char Xend, unsigned char Yend)
{
    SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    SendData((Xstart>>3) & 0xFF);
    SendData((Xend>>3) & 0xFF);
	
    SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
    SendData(Yend & 0xFF);
    SendData((Yend >> 8) & 0xFF);
}

/******************************************************************************
function :	Set Cursor
parameter:
	Xstart : X-axis starting position
	Ystart : Y-axis starting position
******************************************************************************/
void Epd::SetCursor(unsigned char Xstart, unsigned char Ystart)
{
    SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    SendData(Xstart & 0xFF);

    SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    SendData(Ystart & 0xFF);
    SendData((Ystart >> 8) & 0xFF);
}



/******************************************************************************
function :	Initialize the e-Paper register
parameter:
	Mode : Mode selection
******************************************************************************/
int Epd::Init(char Mode)
{
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }
    
    Reset();
    
    int count;
    if(Mode == FULL) {
        WaitUntilIdle();
        SendCommand(0x12); // soft reset
        WaitUntilIdle();

        SendCommand(0x01); //Driver output control
        SendData(0xF9);
        SendData(0x00);
        SendData(0x00);

        SendCommand(0x11); //data entry mode
        SendData(0x03);

		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(0x3C); //BorderWavefrom
		SendData(0x05);	

		SendCommand(0x21); //  Display update control
		SendData(0x00);
		SendData(0x80);	

		SendCommand(0x18); //Read built-in temperature sensor
		SendData(0x80);	
		WaitUntilIdle();
    } 
    else if(Mode == FAST) {
        WaitUntilIdle();
        SendCommand(0x12); // soft reset
        WaitUntilIdle();

        SendCommand(0x18); // Read built-in temperature sensor
		SendData(0x80);	

        SendCommand(0x11); // data entry mode
        SendData(0x03);

		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
		
		SendCommand(0x22); // Load temperature value
		SendData(0xB1);	
        SendCommand(0x20);
        WaitUntilIdle();

		SendCommand(0x1A); //  Write to temperature register
		SendData(0x64);
		SendData(0x00);	

        SendCommand(0x22); //  Load temperature value
		SendData(0x91);
		SendCommand(0x20);	
		WaitUntilIdle();
    }
    else if(Mode == PART) {	
		DigitalWrite(reset_pin, LOW);                //module reset
		DelayMs(1);
		DigitalWrite(reset_pin, HIGH);
		
		SendCommand(0x3C); //BorderWavefrom
		SendData(0x80);	

        SendCommand(0x01); //Driver output control
		SendData(0xF9);	
        SendData(0x00);	
        SendData(0x00);	
	
		SendCommand(0x11); // data entry mode
        SendData(0x03); 
		
		SetWindows(0, 0, EPD_WIDTH-1, EPD_HEIGHT-1);
		SetCursor(0, 0);
    } else {
        return -1;
    }

    return 0;
}

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void Epd::Reset(void)
{
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    DigitalWrite(reset_pin, LOW);                //module reset
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);
    this->count = 0; 
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(void)
{
    int w, h;
    w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;
    SendCommand(0x24);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0xff);
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0xf7);
    SendCommand(0x20);
    WaitUntilIdle();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::Display(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0xf7);
    SendCommand(0x20);
    WaitUntilIdle();
}

void Epd::Display1(const unsigned char* frame_buffer) {
    if(this->count == 0){
        SendCommand(0x24);
        this->count++;
    }else if(this->count > 0 && this->count < 4 ){
        this->count++;
    }
    for(int i = 0; i < this->bufwidth * this->bufheight; i++){
            SendData(frame_buffer[i]);
    }
    if(this->count == 4){
        SendCommand(0x22);
        SendData(0xf7);
        SendCommand(0x20);
        WaitUntilIdle();
        this->count = 0;
    }
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and fast displays
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::Display_Fast(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0xC7);
    SendCommand(0x20);
    WaitUntilIdle();
}




/******************************************************************************
function :	Refresh a base image
parameter:
	frame_buffer : Image data	
******************************************************************************/
void Epd::DisplayPartBaseImage(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }

        SendCommand(0x26);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0xf7);
    SendCommand(0x20);
    WaitUntilIdle();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and partial refresh
parameter:
	frame_buffer : Image data
******************************************************************************/
void Epd::DisplayPart(const unsigned char* frame_buffer)
{
    int w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    int h = EPD_HEIGHT;

    if (frame_buffer != NULL) {
        SendCommand(0x24);
        for (int j = 0; j < h; j++) {
            for (int i = 0; i < w; i++) {
                SendData(pgm_read_byte(&frame_buffer[i + j * w]));
            }
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0xff);
    SendCommand(0x20);
    WaitUntilIdle();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::ClearPart(void)
{
    int w, h;
    w = (EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1);
    h = EPD_HEIGHT;
    SendCommand(0x24);
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            SendData(0xff);
        }
    }

    //DISPLAY REFRESH
    SendCommand(0x22);
    SendData(0x0f);
    SendCommand(0x20);
    WaitUntilIdle();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep()
{
    SendCommand(0x10); //enter deep sleep
    SendData(0x01);
    DelayMs(200);

    DigitalWrite(reset_pin, LOW);
}

/* END OF FILE */


