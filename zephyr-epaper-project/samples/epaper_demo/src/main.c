/*
 * E-Paper Display Demo with LVGL
 *
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 *
 * This demo shows how to use the Waveshare 2.9" e-Paper display
 * with LVGL on Zephyr OS.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <drivers/epd_2in9_v2.h>

#ifdef CONFIG_LVGL
#include "lvgl.h"
#include "lvgl_epaper_driver.h"
#endif

LOG_MODULE_REGISTER(epaper_demo, LOG_LEVEL_INF);

#define EPD_NODE DT_NODELABEL(epd_2in9_v2)

static const struct device *epd_dev = DEVICE_DT_GET(EPD_NODE);

/* LVGL task thread */
#ifdef CONFIG_LVGL
#define LVGL_TASK_PRIORITY 5
#define LVGL_TASK_STACK_SIZE 4096

static K_THREAD_STACK_DEFINE(lvgl_task_stack, LVGL_TASK_STACK_SIZE);
static struct k_thread lvgl_task_thread;

static void lvgl_task_entry(void *p1, void *p2, void *p3)
{
	ARG_UNUSED(p1);
	ARG_UNUSED(p2);
	ARG_UNUSED(p3);

	while (1) {
		lvgl_epaper_task_handler();
		k_msleep(10);
	}
}

static void create_demo_ui(void)
{
	lv_obj_t *scr = lv_scr_act();

	/* Set background to white */
	lv_obj_set_style_bg_color(scr, lv_color_white(), 0);

	/* Create a title label */
	lv_obj_t *title = lv_label_create(scr);
	lv_label_set_text(title, "Zephyr OS\ne-Paper Demo");
	lv_obj_set_style_text_align(title, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
	lv_obj_set_style_text_color(title, lv_color_black(), 0);

	/* Create a box with border */
	lv_obj_t *box = lv_obj_create(scr);
	lv_obj_set_size(box, 100, 60);
	lv_obj_align(box, LV_ALIGN_CENTER, 0, -20);
	lv_obj_set_style_bg_color(box, lv_color_white(), 0);
	lv_obj_set_style_border_color(box, lv_color_black(), 0);
	lv_obj_set_style_border_width(box, 2, 0);

	/* Create text inside the box */
	lv_obj_t *box_label = lv_label_create(box);
	lv_label_set_text(box_label, "WEACT\nESL 2.9\"");
	lv_obj_set_style_text_align(box_label, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_center(box_label);
	lv_obj_set_style_text_color(box_label, lv_color_black(), 0);

	/* Create info text */
	lv_obj_t *info = lv_label_create(scr);
	lv_label_set_text(info, "Resolution: 128x296\nDriver: EPD 2.9\" V2\nLVGL v9");
	lv_obj_set_style_text_align(info, LV_TEXT_ALIGN_CENTER, 0);
	lv_obj_align(info, LV_ALIGN_BOTTOM_MID, 0, -30);
	lv_obj_set_style_text_color(info, lv_color_black(), 0);

	/* Create board info */
	lv_obj_t *board = lv_label_create(scr);
#if defined(CONFIG_BOARD_NRF52840DK_NRF52840)
	lv_label_set_text(board, "Board: nRF52840-DK");
#elif defined(CONFIG_BOARD_NRF52DK_NRF52832)
	lv_label_set_text(board, "Board: nRF52832-DK");
#elif defined(CONFIG_BOARD_NRF54L15DK_NRF54L15_CPUAPP)
	lv_label_set_text(board, "Board: nRF54L15-DK");
#else
	lv_label_set_text(board, "Board: Unknown");
#endif
	lv_obj_align(board, LV_ALIGN_BOTTOM_MID, 0, -10);
	lv_obj_set_style_text_color(board, lv_color_black(), 0);
}
#endif /* CONFIG_LVGL */

/* Basic e-Paper test without LVGL */
static void basic_epaper_test(void)
{
	int ret;
	uint8_t test_buffer[EPD_2IN9_V2_BUFFER_SIZE];

	LOG_INF("Running basic e-Paper test (without LVGL)");

	/* Initialize display */
	ret = epd_2in9_v2_init(epd_dev, EPD_REFRESH_FULL);
	if (ret < 0) {
		LOG_ERR("Failed to initialize display: %d", ret);
		return;
	}

	/* Clear display */
	LOG_INF("Clearing display...");
	ret = epd_2in9_v2_clear(epd_dev);
	if (ret < 0) {
		LOG_ERR("Failed to clear display: %d", ret);
		return;
	}

	k_msleep(2000);

	/* Create a simple pattern */
	LOG_INF("Displaying test pattern...");
	memset(test_buffer, 0x00, EPD_2IN9_V2_BUFFER_SIZE);

	/* Draw horizontal lines */
	for (int y = 0; y < EPD_2IN9_V2_HEIGHT; y += 20) {
		for (int x = 0; x < EPD_2IN9_V2_WIDTH; x++) {
			int byte_idx = (y * EPD_2IN9_V2_WIDTH + x) / 8;
			int bit_idx = 7 - (x % 8);
			test_buffer[byte_idx] &= ~(1 << bit_idx);
		}
	}

	/* Display the pattern */
	ret = epd_2in9_v2_display(epd_dev, test_buffer, EPD_REFRESH_FULL);
	if (ret < 0) {
		LOG_ERR("Failed to display pattern: %d", ret);
		return;
	}

	LOG_INF("Test pattern displayed successfully");

	/* Sleep after 5 seconds */
	k_msleep(5000);
	LOG_INF("Putting display to sleep...");
	epd_2in9_v2_sleep(epd_dev);
}

int main(void)
{
	LOG_INF("=== E-Paper Display Demo ===");
	LOG_INF("Board: %s", CONFIG_BOARD);

	/* Check if e-Paper device is ready */
	if (!device_is_ready(epd_dev)) {
		LOG_ERR("E-Paper device not ready!");
		return -ENODEV;
	}

	LOG_INF("E-Paper device is ready");

#ifdef CONFIG_LVGL
	/* Initialize LVGL with e-Paper */
	int ret = lvgl_epaper_init();
	if (ret < 0) {
		LOG_ERR("Failed to initialize LVGL: %d", ret);
		LOG_INF("Falling back to basic e-Paper test");
		basic_epaper_test();
		return ret;
	}

	LOG_INF("LVGL initialized successfully");

	/* Create demo UI */
	create_demo_ui();

	/* Start LVGL task thread */
	k_thread_create(&lvgl_task_thread, lvgl_task_stack,
			K_THREAD_STACK_SIZEOF(lvgl_task_stack),
			lvgl_task_entry, NULL, NULL, NULL,
			LVGL_TASK_PRIORITY, 0, K_NO_WAIT);

	LOG_INF("LVGL task started");
	LOG_INF("Demo UI created and displayed");

	/* Main loop - can add button handling, animations, etc. */
	while (1) {
		k_msleep(1000);
		LOG_DBG("Main loop running...");
	}
#else
	/* Run basic test if LVGL is not enabled */
	basic_epaper_test();

	while (1) {
		k_msleep(10000);
	}
#endif

	return 0;
}
