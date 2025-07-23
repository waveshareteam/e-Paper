/*****************************************************************************
* | File      	:   GUI_BMPfile.c
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V2.0
* | Date        :   2018-11-12
* | Info        :   
* 1.Change file name: GUI_BMP.c -> GUI_BMPfile.c
* 2.fix: GUI_ReadBmp()
*   Now Xstart and Xstart can control the position of the picture normally, 
*   and support the display of images of any size. If it is larger than 
*   the actual display range, it will not be displayed.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documnetation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to  whom the Software is
# furished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
******************************************************************************/

#include "GUI_BMPfile.h"
#include "GUI_Paint.h"
#include "Debug.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>	//exit()
#include <string.h> //memset()
#include <math.h> //memset()

UBYTE GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure
    
    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Cann't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);
	
    UDOUBLE Image_Byte = bmpInfoHeader.biWidth * bmpInfoHeader.biHeight * 3;
    UBYTE Image[Image_Byte];
    memset(Image, 0xFF, Image_Byte);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    if(readbyte != 24){
        Debug("Bmp image is not 24 bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata[3];
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < bmpInfoHeader.biWidth ; x++) {//Show a line in the line
            if(fread((char *)Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+1, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
			if(fread((char *)Rdata+2, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }

			if(Rdata[0] == 0 && Rdata[1] == 0 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  0;//Black
			}else if(Rdata[0] == 255 && Rdata[1] == 255 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  1;//White
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  2;//Yellow
			}else if(Rdata[0] == 0 && Rdata[1] == 0 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  3;//Red
			// }else if(Rdata[0] == 0 && Rdata[1] == 128 && Rdata[2] == 255){
			// 	Image[x+(y* bmpInfoHeader.biWidth )] =  4;//Orange
			}else if(Rdata[0] == 255 && Rdata[1] == 0 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  5;//Blue
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  6;//Green
			}
            
        }
    }
    fclose(fp);
   
    // Refresh the image to the display buffer based on the displayed orientation
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > Paint.Width || y > Paint.Height) {
                break;
            }
            Paint_SetPixel(Xstart + x, Ystart + y, Image[bmpInfoHeader.biHeight *  bmpInfoHeader.biWidth - 1 -(bmpInfoHeader.biWidth-x-1+(y* bmpInfoHeader.biWidth))]);
		}
    }
    return 0;
}


