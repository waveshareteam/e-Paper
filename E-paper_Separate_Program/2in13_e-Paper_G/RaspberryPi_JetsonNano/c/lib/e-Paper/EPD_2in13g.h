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
#ifndef __EPD_2IN13G_H_
#define __EPD_2IN13G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN13G_WIDTH       122
#define EPD_2IN13G_HEIGHT      250

// Color
#define  EPD_2IN13G_BLACK   0x0
#define  EPD_2IN13G_WHITE   0x1
#define  EPD_2IN13G_YELLOW  0x2
#define  EPD_2IN13G_RED     0x3

void EPD_2IN13G_Init(void);
void EPD_2IN13G_Clear(UBYTE color);
void EPD_2IN13G_Display(UBYTE *Image);
void EPD_2IN13G_Sleep(void);

#endif
