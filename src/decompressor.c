/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "include/decompressor.h"

struct mdo_decompressor_s
{
  const mdo_allocator_t *alloc;
  void *data;
  mdo_decompressor_decompress_cb decompress;
  mdo_decompressor_finalize_cb finalize;
};

int
mdo_decompressor_create (mdo_decompressor_t **decompressor,
                         const mdo_allocator_t *alloc, void *data,
                         mdo_decompressor_decompress_cb decompress,
                         mdo_decompressor_finalize_cb finalize)
{
  if (!alloc)
    alloc = mdo_default_allocator ();

  mdo_decompressor_t *ndc
      = mdo_allocator_malloc (alloc, sizeof (mdo_decompressor_t));
  *decompressor = ndc;

  ndc->alloc = alloc;
  ndc->data = data;
  ndc->decompress = decompress;
  ndc->finalize = finalize;
  return 0;
}

void
mdo_decompressor_delete (mdo_decompressor_t *decompressor)
{
  if (!decompressor)
    return;

  decompressor->finalize (decompressor->data);
  mdo_allocator_free (decompressor->alloc, decompressor);
}

size_t
mdo_decompressor_decompress (const mdo_decompressor_t *decompressor,
                             void *dst_buf, size_t *dst_size_ptr,
                             const void *src_buf, size_t *src_size_ptr)
{
  return decompressor->decompress (decompressor->data, dst_buf, dst_size_ptr,
                                   src_buf, src_size_ptr);
}
