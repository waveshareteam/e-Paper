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
#include "EPD_3in52.h"
#include "Debug.h"

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

unsigned char EPD_3IN52_Flag = 0;

/******************************************************************************
function :	Software reset
parameter:
******************************************************************************/
void EPD_3IN52_Reset(void)
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
void EPD_3IN52_SendCommand(UBYTE Reg)
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
void EPD_3IN52_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

/******************************************************************************
function :	Read Busy
parameter:
******************************************************************************/
void EPD_3IN52_ReadBusy(void)
{
    Debug("e-Paper busy\r\n");
    UBYTE busy;
    do {
        busy = DEV_Digital_Read(EPD_BUSY_PIN);
    } while(!busy);
    DEV_Delay_ms(200);
    Debug("e-Paper busy release\r\n");
}

/**
 * @brief 
 * 
 */
void EPD_3IN52_lut(void)
{
    UBYTE count;
    EPD_3IN52_SendCommand(0x20);        // vcom
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_vcom[count]);
    }
         
    EPD_3IN52_SendCommand(0x21);        // ww --
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_ww[count]);
    }
        
    EPD_3IN52_SendCommand(0x22);        // bw r
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_bw[count]);
    }
        
    EPD_3IN52_SendCommand(0x23);        // wb w
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_bb[count]);
    }
        
    EPD_3IN52_SendCommand(0x24);        // bb b
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_wb[count]);
    }
}

/**
 * @brief 
 * 
 */
void EPD_3IN52_refresh(void)
{
    EPD_3IN52_SendCommand(0x17);
    EPD_3IN52_SendData(0xA5);
    EPD_3IN52_ReadBusy();
    DEV_Delay_ms(200);
}

// LUT download
void EPD_3IN52_lut_GC(void)
{
    UBYTE count;
    EPD_3IN52_SendCommand(0x20);        // vcom
    for(count = 0; count < 56 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R20_GC[count]);
    }
        
    EPD_3IN52_SendCommand(0x21);        // red not use
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R21_GC[count]);
    }
        
    EPD_3IN52_SendCommand(0x24);        // bb b
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R24_GC[count]);
    }
    
    if(EPD_3IN52_Flag == 0)
    {
        EPD_3IN52_SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R22_GC[count]);
        }
            
        EPD_3IN52_SendCommand(0x23);    // wb w
        for(count = 0; count < 42 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R23_GC[count]);
        }
            
        EPD_3IN52_Flag = 1;
    }
        
    else
    {
        EPD_3IN52_SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R23_GC[count]);
        }
            
        EPD_3IN52_SendCommand(0x23);    // wb w
        for(count = 0; count < 42 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R22_GC[count]);
        }
            
       EPD_3IN52_Flag = 0;
    }
}

// LUT download        
void EPD_3IN52_lut_DU(void)
{
    UBYTE count;
    EPD_3IN52_SendCommand(0x20);      // vcom
    for(count = 0; count < 56 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R20_DU[count]);
    }
        
    EPD_3IN52_SendCommand(0x21);     // red not use
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R21_DU[count]);
    }
        
    EPD_3IN52_SendCommand(0x24);    // bb b
    for(count = 0; count < 42 ; count++)
    {
        EPD_3IN52_SendData(EPD_3IN52_lut_R24_DU[count]);
    }
    
    if(EPD_3IN52_Flag == 0)
    {
        EPD_3IN52_SendCommand(0x22);      // bw r
        for(count = 0; count < 56 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R22_DU[count]);
        }
            
        EPD_3IN52_SendCommand(0x23);     // wb w
        for(count = 0; count < 42 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R23_DU[count]);
        }
            
        EPD_3IN52_Flag = 1;
    }
        
    else
    {
        EPD_3IN52_SendCommand(0x22);    // bw r
        for(count = 0; count < 56 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R23_DU[count]);
        }
            
        EPD_3IN52_SendCommand(0x23);   // wb w
        for(count = 0; count < 42 ; count++)
        {
            EPD_3IN52_SendData(EPD_3IN52_lut_R22_DU[count]);
        }
            
        EPD_3IN52_Flag = 0;
    }
}  
            
    




/******************************************************************************
function :	Initialize the e-Paper register
parameter:
******************************************************************************/
void EPD_3IN52_Init(void)
{
    EPD_3IN52_Flag = 0;
    EPD_3IN52_Reset();

    EPD_3IN52_SendCommand(0x00);		// panel setting   PSR
    EPD_3IN52_SendData(0xFF);			// RES1 RES0 REG KW/R     UD    SHL   SHD_N  RST_N	
    EPD_3IN52_SendData(0x01);			// x x x VCMZ TS_AUTO TIGE NORG VC_LUTZ

    EPD_3IN52_SendCommand(0x01);		// POWER SETTING   PWR
    EPD_3IN52_SendData(0x03);			//  x x x x x x VDS_EN VDG_EN	
    EPD_3IN52_SendData(0x10);			//  x x x VCOM_SLWE VGH[3:0]   VGH=20V, VGL=-20V	
    EPD_3IN52_SendData(0x3F);			//  x x VSH[5:0]	VSH = 15V
    EPD_3IN52_SendData(0x3F);			//  x x VSL[5:0]	VSL=-15V
    EPD_3IN52_SendData(0x03);			//  OPTEN VDHR[6:0]  VHDR=6.4V
                                        // T_VDS_OFF[1:0] 00=1 frame; 01=2 frame; 10=3 frame; 11=4 frame
    EPD_3IN52_SendCommand(0x06);		// booster soft start   BTST 
    EPD_3IN52_SendData(0x37);			//  BT_PHA[7:0]  	
    EPD_3IN52_SendData(0x3D);			//  BT_PHB[7:0]	
    EPD_3IN52_SendData(0x3D);			//  x x BT_PHC[5:0]	

    EPD_3IN52_SendCommand(0x60);		// TCON setting			TCON 
    EPD_3IN52_SendData(0x22);			// S2G[3:0] G2S[3:0]   non-overlap = 12		

    EPD_3IN52_SendCommand(0x82);		// VCOM_DC setting		VDCS 
    EPD_3IN52_SendData(0x07);			// x  VDCS[6:0]	VCOM_DC value= -1.9v    00~3f,0x12=-1.9v

    EPD_3IN52_SendCommand(0x30);			 
    EPD_3IN52_SendData(0x09);		

    EPD_3IN52_SendCommand(0xe3);		// power saving			PWS 
    EPD_3IN52_SendData(0x88);			// VCOM_W[3:0] SD_W[3:0]

    EPD_3IN52_SendCommand(0x61);		// resoultion setting 
    EPD_3IN52_SendData(0xf0);			//  HRES[7:3] 0 0 0	
    EPD_3IN52_SendData(0x01);			//  x x x x x x x VRES[8]	
    EPD_3IN52_SendData(0x68);			//  VRES[7:0]

    EPD_3IN52_SendCommand(0x50);			
    EPD_3IN52_SendData(0xB7);			
}


void EPD_3IN52_display(UBYTE* picData)
{
    UWORD i;
    EPD_3IN52_SendCommand(0x13);		     //Transfer new data
    for(i=0;i<(EPD_3IN52_WIDTH*EPD_3IN52_HEIGHT/8);i++)	     
    {
        EPD_3IN52_SendData(*picData);
        picData++;
    }
}

void EPD_3IN52_display_NUM(UBYTE NUM)
{
    UWORD row, column;
    // UWORD pcnt = 0;

    EPD_3IN52_SendCommand(0x13);		     //Transfer new data

    for(column=0; column<EPD_3IN52_HEIGHT; column++)   
    {
        for(row=0; row<EPD_3IN52_WIDTH/8; row++)  
        {
            switch (NUM)
            {
                case EPD_3IN52_WHITE:
                    EPD_3IN52_SendData(0xFF);
                    break;  
                        
                case EPD_3IN52_BLACK:
                    EPD_3IN52_SendData(0x00);
                    break;  
                        
                case EPD_3IN52_Source_Line:
                    EPD_3IN52_SendData(0xAA);  
                    break;
                        
                case EPD_3IN52_Gate_Line:
                    if(column%2)
                        EPD_3IN52_SendData(0xff); //An odd number of Gate line  
                    else
                        EPD_3IN52_SendData(0x00); //The even line Gate  
                    break;			
                        
                case EPD_3IN52_Chessboard:
                    if(row>=(EPD_3IN52_WIDTH/8/2)&&column>=(EPD_3IN52_HEIGHT/2))
                        EPD_3IN52_SendData(0xff);
                    else if(row<(EPD_3IN52_WIDTH/8/2)&&column<(EPD_3IN52_HEIGHT/2))
                        EPD_3IN52_SendData(0xff);										
                    else
                        EPD_3IN52_SendData(0x00);
                    break; 			
                        
                case EPD_3IN52_LEFT_BLACK_RIGHT_WHITE:
                    if(row>=(EPD_3IN52_WIDTH/8/2))
                        EPD_3IN52_SendData(0xff);
                    else
                        EPD_3IN52_SendData(0x00);
                    break;
                            
                case EPD_3IN52_UP_BLACK_DOWN_WHITE:
                    if(column>=(EPD_3IN52_HEIGHT/2))
                        EPD_3IN52_SendData(0xFF);
                    else
                        EPD_3IN52_SendData(0x00);
                    break;
                            
                case EPD_3IN52_Frame:
                    if(column==0||column==(EPD_3IN52_HEIGHT-1))
                        EPD_3IN52_SendData(0x00);						
                    else if(row==0)
                        EPD_3IN52_SendData(0x7F);
                    else if(row==(EPD_3IN52_WIDTH/8-1))
                        EPD_3IN52_SendData(0xFE);					
                    else
                        EPD_3IN52_SendData(0xFF);
                    break; 					
                            
                case EPD_3IN52_Crosstalk:
                    if((row>=(EPD_3IN52_WIDTH/8/3)&&row<=(EPD_3IN52_WIDTH/8/3*2)&&column<=(EPD_3IN52_HEIGHT/3))||(row>=(EPD_3IN52_WIDTH/8/3)&&row<=(EPD_3IN52_WIDTH/8/3*2)&&column>=(EPD_3IN52_HEIGHT/3*2)))
                        EPD_3IN52_SendData(0x00);
                    else
                        EPD_3IN52_SendData(0xFF);
                    break; 					
                            
                case EPD_3IN52_Image:
                        //EPD_3IN52_SendData(gImage_1[pcnt++]);
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
void EPD_3IN52_Clear(void)
{
    UWORD i;
    EPD_3IN52_SendCommand(0x13);		     //Transfer new data
    for(i=0;i<(EPD_3IN52_WIDTH*EPD_3IN52_HEIGHT/8);i++)	     
    {
        EPD_3IN52_SendData(0xFF);
    }
    EPD_3IN52_lut_GC();
	EPD_3IN52_refresh();
}

/******************************************************************************
function :	Enter sleep mode
parameter:
******************************************************************************/
void EPD_3IN52_sleep(void)
{
    EPD_3IN52_SendCommand(0X07);  	//deep sleep
    EPD_3IN52_SendData(0xA5);
}



