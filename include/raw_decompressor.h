/*! @file raw_decompressor.h
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include "decompressor.h"
#include "mdo-utils/allocator.h"
#include "mdo-utils/result.h"

/*! @function mdo_raw_decompressor_create
  Creates a raw decompressor implementation. The raw decompressor simply copies
  all incoming data directly to the output.
  @param new_decompressor The newly created decompressor.
  @param alloc The allocator to use, or NULL for the default.
  @return #mdo_result_t.
 */
mdo_result_t mdo_raw_decompressor_create (mdo_decompressor_t **,
                                          const mdo_allocator_t *);
