/*! @file
 @copyright Copyright (c) 2021 Marceline Cramer
 SPDX-License-Identifier: MIT
 */

#include "include/asset_ctx.h"

#include "allocator.h"
#include "asset_ctx.h"
#include "lump.h"

#include <stdlib.h>

int
mdo_asset_ctx_init (mdo_asset_ctx_t *ctx, const mdo_allocator_t *alloc)
{
  if (!alloc)
    alloc = mdo_default_allocator ();

  ctx->alloc = alloc;

  uv_loop_init (&ctx->loop);
  uv_loop_configure (&ctx->loop, UV_METRICS_IDLE_TIME);

  return 0;
}

uint64_t
mdo_asset_ctx_destroy (mdo_asset_ctx_t *ctx)
{
  uv_run (&ctx->loop, UV_RUN_DEFAULT); /* flush loop */
  uint64_t idle_time = uv_metrics_idle_time (&ctx->loop);
  uv_loop_close (&ctx->loop);
  return idle_time;
}
