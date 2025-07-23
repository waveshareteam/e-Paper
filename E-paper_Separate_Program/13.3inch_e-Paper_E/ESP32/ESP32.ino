#include "EPD_13in3e.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ImageData.h"

void setup() {
    Debug("EPD_13IN3E_test Demo\r\n");
    DEV_Module_Init();

    Debug("e-Paper Init and Clear...\r\n");
    EPD_13IN3E_Init();
    EPD_13IN3E_Clear(EPD_13IN3E_WHITE);
    DEV_Delay_ms(500);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *Image;
    UWORD Imagesize = ((EPD_13IN3E_WIDTH % 2 == 0)? (EPD_13IN3E_WIDTH / 2 ): (EPD_13IN3E_WIDTH / 2 + 1)) * EPD_13IN3E_HEIGHT;
    if((Image = (UBYTE *)malloc(20000)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        DEV_Module_Exit();
        while(1);
    }
    Debug("NewImage:Image\r\n");
    Paint_NewImage(Image, 200, 200, 0, WHITE);
    
    //Select Image
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);

#if 1 // show bmp
    printf("show bmp1-----------------\r\n");
    EPD_13IN3E_DisplayPart(BMP_1, 400, 500, 400, 600);
    DEV_Delay_ms(3000);
#else  // Drawing on the image
    Paint_NewImage(Image, 200, 200, 90, EPD_13IN3E_WHITE);
    Paint_SetScale(6);
    // 1.Select Image
    Debug("SelectImage:Image\r\n");
    Paint_SelectImage(Image);
    Paint_Clear(EPD_13IN3E_WHITE);

    // 2.Drawing on the image
    Debug("Drawing:Image\r\n");
    Paint_DrawPoint(10, 80, EPD_13IN3E_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_13IN3E_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_13IN3E_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_13IN3E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_13IN3E_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_13IN3E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(10, 130, "你好abc", &Font12CN, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawString_CN(10, 150, "微雪电子", &Font24CN, EPD_13IN3E_WHITE, EPD_13IN3E_BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_13IN3E_WHITE, EPD_13IN3E_BLACK);
    Paint_DrawString_EN(145, 0, "Waveshare", &Font16, EPD_13IN3E_BLACK, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 35, "Waveshare", &Font16, EPD_13IN3E_GREEN, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 70, "Waveshare", &Font16, EPD_13IN3E_BLUE, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 105, "Waveshare", &Font16, EPD_13IN3E_RED, EPD_13IN3E_WHITE);
    Paint_DrawString_EN(145, 140, "Waveshare", &Font16, EPD_13IN3E_YELLOW, EPD_13IN3E_WHITE);

    Debug("EPD_Display\r\n");
    EPD_13IN3E_DisplayPart(Image, 1000, 0, 200, 200);
    DEV_Delay_ms(3000);
#endif

    Debug("Clear...\r\n");
    EPD_13IN3E_Clear(EPD_13IN3E_WHITE);
    DEV_Delay_ms(2000);

    Debug("Goto Sleep...\r\n");
    EPD_13IN3E_Sleep();
    free(Image);
    DEV_Delay_ms(2000);

    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();
}


void loop() {

}