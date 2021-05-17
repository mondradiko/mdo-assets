/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include <stddef.h>
#include <stdint.h>

#include "allocator.h"
#include "lump.h"

/* internal types */
typedef struct mdo_asset_ctx_s mdo_asset_ctx_t;

/* functions */
int mdo_asset_ctx_init (mdo_asset_ctx_t *, const mdo_allocator_t *);
uint64_t mdo_asset_ctx_destroy (mdo_asset_ctx_t *);
