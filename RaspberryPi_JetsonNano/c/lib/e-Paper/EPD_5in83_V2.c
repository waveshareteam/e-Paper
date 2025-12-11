/*****************************************************************************
* | File      	:   EPD_5in83_V2.c
* | Author      :   Waveshare team
* | Function    :   5.83inch e-paper V2
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2020-11-23
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
#include "EPD_5in83_V2.h"
#include "Debug.h"

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_5in83_V2_Reset(void)
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
static void EPD_5in83_V2_SendCommand(UBYTE Reg)
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
static void EPD_5in83_V2_SendData(UBYTE Data)
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
static void EPD_5in83_V2_ReadBusy(void)
{
	Debug("e-Paper busy\r\n");
	do {
		DEV_Delay_ms(10);    
	}
	while(!DEV_Digital_Read(EPD_BUSY_PIN));   
	Debug("e-Paper busy release\r\n");
}

/******************************************************************************
function :	Turn On Display
parameter:
******************************************************************************/
static void EPD_5in83_V2_TurnOnDisplay(void)
{
	EPD_5in83_V2_SendCommand(0x12);	//DISPLAY REFRESH 	
	DEV_Delay_ms(100);				//!!!The delay here is necessary, 200uS at least!!!     
	EPD_5in83_V2_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_5in83_V2_Init(void)
{
    EPD_5in83_V2_Reset();

	EPD_5in83_V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_5in83_V2_SendData(0x1F);   

	EPD_5in83_V2_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_5in83_V2_SendData(0x21);
	EPD_5in83_V2_SendData(0x07);

    EPD_5in83_V2_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);  
	EPD_5in83_V2_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

void EPD_5in83_V2_Init_Fast(void)
{
    EPD_5in83_V2_Reset();

	EPD_5in83_V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_5in83_V2_SendData(0x1F);   

	EPD_5in83_V2_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_5in83_V2_SendData(0x29);
	EPD_5in83_V2_SendData(0x07);

    EPD_5in83_V2_SendCommand(0xE0);	
	EPD_5in83_V2_SendData(0x02); 
    EPD_5in83_V2_SendCommand(0xE5);	
	EPD_5in83_V2_SendData(0x5A); 

    EPD_5in83_V2_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);  
	EPD_5in83_V2_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

void EPD_5in83_V2_Init_Part(void)
{
    EPD_5in83_V2_Reset();

	EPD_5in83_V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_5in83_V2_SendData(0x1F);   

    EPD_5in83_V2_SendCommand(0xE0);	
	EPD_5in83_V2_SendData(0x02); 
    EPD_5in83_V2_SendCommand(0xE5);	
	EPD_5in83_V2_SendData(0x6E);

    EPD_5in83_V2_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);  
	EPD_5in83_V2_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

void EPD_5in83_V2_Init_4GRAY(void)
{
    EPD_5in83_V2_Reset();

	EPD_5in83_V2_SendCommand(0X00);			//PANNEL SETTING
	EPD_5in83_V2_SendData(0x1F); 
    
    EPD_5in83_V2_SendCommand(0X06);			//VCOM AND DATA INTERVAL SETTING
	EPD_5in83_V2_SendData(0x27);
	EPD_5in83_V2_SendData(0x27);
    EPD_5in83_V2_SendData(0x18);
	EPD_5in83_V2_SendData(0x17);

	EPD_5in83_V2_SendCommand(0X50);			//VCOM AND DATA INTERVAL SETTING
	EPD_5in83_V2_SendData(0x21);
	EPD_5in83_V2_SendData(0x07);

    EPD_5in83_V2_SendCommand(0xE0);	
	EPD_5in83_V2_SendData(0x02); 
    EPD_5in83_V2_SendCommand(0xE5);	
	EPD_5in83_V2_SendData(0x5F); //0x5A--1.5s, 0x5F--4 Gray

    EPD_5in83_V2_SendCommand(0x04); //POWER ON
	DEV_Delay_ms(100);  
	EPD_5in83_V2_ReadBusy();        //waiting for the electronic paper IC to release the idle signal
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_5in83_V2_Clear(void)
{
    UWORD Width, Height, i;
    Width = (EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1);
    Height = EPD_5in83_V2_HEIGHT;

	EPD_5in83_V2_SendCommand(0x10);
	for(i=0; i<Width*Height; i++) {
		EPD_5in83_V2_SendData(0xff);  
	}
	EPD_5in83_V2_SendCommand(0x13);
	for(i=0; i<Width*Height; i++) {
		EPD_5in83_V2_SendData(0xff);  
	}
	EPD_5in83_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in83_V2_Display(UBYTE *Image)
{
    UWORD Width, Height, i, j;
    Width = (EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1);
    Height = EPD_5in83_V2_HEIGHT;

	EPD_5in83_V2_SendCommand(0x10);
	for(i=0;i<Height;i++) {
		for(j=0; j<Width; j++) {
			EPD_5in83_V2_SendData(Image[i*Width + j]);  
		}
	}
	EPD_5in83_V2_SendCommand(0x13);
	for(i=0;i<Height;i++) {
		for(j=0; j<Width; j++) {
			EPD_5in83_V2_SendData(Image[i*Width + j]);  
		}
	}
    EPD_5in83_V2_TurnOnDisplay();
}


/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_5in83_V2_Display_Partial(const UBYTE *Image, UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
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

    EPD_5in83_V2_SendCommand(0x50);
    EPD_5in83_V2_SendData(0xA9);
    EPD_5in83_V2_SendData(0x07);

    EPD_5in83_V2_SendCommand(0x91);
    EPD_5in83_V2_SendCommand(0x90);
    EPD_5in83_V2_SendData(((Xstart*8) >> 8) & 0xFF);
    EPD_5in83_V2_SendData((Xstart*8) & 0xFF);
    EPD_5in83_V2_SendData(((Xend*8) >> 8) & 0xFF);
    EPD_5in83_V2_SendData((Xend*8) & 0xFF);
    
	EPD_5in83_V2_SendData((Ystart >> 8) & 0xFF);
    EPD_5in83_V2_SendData(Ystart & 0xFF);
    EPD_5in83_V2_SendData((Yend >> 8) & 0xFF);
    EPD_5in83_V2_SendData(Yend & 0xFF);

    EPD_5in83_V2_SendData(0x01);

	EPD_5in83_V2_SendCommand(0x13);   //Write Black and White image to RAM
    for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_5in83_V2_SendData(Image[i]);
	}

	EPD_5in83_V2_TurnOnDisplay();
    EPD_5in83_V2_SendCommand(0x92);
}


void EPD_5in83_V2_Display_4Gray(const UBYTE *Image)
{
    UDOUBLE i,j,k;
    UBYTE temp1,temp2,temp3;
    UWORD Width, Height;
    Width = (EPD_5in83_V2_WIDTH % 8 == 0)? (EPD_5in83_V2_WIDTH / 8 ): (EPD_5in83_V2_WIDTH / 8 + 1);
    Height = EPD_5in83_V2_HEIGHT;
    UDOUBLE IMAGE_COUNTER = Width * Height;
    // old  data
    EPD_5in83_V2_SendCommand(0x10);
    for(i=0; i<IMAGE_COUNTER; i++) { 
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0;
                if(temp2 == 0xC0)
                    temp3 |= 0x01;
                else if(temp2 == 0x00)
                    temp3 |= 0x00; 
                else if(temp2 == 0x80)
                    temp3 |= 0x00; 
                else //0x40
                    temp3 |= 0x01; 
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0) 
                    temp3 |= 0x01;
                else if(temp2 == 0x00) 
                    temp3 |= 0x00;
                else if(temp2 == 0x80)
                    temp3 |= 0x00; 
                else    //0x40
                    temp3 |= 0x01;	
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_5in83_V2_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_5in83_V2_SendCommand(0x13); 
    for(i=0; i<IMAGE_COUNTER; i++) {
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x01;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x00;  //black
                else if(temp2 == 0x80)
                    temp3 |= 0x01;  //gray1
                else //0x40
                    temp3 |= 0x00; //gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x01;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x00;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; //gray1
                else    //0x40
                    temp3 |= 0x00;	//gray2
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_5in83_V2_SendData(temp3);
        // printf("%x",temp3);
    }
    EPD_5in83_V2_TurnOnDisplay();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_5in83_V2_Sleep(void)
{
    EPD_5in83_V2_SendCommand(0X50);	
	EPD_5in83_V2_SendData(0xf7);

	EPD_5in83_V2_SendCommand(0X02);  	//power off
	EPD_5in83_V2_ReadBusy();			//waiting for the electronic paper IC to release the idle signal
	EPD_5in83_V2_SendCommand(0X07);		//deep sleep
	EPD_5in83_V2_SendData(0xA5);
}

