/** @file test-decompressors.c
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "test_common.h"

#include <stdlib.h>
#include <string.h>

#include "mondradiko/raw_decompressor.h"

Test (decompressors, raw_decompressor)
{
  const mdo_allocator_t *alloc = mdo_default_allocator ();

  mdo_decompressor_t *decompressor;
  int result = mdo_raw_decompressor_create (&decompressor, alloc);
  cr_assert_eq (0, result);

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

  /* the raw decompressor should always be able to read more */
  cr_expect_neq (0, read_hint);

  /* ensure decompressor used all data */
  cr_expect_eq (dst_size, buf_size);
  cr_expect_eq (src_size, buf_size);

  /* check buffers for equality */
  int mem_equals = memcmp (dst_buf, src_buf, buf_size);
  cr_expect_eq (0, mem_equals);

  mdo_allocator_free (alloc, src_buf);
  mdo_allocator_free (alloc, dst_buf);
  mdo_decompressor_delete (decompressor);
}
