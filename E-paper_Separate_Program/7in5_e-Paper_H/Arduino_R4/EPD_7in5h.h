/*****************************************************************************
* | File      	:   EPD_7in5h.h
* | Author      :   Waveshare team
* | Function    :   7.5inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2024-08-07
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_7IN5H_H_
#define __EPD_7IN5H_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_7IN5H_WIDTH       800
#define EPD_7IN5H_HEIGHT      480

// Color
#define  EPD_7IN5H_BLACK   0x0
#define  EPD_7IN5H_WHITE   0x1
#define  EPD_7IN5H_YELLOW  0x2
#define  EPD_7IN5H_RED     0x3

void EPD_7IN5H_Init(void);
void EPD_7IN5H_Clear(UBYTE color);
void EPD_7IN5H_Display(const UBYTE *Image);
void EPD_7IN5H_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
void EPD_7IN5H_Sleep(void);

#endif
