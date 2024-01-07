/*****************************************************************************
* | File      	:	EPD_7in5.c
* | Author      :   Waveshare team
* | Function    :   Electronic paper driver
* | Info        :
*----------------
* |	This version:   V3.0
* | Date        :   2023-12-18
* | Info        :
*****************************************************************************
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files(the "Software"), to deal
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
#include "EPD_7in5_V2.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_Reset(void)
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
static void EPD_SendCommand(UBYTE Reg)
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
static void EPD_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_SendData2(UBYTE *pData, UDOUBLE len)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_Write_nByte(pData, len);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Wait until the busy_pin goes LOW
parameter:
******************************************************************************/
static void EPD_WaitUntilIdle(void)
{
    Debug("e-Paper busy\r\n");
	do{
		DEV_Delay_ms(5);  
	}while(!(DEV_Digital_Read(EPD_BUSY_PIN)));   
	DEV_Delay_ms(5);      
    Debug("e-Paper busy release\r\n");
}
/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_7IN5_V2_TurnOnDisplay(void)
{	
    EPD_SendCommand(0x12);			//DISPLAY REFRESH
    DEV_Delay_ms(100);	        //!!!The delay here is necessary, 200uS at least!!!
    EPD_WaitUntilIdle();
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
UBYTE EPD_7IN5_V2_Init(void)
{
    EPD_Reset();
    EPD_SendCommand(0x01);			//POWER SETTING
	EPD_SendData(0x07);
	EPD_SendData(0x07);    //VGH=20V,VGL=-20V
	EPD_SendData(0x3f);		//VDH=15V
	EPD_SendData(0x3f);		//VDL=-15V

	//Enhanced display drive(Add 0x06 command)
	EPD_SendCommand(0x06);			//Booster Soft Start 
	EPD_SendData(0x17);
	EPD_SendData(0x17);   
	EPD_SendData(0x28);		
	EPD_SendData(0x17);	

	EPD_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100); 
	EPD_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

	EPD_SendCommand(0X00);			//PANNEL SETTING
	EPD_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

	EPD_SendCommand(0x61);        	//tres			
	EPD_SendData(0x03);		//source 800
	EPD_SendData(0x20);
	EPD_SendData(0x01);		//gate 480
	EPD_SendData(0xE0);  

	EPD_SendCommand(0X15);		
	EPD_SendData(0x00);		

	EPD_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_SendData(0x10);
	EPD_SendData(0x07);

	EPD_SendCommand(0X60);			//TCON SETTING
	EPD_SendData(0x22);
	
    return 0;
}

UBYTE EPD_7IN5_V2_Init_Fast(void)
{
    EPD_Reset();
    EPD_SendCommand(0X00);			//PANNEL SETTING
    EPD_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f

    EPD_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
    EPD_SendData(0x10);
    EPD_SendData(0x07);

    EPD_SendCommand(0x04); //POWER ON
    DEV_Delay_ms(100); 
	EPD_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal

    //Enhanced display drive(Add 0x06 command)
    EPD_SendCommand(0x06);			//Booster Soft Start 
    EPD_SendData (0x27);
    EPD_SendData (0x27);   
    EPD_SendData (0x18);		
    EPD_SendData (0x17);		

    EPD_SendCommand(0xE0);
    EPD_SendData(0x02);
    EPD_SendCommand(0xE5);
    EPD_SendData(0x5A);
	
    return 0;
}

UBYTE EPD_7IN5_V2_Init_Part(void)
{
    EPD_Reset();

	EPD_SendCommand(0X00);			//PANNEL SETTING
	EPD_SendData(0x1F);   //KW-3f   KWR-2F	BWROTP 0f	BWOTP 1f
	
	EPD_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100); 
	EPD_WaitUntilIdle();        //waiting for the electronic paper IC to release the idle signal
	
	EPD_SendCommand(0xE0);
	EPD_SendData(0x02);
	EPD_SendCommand(0xE5);
	EPD_SendData(0x6E);
	
    return 0;
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_7IN5_V2_Clear(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    UBYTE image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    UWORD i;
    EPD_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }

    EPD_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_ClearBlack(void)
{
    UWORD Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
    UBYTE image[EPD_7IN5_V2_WIDTH / 8] = {0x00};

    UWORD i;
    EPD_SendCommand(0x10);
    for(i=0; i<Width; i++) {
        image[i] = 0x00;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }

    EPD_SendCommand(0x13);
    for(i=0; i<Width; i++) {
        image[i] = 0xFF;
    }
    for(i=0; i<Height; i++)
    {
        EPD_SendData2(image, Width);
    }
    
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_7IN5_V2_Display(UBYTE *blackimage)
{
    UDOUBLE Width, Height;
    Width =(EPD_7IN5_V2_WIDTH % 8 == 0)?(EPD_7IN5_V2_WIDTH / 8 ):(EPD_7IN5_V2_WIDTH / 8 + 1);
    Height = EPD_7IN5_V2_HEIGHT;
	
    EPD_SendCommand(0x10);
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }

    EPD_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        for (UDOUBLE i = 0; i < Width; i++) {
            blackimage[i + j * Width] = ~blackimage[i + j * Width];
        }
    }
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

void EPD_7IN5_V2_Display_Part(UBYTE *blackimage,UDOUBLE x_start, UDOUBLE y_start, UDOUBLE x_end, UDOUBLE y_end)
{
    UDOUBLE Width, Height;
    Width =((x_end - x_start) % 8 == 0)?((x_end - x_start) / 8 ):((x_end - x_start) / 8 + 1);
    Height = y_end - y_start;

    EPD_SendCommand(0x50);
	EPD_SendData(0xA9);
	EPD_SendData(0x07);

	EPD_SendCommand(0x91);		//This command makes the display enter partial mode
	EPD_SendCommand(0x90);		//resolution setting
	EPD_SendData (x_start/256);
	EPD_SendData (x_start%256);   //x-start    

	EPD_SendData (x_end/256);		
	EPD_SendData (x_end%256-1);  //x-end	

	EPD_SendData (y_start/256);  //
	EPD_SendData (y_start%256);   //y-start    

	EPD_SendData (y_end/256);		
	EPD_SendData (y_end%256-1);  //y-end
	EPD_SendData (0x01);
    
    EPD_SendCommand(0x13);
    for (UDOUBLE j = 0; j < Height; j++) {
        EPD_SendData2((UBYTE *)(blackimage+j*Width), Width);
    }
    EPD_7IN5_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_7IN5_V2_Sleep(void)
{
    EPD_SendCommand(0X02);  	//power off
    EPD_WaitUntilIdle();
    EPD_SendCommand(0X07);  	//deep sleep
    EPD_SendData(0xA5);
}
