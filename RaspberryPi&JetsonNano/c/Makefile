DIR_Config   = ./lib/Config
DIR_EPD      = ./lib/e-Paper
DIR_FONTS    = ./lib/Fonts
DIR_GUI      = ./lib/GUI
DIR_Examples = ./examples
DIR_BIN      = ./bin
DIR_LIB      = "$(shell pwd)"

SRC_EPD      = $(wildcard ${DIR_EPD}/*.c)
SRC_FONTS    = $(wildcard ${DIR_FONTS}/*.c)
SRC_GUI      = $(wildcard ${DIR_GUI}/*.c)
SRC_Examples = $(wildcard ${DIR_Examples}/*.c)

SRC_EXAMPLE  = ${SRC_Examples}
SRC_LIB      = ${SRC_EPD} ${SRC_FONTS} ${SRC_GUI}

OBJ_EXAMPLE  = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${SRC_EXAMPLE}))
OBJ_LIB      = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${SRC_LIB}))

OBJ_DEV_RPI = $(wildcard $(DIR_BIN)/dev_hardware_SPI.o $(DIR_BIN)/RPI_sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )
OBJ_DEV_JETSON = $(wildcard $(DIR_BIN)/sysfs_software_spi.o $(DIR_BIN)/sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )

DEBUG = -D DEBUG


# RASPBERRY PI LIBRARIES

#USELIB_RPI = USE_DEV_LIB
USELIB_RPI = USE_BCM2835_LIB
#USELIB_RPI = USE_WIRINGPI_LIB

LIB_RPI = -lm
ifeq ($(USELIB_RPI), USE_BCM2835_LIB)
    LIB_RPI += -lbcm2835
else ifeq ($(USELIB_RPI), USE_WIRINGPI_LIB)
    LIB_RPI += -lwiringPi
endif
DEBUG_RPI = -D $(USELIB_RPI) -D RPI


# JETSON NANO LIBRARIES

USELIB_JETSON = USE_DEV_LIB
#USELIB_JETSON = USE_HARDWARE_LIB

LIB_JETSON = -lm
DEBUG_JETSON = -D $(USELIB_JETSON) -D JETSON


.PHONY : RPI JETSON clean

all:    select_one
RPI:    make_bin_dir RPI_DEV    RPI_lib    RPI_example 
JETSON: make_bin_dir JETSON_DEV JETSON_lib JETSON_example

TARGET_example = epd
TARGET_lib = libepd.so
CC = gcc
CFLAGS += -g -O0 -Wall

select_one:
	$(info Use 'make RPI' or 'make JETSON')

make_bin_dir:
	mkdir -p ${DIR_BIN}

RPI_lib: make_bin_dir ${OBJ_LIB}
	$(CC) -shared -o ${TARGET_lib} ${OBJ_LIB} ${OBJ_DEV_RPI} ${LIB_RPI}

JETSON_lib: make_bin_dir ${OBJ_LIB}
	$(CC) -shared -o ${TARGET_lib} ${OBJ_LIB} ${OBJ_DEV_JETSON} ${LIB_JETSON}

RPI_example: RPI_lib ${OBJ_EXAMPLE}
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_EXAMPLE) -o $(TARGET_example) $(LIB_RPI) $(DEBUG) -L$(DIR_LIB) -lepd -Wl,-rpath=$(DIR_LIB)
    
JETSON_example: JETSON_lib ${OBJ_EXAMPLE}
	echo $(@)
	$(CC) $(CFLAGS) -D JETSON $(OBJ_EXAMPLE) -o $(TARGET_example) $(LIB_JETSON) $(DEBUG) -L$(DIR_LIB) -lepd -Wl,-rpath=$(DIR_LIB)

${DIR_BIN}/%.o:$(DIR_Examples)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config) -I $(DIR_GUI) -I $(DIR_EPD) $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_EPD)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config) $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_FONTS)/%.c 
	$(CC) $(CFLAGS) -c $< -o $@ $(DEBUG)
    
${DIR_BIN}/%.o:$(DIR_GUI)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I $(DIR_Config) $(DEBUG)

RPI_DEV:
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/RPI_sysfs_gpio.c -o $(DIR_BIN)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c  $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)
	
JETSON_DEV:
	$(CC) $(CFLAGS) $(DEBUG_JETSON) -c  $(DIR_Config)/sysfs_software_spi.c -o $(DIR_BIN)/sysfs_software_spi.o $(LIB_JETSON) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSON) -c  $(DIR_Config)/sysfs_gpio.c -o $(DIR_BIN)/sysfs_gpio.o $(LIB_JETSON) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSON) -c  $(DIR_Config)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_JETSON)  $(DEBUG)

clean :
	rm -rf $(DIR_BIN)
	rm -f $(TARGET_example) $(TARGET_lib)

