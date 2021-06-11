/** @file test-asset-ctx.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include "asset_ctx.h"

static void
test_create_and_delete (void **state)
{
  mdo_asset_ctx_t *ctx;
  mdo_result_t result = mdo_asset_ctx_create (&ctx, NULL);
  assert_true (mdo_result_success (result));

  mdo_asset_ctx_delete (ctx);
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (test_create_and_delete),
  };

  return cmocka_run_group_tests (tests, NULL, NULL);
}
