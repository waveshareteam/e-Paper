/*****************************************************************************
* | File      	:   EPD_3in7g.h
* | Author      :   Waveshare team
* | Function    :   3inch7 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-02-26
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_3IN7G_H_
#define __EPD_3IN7G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN7G_WIDTH       240
#define EPD_3IN7G_HEIGHT      416

// Color
#define  EPD_3IN7G_BLACK   0x0
#define  EPD_3IN7G_WHITE   0x1
#define  EPD_3IN7G_YELLOW  0x2
#define  EPD_3IN7G_RED     0x3

void EPD_3IN7G_Init(void);
void EPD_3IN7G_Init_Fast(void);
void EPD_3IN7G_Clear(UBYTE color);
void EPD_3IN7G_Display(const UBYTE *Image);
void EPD_3IN7G_Display_1(const UBYTE *Image);
void EPD_3IN7G_Sleep(void);

#endif
