/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "mondradiko/asset_ctx.h"

struct mdo_asset_ctx_s
{
  const mdo_allocator_t *alloc;
  uv_loop_t loop;
};

int
mdo_asset_ctx_create (mdo_asset_ctx_t **ctx, const mdo_allocator_t *alloc)
{
  if (!alloc)
    alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *new_ctx
      = mdo_allocator_malloc (alloc, sizeof (mdo_asset_ctx_t));
  new_ctx->alloc = alloc;
  *ctx = new_ctx;

  uv_loop_init (&new_ctx->loop);
  uv_loop_configure (&new_ctx->loop, UV_METRICS_IDLE_TIME);

  return 0;
}

uint64_t
mdo_asset_ctx_delete (mdo_asset_ctx_t *ctx)
{
  if (!ctx)
    return 0;

  uv_run (&ctx->loop, UV_RUN_DEFAULT); /* flush loop */
  uint64_t idle_time = uv_metrics_idle_time (&ctx->loop);
  uv_loop_close (&ctx->loop);

  const mdo_allocator_t *alloc = ctx->alloc;
  mdo_allocator_free (alloc, ctx);

  return idle_time;
}

uv_loop_t *
mdo_asset_ctx_get_loop (mdo_asset_ctx_t *ctx)
{
  return &ctx->loop;
}

const mdo_allocator_t *
mdo_asset_ctx_get_alloc (const mdo_asset_ctx_t *ctx)
{
  return ctx->alloc;
}
