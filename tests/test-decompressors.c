/** @file test-decompressors.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include <stdlib.h>
#include <string.h>

#include "mondradiko/raw_decompressor.h"

static void
test_raw_decompressor (void **state)
{
  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_decompressor_t *decompressor;
  mdo_result_t result = mdo_raw_decompressor_create (&decompressor, alloc);
  assert_true (mdo_result_success (result));

  size_t buf_size = 1024;
  int *src_buf = mdo_allocator_calloc (alloc, sizeof (int), buf_size);
  int *dst_buf = mdo_allocator_calloc (alloc, sizeof (int), buf_size);

  /* fill with random data */
  for (size_t i = 0; i < buf_size; i++)
    {
      src_buf[i] = rand ();
    }

  size_t dst_size = buf_size;
  size_t src_size = buf_size;
  size_t read_hint = mdo_decompressor_decompress (
      decompressor, dst_buf, &dst_size, src_buf, &src_size);

  int mem_equals = memcmp (dst_buf, src_buf, buf_size);

  /* the raw decompressor should always be able to read more */
  assert_true (read_hint);

  /* ensure decompressor used all data */
  assert_int_equal (dst_size, buf_size);
  assert_int_equal (src_size, buf_size);

  /* check buffers for equality */
  assert_int_equal (0, mem_equals);

  mdo_allocator_free (alloc, src_buf);
  mdo_allocator_free (alloc, dst_buf);
  mdo_decompressor_delete (decompressor);
}

int
main (void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test (test_raw_decompressor),
  };

  return cmocka_run_group_tests (tests, NULL, NULL);
}
