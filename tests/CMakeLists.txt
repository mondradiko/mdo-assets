# Copyright (c) 2021 Marceline Cramer
# SPDX-License-Identifier: MIT

include (mondradiko_create_test)

set (TESTS_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data/")
configure_file (test_common.h.in ${CMAKE_CURRENT_BINARY_DIR}/test_common.h)

include_directories (${CMAKE_CURRENT_BINARY_DIR})

mondradiko_create_test (${MDO_ASSETS_OBJ} asset-ctx test-asset-ctx.c)
mondradiko_create_test (${MDO_ASSETS_OBJ} decompressors test-decompressors.c)
mondradiko_create_test (${MDO_ASSETS_OBJ} lump test-lump.c)
