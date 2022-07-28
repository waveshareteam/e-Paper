/*****************************************************************************
* | File      	:   EPD_3in0g.h
* | Author      :   Waveshare team
* | Function    :   3inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-07-15
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_3IN0G_H_
#define __EPD_3IN0G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_3IN0G_WIDTH       168
#define EPD_3IN0G_HEIGHT      400

//colour
#define  EPD_3IN0G_BLACK   0x00 
#define  EPD_3IN0G_WHITE   0x55
#define  EPD_3IN0G_YELLOW  0xAA
#define  EPD_3IN0G_RED     0xFF

void EPD_3IN0G_Init(void);
void EPD_3IN0G_Clear(UBYTE color);
void EPD_3IN0G_Display(UBYTE *Image);
void EPD_3IN0G_Sleep(void);

#endif
