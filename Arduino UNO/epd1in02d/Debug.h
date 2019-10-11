/*****************************************************************************
* | File        : Debug.h
* | Author      :   Waveshare team
* | Function    : debug with prntf
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* | This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __DEBUG_H
#define __DEBUG_H

#include "stdio.h"

#define DEV_DEBUG 1
#if DEV_DEBUG
  #define Debug(__info,...) printf("Debug : " __info,##__VA_ARGS__)
#else
  #define DEBUG(__info,...)  
#endif

#endif


