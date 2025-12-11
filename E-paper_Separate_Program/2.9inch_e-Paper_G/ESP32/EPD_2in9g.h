/*****************************************************************************
* | File      	:   EPD_2in9g.h
* | Author      :   Waveshare team
* | Function    :   2.9inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-12-02
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_2IN9G_H_
#define __EPD_2IN9G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_2IN9G_WIDTH        128
#define EPD_2IN9G_HEIGHT       296

// Color
#define  EPD_2IN9G_BLACK   0x0
#define  EPD_2IN9G_WHITE   0x1
#define  EPD_2IN9G_YELLOW  0x2
#define  EPD_2IN9G_RED     0x3

void EPD_2IN9G_Init(void);
void EPD_2IN9G_Init_Fast(void);
void EPD_2IN9G_Clear(const UBYTE color);
void EPD_2IN9G_Display(const UBYTE *Image);
void EPD_2IN9G_Sleep(void);

#endif
