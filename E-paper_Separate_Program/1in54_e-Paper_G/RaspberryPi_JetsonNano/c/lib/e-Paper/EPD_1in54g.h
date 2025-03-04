/*****************************************************************************
* | File      	:   EPD_1in54g.h
* | Author      :   Waveshare team
* | Function    :   1inch54 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_1IN54G_H_
#define __EPD_1IN54G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_1IN54G_WIDTH       200
#define EPD_1IN54G_HEIGHT      200

// Color
#define  EPD_1IN54G_BLACK   0x0
#define  EPD_1IN54G_WHITE   0x1
#define  EPD_1IN54G_YELLOW  0x2
#define  EPD_1IN54G_RED     0x3

void EPD_1IN54G_Init(void);
void EPD_1IN54G_Init_Fast(void);
void EPD_1IN54G_Clear(UBYTE color);
void EPD_1IN54G_Display(const UBYTE *Image);
void EPD_1IN54G_Sleep(void);

#endif
