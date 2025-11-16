# E-Paper Driver Module CMakeLists.txt
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

if(CONFIG_EPAPER_EPD_2IN9_V2)
  zephyr_library()
  zephyr_library_sources(drivers/epaper/epd_2in9_v2.c)
  # Make include directory available globally (use absolute path)
  zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

# Add LVGL if configured
# Use add_subdirectory with explicit binary dir to avoid conflicts
if(CONFIG_LVGL AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/lib/lvgl/CMakeLists.txt)
  add_subdirectory(lib/lvgl ${CMAKE_BINARY_DIR}/epaper_module_lvgl)
endif()
