/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 *
 * Waveshare EPD 2.9" V2 e-Paper Display Driver for Zephyr OS
 * Ported from Waveshare's original driver
 */

#ifndef ZEPHYR_DRIVERS_DISPLAY_EPD_2IN9_V2_H_
#define ZEPHYR_DRIVERS_DISPLAY_EPD_2IN9_V2_H_

#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Display resolution */
#define EPD_2IN9_V2_WIDTH       128
#define EPD_2IN9_V2_HEIGHT      296
#define EPD_2IN9_V2_BUFFER_SIZE ((EPD_2IN9_V2_WIDTH * EPD_2IN9_V2_HEIGHT) / 8)  /* 4736 bytes */

/* Refresh modes */
enum epd_refresh_mode {
	EPD_REFRESH_FULL = 0,
	EPD_REFRESH_FAST,
	EPD_REFRESH_PARTIAL,
	EPD_REFRESH_GRAY4
};

/**
 * @brief Initialize the e-Paper display
 *
 * @param dev Pointer to the device structure
 * @param mode Refresh mode to initialize
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_init(const struct device *dev, enum epd_refresh_mode mode);

/**
 * @brief Clear the display (fill with white)
 *
 * @param dev Pointer to the device structure
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_clear(const struct device *dev);

/**
 * @brief Display an image buffer on the e-Paper
 *
 * @param dev Pointer to the device structure
 * @param image Pointer to image buffer (4736 bytes for 2.9")
 * @param mode Refresh mode to use
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_display(const struct device *dev, const uint8_t *image,
                        enum epd_refresh_mode mode);

/**
 * @brief Display a partial image update
 *
 * @param dev Pointer to the device structure
 * @param image Pointer to image buffer
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_display_partial(const struct device *dev, const uint8_t *image);

/**
 * @brief Display a 4-level grayscale image
 *
 * @param dev Pointer to the device structure
 * @param image Pointer to grayscale image buffer (9472 bytes)
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_display_gray4(const struct device *dev, const uint8_t *image);

/**
 * @brief Put the display into sleep mode
 *
 * @param dev Pointer to the device structure
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_sleep(const struct device *dev);

/**
 * @brief Wake up the display from sleep mode
 *
 * @param dev Pointer to the device structure
 * @param mode Refresh mode to initialize after wakeup
 * @return 0 on success, negative errno on failure
 */
int epd_2in9_v2_wakeup(const struct device *dev, enum epd_refresh_mode mode);

#ifdef __cplusplus
}
#endif

#endif /* ZEPHYR_DRIVERS_DISPLAY_EPD_2IN9_V2_H_ */
