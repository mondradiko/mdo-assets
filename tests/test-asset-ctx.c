/** @file test-asset-ctx.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include "mondradiko/asset_ctx.h"

Test (asset_ctx, create_and_delete)
{
  mdo_asset_ctx_t *ctx;
  cr_assert_eq (0, mdo_asset_ctx_create (&ctx, NULL));
  mdo_asset_ctx_delete (ctx);
}
