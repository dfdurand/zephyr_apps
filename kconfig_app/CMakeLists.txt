# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(kconfig_app)
if (CONFIG_BUILD_APP)
    target_sources(app PRIVATE src/main.c)
    message(" === building kconfig_app application for board: ${CONFIG_BOARD_TARGET} ===")
endif()