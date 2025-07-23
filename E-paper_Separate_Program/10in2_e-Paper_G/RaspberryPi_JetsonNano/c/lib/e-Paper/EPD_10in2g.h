/*****************************************************************************
* | File      	:   EPD_10in2g.h
* | Author      :   Waveshare team
* | Function    :   10.2inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_10IN2G_H_
#define __EPD_10IN2G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_10IN2G_WIDTH       960
#define EPD_10IN2G_HEIGHT      640

// Color
#define  EPD_10IN2G_BLACK   0x0
#define  EPD_10IN2G_WHITE   0x1
#define  EPD_10IN2G_YELLOW  0x2
#define  EPD_10IN2G_RED     0x3

void EPD_10IN2G_Init(void);
void EPD_10IN2G_Clear(UBYTE color);
void EPD_10IN2G_Display(const UBYTE *Image);
void EPD_10IN2G_Sleep(void);

#endif
