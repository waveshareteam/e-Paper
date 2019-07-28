# Make sure to run make on the same directory where Makefile is located
# Make sure to execute the example "epd" as sudo and in the same directory where Makefile is located

DIR_Config   = ./lib/Config
DIR_EPD      = ./lib/e-Paper
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Examples = ./examples
DIR_BIN      = ./bin
DIR_LIB		 = "$(shell pwd)"

SRC_EPD = $(wildcard ${DIR_EPD}/*.c)
SRC_Config = $(wildcard ${DIR_Config}/*.c)
SRC_GUI = $(wildcard ${DIR_GUI}/*.c)
SRC_Examples = $(wildcard ${DIR_Examples}/*.c)
SRC_FONTS = $(wildcard ${DIR_FONTS}/*.c)

SRC_LIB = $(SRC_EPD) $(SRC_Config) $(SRC_GUI) $(SRC_FONTS)
OBJ_LIB = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${SRC_LIB}))

SRC_EXAMPLE = $(SRC_Examples)
OBJ_EXAMPLE = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${SRC_EXAMPLE}))

TARGET_LIB = libepd.so
TARGET_EXAMPLE = epd

USELIB = USE_BCM2835_LIB
# USELIB = USE_WIRINGPI_LIB
DEBUG = -D $(USELIB)
ifeq ($(USELIB), USE_BCM2835_LIB)
    LIB = -lbcm2835 -lm 
else ifeq ($(USELIB), USE_WIRINGPI_LIB)
    LIB = -lwiringPi -lm 
endif

CC = gcc
MSG = -g -O0 -Wall -fpic
CFLAGS += $(MSG) $(DEBUG)

all: make_bin_dir lib example

make_bin_dir: 
	mkdir -p $(DIR_BIN)

lib: make_bin_dir $(OBJ_LIB)
	$(CC) -shared -o $(TARGET_LIB) $(OBJ_LIB) $(LIB)

example: make_bin_dir lib $(OBJ_EXAMPLE)
	$(CC) $(CFLAGS) $(OBJ_EXAMPLE) -o $(TARGET_EXAMPLE) $(LIB) -L$(DIR_LIB) -lepd -Wl,-rpath=$(DIR_LIB)

${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD)
    
${DIR_BIN}/%.o:$(DIR_Config)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ $(LIB)
    
${DIR_BIN}/%.o:$(DIR_EPD)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config)
    
${DIR_BIN}/%.o:$(DIR_FONTS)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@
    
${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_Config)


clean:
	rm -rf $(DIR_BIN)
	rm -f $(TARGET_EXAMPLE) $(TARGET_LIB)
