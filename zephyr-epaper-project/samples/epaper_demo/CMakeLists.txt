# E-Paper Display Demo Application
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(epaper_demo)

target_sources(app PRIVATE src/main.c)

# Include LVGL header path
if(CONFIG_LVGL)
  target_include_directories(app PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../lib/lvgl
  )
endif()
