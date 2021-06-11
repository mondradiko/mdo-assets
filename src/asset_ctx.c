/*! @file asset_ctx.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "asset_ctx.h"

struct mdo_asset_ctx_s
{
  const mdo_allocator_t *alloc;
  mdo_result_t uv_error;

  uv_loop_t loop;
};

static mdo_result_t
handle_uv_error (mdo_asset_ctx_t *ctx, int error)
{
  return LOG_RESULT (ctx->uv_error, uv_strerror (error));
}

mdo_result_t
mdo_asset_ctx_create (mdo_asset_ctx_t **ctx, const mdo_allocator_t *alloc)
{
  if (!alloc)
    alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *new_ctx
      = mdo_allocator_malloc (alloc, sizeof (mdo_asset_ctx_t));
  *ctx = new_ctx;

  new_ctx->alloc = alloc;
  new_ctx->uv_error
      = mdo_result_create (MDO_LOG_ERROR, "uv error: %s", 1, false);

  int uv_result;

  uv_result = uv_loop_init (&new_ctx->loop);
  if (uv_result)
    return handle_uv_error (new_ctx, uv_result);

  uv_result = uv_loop_configure (&new_ctx->loop, UV_METRICS_IDLE_TIME);
  if (uv_result)
    return handle_uv_error (new_ctx, uv_result);

  return MDO_SUCCESS;
}

uint64_t
mdo_asset_ctx_delete (mdo_asset_ctx_t *ctx)
{
  if (!ctx)
    return 0;

  int uv_result;

  uv_result = uv_run (&ctx->loop, UV_RUN_DEFAULT); /* flush loop */
  uint64_t idle_time = uv_metrics_idle_time (&ctx->loop);

  if (uv_result)
    handle_uv_error (ctx, uv_result);

  uv_result = uv_loop_close (&ctx->loop);

  if (uv_result)
    handle_uv_error (ctx, uv_result);

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
