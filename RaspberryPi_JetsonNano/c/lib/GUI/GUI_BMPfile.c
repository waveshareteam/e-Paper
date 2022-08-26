/*****************************************************************************
* | File      	:   GUI_BMPfile.h
* | Author      :   Waveshare team
* | Function    :   Hardware underlying interface
* | Info        :
*                Used to shield the underlying layers of each master
*                and enhance portability
*----------------
* |	This version:   V2.3
* | Date        :   2022-07-27
* | Info        :   
* -----------------------------------------------------------------------------
* V2.3(2022-07-27):
* 1.Add GUI_ReadBmp_RGB_4Color()
* V2.2(2020-07-08):
* 1.Add GUI_ReadBmp_RGB_7Color()
* V2.1(2019-10-10):
* 1.Add GUI_ReadBmp_4Gray()
* V2.0(2018-11-12):
* 1.Change file name: GUI_BMP.h -> GUI_BMPfile.h
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
#include <stdio.h>

UBYTE GUI_ReadBmp(const char *path, UWORD Xstart, UWORD Ystart)
{
    FILE *fp;                     //Define a file pointer
    BMPFILEHEADER bmpFileHeader;  //Define a bmp file header structure
    BMPINFOHEADER bmpInfoHeader;  //Define a bmp info header structure


    // Binary file open
    if((fp = fopen(path, "rb")) == NULL) {
        Debug("Can't open the file!\n");
        exit(0);
    }

    // Set the file pointer from the beginning
    fseek(fp, 0, SEEK_SET);
    fread(&bmpFileHeader, sizeof(BMPFILEHEADER), 1, fp);    //sizeof(BMPFILEHEADER) must be 14
    fread(&bmpInfoHeader, sizeof(BMPINFOHEADER), 1, fp);    //sizeof(BMPINFOHEADER) must be 50
    printf("pixel = %d * %d\r\n", bmpInfoHeader.biWidth, bmpInfoHeader.biHeight);

    UWORD Image_Width_Byte = (bmpInfoHeader.biWidth + 7) / 8;
    UWORD Bmp_Width_Byte = (Image_Width_Byte + 3) & ~3;

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    if(readbyte != 1) {
        Debug("the bmp Image is not a monochrome bitmap!\n");
        exit(0);
    }

    // Determine black and white based on the palette
    UWORD Bcolor, Wcolor;
    UWORD bmprgbquadsize = pow(2, bmpInfoHeader.biBitCount);// 2^1 = 2
    BMPRGBQUAD bmprgbquad[bmprgbquadsize];        //palette

	fread(bmprgbquad, sizeof(BMPRGBQUAD), bmprgbquadsize, fp);
    if(bmprgbquad[0].rgbBlue == 0xff && bmprgbquad[0].rgbGreen == 0xff && bmprgbquad[0].rgbRed == 0xff) {
        Bcolor = BLACK;
        Wcolor = WHITE;
    } else {
        Bcolor = WHITE;
        Wcolor = BLACK;
    }

    // Read image data and paint the pixels
    UWORD x, xb, y, yp;
    UDOUBLE Rdata[1];
    UBYTE color;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(xb = 0; xb < Bmp_Width_Byte; xb+=sizeof Rdata) {//Show a line in the line
			readbyte = sizeof Rdata;
			if (Bmp_Width_Byte-xb < readbyte) readbyte=Bmp_Width_Byte-xb;
            if(fread(Rdata, 1, readbyte, fp) != readbyte) {
                perror("get bmpdata:\r\n");
                break;
            }
			UDOUBLE temp=be32toh(Rdata[0]);
			yp = (bmpInfoHeader.biHeight - y - 1);
			if (Ystart + yp >= Paint.Height) continue;
			for (x=xb*8; x<(xb+sizeof Rdata)*8; x++) {
				if(Xstart + x >= Paint.Width) break;
				color = (temp & (1<<(sizeof Rdata*8-1))) ?Bcolor:Wcolor;
				temp <<= 1;
				Paint_SetPixel(Xstart + x, Ystart + yp, color);
			}
        }
    }
    fclose(fp);

    return 0;
}
/*************************************************************************

*************************************************************************/
UBYTE GUI_ReadBmp_4Gray(const char *path, UWORD Xstart, UWORD Ystart)
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

    UWORD Image_Width_Byte = (bmpInfoHeader.biWidth % 4 == 0)? (bmpInfoHeader.biWidth / 4): (bmpInfoHeader.biWidth / 4 + 1);
    UWORD Bmp_Width_Byte = (bmpInfoHeader.biWidth % 2 == 0)? (bmpInfoHeader.biWidth / 2): (bmpInfoHeader.biWidth / 2 + 1);
    UBYTE Image[Image_Width_Byte * bmpInfoHeader.biHeight * 2];
    memset(Image, 0xFF, Image_Width_Byte * bmpInfoHeader.biHeight * 2);

    // Determine if it is a monochrome bitmap
    int readbyte = bmpInfoHeader.biBitCount;
    printf("biBitCount = %d\r\n",readbyte);
    if(readbyte != 4){
        Debug("Bmp image is not a 4-color bitmap!\n");
        exit(0);
    }
    // Read image data into the cache
    UWORD x, y;
    UBYTE Rdata;
    fseek(fp, bmpFileHeader.bOffset, SEEK_SET);
    
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {//Total display column
        for(x = 0; x < Bmp_Width_Byte; x++) {//Show a line in the line
            if(fread((char *)&Rdata, 1, 1, fp) != 1) {
                perror("get bmpdata:\r\n");
                break;
            }
            if(x < Image_Width_Byte*2) { //bmp
                Image[x + (bmpInfoHeader.biHeight - y - 1) * Image_Width_Byte*2] =  Rdata;
            }
        }
    }
    fclose(fp);
    
    // Refresh the image to the display buffer based on the displayed orientation
    UBYTE color, temp;
    printf("bmpInfoHeader.biWidth = %d\r\n",bmpInfoHeader.biWidth);
    printf("bmpInfoHeader.biHeight = %d\r\n",bmpInfoHeader.biHeight);
    for(y = 0; y < bmpInfoHeader.biHeight; y++) {
        for(x = 0; x < bmpInfoHeader.biWidth; x++) {
            if(x > Paint.Width || y > Paint.Height) {
                break;
            }
            temp = Image[x/2 + y * bmpInfoHeader.biWidth/2] >> ((x%2)? 0:4);//0xf 0x8 0x7 0x0 
            color = temp>>2;                           //11  10  01  00  
            Paint_SetPixel(Xstart + x, Ystart + y, color);
        }
    }
    return 0;
}


UBYTE GUI_ReadBmp_RGB_7Color(const char *path, UWORD Xstart, UWORD Ystart)
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
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  2;//Green
			}else if(Rdata[0] == 255 && Rdata[1] == 0 && Rdata[2] == 0){
				Image[x+(y* bmpInfoHeader.biWidth )] =  3;//Blue
			}else if(Rdata[0] == 0 && Rdata[1] == 0 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  4;//Red
			}else if(Rdata[0] == 0 && Rdata[1] == 255 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  5;//Yellow
			}else if(Rdata[0] == 0 && Rdata[1] == 128 && Rdata[2] == 255){
				Image[x+(y* bmpInfoHeader.biWidth )] =  6;//Orange
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

UBYTE GUI_ReadBmp_RGB_4Color(const char *path, UWORD Xstart, UWORD Ystart)
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
			if(Rdata[0] < 128 && Rdata[1] < 128 && Rdata[2] < 128){
				Image[x+(y* bmpInfoHeader.biWidth )] =  0;//Black
			}else if(Rdata[0] > 127 && Rdata[1] > 127 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  1;//White
			}else if(Rdata[0] < 128 && Rdata[1] > 127 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  2;//Yellow
			}else if(Rdata[0] < 128 && Rdata[1] < 128 && Rdata[2] > 127){
				Image[x+(y* bmpInfoHeader.biWidth )] =  3;//Red
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


