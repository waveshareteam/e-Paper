/*****************************************************************************
* | File      	:   EPD_5in83g.h
* | Author      :   Waveshare team
* | Function    :   5inch83 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2025-09-23
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_5IN83G_H_
#define __EPD_5IN83G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_5IN83G_WIDTH        648
#define EPD_5IN83G_HEIGHT       480

// Color
#define  EPD_5IN83G_BLACK   0x0
#define  EPD_5IN83G_WHITE   0x1
#define  EPD_5IN83G_YELLOW  0x2
#define  EPD_5IN83G_RED     0x3

void EPD_5IN83G_Init(void);
void EPD_5IN83G_Init_Fast(void);
void EPD_5IN83G_Clear(const UBYTE color);
void EPD_5IN83G_Display(const UBYTE *Image);
void EPD_5IN83G_DisplayPart(const UBYTE *Image, UWORD xstart, UWORD ystart, UWORD image_width, UWORD image_heigh);
void EPD_5IN83G_Sleep(void);

#endif
