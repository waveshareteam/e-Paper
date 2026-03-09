/*****************************************************************************
* | File      	:   EPD_3in97g.h
* | Author      :   Waveshare team
* | Function    :   2inch15 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_3IN97G_H_
#define __EPD_3IN97G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN97G_WIDTH        800
// Initialization must use this resolution
#define EPD_3IN97G_HEIGHT_INIT  680
#define EPD_3IN97G_HEIGHT       480

// Color
#define  EPD_3IN97G_BLACK   0x0
#define  EPD_3IN97G_WHITE   0x1
#define  EPD_3IN97G_YELLOW  0x2
#define  EPD_3IN97G_RED     0x3

void EPD_3IN97G_Init(void);
void EPD_3IN97G_Init_Fast(void);
void EPD_3IN97G_Clear(const UBYTE color);
void EPD_3IN97G_Display(const UBYTE *Image);
void EPD_3IN97G_Sleep(void);

#endif
