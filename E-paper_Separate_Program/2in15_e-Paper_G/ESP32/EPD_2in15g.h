/*****************************************************************************
* | File      	:   EPD_2in15g.h
* | Author      :   Waveshare team
* | Function    :   2inch15 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_2IN15G_H_
#define __EPD_2IN15G_H_

#include "Debug.h"
#include "DEV_Config.h"

// Display resolution
#define EPD_2IN15G_WIDTH       160
#define EPD_2IN15G_HEIGHT      296

// Color
#define  EPD_2IN15G_BLACK   0x0
#define  EPD_2IN15G_WHITE   0x1
#define  EPD_2IN15G_YELLOW  0x2
#define  EPD_2IN15G_RED     0x3

void EPD_2IN15G_Init(void);
void EPD_2IN15G_Clear(UBYTE color);
void EPD_2IN15G_Display(const UBYTE *Image);
void EPD_2IN15G_Sleep(void);

#endif
