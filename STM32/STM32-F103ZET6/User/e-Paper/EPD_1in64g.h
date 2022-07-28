/*****************************************************************************
* | File      	:   EPD_1in64g.h
* | Author      :   Waveshare team
* | Function    :   1.64inch e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2022-07-14
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_1IN64G_H_
#define __EPD_1IN64G_H_

#include "DEV_Config.h"

// Display resolution
#define EPD_1IN64G_WIDTH       168
#define EPD_1IN64G_HEIGHT      168

//colour
#define  EPD_1IN64G_BLACK   0x00 
#define  EPD_1IN64G_WHITE   0x55
#define  EPD_1IN64G_YELLOW  0xAA
#define  EPD_1IN64G_RED     0xFF

void EPD_1IN64G_Init(void);
void EPD_1IN64G_Clear(UBYTE color);
void EPD_1IN64G_Display(UBYTE *Image);
void EPD_1IN64G_Sleep(void);

#endif
