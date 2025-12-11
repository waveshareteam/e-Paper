/*****************************************************************************
* | File      	:   EPD_4in2g.h
* | Author      :   Waveshare team
* | Function    :   4inch2 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-01-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_4IN2G_H_
#define __EPD_4IN2G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_4IN2G_WIDTH       400
#define EPD_4IN2G_HEIGHT      300

// Color
#define  EPD_4IN2G_BLACK   0x0
#define  EPD_4IN2G_WHITE   0x1
#define  EPD_4IN2G_YELLOW  0x2
#define  EPD_4IN2G_RED     0x3

void EPD_4IN2G_Init(void);
void EPD_4IN2G_Clear(UBYTE color);
void EPD_4IN2G_Display(const UBYTE *Image);
void EPD_4IN2G_Display_1(const UBYTE *Image);
void EPD_4IN2G_Sleep(void);

#endif
