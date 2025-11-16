/*
 * LVGL Display Driver for e-Paper - Header
 *
 * Copyright (c) 2025
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef LVGL_EPAPER_DRIVER_H_
#define LVGL_EPAPER_DRIVER_H_

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize LVGL for e-Paper display
 *
 * @return 0 on success, negative errno on failure
 */
int lvgl_epaper_init(void);

/**
 * @brief LVGL task handler - must be called periodically
 *
 * This function should be called from a thread or timer to handle
 * LVGL tasks such as animation, input processing, etc.
 */
void lvgl_epaper_task_handler(void);

/**
 * @brief Get the LVGL display object
 *
 * @return Pointer to the LVGL display object
 */
lv_disp_t *lvgl_epaper_get_display(void);

#ifdef __cplusplus
}
#endif

#endif /* LVGL_EPAPER_DRIVER_H_ */
