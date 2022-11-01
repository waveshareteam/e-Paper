/**
 *  @filename   :   epd2in7_V2.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Waveshare
 *
 *  Copyright (C) Waveshare     September 20 2022
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
#include "epd2in7_V2.h"

static const unsigned char LUT_DATA_4Gray[159] =
{
0x40,	0x48,	0x80,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x8,	0x48,	0x10,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x2,	0x48,	0x4,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x20,	0x48,	0x1,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
0xA,	0x19,	0x0,	0x3,	0x8,	0x0,	0x0,					
0x14,	0x1,	0x0,	0x14,	0x1,	0x0,	0x3,					
0xA,	0x3,	0x0,	0x8,	0x19,	0x0,	0x0,					
0x1,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,			
0x22,	0x17,	0x41,	0x0,	0x32,	0x1C
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
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }

    Reset();
    ReadBusy();

    SendCommand(0x12); //SWRESET
    ReadBusy();

    SendCommand(0x45); //set Ram-Y address start/end position          
    SendData(0x00);
    SendData(0x00);
    SendData(0x07); //0x0107-->(263+1)=264
    SendData(0x01);

    SendCommand(0x4F);   // set RAM y address count to 0;    
    SendData(0x00);
    SendData(0x00);

    SendCommand(0x11);   // data entry mode
    SendData(0x03);
    return 0;
}

int Epd::Init_Fast(void) {
    /* this calls the peripheral hardware interface, see epdif */
    if (IfInit() != 0) {
        return -1;
    }

    Reset();
    ReadBusy();

    SendCommand(0x12); //SWRESET
    ReadBusy();

    SendCommand(0x18); //Read built-in temperature sensor
	SendData(0x80);	

	SendCommand(0x22); // Load temperature value
	SendData(0xB1);		
	SendCommand(0x20);	
	ReadBusy();

	SendCommand(0x1A); // Write to temperature register
	SendData(0x64);		
	SendData(0x00);	

	SendCommand(0x45); //set Ram-Y address start/end position          
	SendData(0x00);   
	SendData(0x00);
	SendData(0x07); //0x0107-->(263+1)=264
	SendData(0x01);

	SendCommand(0x4F);   // set RAM y address count to 0;    
	SendData(0x00);
	SendData(0x00);

	SendCommand(0x11);   // data entry mode      
	SendData(0x03);
							
	SendCommand(0x22); // Load temperature value
	SendData(0x91);		
	SendCommand(0x20);	
	ReadBusy(); 
    return 0;
}

void Epd::Init_4Gray(void)
{
    Reset();

	ReadBusy();
	SendCommand(0x12); // soft reset
	ReadBusy();

	SendCommand(0x74); //set analog block control       
	SendData(0x54);
	SendCommand(0x7E); //set digital block control          
	SendData(0x3B);

	SendCommand(0x01); //Driver output control      
	SendData(0x07);
	SendData(0x01);
	SendData(0x00);

	SendCommand(0x11); //data entry mode       
	SendData(0x03);

	SendCommand(0x44); //set Ram-X address start/end position   
	SendData(0x00);
	SendData(0x15);    //0x15-->(21+1)*8=176

	SendCommand(0x45); //set Ram-Y address start/end position          
	SendData(0x00);
	SendData(0x00);
	SendData(0x07);//0x0107-->(263+1)=264
	SendData(0x01);


	SendCommand(0x3C); //BorderWavefrom
	SendData(0x00);	


	SendCommand(0x2C);     //VCOM Voltage
	SendData(LUT_DATA_4Gray[158]);    //0x1C


	SendCommand(0x3F); //EOPQ    
	SendData(LUT_DATA_4Gray[153]);
	
	SendCommand(0x03); //VGH      
	SendData(LUT_DATA_4Gray[154]);

	SendCommand(0x04); //      
	SendData(LUT_DATA_4Gray[155]); //VSH1   
	SendData(LUT_DATA_4Gray[156]); //VSH2   
	SendData(LUT_DATA_4Gray[157]); //VSL   
   
	Lut(); //LUT

	
	SendCommand(0x4E);   // set RAM x address count to 0;
	SendData(0x00);
	SendCommand(0x4F);   // set RAM y address count to 0X199;    
	SendData(0x00);
	SendData(0x00);
    ReadBusy();
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
    Serial.print("e-Paper busy\r\n");
    while(DigitalRead(busy_pin) == 1) {      //1: busy, 0: idle
        DelayMs(100);
    }      
    Serial.print("e-Paper busy release\r\n");
}

/**
 *  @brief: module reset. 
 *          often used to awaken the module in deep sleep, 
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
    DigitalWrite(reset_pin, LOW);
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(200);   
}

/**
 *  @brief: set the look-up tables
 */
void Epd::Lut(void) {
    unsigned int count;
    SendCommand(0x32); //vcom
    for(count = 0; count < 153; count++) {
        SendData(LUT_DATA_4Gray[count]);
    }
}

/******************************************************************************
function :	Turn on display
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

void Epd::TurnOnDisplay_Partial(void)
{
	SendCommand(0x22);
	SendData(0xFF);
	SendCommand(0x20);
	ReadBusy();
}

void Epd::TurnOnDisplay_4GRAY(void)
{
	SendCommand(0x22);
	SendData(0xC7);
	SendCommand(0x20);
	ReadBusy();
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(void)
{
    unsigned int Width, Height;
    Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
    Height = HEIGHT;

    SendCommand(0x24);
    for (unsigned int j = 0; j < Height; j++) {
        for (unsigned int i = 0; i < Width; i++) {
            SendData(0XFF);
        }
    }
	TurnOnDisplay();
}

/******************************************************************************
function :	Sends the image buffer in RAM to e-Paper and displays
parameter:
******************************************************************************/
void Epd::Display(const unsigned char* Image)
{
	unsigned int Width, Height;
	Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
	Height = HEIGHT;

	SendCommand(0x24);
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(pgm_read_byte(&Image[i + j * Width]));
			}
	}
	TurnOnDisplay();
}

void Epd::Display_Fast(const unsigned char* Image)
{
	unsigned int Width, Height;
	Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
	Height = HEIGHT;

	SendCommand(0x24);
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(pgm_read_byte(&Image[i + j * Width]));
			}
	}
	TurnOnDisplay_Fast();
}

void Epd::Display_Base(const unsigned char* Image)
{
	unsigned int Width, Height;
	Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
	Height = HEIGHT;

	SendCommand(0x24);   //Write Black and White image to RAM
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(pgm_read_byte(&Image[i + j * Width]));
			}
	}
	SendCommand(0x26);   //Write Black and White image to RAM
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(pgm_read_byte(&Image[i + j * Width]));
			}
	}
	TurnOnDisplay();	
}

void Epd::Display_Base_color(unsigned char color)
{
	unsigned int Width, Height;
	Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
	Height = HEIGHT;

	SendCommand(0x24);   //Write Black and White image to RAM
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(color);
			}
	}
	SendCommand(0x26);   //Write Black and White image to RAM
	for (unsigned int j = 0; j < Height; j++) {
			for (unsigned int i = 0; i < Width; i++) {
					SendData(color);
			}
	}
	// TurnOnDisplay();	
}

void Epd::Display_Partial(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend)
{

    unsigned int  i, Width;
    unsigned int  IMAGE_COUNTER;

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
	
	Width = Xend -  Xstart;
	IMAGE_COUNTER = Width * (Yend-Ystart);
    Serial.println(IMAGE_COUNTER);

	Xend -= 1;
	Yend -= 1;	
	//Reset
	Reset();

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x80);	
	//	    
	SendCommand(0x44);       // set RAM x address start/end, in page 35
	SendData(Xstart & 0xff);    // RAM x address start at 00h;
	SendData(Xend & 0xff);    // RAM x address end at 0fh(15+1)*8->128 
	SendCommand(0x45);       // set RAM y address start/end, in page 35
	SendData(Ystart & 0xff);    // RAM y address start at 0127h;
	SendData((Ystart>>8) & 0x01);    // RAM y address start at 0127h;
	SendData(Yend & 0xff);    // RAM y address end at 00h;
	SendData((Yend>>8) & 0x01); 

	SendCommand(0x4E);   // set RAM x address count to 0;
	SendData(Xstart & 0xff); 
	SendCommand(0x4F);   // set RAM y address count to 0X127;    
	SendData(Ystart & 0xff);
	SendData((Ystart>>8) & 0x01);


	SendCommand(0x24);   //Write Black and White image to RAM
	for (i = 0; i < IMAGE_COUNTER; i++) {
		SendData(Image[i]);
	}
	TurnOnDisplay_Partial();
}

void Epd::Display_Partial_Not_refresh(unsigned char* Image, unsigned int  Xstart, unsigned int  Ystart, unsigned int  Xend, unsigned int  Yend)
{

    unsigned int  i, Width;
    unsigned int  IMAGE_COUNTER;

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
	
	Width = Xend -  Xstart;
	IMAGE_COUNTER = Width * (Yend-Ystart);
    Serial.println(IMAGE_COUNTER);

	Xend -= 1;
	Yend -= 1;	
	//Reset
	Reset();

	SendCommand(0x3C); //BorderWavefrom
	SendData(0x80);	
	//	    
	SendCommand(0x44);       // set RAM x address start/end, in page 35
	SendData(Xstart & 0xff);    // RAM x address start at 00h;
	SendData(Xend & 0xff);    // RAM x address end at 0fh(15+1)*8->128 
	SendCommand(0x45);       // set RAM y address start/end, in page 35
	SendData(Ystart & 0xff);    // RAM y address start at 0127h;
	SendData((Ystart>>8) & 0x01);    // RAM y address start at 0127h;
	SendData(Yend & 0xff);    // RAM y address end at 00h;
	SendData((Yend>>8) & 0x01); 

	SendCommand(0x4E);   // set RAM x address count to 0;
	SendData(Xstart & 0xff); 
	SendCommand(0x4F);   // set RAM y address count to 0X127;    
	SendData(Ystart & 0xff);
	SendData((Ystart>>8) & 0x01);


	SendCommand(0x24);   //Write Black and White image to RAM
	for (i = 0; i < IMAGE_COUNTER; i++) {
		SendData(Image[i]);
	}
	// TurnOnDisplay_Partial();
}

void Epd::Display4Gray(const unsigned char *Image)
{
    int i,j,k;
    unsigned char temp1,temp2,temp3;

    SendCommand(0x24);	       
    for(i=0;i<5808;i++)	               //5808*4  46464
    {
        temp3=0;
        for(j=0;j<2;j++)	
        {
            temp1 = pgm_read_byte(&Image[i*2+j]);
            for(k=0;k<2;k++)	
            {
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)
                    temp3 |= 0x00;//white
                else if(temp2 == 0x00)
                    temp3 |= 0x01;  //black
                else if(temp2 == 0x80) 
                    temp3 |= 0x01;  //gray1
                else //0x40
                    temp3 |= 0x00; //gray2
                temp3 <<= 1;	
                
                temp1 <<= 2;
                temp2 = temp1&0xC0 ;
                if(temp2 == 0xC0)  //white
                    temp3 |= 0x00;
                else if(temp2 == 0x00) //black
                    temp3 |= 0x01;
                else if(temp2 == 0x80)
                    temp3 |= 0x01; //gray1
                else    //0x40
                        temp3 |= 0x00;	//gray2	
                if(j!=1 || k!=1)				
                    temp3 <<= 1;
                
                temp1 <<= 2;
            }
            
         }
        SendData(temp3);			
    }
    // new  data
    SendCommand(0x26);	       
    for(i=0;i<5808;i++)	               //5808*4  46464
    {
        temp3=0;
        for(j=0;j<2;j++)	
        {
            temp1 = pgm_read_byte(&Image[i*2+j]);
            for(k=0;k<2;k++)	
            {
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
        SendData(temp3);	
    }

    TurnOnDisplay_4GRAY();
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power. 
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a 
 *         check code, the command would be executed if check code = 0xA5. 
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep() {
  SendCommand(0X10);
  SendData(0x01);
}
