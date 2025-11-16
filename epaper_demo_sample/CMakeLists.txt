# E-Paper Display Demo Application
#
# Copyright (c) 2025
# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

# Add the epaper module to the module path
list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}/../zephyr-epaper-project
)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(epaper_demo)

target_sources(app PRIVATE src/main.c)
