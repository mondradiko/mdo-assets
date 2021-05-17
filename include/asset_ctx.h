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

/*! @function
  @brief Initializes an asset_ctx.
  @param ctx The asset_ctx to initialize.
  @param alloc An allocator to use, or NULL for the default.
  @return Zero for success, non-zero otherwise.
 */
int mdo_asset_ctx_init (mdo_asset_ctx_t *, const mdo_allocator_t *);

/*! @function
  @brief Destroys an asset_ctx.
  @param ctx The asset_ctx to destroy.
  @return The time that the event loop spent in idle, in nanoseconds.
 */
uint64_t mdo_asset_ctx_destroy (mdo_asset_ctx_t *);
