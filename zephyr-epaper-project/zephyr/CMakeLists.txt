# E-Paper Driver Module - Zephyr Build File
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0
#
# This file is processed by Zephyr when the module is included

if(CONFIG_EPAPER_EPD_2IN9_V2)
  zephyr_library()
  zephyr_library_sources(${CMAKE_CURRENT_LIST_DIR}/../drivers/epaper/epd_2in9_v2.c)
  # Make include directory available globally
  zephyr_include_directories(${CMAKE_CURRENT_LIST_DIR}/../include)
endif()

# LVGL Integration
# Add LVGL e-Paper driver if both are enabled
if(CONFIG_EPAPER_EPD_2IN9_V2 AND CONFIG_LVGL)
  if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../lib/lvgl/lvgl_epaper_driver.c)
    zephyr_library_sources(${CMAKE_CURRENT_LIST_DIR}/../lib/lvgl/lvgl_epaper_driver.c)
  endif()
endif()
