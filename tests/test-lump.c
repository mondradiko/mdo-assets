/** @file test-lump.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include "mondradiko/lump.h"
#include "mondradiko/raw_decompressor.h"

Test (lump, create_and_delete)
{
  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *ctx;
  mdo_result_t result = mdo_asset_ctx_create (&ctx, alloc);
  cr_assert (mdo_result_success (result));

  mdo_lump_t *lump;
  result = mdo_lump_create (&lump, ctx);
  cr_expect (mdo_result_success (result));

  mdo_lump_delete (lump);
  mdo_asset_ctx_delete (ctx);
}

Test (lump, load_from_file)
{
  static const char *LUMP_PATH = TESTS_DATA_PATH "test-lump.bin";
  cr_log_info ("loading test lump: %s", LUMP_PATH);

  static const uint64_t LUMP_HASH = 0x1f67dd1923b78915;
  cr_log_info ("expected hash: %lx", LUMP_HASH);

  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_asset_ctx_t *ctx;
  mdo_result_t result = mdo_asset_ctx_create (&ctx, alloc);
  cr_assert (mdo_result_success (result));

  mdo_decompressor_t *dc;
  result = mdo_raw_decompressor_create (&dc, alloc);
  cr_expect (mdo_result_success (result));

  mdo_lump_t *lump;
  result = mdo_lump_create (&lump, ctx);
  cr_expect (mdo_result_success (result));

  result = mdo_lump_load_from_file (lump, dc, LUMP_PATH, 128 * 1024 * 1024);
  cr_expect (mdo_result_success (result));

  uint64_t hash = mdo_lump_hash (lump);
  cr_log_info ("result hash: %lx", hash);
  cr_expect_eq (LUMP_HASH, hash);

  mdo_lump_delete (lump);
  mdo_asset_ctx_delete (ctx);
}
