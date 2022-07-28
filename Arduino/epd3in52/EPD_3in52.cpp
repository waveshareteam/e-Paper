/*****************************************************************************
* | File      	:   EPD_3IN52.C
* | Author      :   Waveshare team
* | Function    :   3.52inch e-paper
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-05-07
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
#include <stdlib.h>
#include "EPD_3in52.h"
#include "imagedata.h"



//GC 0.9S
static const UBYTE EPD_3IN52_lut_R20_GC[] =
{
  0x01,0x0f,0x0f,0x0f,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};							  
static const UBYTE EPD_3IN52_lut_R21_GC[] =
{
  0x01,0x4f,0x8f,0x0f,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};						 
static const UBYTE EPD_3IN52_lut_R22_GC[] =
{
  0x01,0x0f,0x8f,0x0f,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const UBYTE EPD_3IN52_lut_R23_GC[] =
{
  0x01,0x4f,0x8f,0x4f,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const UBYTE EPD_3IN52_lut_R24_GC[] =
{
  0x01,0x0f,0x8f,0x4f,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};


// DU 0.3s
static const UBYTE EPD_3IN52_lut_R20_DU[] =
{
  0x01,0x0f,0x01,0x00,0x00,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};							  
static const UBYTE EPD_3IN52_lut_R21_DU[] =
{
  0x01,0x0f,0x01,0x00,0x00,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};						 
static const UBYTE EPD_3IN52_lut_R22_DU[] =
{
  0x01,0x8f,0x01,0x00,0x00,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const UBYTE EPD_3IN52_lut_R23_DU[] =
{
  0x01,0x4f,0x01,0x00,0x00,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};
static const UBYTE EPD_3IN52_lut_R24_DU[] =
{
  0x01,0x0f,0x01,0x00,0x00,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

// 
static const UBYTE EPD_3IN52_lut_vcom[] =
{
  0x01,0x19,0x19,0x19,0x19,0x01,0x01,
  0x01,0x19,0x19,0x19,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

static const UBYTE EPD_3IN52_lut_ww[] =
{
  0x01,0x59,0x99,0x59,0x99,0x01,0x01,
  0x01,0x59,0x99,0x19,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00

};

static const UBYTE EPD_3IN52_lut_bw[] =
{
  0x01,0x59,0x99,0x59,0x99,0x01,0x01,
  0x01,0x59,0x99,0x19,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

static const UBYTE EPD_3IN52_lut_wb[] =
{	
  0x01,0x19,0x99,0x59,0x99,0x01,0x01,
  0x01,0x59,0x99,0x59,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

static const UBYTE EPD_3IN52_lut_bb[] =
{
  0x01,0x19,0x99,0x59,0x99,0x01,0x01,
  0x01,0x59,0x99,0x59,0x01,0x01,0x01,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00
};

Epd::~Epd() {
};

Epd::Epd() {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    EPD_3IN52_Flag = 0;
};

int Epd::Init(void) {
    if (IfInit() != 0) {
        return -1;
    }
    Reset();

    EPD_3IN52_Flag = 0;

    SendCommand(0x00);		// panel setting   PSR
    SendData(0xFF);			// RES1 RES0 REG KW/R     UD    SHL   SHD_N  RST_N	
    SendData(0x01);			// x x x VCMZ TS_AUTO TIGE NORG VC_LUTZ

    SendCommand(0x01);		// POWER SETTING   PWR
    SendData(0x03);			//  x x x x x x VDS_EN VDG_EN	
    SendData(0x10);			//  x x x VCOM_SLWE VGH[3:0]   VGH=20V, VGL=-20V	
    SendData(0x3F);			//  x x VSH[5:0]	VSH = 15V
    SendData(0x3F);			//  x x VSL[5:0]	VSL=-15V
    SendData(0x03);			//  OPTEN VDHR[6:0]  VHDR=6.4V
                                        // T_VDS_OFF[1:0] 00=1 frame; 01=2 frame; 10=3 frame; 11=4 frame
    SendCommand(0x06);		// booster soft start   BTST 
    SendData(0x37);			//  BT_PHA[7:0]  	
    SendData(0x3D);			//  BT_PHB[7:0]	
    SendData(0x3D);			//  x x BT_PHC[5:0]	

    SendCommand(0x60);		// TCON setting			TCON 
    SendData(0x22);			// S2G[3:0] G2S[3:0]   non-overlap = 12		

    SendCommand(0x82);		// VCOM_DC setting		VDCS 
    SendData(0x07);			// x  VDCS[6:0]	VCOM_DC value= -1.9v    00~3f,0x12=-1.9v

    SendCommand(0x30);			 
    SendData(0x09);		

    SendCommand(0xe3);		// power saving			PWS 
    SendData(0x88);			// VCOM_W[3:0] SD_W[3:0]

    SendCommand(0x61);		// resoultion setting 
    SendData(0xf0);			//  HRES[7:3] 0 0 0	
    SendData(0x01);			//  x x x x x x x VRES[8]	
    SendData(0x68);			//  VRES[7:0]

    SendCommand(0x50);			
    SendData(0xB7);		
    return 0;
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
    Serial.print("e-Paper busy \r\n ");
    UBYTE busy;
    do {
        busy = DigitalRead(busy_pin);
    } while(busy);    
    DelayMs(200);
    Serial.print("e-Paper busy release \r\n ");
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void) {
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
    DigitalWrite(reset_pin, LOW);                //module reset    
    DelayMs(2);
    DigitalWrite(reset_pin, HIGH);
    DelayMs(20);    
}

void Epd::lut(void)
{
    UBYTE count;
    SendCommand(0x20);        // vcom
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_vcom[count]);
    }
         
    SendCommand(0x21);        // ww --
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_ww[count]);
    }
        
    SendCommand(0x22);        // bw r
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_bw[count]);
    }
        
    SendCommand(0x23);        // wb w
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_bb[count]);
    }
        
    SendCommand(0x24);        // bb b
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_wb[count]);
    }
}

void Epd::refresh(void)
{
    SendCommand(0x17);
    SendData(0xA5);
    ReadBusy();
    DelayMs(200);
}

// LUT download
void Epd::lut_GC(void)
{
    UBYTE count;
    SendCommand(0x20);        // vcom
    for(count = 0; count < 56 ; count++)
    {
        SendData(EPD_3IN52_lut_R20_GC[count]);
    }
        
    SendCommand(0x21);        // red not use
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_R21_GC[count]);
    }
        
    SendCommand(0x24);        // bb b
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_R24_GC[count]);
    }
    
    if(EPD_3IN52_Flag == 0)
    {
        SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            SendData(EPD_3IN52_lut_R22_GC[count]);
        }
            
        SendCommand(0x23);    // wb w
        for(count = 0; count < 42 ; count++)
        {
            SendData(EPD_3IN52_lut_R23_GC[count]);
        }
            
        EPD_3IN52_Flag = 1;
    }
        
    else
    {
        SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            SendData(EPD_3IN52_lut_R23_GC[count]);
        }
            
        SendCommand(0x23);    // wb w
        for(count = 0; count < 42 ; count++)
        {
            SendData(EPD_3IN52_lut_R22_GC[count]);
        }
            
       EPD_3IN52_Flag = 0;
    }
}

// LUT download        
void Epd::lut_DU(void)
{
    UBYTE count;
    SendCommand(0x20);      // vcom
    for(count = 0; count < 56 ; count++)
    {
        SendData(EPD_3IN52_lut_R20_DU[count]);
    }
        
    SendCommand(0x21);     // red not use
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_R21_DU[count]);
    }
        
    SendCommand(0x24);    // bb b
    for(count = 0; count < 42 ; count++)
    {
        SendData(EPD_3IN52_lut_R24_DU[count]);
    }
    
    if(EPD_3IN52_Flag == 0)
    {
        SendCommand(0x22);      // bw r
        for(count = 0; count < 56 ; count++)
        {
            SendData(EPD_3IN52_lut_R22_DU[count]);
        }
            
        SendCommand(0x23);     // wb w
        for(count = 0; count < 42 ; count++)
        {
            SendData(EPD_3IN52_lut_R23_DU[count]);
        }
            
        EPD_3IN52_Flag = 1;
    }
        
    else
    {
        SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            SendData(EPD_3IN52_lut_R23_DU[count]);
        }
            
        SendCommand(0x23);   // wb w
        for(count = 0; count < 42 ; count++)
        {
            SendData(EPD_3IN52_lut_R22_DU[count]);
        }
            
        EPD_3IN52_Flag = 0;
    }
}  
    
void Epd::display(UBYTE* picData)
{
    UWORD i;
    SendCommand(0x13);		     //Transfer new data
    for(i=0;i<(width*height/8);i++)	     
    {
        SendData(pgm_read_byte(&picData[i]));
    }
}

void Epd::display_part(UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh)
{
    UWORD i,j;
    SendCommand(0x13);		     //Transfer new data
    for(i=0; i<height; i++)
        for(j=0; j<(width/8); j++)
        {
            if((j >= xstart/8) && (j < (image_width + xstart)/8) && (i >= ystart) && (i <= (ystart + image_heigh)) )
            {
                SendData(Image[(i-ystart) * image_width/8 + j - xstart/8]);
                // Serial.print(Image[(i-ystart) * image_width/8 + j - xstart], HEX);
                // Serial.print(" ");
            }
            else
            {
                SendData(0x00);
            }
        }
}


void Epd::display_NUM(UBYTE NUM)
{
    UWORD row, column;
    // UWORD pcnt = 0;

    SendCommand(0x13);		     //Transfer new data

    for(column=0; column<height; column++)   
    {
        for(row=0; row<width/8; row++)  
        {
            switch (NUM)
            {
                case EPD_3IN52_WHITE:
                    SendData(0xFF);
                    break;  
                        
                case EPD_3IN52_BLACK:
                    SendData(0x00);
                    break;  
                        
                case EPD_3IN52_Source_Line:
                    SendData(0xAA);  
                    break;
                        
                case EPD_3IN52_Gate_Line:
                    if(column%2)
                        SendData(0xff); //An odd number of Gate line  
                    else
                        SendData(0x00); //The even line Gate  
                    break;			
                        
                case EPD_3IN52_Chessboard:
                    if(row>=(width/8/2)&&column>=(height/2))
                        SendData(0xff);
                    else if(row<(width/8/2)&&column<(height/2))
                        SendData(0xff);										
                    else
                        SendData(0x00);
                    break; 			
                        
                case EPD_3IN52_LEFT_BLACK_RIGHT_WHITE:
                    if(row>=(width/8/2))
                        SendData(0xff);
                    else
                        SendData(0x00);
                    break;
                            
                case EPD_3IN52_UP_BLACK_DOWN_WHITE:
                    if(column>=(height/2))
                        SendData(0xFF);
                    else
                        SendData(0x00);
                    break;
                            
                case EPD_3IN52_Frame:
                    if(column==0||column==(height-1))
                        SendData(0x00);						
                    else if(row==0)
                        SendData(0x7F);
                    else if(row==(width/8-1))
                        SendData(0xFE);					
                    else
                        SendData(0xFF);
                    break; 					
                            
                case EPD_3IN52_Crosstalk:
                    if((row>=(width/8/3)&&row<=(width/8/3*2)&&column<=(height/3))||(row>=(width/8/3)&&row<=(width/8/3*2)&&column>=(height/3*2)))
                        SendData(0x00);
                    else
                        SendData(0xFF);
                    break; 					
                            
                case EPD_3IN52_Image:
                        //SendData(gImage_1[pcnt++]);
                    break;  
                                        
                default:
                    break;
            }
        }
    }	
}

/******************************************************************************
function :	Clear screen
parameter:
******************************************************************************/
void Epd::Clear(void)
{
    UWORD i;
    SendCommand(0x13);		     //Transfer new data
    for(i=0;i<(width*height/8);i++)	     
    {
        SendData(0xFF);
    }
    lut_GC();
	refresh();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void Epd::sleep(void)
{
    SendCommand(0X07);  	//deep sleep
    SendData(0xA5);
}