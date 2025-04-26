/*
    This sketch is for creating an Ebook using AMB23 and SD Card.

    1. User have to save the ebook in .txt format to an SD Card. 
    2. The text file's content will be read and displayed on the E-paper display.
    3. The content will be divided into pages based the given page size. 
    4. Changing of pages can be done by pressing on the button.

    Language supported: English
    Fontsize choices: 16,20,24
    Orientation choices: 0 degree - 0 (landscape), 90 degree - 1(portrait), 180 degree- 2(landscape), 270 degree - 3(portrait)
    E-book supported format: .txt

    To Note: 
    ! SPI pin definition can be found in "epdif_7in5.h".
    ! For the content in text file, try not to have too many empty lines between each sentence.

    Example guide:
    https://www.amebaiot.com/en/amebapro2-arduino-epaper-ebook/
 */

#include <SPI.h>
#include "epdif_7in5.h"
#include "epdpaint.h"
#include "AmebaFatFS.h"
#include "fonts.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <itoa.h>

// SPI Pin definition
// Any GPIO pins can be used for BUSY_PIN, RST_PIN, and DC_PIN. You may refer to the pinmap of the board that you are using.
#define BUSY_PIN        4
#define RST_PIN         5
#define DC_PIN          6
#define CS_PIN          SPI_SS

EpdIf7in5 epd7in5(BUSY_PIN, RST_PIN, DC_PIN, CS_PIN);

#define COLOR_BLACK        0
#define COLOR_WHITE        1

#define fontsize 20
#define rotate 3

// PAGE SIZE definition 
// Landscape
#if rotate == 1 || rotate == 3
    #if fontsize == 16
        #define PAGESIZE 1600
    #elif fontsize == 20
        #define PAGESIZE 900
    #elif fontsize == 24
        #define PAGESIZE 700
    #endif
// Portrait
#elif rotate == 0 || rotate == 2
    #if fontsize == 16
        #define PAGESIZE 1300
    #elif fontsize == 20
        #define PAGESIZE 800
    #elif fontsize == 24
        #define PAGESIZE 600
    #endif
#endif

// E-paper display dimension (Dimension in pixel)
#define EPDwidth 800
#define EPDheight 480

// Memory buffer storing content to be displayed, display will not update unless
// DisplayFrame function is called. (800x480/8bits = 48000), 1 pixel = 8 bits
unsigned char image[48000]; 
Paint paint(image, 800, 480); // width should be the multiple of 8

char string[PAGESIZE];
char buffer[PAGESIZE];
char buffer0[200][200];

uint32_t pos = 0;
uint32_t sum_page = 1;
uint32_t last_page_check = 0;
uint32_t buf_size = PAGESIZE;
int spacing = 0;

// Open and read the text file from SD Card
// change the text file name to the one you would like read and display on E-paper display
char filename[] = "User_Ebook.txt";
AmebaFatFS fs;
File file;
char absolute_filename[128];

//Button setting
int buttonPin = 2;
int buttonState = 0;

void setup() {
    Epd_display();
    fs.begin();
    sprintf(absolute_filename, "%s%s", fs.getRootPath(), filename);
    file = fs.open(absolute_filename);
    memset(string, 0, buf_size);
    pinMode(buttonPin, INPUT);
}

void loop() {
    buttonState = digitalRead(buttonPin);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {
        read_page();
    }

    // Restart when it reaches the last page after button is pressed
    if (last_page_check == 1) {
        pos = 0;
        sum_page = 1;
        last_page_check = 0;
        buf_size = PAGESIZE;
        file.close();
        file = fs.open(absolute_filename);
    }
    delay(200);
}

void read_page() {
    //Epd epd;
    if (file.available()) {
        file.seek(pos);
        if (((file.size()) - pos) < PAGESIZE) {
            memset(&string[0], 0, buf_size);
            buf_size = file.size() - pos + 1;
            last_page_check = 1;
        }
        file.read(string, buf_size - 1);
        pos = file.position();
        paint.LongTextWrap(buffer, string, fontsize, rotate);

        int word_counter = 0;
        int line_counter = 0;
        for (unsigned int i = 0; i <= (strlen(buffer)); i++) {
            if (buffer[i] == '\n') {
                buffer0[line_counter][word_counter] = '\0';
                line_counter++;
                word_counter = 0;
            } else {
                buffer0[line_counter][word_counter] = buffer[i];
                word_counter++;
            }
        }

        for (int i = 0; i <= line_counter; i++) {
            if (buf_size - 1 == PAGESIZE - 1) {
                if (i == line_counter) {
                    int last_line_len = strlen(buffer0[line_counter]);
                    pos = pos - last_line_len;
                }
            }
        }

        // ============== Display onto EPD =============== //
        int EPD_limit;
        int k = 0;
        paint.Clear(COLOR_WHITE);
        paint.SetRotate(rotate);

        //-- if it is not the last page --//
        if (buf_size - 1 == PAGESIZE - 1) {
            if (fontsize == 16) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 9;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize)- 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i < line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font16, COLOR_BLACK);
                    k += spacing;
                }
            } else if (fontsize == 20) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 6;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize)- 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i < line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font20, COLOR_BLACK);
                    k += spacing;
                }
            } else if (fontsize == 24) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 6;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize)- 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i < line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font24, COLOR_BLACK);
                    k += spacing;
                }
            }
        // -- if its the last page -- //
        } else if (buf_size - 1 < PAGESIZE - 1) {
            if (fontsize == 16) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 9;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize) - 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i <= line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font16, COLOR_BLACK);
                    k += spacing;
                }
            } else if (fontsize == 20) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 6;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize) - 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i <= line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font20, COLOR_BLACK);
                    k += spacing;
                }
            } else if (fontsize == 24) {
                if (rotate == 1 || rotate == 3) {
                    EPD_limit = (EPDwidth/ fontsize) - 6;
                } else if (rotate == 0 || rotate == 2) {
                    EPD_limit = (EPDheight/ fontsize) - 5;
                }

                if (line_counter < EPD_limit) {
                    spacing = fontsize + 5;
                } else if (line_counter >= EPD_limit) {
                    spacing = fontsize;
                }

                for (int i = 0; i <= line_counter; i++) {
                    paint.DrawStringAt(0, k, buffer0[i], &Font24, COLOR_BLACK);  
                    k += spacing;
                }
            }
        }

        // Print page number on each page 
        if (rotate == 0 || rotate == 2) {
            char page_number[16];
            itoa(sum_page, page_number, 10);
            paint.DrawStringAt(780,450, page_number, &Font16, COLOR_BLACK);
        } else if(rotate == 1 || rotate == 3) {
            char page_number[16];
            itoa(sum_page, page_number, 10);
            paint.DrawStringAt(450, 780, page_number, &Font16, COLOR_BLACK);
        }
        sum_page++;
    }
    epd7in5.DisplayFrame(paint.GetImage());
}


void Epd_display() {
    Serial.begin(115200);
    Serial.print("e-Paper init \r\n ");

    if (epd7in5.Init() != 0) {
        Serial.print("e-Paper init failed\r\n ");
    } else {
        Serial.print("e-Paper Display\r\n ");
        // Cover page 
        if (rotate == 0 || rotate == 2) {
            paint.Clear(COLOR_BLACK);
            paint.SetRotate(rotate);
            paint.DrawStringAt(130, 230, "Press the button to get started!", &Font24, COLOR_WHITE);
            epd7in5.DisplayFrame(paint.GetImage());
        } else if(rotate == 1 || rotate == 3) {
            paint.Clear(COLOR_BLACK);
            paint.SetRotate(rotate);
            paint.DrawStringAt(100, 270,"Press the button", &Font24, COLOR_WHITE);
            paint.DrawStringAt(110, 330,"to get started!", &Font24, COLOR_WHITE);
            epd7in5.DisplayFrame(paint.GetImage());
        }
    }
}
