#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>

// Pin definition
#if 0
#define BUSY_Pin    EPD_BUSY_Pin
#define RES_Pin     EPD_RES_Pin
#define DC_Pin      EPD_SPI_CS_Pin
#define CS_Pin      EPD_SPI_MOSI_Pin
#define SCK_Pin     EPD_SPI_MISO_Pin
#define SDI_Pin     EPD_SPI_CLK_Pin

#define EPD_W21_MOSI_0 digitalWrite(SDI_Pin, LOW)
#define EPD_W21_MOSI_1 digitalWrite(SDI_Pin, HIGH)
#define EPD_W21_CLK_0 digitalWrite(SCK_Pin, LOW)
#define EPD_W21_CLK_1 digitalWrite(SCK_Pin, HIGH)
#define EPD_W21_CS_0 digitalWrite(CS_Pin, LOW)
#define EPD_W21_CS_1 digitalWrite(CS_Pin, HIGH)
#define EPD_W21_DC_0 digitalWrite(DC_Pin, LOW)
#define EPD_W21_DC_1 digitalWrite(DC_Pin, HIGH)
#define EPD_W21_RST_0 digitalWrite(RES_Pin, LOW)
#define EPD_W21_RST_1 digitalWrite(RES_Pin, HIGH)
#define isEPD_W21_BUSY digitalRead(BUSY_Pin)
#endif

#define MONOMSB_MODE 1
#define MONOLSB_MODE 2
#define RED_MODE 3
// Display resolution
#define EPD_WIDTH 128
#define EPD_HEIGHT 296
#define MAX_LINE_BYTES 16  // =128/8
#define ALLSCREEN_GRAGHBYTES 4736

// EPD2IN9 commands
#define DRIVER_OUTPUT_CONTROL 0x01
#define BOOSTER_SOFT_START_CONTROL 0x0C
#define DEEP_SLEEP_MODE 0x10
#define DATA_ENTRY_MODE_SETTING 0x11
#define MASTER_ACTIVATION 0x20
#define DISPLAY_UPDATE_CONTROL_2 0x22
#define WRITE_RAM 0x24
#define WRITE_VCOM_REGISTER 0x2C
#define WRITE_LUT_REGISTER 0x32
#define SET_DUMMY_LINE_PERIOD 0x3A
#define SET_GATE_TIME 0x3B
#define BORDER_WAVEFORM_CONTROL 0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION 0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION 0x45
#define SET_RAM_X_ADDRESS_COUNTER 0x4E
#define SET_RAM_Y_ADDRESS_COUNTER 0x4F
#define TERMINATE_FRAME_READ_WRITE 0xFF

class EpdIf {
public:
    EpdIf(int BUSY_Pin, int RES_Pin, int DC_Pin, int CS_Pin, int SCK_Pin, int SDI_Pin);
    ~EpdIf(void);
    void EPD_Dis_Part(
        unsigned int x_start,
        unsigned int y_start,
        const unsigned char* datas,
        unsigned int PART_COLUMN,
        unsigned int PART_LINE);
    void EPD_SetFrame(
        const unsigned char* image_buffer,
        int x,
        int y,
        int image_width,
        int image_height);
    void EPD_SetRAMValue_BaseMap(const unsigned char* datas);
    void EPD_SetFrameMemory(const unsigned char* image_buffer);
    void EPD_UpdateDisplay(void);
    void EPD_Part_Update(void);
    void EPD_ClearScreen_White(void);
    void EPD_ClearScreen_Black(void);

    void EPD_Busy(void);
    void EPD_Reset(void);
    void EPD_Sleep(void);

private:
    // Driver
    void driver_delay_us(unsigned int xus);
    void driver_delay_xms(unsigned long xms);
    void DELAY_S(unsigned int delaytime);
    // SPI
    void SpiDelay(unsigned char xrate);
    void SpiTransfer(unsigned char value);
    void SendData(unsigned char command);
    void SendCommand(unsigned char command);
    // EPD initialization
    void EPD_Init(void);  // Hardware initialization
    void EPD_InitText(void);
    void EPD_Part_Init(void);
    void SetLUT(const unsigned char* lut);

    int _BUSY_Pin;
    int _RES_Pin;
    int _DC_Pin;
    int _CS_Pin;
    int _SCK_Pin;
    int _SDI_Pin;
    void EPD_W21_MOSI_0 (void) { digitalWrite(_SDI_Pin, LOW); }
    void EPD_W21_MOSI_1 (void) { digitalWrite(_SDI_Pin, HIGH); }
    void EPD_W21_CLK_0 (void) { digitalWrite(_SCK_Pin, LOW); }
    void EPD_W21_CLK_1 (void) { digitalWrite(_SCK_Pin, HIGH); }
    void EPD_W21_CS_0 (void) { digitalWrite(_CS_Pin, LOW); }
    void EPD_W21_CS_1 (void) { digitalWrite(_CS_Pin, HIGH); }
    void EPD_W21_DC_0 (void) { digitalWrite(_DC_Pin, LOW); }
    void EPD_W21_DC_1 (void) { digitalWrite(_DC_Pin, HIGH); }
    void EPD_W21_RST_0 (void) { digitalWrite(_RES_Pin, LOW); }
    void EPD_W21_RST_1 (void) { digitalWrite(_RES_Pin, HIGH); }
    int isEPD_W21_BUSY (void) { return digitalRead(_BUSY_Pin); }
};
#endif
