#ifndef EPDIF_2IN9_V2_H
#define EPDIF_2IN9_V2_H

#include <Arduino.h>

// Display resolution
#define EPD_WIDTH       128
#define EPD_HEIGHT      296

class EpdIf2in9V2 {
public:
    EpdIf2in9V2(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin);
    ~EpdIf2in9V2(void);
    int width;
    int height;

    static int  IfInit(void);
    static void DigitalWrite(int pin, int value); 
    static int  DigitalRead(int pin);
    static void DelayMs(unsigned int delaytime);
    static void SpiTransfer(unsigned char data);
    
    int  Init(void);
    int  Epdif_Init(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetFrameMemory(
        const unsigned char* image_buffer,
        int x,
        int y,
        int image_width,
        int image_height
    );
    void SetFrameMemory_Partial(
        const unsigned char* image_buffer,
        int x,
        int y,
        int image_width,
        int image_height
    );
    void SetFrameMemory(const unsigned char* image_buffer);
    void SetFrameMemory_Base(const unsigned char* image_buffer);
    void ClearFrameMemory(unsigned char color);
    //void ClearFrameMemory_Base(unsigned char color);
    void DisplayFrame(void);
	void DisplayFrame_Partial(void);
    void Sleep(void);

private:
    void SetLut(unsigned char *lut);
    void SetLut_by_host(unsigned char *lut);
    void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
    void SetMemoryPointer(int x, int y);

    static int _BUSY_Pin;
    static int _RES_Pin;
    static int _DC_Pin;
    static int _CS_Pin;
};

#endif
