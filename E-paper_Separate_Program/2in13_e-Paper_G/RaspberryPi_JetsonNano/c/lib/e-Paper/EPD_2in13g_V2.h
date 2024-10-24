/*****************************************************************************
* | File      	:   EPD_2in13g.h
* | Author      :   Waveshare team
* | Function    :   2inch13 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-05-29
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_2IN13G_V2_H_
#define __EPD_2IN13G_V2_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN13G_V2_WIDTH       122
#define EPD_2IN13G_V2_HEIGHT      250

// Color
#define  EPD_2IN13G_V2_BLACK   0x0
#define  EPD_2IN13G_V2_WHITE   0x1
#define  EPD_2IN13G_V2_YELLOW  0x2
#define  EPD_2IN13G_V2_RED     0x3

void EPD_2IN13G_V2_Init(void);
void EPD_2IN13G_V2_Init_Fast(void);
void EPD_2IN13G_V2_Clear(UBYTE color);
void EPD_2IN13G_V2_Display(UBYTE *Image);
void EPD_2IN13G_V2_Sleep(void);

#endif
