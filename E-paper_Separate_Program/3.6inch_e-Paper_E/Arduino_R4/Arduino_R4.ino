#include "EPD_3in6e.h"
#include "GUI_Paint.h"
#include "fonts.h"
#include "ImageData.h"

void setup() {
    Debug("EPD_3IN6E_test Demo\r\n");
    DEV_Module_Init();

    Debug("e-Paper Init and Clear...\r\n");
    EPD_3IN6E_Init();
    EPD_3IN6E_Clear(EPD_3IN6E_WHITE); // WHITE  
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    UDOUBLE Imagesize = ((EPD_3IN6E_WIDTH % 2 == 0)? (EPD_3IN6E_WIDTH / 2 ): (EPD_3IN6E_WIDTH / 2 + 1)) * EPD_3IN6E_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize/6)) == NULL) {
        Debug("Failed to apply for black memory...\r\n");
        while(1);
    }
    Debug("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_3IN6E_WIDTH/2, EPD_3IN6E_HEIGHT/3, 0, EPD_3IN6E_WHITE);
    Paint_SetScale(6);

#if 0 // show bmp
    EPD_3IN6E_Init();
    Debug("show bmp1-----------------\r\n");
    EPD_3IN6E_Display(BMP_1);
    DEV_Delay_ms(3000);
#endif

#if 1   // Drawing on the image
    EPD_3IN6E_Init();
    Paint_NewImage(BlackImage, EPD_3IN6E_WIDTH/2, EPD_3IN6E_HEIGHT/3, 90, EPD_3IN6E_WHITE);
    Paint_SetScale(6);
    // 1.Select Image
    Debug("SelectImage:BlackImage\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(EPD_3IN6E_WHITE);

    // 2.Drawing on the image
    Debug("Drawing:BlackImage\r\n");
    Paint_DrawPoint(10, 80, EPD_3IN6E_RED, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 90, EPD_3IN6E_BLUE, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawPoint(10, 100, EPD_3IN6E_GREEN, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawLine(20, 70, 70, 120, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawLine(70, 70, 20, 120, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
    Paint_DrawRectangle(20, 70, 70, 120, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawRectangle(80, 70, 130, 120, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawCircle(45, 95, 20, EPD_3IN6E_BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);
    Paint_DrawCircle(105, 95, 20, EPD_3IN6E_WHITE, DOT_PIXEL_1X1, DRAW_FILL_FULL);
    Paint_DrawLine(85, 95, 125, 95, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawLine(105, 75, 105, 115, EPD_3IN6E_YELLOW, DOT_PIXEL_1X1, LINE_STYLE_DOTTED);
    Paint_DrawString_CN(10, 130, "你好abc", &Font12CN, EPD_3IN6E_BLACK, EPD_3IN6E_WHITE);
    Paint_DrawString_CN(10, 150, "微雪电子", &Font24CN, EPD_3IN6E_WHITE, EPD_3IN6E_BLACK);
    Paint_DrawNum(10, 33, 123456789, &Font12, EPD_3IN6E_BLACK, EPD_3IN6E_WHITE);
    Paint_DrawNum(10, 50, 987654321, &Font16, EPD_3IN6E_WHITE, EPD_3IN6E_BLACK);
    Paint_DrawString_EN(145, 50, "hello", &Font16, EPD_3IN6E_BLACK, EPD_3IN6E_WHITE);
    Paint_DrawString_EN(145, 70, "hello", &Font16, EPD_3IN6E_GREEN, EPD_3IN6E_WHITE);
    Paint_DrawString_EN(145, 90, "hello", &Font16, EPD_3IN6E_BLUE, EPD_3IN6E_WHITE);
    Paint_DrawString_EN(145, 110, "hello", &Font16, EPD_3IN6E_RED, EPD_3IN6E_WHITE);
    Paint_DrawString_EN(145, 130, "hello", &Font16, EPD_3IN6E_YELLOW, EPD_3IN6E_WHITE);

    Debug("EPD_Display\r\n");
    EPD_3IN6E_DisplayPart(BlackImage,200,0,200,200);
    DEV_Delay_ms(3000);
#endif

    Debug("Clear...\r\n");
    EPD_3IN6E_Clear(EPD_3IN6E_WHITE);

    Debug("Goto Sleep...\r\n");
    EPD_3IN6E_Sleep();
    free(BlackImage);
    BlackImage = NULL;
    DEV_Delay_ms(2000); // important, at least 2s
    // close 5V
    Debug("close 5V, Module enters 0 power consumption ...\r\n");
    DEV_Module_Exit();

}

void loop() {
  

}
