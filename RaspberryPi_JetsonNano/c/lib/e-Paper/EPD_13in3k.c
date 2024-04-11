/*****************************************************************************
* | File      	:  	EPD_13in3k.c
* | Author      :   Waveshare team
* | Function    :   13.3inch e-paper (k)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-07-18
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
#include "EPD_13in3k.h"
#include "Debug.h"

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

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
static void EPD_13IN3K_Reset(void)
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
static void EPD_13IN3K_SendCommand(UBYTE Reg)
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
static void EPD_13IN3K_SendData(UBYTE Data)
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
void EPD_13IN3K_ReadBusy(void)
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
static void EPD_13IN3K_TurnOnDisplay(void)
{
	EPD_13IN3K_SendCommand(0x22); //Display Update Control
	EPD_13IN3K_SendData(0xF7);
	EPD_13IN3K_SendCommand(0x20); //Activate Display Update Sequence
	EPD_13IN3K_ReadBusy();
}

static void EPD_13IN3K_TurnOnDisplay_Part(void)
{
	EPD_13IN3K_SendCommand(0x22); //Display Update Control
	EPD_13IN3K_SendData(0xCF);
	EPD_13IN3K_SendCommand(0x20); //Activate Display Update Sequence
	EPD_13IN3K_ReadBusy();
}

static void EPD_13IN3K_TurnOnDisplay_4GRAY(void)
{
	EPD_13IN3K_SendCommand(0x22); //Display Update Control
	EPD_13IN3K_SendData(0xC7);
	EPD_13IN3K_SendCommand(0x20); //Activate Display Update Sequence
	EPD_13IN3K_ReadBusy();
}

static void EPD_13IN3K_Lut(const UBYTE *LUT)
{
    unsigned int count;
    EPD_13IN3K_SendCommand(0x32);
    for(count = 0; count < 105 ; count++) {
        EPD_13IN3K_SendData(LUT[count]);
    }

    EPD_13IN3K_SendCommand(0x03);
	EPD_13IN3K_SendData(LUT[105]);

	EPD_13IN3K_SendCommand(0x04);  
	EPD_13IN3K_SendData(LUT[106]);
	EPD_13IN3K_SendData(LUT[107]);
	EPD_13IN3K_SendData(LUT[108]); 

	EPD_13IN3K_SendCommand(0x2C);
	EPD_13IN3K_SendData(LUT[109]);
}

/******************************************************************************
function :	Setting the display window
parameter:
******************************************************************************/
static void EPD_13IN3K_SetWindows(UWORD Xstart, UWORD Ystart, UWORD Xend, UWORD Yend)
{
    EPD_13IN3K_SendCommand(0x44); // SET_RAM_X_ADDRESS_START_END_POSITION
    EPD_13IN3K_SendData(Xstart & 0xFF);
    EPD_13IN3K_SendData((Xstart>>8) & 0x03);
    EPD_13IN3K_SendData(Xend & 0xFF);
    EPD_13IN3K_SendData((Xend>>8) & 0x03);
	
    EPD_13IN3K_SendCommand(0x45); // SET_RAM_Y_ADDRESS_START_END_POSITION
    EPD_13IN3K_SendData(Ystart & 0xFF);
    EPD_13IN3K_SendData((Ystart>>8) & 0x03);
    EPD_13IN3K_SendData(Yend & 0xFF);
    EPD_13IN3K_SendData((Yend>>8) & 0x03);
}

/******************************************************************************
function :	Set Cursor
parameter:
******************************************************************************/
static void EPD_13IN3K_SetCursor(UWORD Xstart, UWORD Ystart)
{
    EPD_13IN3K_SendCommand(0x4E); // SET_RAM_X_ADDRESS_COUNTER
    EPD_13IN3K_SendData(Xstart & 0xFF);
    EPD_13IN3K_SendData((Xstart>>8) & 0x03);

    EPD_13IN3K_SendCommand(0x4F); // SET_RAM_Y_ADDRESS_COUNTER
    EPD_13IN3K_SendData(Ystart & 0xFF);
    EPD_13IN3K_SendData((Ystart>>8) & 0x03);
}



/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_13IN3K_Init(void)
{
	EPD_13IN3K_Reset();
	DEV_Delay_ms(100);

	EPD_13IN3K_ReadBusy();   
	EPD_13IN3K_SendCommand(0x12);  //SWRESET
	EPD_13IN3K_ReadBusy();   

	EPD_13IN3K_SendCommand(0x0C); //set soft start     
	EPD_13IN3K_SendData(0xAE);
	EPD_13IN3K_SendData(0xC7);
	EPD_13IN3K_SendData(0xC3);
	EPD_13IN3K_SendData(0xC0);
	EPD_13IN3K_SendData(0x80);

	EPD_13IN3K_SendCommand(0x01);   //      drive output control    
	EPD_13IN3K_SendData((EPD_13IN3K_HEIGHT-1)%256); //  Y  
	EPD_13IN3K_SendData((EPD_13IN3K_HEIGHT-1)/256); //  Y 
	EPD_13IN3K_SendData(0x00);

	EPD_13IN3K_SendCommand(0x11);        //    data  entry  mode
	EPD_13IN3K_SendData(0x03);           //       X-mode  x+ y-    

	EPD_13IN3K_SetWindows(0, 0, EPD_13IN3K_WIDTH-1, EPD_13IN3K_HEIGHT-1);

	EPD_13IN3K_SendCommand(0x3C);        // Border       Border setting 
	EPD_13IN3K_SendData(0x01);

	EPD_13IN3K_SendCommand(0x18); // use the internal temperature sensor
	EPD_13IN3K_SendData(0x80);

    // EPD_13IN3K_SendCommand(0x22);  
    // EPD_13IN3K_SendData(0xB1); 
    // EPD_13IN3K_SendCommand(0x20); 
	
	EPD_13IN3K_SetCursor(0, 0);
	
	EPD_13IN3K_ReadBusy();
}

void EPD_13IN3K_Init_Part(void)
{
	EPD_13IN3K_Reset();
	DEV_Delay_ms(100); 

	EPD_13IN3K_SendCommand(0x3C);        // Border       Border setting 
	EPD_13IN3K_SendData(0x80);

    EPD_13IN3K_Lut(Lut_Partial);

    EPD_13IN3K_SendCommand(0x37); 
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00); 
    EPD_13IN3K_SendData(0x40);  
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00);   
    EPD_13IN3K_SendData(0x00);  
    EPD_13IN3K_SendData(0x00);  

    EPD_13IN3K_SendCommand(0x3C); 
    EPD_13IN3K_SendData(0x80);   

    EPD_13IN3K_SendCommand(0x22); 
    EPD_13IN3K_SendData(0xC0);   
    EPD_13IN3K_SendCommand(0x20); 

    EPD_13IN3K_ReadBusy();
}

void EPD_13IN3K_Init_4GRAY(void)
{
	EPD_13IN3K_Reset();
	DEV_Delay_ms(100);

	EPD_13IN3K_ReadBusy();   
	EPD_13IN3K_SendCommand(0x12);  //SWRESET
	EPD_13IN3K_ReadBusy();   

	EPD_13IN3K_SendCommand(0x0C); //set soft start     
	EPD_13IN3K_SendData(0xAE);
	EPD_13IN3K_SendData(0xC7);
	EPD_13IN3K_SendData(0xC3);
	EPD_13IN3K_SendData(0xC0);
	EPD_13IN3K_SendData(0x80);

	EPD_13IN3K_SendCommand(0x01);   //      drive output control    
	EPD_13IN3K_SendData((EPD_13IN3K_HEIGHT-1)%256); //  Y  
	EPD_13IN3K_SendData((EPD_13IN3K_HEIGHT-1)/256); //  Y 
	EPD_13IN3K_SendData(0x00);

	EPD_13IN3K_SendCommand(0x11);        //    data  entry  mode
	EPD_13IN3K_SendData(0x03);           //       X-mode  x+ y-    

	EPD_13IN3K_SetWindows(0, 0, EPD_13IN3K_WIDTH-1, EPD_13IN3K_HEIGHT-1);

	EPD_13IN3K_SendCommand(0x3C);        // Border       Border setting 
	EPD_13IN3K_SendData(0x00);

	EPD_13IN3K_SendCommand(0x18); // use the internal temperature sensor
	EPD_13IN3K_SendData(0x80);
	
	EPD_13IN3K_SetCursor(0, 0);

    EPD_13IN3K_Lut(LUT_DATA_4Gray);
	
	EPD_13IN3K_ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void EPD_13IN3K_Clear(void)
{
	UWORD i, j;
	UWORD height = EPD_13IN3K_HEIGHT;
	UWORD width = EPD_13IN3K_WIDTH/8;	
	
	EPD_13IN3K_SendCommand(0x24);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(0xff);
	}
	EPD_13IN3K_TurnOnDisplay();
}

void EPD_13IN3K_color_Base(UBYTE color)
{
	UWORD i, j;
	UWORD height = EPD_13IN3K_HEIGHT;
	UWORD width = EPD_13IN3K_WIDTH/8;	
	
	EPD_13IN3K_SendCommand(0x24);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(color);
	}

    EPD_13IN3K_SendCommand(0x26);
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(color);
	}
	EPD_13IN3K_TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void EPD_13IN3K_Display(UBYTE *Image)
{
	UWORD i, j;
	UWORD height = EPD_13IN3K_HEIGHT;
	UWORD width = EPD_13IN3K_WIDTH/8;
	
	EPD_13IN3K_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(Image[j + i*width]);
	}
	EPD_13IN3K_TurnOnDisplay();	
}

void EPD_13IN3K_Display_Base(UBYTE *Image)
{
	UWORD i, j;
	UWORD height = EPD_13IN3K_HEIGHT;
	UWORD width = EPD_13IN3K_WIDTH/8;
	
	EPD_13IN3K_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(Image[j + i*width]);
	}

    EPD_13IN3K_SendCommand(0x26);   //write RAM for black(0)/white (1)
	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
			EPD_13IN3K_SendData(Image[j + i*width]);
	}
	EPD_13IN3K_TurnOnDisplay();	
}

void EPD_13IN3K_Display_Part(UBYTE *Image, UWORD x, UWORD y, UWORD w, UWORD l)
{
	UWORD Ystart = y;
	UWORD Yend =  y + l;
    UWORD Xstart = x;
    UWORD Xend = x + w;


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

	EPD_13IN3K_SetWindows(Xstart*8, y, Xend*8, y+l-1);

	EPD_13IN3K_SetCursor(Xstart*8, y);

	EPD_13IN3K_SendCommand(0x24);   //write RAM for black(0)/white (1)
	for (i = 0; i < IMAGE_COUNTER; i++) {
	    EPD_13IN3K_SendData(Image[i]);
	}

	EPD_13IN3K_TurnOnDisplay_Part();	
}

void EPD_13IN3K_4GrayDisplay(UBYTE *Image)
{
    UDOUBLE i,j,k;
    UBYTE temp1,temp2,temp3;
    UWORD height = EPD_13IN3K_HEIGHT;
	UWORD width = EPD_13IN3K_WIDTH/8;

    // old  data
    EPD_13IN3K_SendCommand(0x24);
    for(i=0; i<height*width; i++) {            
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;
                else if(temp2 == 0x00)
                    temp3 |= 0x01; 
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else //0x40
                    temp3 |= 0x00; 
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0) 
                    temp3 |= 0x00;
                else if(temp2 == 0x00) 
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; 
                else    //0x40
                    temp3 |= 0x00;	
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }

        }
        EPD_13IN3K_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_13IN3K_SendCommand(0x26);   //write RAM for black(0)/white (1)
    for(i=0; i<height*width; i++) {        
        temp3=0;
        for(j=0; j<2; j++) {
            temp1 = Image[i*2+j];
            for(k=0; k<2; k++) {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x01;  //black
                else if(temp2 == 0x80)
                    temp3 |= 0x00;  //gray1
                else //0x40
                    temp3 |= 0x01; //gray2
                temp3 <<= 1;

                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x00;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x00; //gray1
                else    //0x40
                    temp3 |= 0x01;	//gray2
                if(j!=1 || k!=1)
                    temp3 <<= 1;

                temp1 <<= 2;
            }
        }
        EPD_13IN3K_SendData(temp3);
        // printf("%x",temp3);
    }

    EPD_13IN3K_TurnOnDisplay_4GRAY();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_13IN3K_Sleep(void)
{
	EPD_13IN3K_SendCommand(0x10); //enter deep sleep
	EPD_13IN3K_SendData(0x03); 
	DEV_Delay_ms(100);
}
