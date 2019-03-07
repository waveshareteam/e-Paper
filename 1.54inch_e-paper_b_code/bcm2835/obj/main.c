#include <signal.h>     //signal()
#include <stdlib.h>     //exit()
#include "ImageData.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "EPD_1in54b.h"

void Handler(int signo)
{
    //System Exit
    printf("\r\nHandler:exit...\r\n");
    // EPD_Sleep();
    DEV_ModuleExit();
    exit(0);
}

int main(void)
{
    printf("1.54inch e-Paper B demo\r\n");
    DEV_ModuleInit();

    // Exception handling:ctrl + c
    signal(SIGINT, Handler);

    printf("epd init and clear------------------------\r\n");
    if(EPD_Init()) {
        printf("e-Paper init failed\r\n");
    }
    EPD_Clear();
    DEV_Delay_ms(200);

    //Create a new image cache named IMAGE_BW and fill it with white
    UBYTE *BlackImage, *RedImage;
    UWORD Imagesize = ((EPD_WIDTH % 8 == 0)? (EPD_WIDTH / 8 ): (EPD_WIDTH / 8 + 1)) * EPD_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        exit(0);
    }
    if((RedImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for red memory...\r\n");
        exit(0);
    }
    printf("NewImage:BlackImage and RedImage\r\n");
    Paint_NewImage(BlackImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);
    Paint_NewImage(RedImage, EPD_WIDTH, EPD_HEIGHT, 270, WHITE);

    //Select Image
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);

#if 1   // show bmp
    printf("show windows------------------------\r\n");
    printf("read black bmp\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    GUI_ReadBmp("./pic/100x100.bmp", 50, 50);

    printf("read red bmp\r\n");
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    
    EPD_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);

    printf("show bmp------------------------\r\n");
    printf("read black bmp\r\n");
    Paint_SelectImage(BlackImage);
    GUI_ReadBmp("./pic/1in54b-b.bmp", 0, 0);
    printf("read red bmp\r\n");
    Paint_SelectImage(RedImage);
    GUI_ReadBmp("./pic/1in54b-r.bmp", 0, 0);

    EPD_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif
    
#if 1   //show image for array
    printf("show image for array------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_DrawBitMap(IMAGE_BLACK);

    Paint_SelectImage(RedImage);
    Paint_DrawBitMap(IMAGE_RED);

    EPD_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif
    
#if 1   //Drawing
    printf("Drawing------------------------\r\n");
    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 10, BLACK, DOT_PIXEL_1X1, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 25, BLACK, DOT_PIXEL_2X2, DOT_STYLE_DFT);
    Paint_DrawLine(20, 10, 70, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawLine(70, 10, 20, 60, BLACK, LINE_STYLE_SOLID, DOT_PIXEL_1X1);
    Paint_DrawRectangle(20, 10, 70, 60, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);
    Paint_DrawCircle(170, 85, 20, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawString_EN(5, 70, "hello world", &Font16, WHITE, BLACK);
    Paint_DrawString_CN(5, 160,"Î¢Ñ©µç×Ó", &Font24CN, WHITE, BLACK);
    
    Paint_SelectImage(RedImage);
    Paint_Clear(WHITE);
    Paint_DrawPoint(5, 40, BLACK, DOT_PIXEL_3X3, DOT_STYLE_DFT);
    Paint_DrawPoint(5, 55, BLACK, DOT_PIXEL_4X4, DOT_STYLE_DFT);
    Paint_DrawLine(170, 15, 170, 55, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);
    Paint_DrawLine(150, 35, 190, 35, BLACK, LINE_STYLE_DOTTED, DOT_PIXEL_1X1);    
    Paint_DrawRectangle(85, 10, 130, 60, BLACK, DRAW_FILL_FULL, DOT_PIXEL_1X1);
    Paint_DrawCircle(170, 35, 20, BLACK, DRAW_FILL_EMPTY, DOT_PIXEL_1X1);    
    Paint_DrawString_EN(5, 90, "waveshare", &Font20, BLACK, WHITE);
    Paint_DrawNum(5, 120, 123456789, &Font20, BLACK, WHITE);
    Paint_DrawString_CN(5, 135,"ÄãºÃabcÊ÷Ý®ÅÉ", &Font12CN, BLACK, WHITE);

    EPD_Display(BlackImage, RedImage);
    DEV_Delay_ms(2000);
#endif

    printf("Goto Sleep mode...\r\n");
    EPD_Sleep();
    free(BlackImage);
    free(RedImage);
    BlackImage = NULL;
    RedImage = NULL;

    return 0;
}
