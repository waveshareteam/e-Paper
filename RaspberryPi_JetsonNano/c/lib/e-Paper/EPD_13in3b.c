/*****************************************************************************
* | File      	:  	EPD_13in3b.c
* | Author      :   Waveshare team
* | Function    :   13.3inch e-paper (B)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-04-08
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
#include "EPD_13in3b.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_13IN3B_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(100);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_13IN3B_SendCommand(UBYTE Reg)
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
static void EPD_13IN3B_SendData(UBYTE Data)
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
void EPD_13IN3B_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	while(1)
	{	 //=1 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==0) 
			break;
		DEV_Delay_ms(20);
	}
	DEV_Delay_ms(20);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_13IN3B_TurnOnDisplay(void)
{
	EPD_13IN3B_SendCommand(0x22); //Display Update Control
	EPD_13IN3B_SendData(0xF7);
	EPD_13IN3B_SendCommand(0x20); //Activate Display Update Sequence
	EPD_13IN3B_ReadBusy();
}

static void EPD_13IN3B_TurnOnDisplay_Part(void)
{
	EPD_13IN3B_SendCommand(0x22); //Display Update Control
	EPD_13IN3B_SendData(0xFF);
	EPD_13IN3B_SendCommand(0x20); //Activate Display Update Sequence
	EPD_13IN3B_ReadBusy();
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_13IN3B_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_13IN3B_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_13IN3B_SendData(Xstart & 0xFF);
    EPD_13IN3B_SendData((Xstart>>8) & 0x03);
    EPD_13IN3B_SendData(Xend & 0xFF);
    EPD_13IN3B_SendData((Xend>>8) & 0x03);
	
    EPD_13IN3B_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_13IN3B_SendData(Ystart & 0xFF);
    EPD_13IN3B_SendData((Ystart>>8) & 0x03);
    EPD_13IN3B_SendData(Yend & 0xFF);
    EPD_13IN3B_SendData((Yend>>8) & 0x03);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_13IN3B_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_13IN3B_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_13IN3B_SendData(Xstart & 0xFF);
    EPD_13IN3B_SendData((Xstart>>8) & 0x03);

    EPD_13IN3B_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_13IN3B_SendData(Ystart & 0xFF);
    EPD_13IN3B_SendData((Ystart>>8) & 0x03);
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_13IN3B_Init(void)
{
	EPD_13IN3B_Reset();
	DEV_Delay_ms(100);

	EPD_13IN3B_ReadBusy();   
	EPD_13IN3B_SendCommand(0x12);
	EPD_13IN3B_ReadBusy();   

	EPD_13IN3B_SendCommand(0x0C);    
	EPD_13IN3B_SendData(0xAE);
	EPD_13IN3B_SendData(0xC7);
	EPD_13IN3B_SendData(0xC3);
	EPD_13IN3B_SendData(0xC0);
	EPD_13IN3B_SendData(0x80);

	EPD_13IN3B_SendCommand(0x01); 
	EPD_13IN3B_SendData((EPD_13IN3B_HEIGHT-1)%256);
	EPD_13IN3B_SendData((EPD_13IN3B_HEIGHT-1)/256);
	EPD_13IN3B_SendData(0x00);

	EPD_13IN3B_SendCommand(0x11);    
	EPD_13IN3B_SendData(0x03); 

	EPD_13IN3B_SetWindows(0, 0, EPD_13IN3B_WIDTH-1, EPD_13IN3B_HEIGHT-1);

	EPD_13IN3B_SendCommand(0x3C); 
	EPD_13IN3B_SendData(0x01);

	EPD_13IN3B_SendCommand(0x18); 
	EPD_13IN3B_SendData(0x80);

    // EPD_13IN3B_SendCommand(0x22);  
    // EPD_13IN3B_SendData(0xB1); 
    // EPD_13IN3B_SendCommand(0x20); 
	
	EPD_13IN3B_SetCursor(0, 0);
	
	EPD_13IN3B_ReadBusy();
}
/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_13IN3B_Clear(void)
{
	UWORD i, j;
	UWORD height = EPD_13IN3B_HEIGHT;
	UWORD width = EPD_13IN3B_WIDTH/8;	
	
	EPD_13IN3B_SendCommand(0x24); 
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3B_SendData(0xff);
	}

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < height; j++) {
        for (UWORD i = 0; i < width; i++) {
            EPD_13IN3B_SendData(0x00);
        }
    }
    
	EPD_13IN3B_TurnOnDisplay();
}

void EPD_13IN3B_Clear_Black(void)
{
    UWORD Width, Height;
    Width = (EPD_13IN3B_WIDTH % 8 == 0)? (EPD_13IN3B_WIDTH / 8 ): (EPD_13IN3B_WIDTH / 8 + 1);
    Height = EPD_13IN3B_HEIGHT;

    EPD_13IN3B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(0x00);
        }
    }

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(0x00);
        }
    }

    EPD_13IN3B_TurnOnDisplay();
}

void EPD_13IN3B_Clear_Red(void)
{
    UWORD Width, Height;
    Width = (EPD_13IN3B_WIDTH % 8 == 0)? (EPD_13IN3B_WIDTH / 8 ): (EPD_13IN3B_WIDTH / 8 + 1);
    Height = EPD_13IN3B_HEIGHT;

    EPD_13IN3B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(0xFF);
        }
    }

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(0xFF);
        }
    }

    EPD_13IN3B_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_13IN3B_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_13IN3B_WIDTH % 8 == 0)? (EPD_13IN3B_WIDTH / 8 ): (EPD_13IN3B_WIDTH / 8 + 1);
    Height = EPD_13IN3B_HEIGHT;

    EPD_13IN3B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_13IN3B_TurnOnDisplay();
}

//Partial refresh of background display, this function is necessary, please do not delete it!!!
void EPD_13IN3B_Display_Base(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_13IN3B_WIDTH % 8 == 0)? (EPD_13IN3B_WIDTH / 8 ): (EPD_13IN3B_WIDTH / 8 + 1);
    Height = EPD_13IN3B_HEIGHT;

    EPD_13IN3B_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_13IN3B_TurnOnDisplay();

    EPD_13IN3B_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_13IN3B_SendData(blackimage[i + j * Width]);
        }
    }
}

//Partial refresh display
void EPD_13IN3B_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
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
    

    UDOUBLE i, Width;
	Width = Xend -  Xstart;
	UDOUBLE IMAGE_COUNTER = Width * (Yend-Ystart);
    printf("IMAGE_COUNTER = %d\r\n", IMAGE_COUNTER);

    Xstart = Xstart * 8;
	Xend =Xend * 8 - 1;
	Yend -= 1;	

    EPD_13IN3B_SendCommand(0x3C);    
    EPD_13IN3B_SendData(0x80);

    EPD_13IN3B_SendCommand(0x44);
    EPD_13IN3B_SendData(Xstart & 0xFF);
    EPD_13IN3B_SendData((Xstart>>8) & 0x03);
    EPD_13IN3B_SendData(Xend & 0xFF);
    EPD_13IN3B_SendData((Xend>>8) & 0x03);
	
    EPD_13IN3B_SendCommand(0x45);   
    EPD_13IN3B_SendData(Ystart & 0xff);   
    EPD_13IN3B_SendData((Ystart>>8) & 0x01);  
    EPD_13IN3B_SendData(Yend & 0xff);
    EPD_13IN3B_SendData((Yend>>8) & 0x03); 

    EPD_13IN3B_SendCommand(0x4E); 
    EPD_13IN3B_SendData(Xstart & 0xFF);
    EPD_13IN3B_SendData((Xstart>>8) & 0x03);
    EPD_13IN3B_SendCommand(0x4F);   
    EPD_13IN3B_SendData(Ystart & 0xff);
    EPD_13IN3B_SendData((Ystart>>8) & 0x03);

    EPD_13IN3B_SendCommand(0x24); 
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_13IN3B_SendData(Image[i]);
	}

	EPD_13IN3B_TurnOnDisplay_Part();

    EPD_13IN3B_SendCommand(0x26); 
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_13IN3B_SendData(Image[i]);
	}

}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_13IN3B_Sleep(void)
{
	EPD_13IN3B_SendCommand(0x10); 
	EPD_13IN3B_SendData(0x03); 
	DEV_Delay_ms(100);
}
