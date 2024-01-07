/*****************************************************************************
* | File      	:   EPD_2in9b_V4.c
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper b V4
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-12-18
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
#include "EPD_2in9b_V4.h"
#include "Debug.h"
#include <time.h>

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_2IN9B_V4_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(2);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_2IN9B_V4_SendCommand(UBYTE Reg)
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
static void EPD_2IN9B_V4_SendData(UBYTE Data)
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
void EPD_2IN9B_V4_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
	while(1)
	{	 //=1 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==0) 
			break;
		DEV_Delay_ms(50);
	}
    Debug("e-Paper busy release\r\n");
    DEV_Delay_ms(200);
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_2IN9B_V4_TurnOnDisplay(void)
{
	EPD_2IN9B_V4_SendCommand(0x22); //Display Update Control
	EPD_2IN9B_V4_SendData(0xF7);
	EPD_2IN9B_V4_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2IN9B_V4_ReadBusy();
}

static void EPD_2IN9B_V4_TurnOnDisplay_Base(void)
{
	EPD_2IN9B_V4_SendCommand(0x22); //Display Update Control
	EPD_2IN9B_V4_SendData(0xF4);
	EPD_2IN9B_V4_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2IN9B_V4_ReadBusy();
}

static void EPD_2IN9B_V4_TurnOnDisplay_Partial(void)
{
	EPD_2IN9B_V4_SendCommand(0x22); //Display Update Control
	EPD_2IN9B_V4_SendData(0x1C);
	EPD_2IN9B_V4_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2IN9B_V4_ReadBusy();
}

static void EPD_2IN9B_V4_TurnOnDisplay_Fast(void)
{
	EPD_2IN9B_V4_SendCommand(0x22); //Display Update Control
	EPD_2IN9B_V4_SendData(0xC7);
	EPD_2IN9B_V4_SendCommand(0x20); //Activate Display Update Sequence
	EPD_2IN9B_V4_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_2IN9B_V4_Init(void)
{
    EPD_2IN9B_V4_Reset();

    EPD_2IN9B_V4_ReadBusy();   
    EPD_2IN9B_V4_SendCommand(0x12);  //SWRESET
    EPD_2IN9B_V4_ReadBusy();   
        
    EPD_2IN9B_V4_SendCommand(0x01); //Driver output control      
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)%256);    
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)/256);
    EPD_2IN9B_V4_SendData(0x00);

    EPD_2IN9B_V4_SendCommand(0x11); //data entry mode       
    EPD_2IN9B_V4_SendData(0x03);

    EPD_2IN9B_V4_SendCommand(0x44); //set Ram-X address start/end position   
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendData(EPD_2IN9B_V4_WIDTH/8-1);   

    EPD_2IN9B_V4_SendCommand(0x45); //set Ram-Y address start/end position          
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendData(0x00); 
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)%256);    
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)/256);

    EPD_2IN9B_V4_SendCommand(0x3C); //BorderWavefrom
    EPD_2IN9B_V4_SendData(0x05);	

    EPD_2IN9B_V4_SendCommand(0x21); //  Display update control
    EPD_2IN9B_V4_SendData(0x00);		
    EPD_2IN9B_V4_SendData(0x80);	

    EPD_2IN9B_V4_SendCommand(0x18); //Read built-in temperature sensor
    EPD_2IN9B_V4_SendData(0x80);	

    EPD_2IN9B_V4_SendCommand(0x4E);   // set RAM x address count to 0;
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendCommand(0x4F);   // set RAM y address count to 0X199;    
    EPD_2IN9B_V4_SendData(0x00);    
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_ReadBusy();
}

void EPD_2IN9B_V4_Init_Fast(void)
{
    EPD_2IN9B_V4_Reset();

    EPD_2IN9B_V4_ReadBusy();   
    EPD_2IN9B_V4_SendCommand(0x12);  //SWRESET
    EPD_2IN9B_V4_ReadBusy();   	

    EPD_2IN9B_V4_SendCommand(0x18); //Read built-in temperature sensor
    EPD_2IN9B_V4_SendData(0x80);

    EPD_2IN9B_V4_SendCommand(0x22); // Load temperature value
    EPD_2IN9B_V4_SendData(0xB1);		
    EPD_2IN9B_V4_SendCommand(0x20);	
    EPD_2IN9B_V4_ReadBusy();   

    EPD_2IN9B_V4_SendCommand(0x1A); // Write to temperature register
    EPD_2IN9B_V4_SendData(0x5a);		// 90		
    EPD_2IN9B_V4_SendData(0x00);	
                
    EPD_2IN9B_V4_SendCommand(0x22); // Load temperature value
    EPD_2IN9B_V4_SendData(0x91);		
    EPD_2IN9B_V4_SendCommand(0x20);	
    EPD_2IN9B_V4_ReadBusy();  

    EPD_2IN9B_V4_SendCommand(0x01); //Driver output control      
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)%256);    
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)/256);
    EPD_2IN9B_V4_SendData(0x00);

    EPD_2IN9B_V4_SendCommand(0x11); //data entry mode       
    EPD_2IN9B_V4_SendData(0x03);

    EPD_2IN9B_V4_SendCommand(0x44); //set Ram-X address start/end position   
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendData(EPD_2IN9B_V4_WIDTH/8-1);   

    EPD_2IN9B_V4_SendCommand(0x45); //set Ram-Y address start/end position          
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendData(0x00); 
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)%256);    
    EPD_2IN9B_V4_SendData((EPD_2IN9B_V4_HEIGHT-1)/256);	

    EPD_2IN9B_V4_SendCommand(0x4E);   // set RAM x address count to 0;
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_SendCommand(0x4F);   // set RAM y address count to 0X199;    
    EPD_2IN9B_V4_SendData(0x00);    
    EPD_2IN9B_V4_SendData(0x00);
    EPD_2IN9B_V4_ReadBusy();	

}



/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_2IN9B_V4_Clear(void)
{
    UWORD Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V4_HEIGHT;

    //send black data
    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0xFF);
        }
    }

    //send red data
    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0x00);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay();
}

void EPD_2IN9B_V4_Clear_Fast(void)
{
    UWORD Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V4_HEIGHT;

    //send black data
    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0xFF);
        }
    }

    //send red data
    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0x00);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay_Fast();
}

void EPD_2IN9B_V4_Clear_Black_Fast(void)
{
    UWORD Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V4_HEIGHT;

    //send black data
    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0x00);
        }
    }

    //send red data
    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0x00);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay_Fast();
}

void EPD_2IN9B_V4_Clear_Red_Fast(void)
{
    UWORD Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    UWORD Height = EPD_2IN9B_V4_HEIGHT;

    //send black data
    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0xFF);
        }
    }

    //send red data
    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(0xFF);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay_Fast();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_2IN9B_V4_Display(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    Height = EPD_2IN9B_V4_HEIGHT;

    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay();
}

void EPD_2IN9B_V4_Display_Fast(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    Height = EPD_2IN9B_V4_HEIGHT;

    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay_Fast();
}

void EPD_2IN9B_V4_Display_Base(const UBYTE *blackimage, const UBYTE *ryimage)
{
    UWORD Width, Height;
    Width = (EPD_2IN9B_V4_WIDTH % 8 == 0)? (EPD_2IN9B_V4_WIDTH / 8 ): (EPD_2IN9B_V4_WIDTH / 8 + 1);
    Height = EPD_2IN9B_V4_HEIGHT;

    EPD_2IN9B_V4_SendCommand(0x24);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(blackimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(~ryimage[i + j * Width]);
        }
    }

    EPD_2IN9B_V4_TurnOnDisplay_Base();

    EPD_2IN9B_V4_SendCommand(0x26);
    for (UWORD j = 0; j < Height; j++) {
        for (UWORD i = 0; i < Width; i++) {
            EPD_2IN9B_V4_SendData(blackimage[i + j * Width]);
        }
    }
}

//Partial refresh display
void EPD_2IN9B_V4_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
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
	UWORD IMAGE_COUNTER = Width * (Yend-Ystart);

	Xend -= 1;
	Yend -= 1;	

    EPD_2IN9B_V4_SendCommand(0x44);       // set RAM x address start/end, in page 35
    EPD_2IN9B_V4_SendData(Xstart & 0xff);    // RAM x address start at 00h;
    EPD_2IN9B_V4_SendData(Xend & 0xff);    // RAM x address end at 0fh(15+1)*8->128 
    EPD_2IN9B_V4_SendCommand(0x45);       // set RAM y address start/end, in page 35
    EPD_2IN9B_V4_SendData(Ystart & 0xff);    // RAM y address start at 0127h;
    EPD_2IN9B_V4_SendData((Ystart>>8) & 0x01);    // RAM y address start at 0127h;
    EPD_2IN9B_V4_SendData(Yend & 0xff);    // RAM y address end at 00h;
    EPD_2IN9B_V4_SendData((Yend>>8) & 0x01); 

    EPD_2IN9B_V4_SendCommand(0x4E);   // set RAM x address count to 0;
    EPD_2IN9B_V4_SendData(Xstart & 0xff); 
    EPD_2IN9B_V4_SendCommand(0x4F);   // set RAM y address count to 0X127;    
    EPD_2IN9B_V4_SendData(Ystart & 0xff);
    EPD_2IN9B_V4_SendData((Ystart>>8) & 0x01);


    EPD_2IN9B_V4_SendCommand(0x24);   //Write Black and White image to RAM
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_2IN9B_V4_SendData(Image[i]);
	}
	EPD_2IN9B_V4_TurnOnDisplay_Partial();

}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_2IN9B_V4_Sleep(void)
{
    EPD_2IN9B_V4_SendCommand(0x10); //enter deep sleep
	EPD_2IN9B_V4_SendData(0x01); 
	DEV_Delay_ms(100);
}
