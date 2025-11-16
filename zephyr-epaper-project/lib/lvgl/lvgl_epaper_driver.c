/*
 * LVGL Display Driver for e-Paper
 *
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <drivers/epd_2in9_v2.h>
#include "lvgl.h"

LOG_MODULE_REGISTER(lvgl_epaper, CONFIG_LOG_DEFAULT_LEVEL);

#define EPD_NODE DT_NODELABEL(epd_2in9_v2)

#if !DT_NODE_EXISTS(EPD_NODE)
#error "e-Paper device node not found in device tree"
#endif

static const struct device *epd_dev = DEVICE_DT_GET(EPD_NODE);

/* Display buffer */
static uint8_t disp_buf[EPD_2IN9_V2_BUFFER_SIZE];

/* LVGL display buffer */
static lv_disp_draw_buf_t disp_draw_buf;
static lv_disp_drv_t disp_drv;
static lv_disp_t *disp;

/* Zephyr logging function for LVGL */
void lv_zephyr_log(const char *buf)
{
	LOG_INF("%s", buf);
}

/* Flush callback - called when LVGL has rendered the content */
static void lvgl_flush_cb(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	uint16_t width = area->x2 - area->x1 + 1;
	uint16_t height = area->y2 - area->y1 + 1;
	uint32_t num_pixels = width * height;

	LOG_DBG("Flushing area: x1=%d, y1=%d, x2=%d, y2=%d",
		area->x1, area->y1, area->x2, area->y2);

	/* For full screen update */
	if (area->x1 == 0 && area->y1 == 0 &&
	    area->x2 == (EPD_2IN9_V2_WIDTH - 1) &&
	    area->y2 == (EPD_2IN9_V2_HEIGHT - 1)) {

		/* Convert LVGL buffer to e-Paper format */
		memset(disp_buf, 0xFF, EPD_2IN9_V2_BUFFER_SIZE);

		for (uint32_t i = 0; i < num_pixels; i++) {
			uint16_t x = area->x1 + (i % width);
			uint16_t y = area->y1 + (i / width);
			uint32_t byte_index = (y * EPD_2IN9_V2_WIDTH + x) / 8;
			uint8_t bit_index = 7 - (x % 8);

			/* In LVGL, 0 = black, 1 = white
			 * In e-Paper, 0 = black, 1 = white (same)
			 */
			if (color_p[i].full == 0) {  /* Black pixel */
				disp_buf[byte_index] &= ~(1 << bit_index);
			}
		}

		/* Display the buffer on e-Paper */
		epd_2in9_v2_display(epd_dev, disp_buf, EPD_REFRESH_FULL);
	} else {
		/* Partial update not implemented in this basic driver */
		LOG_WRN("Partial update requested but not implemented");
	}

	/* Inform LVGL that flushing is complete */
	lv_disp_flush_ready(disp_drv);
}

/* Rounder callback - adjust the area to update */
static void lvgl_rounder_cb(lv_disp_drv_t *disp_drv, lv_area_t *area)
{
	/* For e-Paper, we typically update the full screen */
	/* Round to full screen update */
	area->x1 = 0;
	area->y1 = 0;
	area->x2 = EPD_2IN9_V2_WIDTH - 1;
	area->y2 = EPD_2IN9_V2_HEIGHT - 1;
}

/* Set pixel callback - optional, for direct pixel manipulation */
static void lvgl_set_px_cb(lv_disp_drv_t *disp_drv, uint8_t *buf,
                           lv_coord_t buf_w, lv_coord_t x, lv_coord_t y,
                           lv_color_t color, lv_opa_t opa)
{
	uint32_t byte_index = (y * EPD_2IN9_V2_WIDTH + x) / 8;
	uint8_t bit_index = 7 - (x % 8);

	if (color.full == 0) {  /* Black */
		buf[byte_index] &= ~(1 << bit_index);
	} else {  /* White */
		buf[byte_index] |= (1 << bit_index);
	}
}

/* Initialize LVGL for e-Paper display */
int lvgl_epaper_init(void)
{
	if (!device_is_ready(epd_dev)) {
		LOG_ERR("e-Paper device not ready");
		return -ENODEV;
	}

	LOG_INF("Initializing LVGL for e-Paper display");

	/* Initialize LVGL */
	lv_init();

	/* Initialize the e-Paper display */
	int ret = epd_2in9_v2_init(epd_dev, EPD_REFRESH_FULL);
	if (ret < 0) {
		LOG_ERR("Failed to initialize e-Paper display: %d", ret);
		return ret;
	}

	/* Clear the display */
	ret = epd_2in9_v2_clear(epd_dev);
	if (ret < 0) {
		LOG_ERR("Failed to clear e-Paper display: %d", ret);
		return ret;
	}

	/* Initialize display buffer for LVGL */
	lv_disp_draw_buf_init(&disp_draw_buf, disp_buf, NULL, EPD_2IN9_V2_BUFFER_SIZE * 8);

	/* Initialize and register display driver */
	lv_disp_drv_init(&disp_drv);
	disp_drv.hor_res = EPD_2IN9_V2_WIDTH;
	disp_drv.ver_res = EPD_2IN9_V2_HEIGHT;
	disp_drv.draw_buf = &disp_draw_buf;
	disp_drv.flush_cb = lvgl_flush_cb;
	disp_drv.rounder_cb = lvgl_rounder_cb;
	disp_drv.set_px_cb = lvgl_set_px_cb;

	/* For monochrome displays */
	disp_drv.full_refresh = 1;  /* Always do full refresh for e-Paper */
	disp_drv.screen_transp = 0;

	disp = lv_disp_drv_register(&disp_drv);
	if (disp == NULL) {
		LOG_ERR("Failed to register LVGL display driver");
		return -ENOMEM;
	}

	/* Set mono theme for e-Paper */
	lv_theme_t *theme = lv_theme_mono_init(disp, false, &lv_font_montserrat_12);
	lv_disp_set_theme(disp, theme);

	LOG_INF("LVGL initialized successfully for e-Paper display");
	return 0;
}

/* LVGL task handler - should be called periodically */
void lvgl_epaper_task_handler(void)
{
	lv_task_handler();
}

/* Get the LVGL display object */
lv_disp_t *lvgl_epaper_get_display(void)
{
	return disp;
}
