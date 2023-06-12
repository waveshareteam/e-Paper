/*****************************************************************************
* | File      	:   EPD_2in13g.c
* | Author      :   Waveshare team
* | Function    :   2inch13 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-05-29
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
#include "EPD_2in13g.h"
#include <stdlib.h>
Epd::~Epd() {
};
Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    WIDTH = EPD_2IN13G_WIDTH;
    HEIGHT = EPD_2IN13G_HEIGHT;
};
/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void Epd::Reset(void)
{
    DigitalWrite(RST_PIN, 1);
    DelayMs(200);
    DigitalWrite(RST_PIN, 0);
    DelayMs(2);
    DigitalWrite(RST_PIN, 1);
    DelayMs(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
void Epd::SendCommand(UBYTE Reg)
{
    DigitalWrite(DC_PIN, 0);
    DigitalWrite(CS_PIN, 0);   
    SpiTransfer(Reg);
    DigitalWrite(CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
void Epd::SendData(UBYTE Data)
{
    DigitalWrite(DC_PIN, 1);
    DigitalWrite(CS_PIN, 0);
    SpiTransfer(Data);
    DigitalWrite(CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes HIGH
parameter:
******************************************************************************/
void Epd::ReadBusy(void)
{
    DelayMs(100);
    Serial.println("busy");
    while(DigitalRead(BUSY_PIN) != 1) {      //HIGH: idle, LOW: busy
        DelayMs(100);
    }
    Serial.println("idie");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
void Epd::TurnOnDisplay(void)
{

    SendCommand(0x12); // DISPLAY_REFRESH
    SendData(0x00);
    ReadBusy();

}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
void Epd::SetWindows(void)
{
    SendCommand(0x61); //TRES
    SendData(Source_BITS/256);		// EPD_2IN13G_WIDTH_H
    SendData(Source_BITS%256);		// EPD_2IN13G_WIDTH_L
    SendData(EPD_2IN13G_HEIGHT/256);			// EPD_2IN13G_HEIGHT_H
    SendData(EPD_2IN13G_HEIGHT%256); 		// EPD_2IN13G_HEIGHT_L	
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
int Epd::Init(void)
{
    if (IfInit() != 0) {
        return -1;
    }
    Reset();
    ReadBusy();
    SendCommand(0x4D);
    SendData(0x78);

    SendCommand(0x00);	//PSR
    SendData(0x0F);
    SendData(0x29);

    SendCommand(0x01);	//PWRR
    SendData(0x07);
    SendData(0x00);

    SendCommand(0x03);	//POFS
    SendData(0x10);
    SendData(0x54);
    SendData(0x44);

    SendCommand(0x06);	//BTST_P
    SendData(0x05);
    SendData(0x00);
    SendData(0x3F);
    SendData(0x0A);
    SendData(0x25);
    SendData(0x12);
    SendData(0x1A); 

    SendCommand(0x50);	//CDI
    SendData(0x37);

    SendCommand(0x60);	//TCON
    SendData(0x02);
    SendData(0x02);

    SetWindows();  // Setting the display window

    SendCommand(0xE7);
    SendData(0x1C);

    SendCommand(0xE3);	
    SendData(0x22);

    SendCommand(0xB4);
    SendData(0xD0);
    SendCommand(0xB5);
    SendData(0x03);

    SendCommand(0xE9);
    SendData(0x01); 

    SendCommand(0x30);
    SendData(0x08);  

    SendCommand(0x04);
    ReadBusy(); 
    return 0;

}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(UBYTE color)
{
    UWORD Width, Height;
    Width = Source_BITS / 4 ;
    Height = EPD_2IN13G_HEIGHT;

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            SendData((color << 6) | (color << 4) | (color << 2) | color);
        }
    }

    TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_2IN13G_WIDTH % 4 == 0)? (EPD_2IN13G_WIDTH / 4 ): (EPD_2IN13G_WIDTH / 4 + 1);
    Height = EPD_2IN13G_HEIGHT;

    SendCommand(0x10);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < (Source_BITS/4); i++) {
            if(i < 31 ) 
            SendData(pgm_read_byte(&Image[i + j * Width]));
            else SendData(0x00);
        }
    }
    TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::Sleep(void)
{
    SendCommand(0x02); //power off
    ReadBusy();       //waiting for the electronic paper IC to release the idle signal
    DelayMs(100);           //!!!The delay here is necessary,100mS at least!!! 
    
    SendCommand(0x07);  //deep sleep
    SendData(0XA5);
}
