/*****************************************************************************
* | File      	:   EPD_5in0.c
* | Author      :   Waveshare team
* | Function    :   5inch e-paper 
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-11-04
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
#include "EPD_5in0.h"
#include "Debug.h"
#include <time.h> 

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in0_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_5in0_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_5in0_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_5in0_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    while(DEV_Digital_Read(EPD_BUSY_PIN) == 1) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(1);
    }
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display full
parameter:
******************************************************************************/
static void EPD_5in0_TurnOnDisplay(void)
{ 
    EPD_5in0_SendCommand(0x22);
    EPD_5in0_SendData(0xF7);
	EPD_5in0_SendCommand(0x20);
    EPD_5in0_ReadBusy();

}

static void EPD_5in0_TurnOnDisplay_Part(void)
{
    EPD_5in0_SendCommand(0x22);
    EPD_5in0_SendData(0xFF);
    EPD_5in0_SendCommand(0x20);
    EPD_5in0_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5in0_Init(void)
{
    EPD_5in0_Reset();

    EPD_5in0_ReadBusy();
    EPD_5in0_SendCommand(0x12);  //SWRESET
    EPD_5in0_ReadBusy();

    EPD_5in0_SendCommand(0x18);
    EPD_5in0_SendData(0x80);

    EPD_5in0_SendCommand(0x0C);
	EPD_5in0_SendData(0xAE);
	EPD_5in0_SendData(0xC7);
	EPD_5in0_SendData(0xC3);
	EPD_5in0_SendData(0xC0);
	EPD_5in0_SendData(0x80);   //0x80 0x20

    EPD_5in0_SendCommand(0x01); //Driver output control
    EPD_5in0_SendData((EPD_5in0_HEIGHT-1)%256);
    EPD_5in0_SendData((EPD_5in0_HEIGHT-1)/256);
    EPD_5in0_SendData(0x02);


    EPD_5in0_SendCommand(0x11); //data entry mode       
	EPD_5in0_SendData(0x01);

	EPD_5in0_SendCommand(0x44); //set Ram-X address start/end position   
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendData((EPD_5in0_WIDTH-1)%256);    
	EPD_5in0_SendData((EPD_5in0_WIDTH-1)/256);

	EPD_5in0_SendCommand(0x45); //set Ram-Y address start/end position    
    EPD_5in0_SendData((EPD_5in0_HEIGHT-1)%256);    
	EPD_5in0_SendData((EPD_5in0_HEIGHT-1)/256);  
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendData(0x00);

    EPD_5in0_SendCommand(0x4E);   // set RAM x address count to 0;
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendCommand(0x4F);   // set RAM y address count to 0X199;    
	EPD_5in0_SendData(0x00);
	EPD_5in0_SendData(0x00);
    EPD_5in0_ReadBusy();

    EPD_5in0_SendCommand(0x3C); //BorderWavefrom
    EPD_5in0_SendData(0x01);


}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in0_Clear(void)
{
    UWORD Width, Height;
    Width = (EPD_5in0_WIDTH % 8 == 0)? (EPD_5in0_WIDTH / 8 ): (EPD_5in0_WIDTH / 8 + 1);
    Height = EPD_5in0_HEIGHT;

    EPD_5in0_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(0XFF);
        }
    }
    EPD_5in0_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(0XFF);
        }
    }
    EPD_5in0_TurnOnDisplay();
}

void EPD_5in0_Clear_Black(void)
{
    UWORD Width, Height;
    Width = (EPD_5in0_WIDTH % 8 == 0)? (EPD_5in0_WIDTH / 8 ): (EPD_5in0_WIDTH / 8 + 1);
    Height = EPD_5in0_HEIGHT;

    EPD_5in0_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(0X00);
        }
    }
    EPD_5in0_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(0X00);
        }
    }
    EPD_5in0_TurnOnDisplay();
}


/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in0_Display(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_5in0_WIDTH % 8 == 0)? (EPD_5in0_WIDTH / 8 ): (EPD_5in0_WIDTH / 8 + 1);
    Height = EPD_5in0_HEIGHT;

    EPD_5in0_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(Image[i + j * Width]);
        }
    }
    EPD_5in0_TurnOnDisplay();
}

void EPD_5in0_Display_Base(const UBYTE *Image)
{
    UWORD Width, Height;
    Width = (EPD_5in0_WIDTH % 8 == 0)? (EPD_5in0_WIDTH / 8 ): (EPD_5in0_WIDTH / 8 + 1);
    Height = EPD_5in0_HEIGHT;

    EPD_5in0_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(Image[i + j * Width]);
        }
    }
    
    EPD_5in0_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_5in0_SendData(Image[i + j * Width]);
        }
    }
    EPD_5in0_TurnOnDisplay();
}

void EPD_5in0_Display_Base_fImage(UBYTE *Image)
{
	UWORD i, j, k;
	UWORD height = EPD_5in0_HEIGHT;
	UWORD width = EPD_5in0_WIDTH/8;
	
	EPD_5in0_SendCommand(0x24);   //write RAM for black(0)/white (1)
    k = 0 ;
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
        {
            if(i< height/2 & j< width/2)
                EPD_5in0_SendData(Image[k++]);
            else
                EPD_5in0_SendData(0xff);
        }
	}

	EPD_5in0_SendCommand(0x26);   //write RAM for black(0)/white (1)
	k = 0 ;
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
        {
            if(i< height/2 & j< width/2)
                EPD_5in0_SendData(Image[k++]);
            else
                EPD_5in0_SendData(0xff);
        }
	}
	EPD_5in0_TurnOnDisplay();	
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in0_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
	if((Xstart % 8 + Xend % 8 == 8 && Xstart % 8 > Xend % 8) || Xstart % 8 + Xend % 8 == 0 || (Xend - Xstart)%8 == 0)
    {
        Xstart = Xstart / 8 ;
        Xend = Xend / 8;
    }
    else
    {
        Xstart = Xstart / 8 ;
        Xend = Xend % 8 == 0 ? Xend / 8 : Xend / 8 + 1;
    }
    

    UWORD i, Width;
	Width = Xend -  Xstart;
	UDOUBLE IMAGE_COUNTER = Width * (Yend-Ystart);

    Xend -= 1;
	Yend -= 1;	

    EPD_5in0_Reset();

    EPD_5in0_SendCommand(0x18);
    EPD_5in0_SendData(0x80);

    EPD_5in0_SendCommand(0x3C);
    EPD_5in0_SendData(0x80);

	EPD_5in0_SendCommand(0x44);
    EPD_5in0_SendData((Xstart*8) & 0xFF);
    EPD_5in0_SendData(((Xstart*8) >> 8) & 0xFF);
    EPD_5in0_SendData((Xend*8) & 0xFF);
    EPD_5in0_SendData(((Xend*8) >> 8) & 0xFF);
	
    EPD_5in0_SendCommand(0x45);
    EPD_5in0_SendData(Yend & 0xFF);
    EPD_5in0_SendData((Yend >> 8) & 0xFF);
    EPD_5in0_SendData(Ystart & 0xFF);
    EPD_5in0_SendData((Ystart >> 8) & 0xFF);
    
    EPD_5in0_SendCommand(0x4E); 
    EPD_5in0_SendData((Xstart*8) & 0xFF);
    EPD_5in0_SendData(((Xstart*8) >> 8) & 0xFF);
	
    EPD_5in0_SendCommand(0x4F);
    EPD_5in0_SendData(Ystart & 0xFF);
    EPD_5in0_SendData((Ystart >> 8) & 0xFF);

	EPD_5in0_SendCommand(0x24);   //Write Black and White image to RAM
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_5in0_SendData(Image[i]);
	}

	EPD_5in0_TurnOnDisplay_Part();
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in0_Sleep(void)
{
    EPD_5in0_SendCommand(0x10); //enter deep sleep
    EPD_5in0_SendData(0x01);
    DEV_Delay_ms(100);
}
