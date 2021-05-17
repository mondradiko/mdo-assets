/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include <stddef.h>
#include <stdint.h>
#include <uv.h>

#include "allocator.h"

/** @typedef mdo_asset_ctx_t
  @brief Stores all the state required to load lumps.
 */
typedef struct mdo_asset_ctx_s mdo_asset_ctx_t;

/*! @function mdo_asset_ctx_create
  @brief Creates an asset_ctx.
  @param ctx The asset_ctx to create.
  @param alloc An allocator to use, or NULL for the default.
  @return Zero for success, non-zero otherwise.
 */
int mdo_asset_ctx_create (mdo_asset_ctx_t **, const mdo_allocator_t *);

/*! @function mdo_asset_ctx_delete
  @brief Deletes an asset_ctx.
  @param ctx The asset_ctx to delete.
  @return The time that the event loop spent in idle, in nanoseconds.
 */
uint64_t mdo_asset_ctx_delete (mdo_asset_ctx_t *);

/*! @function mdo_asset_ctx_get_loop
  @brief Retrieves an asset contexts' loop.
  @param ctx The asset_ctx to retrieve the loop from.
  @return A handle to a uv_loop_t.
 */
uv_loop_t *mdo_asset_ctx_get_loop (mdo_asset_ctx_t *);

/*! @function mdo_asset_ctx_get_alloc
  @brief Retrieves an asset contexts' allocator.
  @param ctx The asset_ctx to retrieve the allocator from.
  @return A handle to an mdo_allocator_t.
*/
const mdo_allocator_t *mdo_asset_ctx_get_alloc (const mdo_asset_ctx_t *);
