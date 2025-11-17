/*
 * Simple GPIO test for e-Paper BUSY pin
 * This just reads the pin state without waiting
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(gpio_test, LOG_LEVEL_INF);

/* BUSY pin is P1.8 (Arduino D7) */
#define BUSY_NODE DT_NODELABEL(gpio1)

static const struct gpio_dt_spec busy_pin = {
	.port = DEVICE_DT_GET(BUSY_NODE),
	.pin = 8,
	.dt_flags = GPIO_INPUT
};

int main(void)
{
	int ret;
	int pin_state;

	LOG_INF("=== E-Paper BUSY Pin Test ===");
	LOG_INF("Testing P1.8 (Arduino D7)");

	/* Check if GPIO device is ready */
	if (!device_is_ready(busy_pin.port)) {
		LOG_ERR("GPIO device not ready!");
		return -ENODEV;
	}

	/* Configure pin as input */
	ret = gpio_pin_configure_dt(&busy_pin, GPIO_INPUT);
	if (ret < 0) {
		LOG_ERR("Failed to configure BUSY pin: %d", ret);
		return ret;
	}

	LOG_INF("BUSY pin configured successfully");

	/* Read pin state continuously */
	while (1) {
		pin_state = gpio_pin_get_dt(&busy_pin);
		LOG_INF("BUSY pin state: %d %s", pin_state,
		        pin_state ? "(HIGH - display busy or not connected)" :
		        "(LOW - display ready)");
		k_msleep(1000);
	}

	return 0;
}
