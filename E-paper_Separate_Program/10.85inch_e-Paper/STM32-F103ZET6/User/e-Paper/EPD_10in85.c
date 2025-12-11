/*****************************************************************************
* | File      	:  	EPD_10in85.c
* | Author      :   Waveshare team
* | Function    :   10.85inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-19
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
#include "EPD_10in85.h"
#include "Debug.h"
#include <time.h> 

unsigned char partFlag0=0;
unsigned char partFlag1=0;
unsigned char oldDatas[EPD_10in85_WIDTH*EPD_10in85_HEIGHT/8];
/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_10in85_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(10);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(20);
}

/******************************************************************************
function :	send command
parameter:
     Reg : Command register
******************************************************************************/
static void EPD_10in85_SendCommand_0(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}
static void EPD_10in85_SendCommand_1(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}
static void EPD_10in85_SendCommand_ALL(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);

    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}
/******************************************************************************
function :	send data
parameter:
    Data : Write data
******************************************************************************/
static void EPD_10in85_SendData_0(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}

static void EPD_10in85_SendData_1(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

static void EPD_10in85_SendnData_0(UBYTE *Data, UDOUBLE Len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_SPI_WritenByte(Data, Len);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
}

static void EPD_10in85_SendnData_1(UBYTE *Data, UDOUBLE Len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WritenByte(Data, Len);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}

static void EPD_10in85_SendData_ALL(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_M_PIN, 0);
    DEV_Digital_Write(EPD_CS_S_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_M_PIN, 1);
    DEV_Digital_Write(EPD_CS_S_PIN, 1);
}
/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
void EPD_10in85_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    // DEV_Delay_ms(1000);
	while(1)
	{	 //=0 BUSY
		if(DEV_Digital_Read(EPD_BUSY_PIN)==1) 
			break;
		DEV_Delay_ms(10);
	}
	DEV_Delay_ms(10);
    Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_10in85_TurnOnDisplay(void)
{
	EPD_10in85_SendCommand_ALL(0x12);
	EPD_10in85_ReadBusy();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_10in85_Init(void)
{
	EPD_10in85_Reset();

	EPD_10in85_SendCommand_ALL(0x4D);
	EPD_10in85_SendData_ALL(0x55);

	EPD_10in85_SendCommand_ALL(0xA6);
	EPD_10in85_SendData_ALL(0x38);

	EPD_10in85_SendCommand_ALL(0xB4);
	EPD_10in85_SendData_ALL(0x5D);

	EPD_10in85_SendCommand_ALL(0xB6);
	EPD_10in85_SendData_ALL(0x80);

	EPD_10in85_SendCommand_ALL(0xB7);
	EPD_10in85_SendData_ALL(0x00);

	EPD_10in85_SendCommand_ALL(0xF7);
	EPD_10in85_SendData_ALL(0x02);

	EPD_10in85_SendCommand_1(0xAE);
	EPD_10in85_SendData_1(0xA0);

	EPD_10in85_SendCommand_ALL(0xE0);
	EPD_10in85_SendData_ALL(0x01);

	EPD_10in85_SendCommand_ALL(0x00);//Panel setting	
	EPD_10in85_SendData_ALL(0x9F);
	EPD_10in85_SendData_ALL(0x0D);

	EPD_10in85_SendCommand_ALL(0x06);//boost
	EPD_10in85_SendData_ALL(0x57);
	EPD_10in85_SendData_ALL(0x24);
	EPD_10in85_SendData_ALL(0x28);
	EPD_10in85_SendData_ALL(0x32);
	EPD_10in85_SendData_ALL(0x08);	
	EPD_10in85_SendData_ALL(0x48);

	EPD_10in85_SendCommand_ALL(0x61);//resolution setting
	EPD_10in85_SendData_ALL(0X02);//680   1360*480
	EPD_10in85_SendData_ALL(0XA8);
	EPD_10in85_SendData_ALL(0X01);//480
	EPD_10in85_SendData_ALL(0XE0);

	EPD_10in85_SendCommand_ALL(0x62);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);	

	EPD_10in85_SendCommand_ALL(0x60);
	EPD_10in85_SendData_ALL(0x31);

	EPD_10in85_SendCommand_ALL(0x50);//VCOM and DATA interval setting(CDI)	
	EPD_10in85_SendData_ALL(0x97);

	EPD_10in85_SendCommand_ALL(0xE8);  
	EPD_10in85_SendData_ALL(0x01); 

	EPD_10in85_SendCommand_ALL(0x04);  //power on  
	DEV_Delay_ms(200);  
	EPD_10in85_ReadBusy();          //waiting for the electronic paper IC to release the idle signal
}

void EPD_10in85_Init_Part(void)
{
	EPD_10in85_Reset();

	EPD_10in85_SendCommand_ALL(0x4D);
	EPD_10in85_SendData_ALL(0x55);

	EPD_10in85_SendCommand_ALL(0xA6);
	EPD_10in85_SendData_ALL(0x38);

	EPD_10in85_SendCommand_ALL(0xB4);
	EPD_10in85_SendData_ALL(0x5D);

	EPD_10in85_SendCommand_ALL(0xB6);
	EPD_10in85_SendData_ALL(0x80);

	EPD_10in85_SendCommand_ALL(0xB7);
	EPD_10in85_SendData_ALL(0x00);

	EPD_10in85_SendCommand_ALL(0xF7);
	EPD_10in85_SendData_ALL(0x02);

	EPD_10in85_SendCommand_1(0xAE);
	EPD_10in85_SendData_1(0xA0);

	EPD_10in85_SendCommand_ALL(0xE0);
	EPD_10in85_SendData_ALL(0x01);

	EPD_10in85_SendCommand_ALL(0x00);//Panel setting	
	EPD_10in85_SendData_ALL(0x9F);
	EPD_10in85_SendData_ALL(0x0D);

	EPD_10in85_SendCommand_ALL(0x06);//boost
	EPD_10in85_SendData_ALL(0x57);
	EPD_10in85_SendData_ALL(0x24);
	EPD_10in85_SendData_ALL(0x28);
	EPD_10in85_SendData_ALL(0x32);
	EPD_10in85_SendData_ALL(0x08);	
	EPD_10in85_SendData_ALL(0x48);


	EPD_10in85_SendCommand_ALL(0x61);//resolution setting
	EPD_10in85_SendData_ALL(0X02);//680   1360*480
	EPD_10in85_SendData_ALL(0XA8);
	EPD_10in85_SendData_ALL(0X01);//480
	EPD_10in85_SendData_ALL(0XE0);

	EPD_10in85_SendCommand_ALL(0x62);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);
	EPD_10in85_SendData_ALL(0x00);	

	EPD_10in85_SendCommand_ALL(0x82);  // VCOM DC Setting
	EPD_10in85_SendData_ALL(0x12);  // 1-0=11: internal power
	
	EPD_10in85_SendCommand_ALL(0x60);
	EPD_10in85_SendData_ALL(0x31);
	
	EPD_10in85_SendCommand_ALL(0x50);  // VCOM and DATA interval setting    
	EPD_10in85_SendData_ALL(0x97);
		
	EPD_10in85_SendCommand_ALL(0x50);  // VCOM and DATA interval setting    
	EPD_10in85_SendData_ALL(0x97);

	EPD_10in85_SendCommand_ALL(0xE0);  
	EPD_10in85_SendData_ALL(0x03); 
	EPD_10in85_SendCommand_ALL(0xE5);  //100du
	EPD_10in85_SendData_ALL(0x64); 

    EPD_10in85_SendCommand_ALL(0x04); //POWER ON
    DEV_Delay_ms(100);  
    EPD_10in85_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_10in85_Clear(void)
{
	UWORD Width, Height;
    Width = (EPD_10in85_WIDTH % 8 == 0)? (EPD_10in85_WIDTH / 8 ): (EPD_10in85_WIDTH / 8 + 1);
    Height = EPD_10in85_HEIGHT;

    UBYTE buf[85] = {0x00};
    
    EPD_10in85_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }	
    EPD_10in85_SendCommand_0(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }	

    EPD_10in85_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }	
    EPD_10in85_SendCommand_1(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }

	EPD_10in85_TurnOnDisplay();
}


void EPD_10in85_Clear_Black(void)
{
	UWORD Width, Height;
    Width = (EPD_10in85_WIDTH % 8 == 0)? (EPD_10in85_WIDTH / 8 ): (EPD_10in85_WIDTH / 8 + 1);
    Height = EPD_10in85_HEIGHT;
	
    UBYTE buf[85] = {0x00};

    EPD_10in85_SendCommand_0(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
    }	

    EPD_10in85_SendCommand_1(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
    }
    

    for (UWORD j = 0; j < Width; j++) {
        buf[j] = 0xFF;
    }

    EPD_10in85_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }

    EPD_10in85_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }



	EPD_10in85_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
	Image : Image data
******************************************************************************/
void EPD_10in85_Display(const UBYTE *Image)
{
	UWORD Width, Height;
    Width = (EPD_10in85_WIDTH % 8 == 0)? (EPD_10in85_WIDTH / 8 ): (EPD_10in85_WIDTH / 8 + 1);
    Height = EPD_10in85_HEIGHT;

    UBYTE buf[85] = {0xFF};
    
    for (UWORD j = 0; j < Width; j++) {
        buf[j] = 0xFF;
    }
	
    EPD_10in85_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
    }

    EPD_10in85_SendCommand_0(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0((UBYTE *)&Image[j*Width*2],Width);
    }

    EPD_10in85_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
    }	
    EPD_10in85_SendCommand_1(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1((UBYTE *)&Image[j*2*Width + Width],Width);
    }
	
	EPD_10in85_TurnOnDisplay();	
}

void EPD_10in85_Display2(const UBYTE *Image)
{
	UWORD Width, Height;
    Width = (EPD_10in85_WIDTH % 8 == 0)? (EPD_10in85_WIDTH / 8 ): (EPD_10in85_WIDTH / 8 + 1);
    Height = EPD_10in85_HEIGHT;

    UBYTE buf[85] = {0xFF};
    
    for (UWORD j = 0; j < Width; j++) {
        buf[j] = 0xFF;
    }
	
    EPD_10in85_SendCommand_0(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0(buf,Width);
    }

    EPD_10in85_SendCommand_0(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_0((UBYTE *)&Image[j*Width],Width);
    }

    EPD_10in85_SendCommand_1(0x10);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }	
    EPD_10in85_SendCommand_1(0x13);
    for (UWORD j = 0; j < Height; j++) {
        EPD_10in85_SendnData_1(buf,Width);
        for (UWORD j = 0; j < Width; j++) {
            buf[j] = 0xFF;
        }
    }
	
	EPD_10in85_TurnOnDisplay();	
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and partial refresh
parameter:
	Image : Image data
******************************************************************************/
void EPD_10in85b_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
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
    

    UWORD Width, Height;
	Width = Xend -  Xstart;
    Height = Yend - Ystart;
	UWORD IMAGE_COUNTER = Width * Height;

    if((Xend*8)<EPD_10in85_WIDTH)
    {
        EPD_10in85_SendCommand_ALL(0x61);
        EPD_10in85_SendData_ALL((Width*8)/256);
        EPD_10in85_SendData_ALL((Width*8)%256);
        EPD_10in85_SendData_ALL((Height+1)/256);
        EPD_10in85_SendData_ALL((Height+1)%256);

        EPD_10in85_SendCommand_ALL(0x62);
        EPD_10in85_SendData_ALL((Xstart*8)/256);
        EPD_10in85_SendData_ALL((Xstart*8)%256);
        EPD_10in85_SendData_ALL(Ystart/256);
        EPD_10in85_SendData_ALL(Ystart%256);

        if(partFlag0 == 0)
        {
            partFlag0 = 1;
            EPD_10in85_SendCommand_0(0x10);
            for (UWORD j = 0; j < Height; j++) {
                for (UWORD i = 0; i < Width; i++) {
                EPD_10in85_SendData_0(0xFF);
                }
            }
        }

        EPD_10in85_SendCommand_0(0x13);
            for (UWORD j = 0; j < Height; j++) {
                for (UWORD i = 0; i < Width; i++) {
                    EPD_10in85_SendData_0(Image[j*Width + i]);
                }
            }

        EPD_10in85_TurnOnDisplay();

        EPD_10in85_SendCommand_0(0x10);
        for (UWORD i = 0; i < IMAGE_COUNTER; i++) {
            EPD_10in85_SendData_0(Image[i]);
        }
    }
    else if((Xstart*8)>EPD_10in85_WIDTH)
    {
        EPD_10in85_SendCommand_ALL(0x61);
        EPD_10in85_SendData_ALL((Width*8)/256);
        EPD_10in85_SendData_ALL((Width*8)%256);
        EPD_10in85_SendData_ALL(Height/256);
        EPD_10in85_SendData_ALL(Height%256);

        EPD_10in85_SendCommand_ALL(0x62);
        EPD_10in85_SendData_ALL((Xstart*8-EPD_10in85_WIDTH)/256);
        EPD_10in85_SendData_ALL((Xstart*8-EPD_10in85_WIDTH)%256);
        EPD_10in85_SendData_ALL(Ystart/256);
        EPD_10in85_SendData_ALL(Ystart%256);

        if(partFlag1 == 0)
        {
            partFlag1 = 1;
            EPD_10in85_SendCommand_1(0x10);
            for (UWORD i = 0; i < IMAGE_COUNTER; i++) {
                EPD_10in85_SendData_1(0xFF);
            }
        }

        EPD_10in85_SendCommand_1(0x13);
        for (UWORD i = 0; i < IMAGE_COUNTER; i++) {
            EPD_10in85_SendData_1(Image[i]);
        }

        EPD_10in85_TurnOnDisplay();

        EPD_10in85_SendCommand_1(0x10);
        for (UWORD i = 0; i < IMAGE_COUNTER; i++) {
            EPD_10in85_SendData_1(Image[i]);
        }
    }
    else
    {
        UWORD Width0 = EPD_10in85_WIDTH/8 - Xstart;
        UWORD Width1 = Xend - EPD_10in85_WIDTH/8;


        EPD_10in85_SendCommand_0(0x61);
        EPD_10in85_SendData_0((Width0*8)/256);
        EPD_10in85_SendData_0((Width0*8)%256);
        EPD_10in85_SendData_0(Height/256);
        EPD_10in85_SendData_0(Height%256);

        EPD_10in85_SendCommand_0(0x62);
        EPD_10in85_SendData_0((Xstart*8)/256);
        EPD_10in85_SendData_0((Xstart*8)%256);
        EPD_10in85_SendData_0(Ystart/256);
        EPD_10in85_SendData_0(Ystart%256);

        EPD_10in85_SendCommand_1(0x61);
        EPD_10in85_SendData_1((Width1*8)/256);
        EPD_10in85_SendData_1((Width1*8)%256);
        EPD_10in85_SendData_1(Height/256);
        EPD_10in85_SendData_1(Height%256);

        EPD_10in85_SendCommand_1(0x62);
        EPD_10in85_SendData_1(0x00);
        EPD_10in85_SendData_1(0x00);
        EPD_10in85_SendData_1(Ystart/256);
        EPD_10in85_SendData_1(Ystart%256);

        if(partFlag0 == 0)
        {
            partFlag0 = 1;
            EPD_10in85_SendCommand_0(0x10);
            for (UWORD j = 0; j < Height; j++) {
                for (UWORD i = 0; i < Width0; i++) {
                EPD_10in85_SendData_0(0xFF);
                }
            }
        }

        EPD_10in85_SendCommand_0(0x13);
        for (UWORD j = 0; j < Height; j++) {
            for (UWORD i = 0; i < Width0; i++) {
                EPD_10in85_SendData_0(Image[j*Width + i]);
            }
        }
        
        if(partFlag1 == 0)
        {
            partFlag1 = 1;
            EPD_10in85_SendCommand_1(0x10);
            for (UWORD j = 0; j < Height; j++) {
                for (UWORD i = 0; i < Width1; i++) {
                    EPD_10in85_SendData_1(0xFF);
                }
            }
        }

        EPD_10in85_SendCommand_1(0x13);
        for (UWORD j = 0; j < Height; j++) {
            for (UWORD i = 0; i < Width1; i++) {
                EPD_10in85_SendData_1(Image[j*Width + i + Width0]);
            }
        }
        EPD_10in85_TurnOnDisplay();

        EPD_10in85_SendCommand_0(0x10);
        for (UWORD j = 0; j < Height; j++) {
            for (UWORD i = 0; i < Width0; i++) {
                EPD_10in85_SendData_0(Image[j*Width + i]);
            }
        }

        EPD_10in85_SendCommand_1(0x10);
        for (UWORD j = 0; j < Height; j++) {
            for (UWORD i = 0; i < Width1; i++) {
                EPD_10in85_SendData_1(Image[j*Width + i + Width0]);
            }
        }
    }
	
}


/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_10in85_Sleep(void)
{
    EPD_10in85_SendCommand_ALL(0x02); 
	DEV_Delay_ms(100);  
	EPD_10in85_ReadBusy();       

	EPD_10in85_SendCommand_ALL(0X07); //enter deep sleep
	EPD_10in85_SendData_ALL(0xA5); 
	DEV_Delay_ms(100);
}
