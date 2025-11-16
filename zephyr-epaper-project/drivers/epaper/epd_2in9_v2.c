/*
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 *
 * Waveshare EPD 2.9" V2 e-Paper Display Driver for Zephyr OS
 * Ported from Waveshare's original driver
 */

#define DT_DRV_COMPAT waveshare_epd_2in9_v2

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <drivers/epd_2in9_v2.h>

LOG_MODULE_REGISTER(epd_2in9_v2, CONFIG_LOG_DEFAULT_LEVEL);

/* LUT tables for different refresh modes */
static const uint8_t LUT_PARTIAL[159] = {
	0x0,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x80,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x40,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0A,0x0,0x0,0x0,0x0,0x0,0x2,
	0x1,0x0,0x0,0x0,0x0,0x0,0x0,
	0x1,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
	0x22,0x17,0x41,0xB0,0x32,0x36,
};

static const uint8_t LUT_FULL[159] = {
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x19,0x19,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x24,0x42,0x22,0x22,0x23,0x32,0x00,0x00,0x00,
	0x22,0x17,0x41,0xAE,0x32,0x38,
};

static const uint8_t LUT_GRAY4[159] = {
	0x00,0x60,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x20,0x60,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x28,0x60,0x14,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x2A,0x60,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x90,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x02,0x00,0x05,0x14,0x00,0x00,
	0x1E,0x1E,0x00,0x00,0x00,0x00,0x01,
	0x00,0x02,0x00,0x05,0x14,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x24,0x22,0x22,0x22,0x23,0x32,0x00,0x00,0x00,
	0x22,0x17,0x41,0xAE,0x32,0x28,
};

static const uint8_t LUT_FAST[159] = {
	0x80,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x0,0x0,
	0x10,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0,0x0,
	0x80,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x0,0x0,0x0,
	0x10,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x20,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x14,0x8,0x0,0x0,0x0,0x0,0x1,
	0xA,0xA,0x0,0xA,0xA,0x0,0x1,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x14,0x8,0x0,0x1,0x0,0x0,0x1,
	0x0,0x0,0x0,0x0,0x0,0x0,0x1,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x44,0x44,0x44,0x44,0x44,0x44,0x0,0x0,0x0,
	0x22,0x17,0x41,0x0,0x32,0x36
};

/* Device configuration structure */
struct epd_2in9_v2_config {
	struct spi_dt_spec spi;
	struct gpio_dt_spec rst;
	struct gpio_dt_spec dc;
	struct gpio_dt_spec busy;
	uint16_t width;
	uint16_t height;
};

/* Device data structure */
struct epd_2in9_v2_data {
	enum epd_refresh_mode current_mode;
	bool sleeping;
};

/* Low-level functions */
static int epd_reset(const struct device *dev)
{
	const struct epd_2in9_v2_config *config = dev->config;

	gpio_pin_set_dt(&config->rst, 1);
	k_msleep(10);
	gpio_pin_set_dt(&config->rst, 0);
	k_msleep(2);
	gpio_pin_set_dt(&config->rst, 1);
	k_msleep(10);

	return 0;
}

static int epd_send_command(const struct device *dev, uint8_t cmd)
{
	const struct epd_2in9_v2_config *config = dev->config;
	struct spi_buf tx_buf = {
		.buf = &cmd,
		.len = 1
	};
	struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	gpio_pin_set_dt(&config->dc, 0);  /* Command mode */
	return spi_write_dt(&config->spi, &tx_bufs);
}

static int epd_send_data(const struct device *dev, uint8_t data)
{
	const struct epd_2in9_v2_config *config = dev->config;
	struct spi_buf tx_buf = {
		.buf = &data,
		.len = 1
	};
	struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	gpio_pin_set_dt(&config->dc, 1);  /* Data mode */
	return spi_write_dt(&config->spi, &tx_bufs);
}

static int epd_send_data_multi(const struct device *dev, const uint8_t *data, size_t len)
{
	const struct epd_2in9_v2_config *config = dev->config;
	struct spi_buf tx_buf = {
		.buf = (uint8_t *)data,
		.len = len
	};
	struct spi_buf_set tx_bufs = {
		.buffers = &tx_buf,
		.count = 1
	};

	gpio_pin_set_dt(&config->dc, 1);  /* Data mode */
	return spi_write_dt(&config->spi, &tx_bufs);
}

static void epd_wait_busy(const struct device *dev)
{
	const struct epd_2in9_v2_config *config = dev->config;

	LOG_DBG("Waiting for display...");
	while (gpio_pin_get_dt(&config->busy) == 1) {
		k_msleep(10);
	}
	k_msleep(50);
	LOG_DBG("Display ready");
}

static int epd_load_lut(const struct device *dev, const uint8_t *lut)
{
	int ret;

	/* Send LUT command */
	ret = epd_send_command(dev, 0x32);
	if (ret < 0) {
		return ret;
	}

	/* Send LUT data (153 bytes) */
	for (int i = 0; i < 153; i++) {
		ret = epd_send_data(dev, lut[i]);
		if (ret < 0) {
			return ret;
		}
	}

	epd_wait_busy(dev);

	/* Set additional LUT parameters */
	epd_send_command(dev, 0x3f);
	epd_send_data(dev, lut[153]);

	epd_send_command(dev, 0x03);  /* Gate voltage */
	epd_send_data(dev, lut[154]);

	epd_send_command(dev, 0x04);  /* Source voltage */
	epd_send_data(dev, lut[155]);  /* VSH */
	epd_send_data(dev, lut[156]);  /* VSH2 */
	epd_send_data(dev, lut[157]);  /* VSL */

	epd_send_command(dev, 0x2c);  /* VCOM */
	epd_send_data(dev, lut[158]);

	return 0;
}

static void epd_set_window(const struct device *dev, uint16_t x_start, uint16_t y_start,
                           uint16_t x_end, uint16_t y_end)
{
	epd_send_command(dev, 0x44);  /* SET_RAM_X_ADDRESS_START_END_POSITION */
	epd_send_data(dev, (x_start >> 3) & 0xFF);
	epd_send_data(dev, (x_end >> 3) & 0xFF);

	epd_send_command(dev, 0x45);  /* SET_RAM_Y_ADDRESS_START_END_POSITION */
	epd_send_data(dev, y_start & 0xFF);
	epd_send_data(dev, (y_start >> 8) & 0xFF);
	epd_send_data(dev, y_end & 0xFF);
	epd_send_data(dev, (y_end >> 8) & 0xFF);
}

static void epd_set_cursor(const struct device *dev, uint16_t x, uint16_t y)
{
	epd_send_command(dev, 0x4E);  /* SET_RAM_X_ADDRESS_COUNTER */
	epd_send_data(dev, x & 0xFF);

	epd_send_command(dev, 0x4F);  /* SET_RAM_Y_ADDRESS_COUNTER */
	epd_send_data(dev, y & 0xFF);
	epd_send_data(dev, (y >> 8) & 0xFF);
}

static int epd_turn_on_display(const struct device *dev)
{
	epd_send_command(dev, 0x22);  /* Display Update Control */
	epd_send_data(dev, 0xC7);
	epd_send_command(dev, 0x20);  /* Activate Display Update Sequence */
	epd_wait_busy(dev);
	return 0;
}

static int epd_turn_on_display_partial(const struct device *dev)
{
	epd_send_command(dev, 0x22);  /* Display Update Control */
	epd_send_data(dev, 0x0F);
	epd_send_command(dev, 0x20);  /* Activate Display Update Sequence */
	epd_wait_busy(dev);
	return 0;
}

/* Public API functions */
int epd_2in9_v2_init(const struct device *dev, enum epd_refresh_mode mode)
{
	const struct epd_2in9_v2_config *config = dev->config;
	struct epd_2in9_v2_data *data = dev->data;
	int ret;

	LOG_INF("Initializing EPD 2.9\" V2 display");

	/* Reset display */
	epd_reset(dev);
	k_msleep(100);

	epd_wait_busy(dev);

	/* Software reset */
	epd_send_command(dev, 0x12);
	epd_wait_busy(dev);

	/* Driver output control */
	epd_send_command(dev, 0x01);
	epd_send_data(dev, 0x27);
	epd_send_data(dev, 0x01);
	epd_send_data(dev, 0x00);

	/* Data entry mode */
	epd_send_command(dev, 0x11);
	epd_send_data(dev, 0x03);

	/* Set window */
	epd_set_window(dev, 0, 0, config->width - 1, config->height - 1);

	if (mode == EPD_REFRESH_GRAY4) {
		/* Gray4 mode specific initialization */
		epd_send_command(dev, 0x74);  /* Set analog block control */
		epd_send_data(dev, 0x54);
		epd_send_command(dev, 0x7E);  /* Set digital block control */
		epd_send_data(dev, 0x3B);

		epd_send_command(dev, 0x3C);
		epd_send_data(dev, 0x00);
	} else if (mode == EPD_REFRESH_FAST) {
		epd_send_command(dev, 0x3C);
		epd_send_data(dev, 0x05);
	}

	/* Display update control */
	epd_send_command(dev, 0x21);
	epd_send_data(dev, 0x00);
	epd_send_data(dev, 0x80);

	/* Set cursor */
	epd_set_cursor(dev, 0, 0);
	epd_wait_busy(dev);

	/* Load appropriate LUT */
	switch (mode) {
	case EPD_REFRESH_FULL:
		ret = epd_load_lut(dev, LUT_FAST);
		break;
	case EPD_REFRESH_FAST:
		ret = epd_load_lut(dev, LUT_FULL);
		break;
	case EPD_REFRESH_GRAY4:
		ret = epd_load_lut(dev, LUT_GRAY4);
		break;
	default:
		ret = epd_load_lut(dev, LUT_FAST);
		break;
	}

	data->current_mode = mode;
	data->sleeping = false;

	LOG_INF("EPD 2.9\" V2 initialized successfully");
	return ret;
}

int epd_2in9_v2_clear(const struct device *dev)
{
	const struct epd_2in9_v2_config *config = dev->config;
	uint16_t buffer_size = (config->width * config->height) / 8;

	LOG_INF("Clearing display");

	/* Write white to both RAM buffers */
	epd_send_command(dev, 0x24);  /* Write RAM (black/white) */
	for (uint16_t i = 0; i < buffer_size; i++) {
		epd_send_data(dev, 0xFF);
	}

	epd_send_command(dev, 0x26);  /* Write RAM (red/white) */
	for (uint16_t i = 0; i < buffer_size; i++) {
		epd_send_data(dev, 0xFF);
	}

	return epd_turn_on_display(dev);
}

int epd_2in9_v2_display(const struct device *dev, const uint8_t *image,
                        enum epd_refresh_mode mode)
{
	const struct epd_2in9_v2_config *config = dev->config;
	struct epd_2in9_v2_data *data = dev->data;
	uint16_t buffer_size = (config->width * config->height) / 8;

	if (image == NULL) {
		return -EINVAL;
	}

	LOG_INF("Displaying image (mode: %d)", mode);

	/* Reinitialize if mode changed */
	if (data->current_mode != mode) {
		epd_2in9_v2_init(dev, mode);
	}

	epd_send_command(dev, 0x24);  /* Write RAM for black/white */
	epd_send_data_multi(dev, image, buffer_size);

	return epd_turn_on_display(dev);
}

int epd_2in9_v2_display_partial(const struct device *dev, const uint8_t *image)
{
	const struct epd_2in9_v2_config *config = dev->config;
	uint16_t buffer_size = (config->width * config->height) / 8;

	if (image == NULL) {
		return -EINVAL;
	}

	LOG_INF("Displaying partial update");

	/* Reset */
	gpio_pin_set_dt(&config->rst, 0);
	k_msleep(1);
	gpio_pin_set_dt(&config->rst, 1);
	k_msleep(2);

	epd_load_lut(dev, LUT_PARTIAL);

	epd_send_command(dev, 0x37);
	for (int i = 0; i < 10; i++) {
		epd_send_data(dev, (i == 5) ? 0x40 : 0x00);
	}

	epd_send_command(dev, 0x3C);  /* Border waveform */
	epd_send_data(dev, 0x80);

	epd_send_command(dev, 0x22);
	epd_send_data(dev, 0xC0);
	epd_send_command(dev, 0x20);
	epd_wait_busy(dev);

	epd_set_window(dev, 0, 0, config->width - 1, config->height - 1);
	epd_set_cursor(dev, 0, 0);

	epd_send_command(dev, 0x24);  /* Write RAM */
	epd_send_data_multi(dev, image, buffer_size);

	return epd_turn_on_display_partial(dev);
}

int epd_2in9_v2_display_gray4(const struct device *dev, const uint8_t *image)
{
	const struct epd_2in9_v2_config *config = dev->config;
	uint16_t buffer_size = (config->width * config->height) / 8;
	uint8_t temp1, temp2, temp3;

	if (image == NULL) {
		return -EINVAL;
	}

	LOG_INF("Displaying 4-level grayscale image");

	/* Process and send old data */
	epd_send_command(dev, 0x24);
	for (uint16_t i = 0; i < buffer_size; i++) {
		temp3 = 0;
		for (uint8_t j = 0; j < 2; j++) {
			temp1 = image[i * 2 + j];
			for (uint8_t k = 0; k < 2; k++) {
				temp2 = temp1 & 0xC0;
				if (temp2 == 0xC0 || temp2 == 0x40) {
					temp3 |= 0x00;
				} else {
					temp3 |= 0x01;
				}
				if (!(j == 1 && k == 1)) {
					temp3 <<= 1;
				}
				temp1 <<= 2;

				temp2 = temp1 & 0xC0;
				if (temp2 == 0xC0 || temp2 == 0x40) {
					temp3 |= 0x00;
				} else {
					temp3 |= 0x01;
				}
				if (!(j == 1 && k == 1)) {
					temp3 <<= 1;
				}
				temp1 <<= 2;
			}
		}
		epd_send_data(dev, temp3);
	}

	/* Process and send new data */
	epd_send_command(dev, 0x26);
	for (uint16_t i = 0; i < buffer_size; i++) {
		temp3 = 0;
		for (uint8_t j = 0; j < 2; j++) {
			temp1 = image[i * 2 + j];
			for (uint8_t k = 0; k < 2; k++) {
				temp2 = temp1 & 0xC0;
				if (temp2 == 0xC0) {
					temp3 |= 0x00;  /* White */
				} else if (temp2 == 0x00) {
					temp3 |= 0x01;  /* Black */
				} else if (temp2 == 0x80) {
					temp3 |= 0x00;  /* Gray1 */
				} else {
					temp3 |= 0x01;  /* Gray2 */
				}
				if (!(j == 1 && k == 1)) {
					temp3 <<= 1;
				}
				temp1 <<= 2;

				temp2 = temp1 & 0xC0;
				if (temp2 == 0xC0) {
					temp3 |= 0x00;
				} else if (temp2 == 0x00) {
					temp3 |= 0x01;
				} else if (temp2 == 0x80) {
					temp3 |= 0x00;
				} else {
					temp3 |= 0x01;
				}
				if (!(j == 1 && k == 1)) {
					temp3 <<= 1;
				}
				temp1 <<= 2;
			}
		}
		epd_send_data(dev, temp3);
	}

	return epd_turn_on_display(dev);
}

int epd_2in9_v2_sleep(const struct device *dev)
{
	struct epd_2in9_v2_data *data = dev->data;

	LOG_INF("Putting display to sleep");

	epd_send_command(dev, 0x10);  /* Enter deep sleep */
	epd_send_data(dev, 0x01);
	k_msleep(100);

	data->sleeping = true;
	return 0;
}

int epd_2in9_v2_wakeup(const struct device *dev, enum epd_refresh_mode mode)
{
	struct epd_2in9_v2_data *data = dev->data;

	if (!data->sleeping) {
		return 0;
	}

	LOG_INF("Waking up display");
	return epd_2in9_v2_init(dev, mode);
}

/* Device initialization */
static int epd_2in9_v2_driver_init(const struct device *dev)
{
	const struct epd_2in9_v2_config *config = dev->config;
	int ret;

	LOG_INF("Initializing EPD 2.9\" V2 driver");

	/* Check SPI device */
	if (!spi_is_ready_dt(&config->spi)) {
		LOG_ERR("SPI device not ready");
		return -ENODEV;
	}

	/* Configure GPIOs */
	if (!gpio_is_ready_dt(&config->rst)) {
		LOG_ERR("RST GPIO not ready");
		return -ENODEV;
	}
	ret = gpio_pin_configure_dt(&config->rst, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure RST pin");
		return ret;
	}

	if (!gpio_is_ready_dt(&config->dc)) {
		LOG_ERR("DC GPIO not ready");
		return -ENODEV;
	}
	ret = gpio_pin_configure_dt(&config->dc, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		LOG_ERR("Failed to configure DC pin");
		return ret;
	}

	if (!gpio_is_ready_dt(&config->busy)) {
		LOG_ERR("BUSY GPIO not ready");
		return -ENODEV;
	}
	ret = gpio_pin_configure_dt(&config->busy, GPIO_INPUT);
	if (ret < 0) {
		LOG_ERR("Failed to configure BUSY pin");
		return ret;
	}

	LOG_INF("EPD 2.9\" V2 driver initialized");
	return 0;
}

/* Device instantiation macro */
#define EPD_2IN9_V2_DEFINE(inst)                                                \
	static struct epd_2in9_v2_data epd_2in9_v2_data_##inst = {              \
		.current_mode = EPD_REFRESH_FULL,                               \
		.sleeping = true,                                               \
	};                                                                      \
	                                                                        \
	static const struct epd_2in9_v2_config epd_2in9_v2_config_##inst = {    \
		.spi = SPI_DT_SPEC_INST_GET(inst, SPI_OP_MODE_MASTER |          \
					    SPI_TRANSFER_MSB | SPI_WORD_SET(8), 0), \
		.rst = GPIO_DT_SPEC_INST_GET(inst, rst_gpios),                  \
		.dc = GPIO_DT_SPEC_INST_GET(inst, dc_gpios),                    \
		.busy = GPIO_DT_SPEC_INST_GET(inst, busy_gpios),                \
		.width = DT_INST_PROP(inst, width),                             \
		.height = DT_INST_PROP(inst, height),                           \
	};                                                                      \
	                                                                        \
	DEVICE_DT_INST_DEFINE(inst,                                             \
			      epd_2in9_v2_driver_init,                          \
			      NULL,                                             \
			      &epd_2in9_v2_data_##inst,                         \
			      &epd_2in9_v2_config_##inst,                       \
			      POST_KERNEL,                                      \
			      CONFIG_EPAPER_EPD_2IN9_V2_INIT_PRIORITY,          \
			      NULL);

DT_INST_FOREACH_STATUS_OKAY(EPD_2IN9_V2_DEFINE)
