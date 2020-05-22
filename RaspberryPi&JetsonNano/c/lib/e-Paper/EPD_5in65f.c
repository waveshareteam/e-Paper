#include "EPD_5in65f.h"

static void EPD_5IN65F_Reset(void)
{
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
    DEV_Digital_Write(EPD_RST_PIN, 0);
    DEV_Delay_ms(1);
    DEV_Digital_Write(EPD_RST_PIN, 1);
    DEV_Delay_ms(200);
}


static void EPD_5IN65F_SendCommand(UBYTE Reg)
{
    DEV_Digital_Write(EPD_DC_PIN, 0);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Reg);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}

static void EPD_5IN65F_SendData(UBYTE Data)
{
    DEV_Digital_Write(EPD_DC_PIN, 1);
    DEV_Digital_Write(EPD_CS_PIN, 0);
    DEV_SPI_WriteByte(Data);
    DEV_Digital_Write(EPD_CS_PIN, 1);
}


static void EPD_5IN65F_BusyHigh(void)// If BUSYN=0 then waiting
{
    while(!(DEV_Digital_Read(EPD_BUSY_PIN)));
}

static void EPD_5IN65F_BusyLow(void)// If BUSYN=1 then waiting
{
    while(DEV_Digital_Read(EPD_BUSY_PIN));
}


void EPD_5IN65F_Init(void)
{
	EPD_5IN65F_Reset();
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x00);
    EPD_5IN65F_SendData(0xEF);
    EPD_5IN65F_SendData(0x08);
    EPD_5IN65F_SendCommand(0x01);
    EPD_5IN65F_SendData(0x37);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendData(0x23);
    EPD_5IN65F_SendData(0x23);
    EPD_5IN65F_SendCommand(0x03);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendCommand(0x06);
    EPD_5IN65F_SendData(0xC7);
    EPD_5IN65F_SendData(0xC7);
    EPD_5IN65F_SendData(0x1D);
    EPD_5IN65F_SendCommand(0x30);
    EPD_5IN65F_SendData(0x3C);
    EPD_5IN65F_SendCommand(0x40);
    EPD_5IN65F_SendData(0x00);
    EPD_5IN65F_SendCommand(0x50);
    EPD_5IN65F_SendData(0x37);
    EPD_5IN65F_SendCommand(0x60);
    EPD_5IN65F_SendData(0x22);
    EPD_5IN65F_SendCommand(0x61);
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0xE3);
    EPD_5IN65F_SendData(0xAA);
	
	DEV_Delay_ms(100);
    EPD_5IN65F_SendCommand(0x50);
    EPD_5IN65F_SendData(0x37);
}

void EPD_5IN65F_Clear(UBYTE color)
{
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);
    for(int i=0; i<EPD_5IN65F_HEIGHT; i++) {
        for(int j=0; j<EPD_5IN65F_WIDTH/2; j++)
            EPD_5IN65F_SendData((color<<4)|color);
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
    DEV_Delay_ms(500);
}

void EPD_5IN65F_Display(const UBYTE *image)
{
    unsigned long i,j;
    EPD_5IN65F_SendCommand(0x61);//Set Resolution setting
    EPD_5IN65F_SendData(0x02);
    EPD_5IN65F_SendData(0x58);
    EPD_5IN65F_SendData(0x01);
    EPD_5IN65F_SendData(0xC0);
    EPD_5IN65F_SendCommand(0x10);
    for(i=0; i<EPD_5IN65F_HEIGHT; i++) {
        for(j=0; j<EPD_5IN65F_WIDTH/2; j++)
            EPD_5IN65F_SendData(image[j+((EPD_5IN65F_WIDTH/2)*i)]);
    }
    EPD_5IN65F_SendCommand(0x04);//0x04
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x12);//0x12
    EPD_5IN65F_BusyHigh();
    EPD_5IN65F_SendCommand(0x02);  //0x02
    EPD_5IN65F_BusyLow();
	DEV_Delay_ms(200);
}


void EPD_5IN65F_Sleep(void)
{
    DEV_Delay_ms(100);
    EPD_5IN65F_SendCommand(0x07);
    EPD_5IN65F_SendData(0xA5);
    DEV_Delay_ms(100);
	DEV_Digital_Write(EPD_RST_PIN, 0); // Reset
}

