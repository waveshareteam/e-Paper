/*
 * LVGL Configuration for e-Paper Display
 *
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LV_CONF_H
#define LV_CONF_H

#include <zephyr/autoconf.h>

/*====================
   COLOR SETTINGS
 *====================*/

/* Color depth: 1 (1 byte per pixel), 8 (RGB332), 16 (RGB565), 32 (ARGB8888) */
#define LV_COLOR_DEPTH CONFIG_LVGL_COLOR_DEPTH

/*=========================
   MEMORY SETTINGS
 *=========================*/

/* 1: use custom malloc/free, 0: use the built-in `lv_mem_alloc()` and `lv_mem_free()` */
#define LV_MEM_CUSTOM 1
#if LV_MEM_CUSTOM
#  define LV_MEM_CUSTOM_INCLUDE <stdlib.h>
#  define LV_MEM_CUSTOM_ALLOC   malloc
#  define LV_MEM_CUSTOM_FREE    free
#  define LV_MEM_CUSTOM_REALLOC realloc
#else
#  define LV_MEM_SIZE (48U * 1024U)  /* [bytes] */
#endif

/* Number of buffer allocations */
#define LV_MEM_BUF_MAX_NUM 16

/*====================
   HAL SETTINGS
 *====================*/

/* Default display refresh period in milliseconds */
#define LV_DISP_DEF_REFR_PERIOD 5000  /* 5 seconds for e-Paper */

/* Input device read period in milliseconds */
#define LV_INDEV_DEF_READ_PERIOD 30

/* DPI (Dots per inch) */
#define LV_DPI_DEF CONFIG_LVGL_DPI

/*=================
   DRAWING
 *=================*/

/* Enable complex draw engine */
#define LV_DRAW_COMPLEX 1

/* Align memory for better performance */
#define LV_USE_DRAW_MASKS 1

/*=================
   FEATURE USAGE
 *=================*/

/* Enable the built-in styles */
#define LV_USE_THEMES 1
#define LV_USE_THEME_DEFAULT 1
#define LV_USE_THEME_BASIC 1
#define LV_USE_THEME_MONO 1  /* Monochrome theme for e-Paper */

/* Enable built-in fonts */
#define LV_FONT_MONTSERRAT_8  1
#define LV_FONT_MONTSERRAT_10 1
#define LV_FONT_MONTSERRAT_12 1
#define LV_FONT_MONTSERRAT_14 1
#define LV_FONT_MONTSERRAT_16 1
#define LV_FONT_MONTSERRAT_18 0
#define LV_FONT_MONTSERRAT_20 0
#define LV_FONT_MONTSERRAT_22 0
#define LV_FONT_MONTSERRAT_24 0

/* Default font */
#define LV_FONT_DEFAULT &lv_font_montserrat_12

/*================
   LOGGING
 *================*/

/* Enable the log module */
#ifdef CONFIG_LVGL_USE_LOG
#  define LV_USE_LOG 1
#else
#  define LV_USE_LOG 0
#endif

#if LV_USE_LOG
/* Log level: 0=Trace, 1=Info, 2=Warn, 3=Error, 4=User, 5=None */
#  define LV_LOG_LEVEL CONFIG_LVGL_LOG_LEVEL

/* 1: Print log with 'printf'; 0: Custom log function */
#  define LV_LOG_PRINTF 0
#  if LV_LOG_PRINTF == 0
#    define LV_LOG_USER_CB lv_zephyr_log
     void lv_zephyr_log(const char *buf);
#  endif
#endif

/*=================
   OTHERS
 *=================*/

/* 1: Show CPU usage and FPS count */
#define LV_USE_PERF_MONITOR 0

/* 1: Show the used memory and the fragmentation */
#define LV_USE_MEM_MONITOR 0

/* Enable snapshot support */
#define LV_USE_SNAPSHOT 1

/* Garbage Collector settings */
#define LV_ENABLE_GC 0

/*==================
 * EXAMPLES
 *==================*/

/* Enable examples */
#define LV_BUILD_EXAMPLES 0

/*--END OF LV_CONF_H--*/

#endif /* LV_CONF_H */
