/** @file test-lump.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include "lump.h"
#include "raw_decompressor.h"

static void
test_create_and_delete (void **state)
{
  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *ctx;
  mdo_result_t result = mdo_asset_ctx_create (&ctx, alloc);
  assert_true (mdo_result_success (result));

  mdo_lump_t *lump;
  result = mdo_lump_create (&lump, ctx);
  assert_true (mdo_result_success (result));

  mdo_lump_delete (lump);
  mdo_asset_ctx_delete (ctx);
}

static void
test_load_from_file (void **state)
{
  static const char *LUMP_PATH = TESTS_DATA_PATH "test-lump.bin";
  LOG_INF ("loading test lump: %s", LUMP_PATH);

  static const uint64_t LUMP_HASH = 0x1f67dd1923b78915;
  LOG_INF ("expected hash: %lx", LUMP_HASH);

  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *ctx;
  mdo_result_t result = mdo_asset_ctx_create (&ctx, alloc);
  assert_true (mdo_result_success (result));

  mdo_decompressor_t *dc;
  result = mdo_raw_decompressor_create (&dc, alloc);
  assert_true (mdo_result_success (result));

  mdo_lump_t *lump;
  result = mdo_lump_create (&lump, ctx);
  assert_true (mdo_result_success (result));

  result = mdo_lump_load_from_file (lump, dc, LUMP_PATH, 128 * 1024 * 1024);
  assert_true (mdo_result_success (result));

  uint64_t hash = mdo_lump_hash (lump);
  LOG_INF ("result hash: %lx", hash);
  assert_int_equal (LUMP_HASH, hash);

  mdo_lump_delete (lump);
  mdo_asset_ctx_delete (ctx);
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (test_create_and_delete),
    cmocka_unit_test (test_load_from_file),
  };

  return cmocka_run_group_tests (tests, NULL, NULL);
}
