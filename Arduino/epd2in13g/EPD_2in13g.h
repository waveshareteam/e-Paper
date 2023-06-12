/*****************************************************************************
* | File      	:   EPD_2in13g.h
* | Author      :   Waveshare team
* | Function    :   2inch13 e-paper (G)
* | Info        :
*----------------
* |	This version:   V1.0
* | Date        :   2023-05-29
* | Info        :
* -----------------------------------------------------------------------------
******************************************************************************/
#ifndef __EPD_2IN13G_H_
#define __EPD_2IN13G_H_

#include "epdif.h"

// Display resolution
#define EPD_2IN13G_WIDTH       122
#define EPD_2IN13G_HEIGHT      250

#define UWORD   unsigned int
#define UBYTE   unsigned char
#define UDOUBLE unsigned long
// Color
#define  BLACK   0x0
#define  WHITE   0x1
#define  YELLOW  0x2
#define  RED     0x3
#define  Source_BITS  128
class Epd : EpdIf {
public:
    unsigned long WIDTH;
    unsigned long HEIGHT;

    Epd();
    ~Epd();
    int  Init();
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void Reset(void);
    void SetWindows(void);
    void ReadBusy(void);
    void TurnOnDisplay(void);
    void Clear(UBYTE color);
    void Display(const UBYTE *Image);
    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
};

#endif /* EPD4IN37_H */

/* END OF FILE */

