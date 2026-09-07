#ifndef EPDIF_4IN2_H
#define EPDIF_4IN2_H

#include <Arduino.h>

// Display resolution
#define EPD_WIDTH       400
#define EPD_HEIGHT      300

// EPD4IN2 commands
#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define LUT_FOR_VCOM                                0x20 
#define LUT_WHITE_TO_WHITE                          0x21
#define LUT_BLACK_TO_WHITE                          0x22
#define LUT_WHITE_TO_BLACK                          0x23
#define LUT_BLACK_TO_BLACK                          0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_COMMAND                  0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GSST_SETTING                                0x65
#define GET_STATUS                                  0x71
#define AUTO_MEASUREMENT_VCOM                       0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAMMING                          0xA1
#define READ_OTP                                    0xA2
#define POWER_SAVING                                0xE3

extern const unsigned char lut_vcom0[];
extern const unsigned char lut_ww[];
extern const unsigned char lut_bw[];
extern const unsigned char lut_bb[];
extern const unsigned char lut_wb[];

extern const unsigned char EPD_4IN2_4Gray_lut_vcom[];
extern const unsigned char EPD_4IN2_4Gray_lut_ww[];
extern const unsigned char EPD_4IN2_4Gray_lut_bw[];
extern const unsigned char EPD_4IN2_4Gray_lut_wb[];
extern const unsigned char EPD_4IN2_4Gray_lut_bb[];

class EpdIf4in2 {
public:
    EpdIf4in2(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin);
    ~EpdIf4in2(void);

    static int  IfInit(void);
    static void DigitalWrite(int pin, int value); 
    static int  DigitalRead(int pin);
    static void DelayMs(unsigned int delaytime);
    static void SpiTransfer(unsigned char data);

    int  Init(void);
	int  Init_4Gray(void);
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetFrame_Partial(const unsigned char* frame_buffer, int x, int y, int w, int l);
    void Set_4GrayDisplay(const char *Image, int x, int y, int w, int l);
	void SetLut(void);
	void set4Gray_lut(void);
    void DisplayFrame(const unsigned char* frame_buffer);
    void DisplayFrame(void);
    void ClearFrame(void);
    void Sleep(void);
    
private:
    static int _BUSY_Pin;
    static int _RES_Pin;
    static int _DC_Pin;
    static int _CS_Pin;

    int width;
    int height;
};

#endif
