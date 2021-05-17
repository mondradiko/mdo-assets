/*! @file
 @copyright Copyright (c) 2021 Marceline Cramer
 SPDX-License-Identifier: MIT
 */

#pragma once

#include "include/asset_ctx.h"

#include <uv.h>

struct mdo_asset_ctx_s
{
  const mdo_allocator_t *alloc;
  uv_loop_t loop;
};
