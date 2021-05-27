/*! @file decompressor.h
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include "mondradiko/allocator.h"

/* for size_t */
#include <stddef.h>

/*! @function mdo_decompressor_cb
  Callback to decompress incoming lump data.
  @param data User-set data. See mdo_decompressor_create().
  @param dst_buf The destination buffer to copy to.
  @param dst_size_ptr
  When called, stores the maximum number of bytes available left in dst_buf.
  If there isn't enough available destination space available (overflow), then
  the implementation of this callback should quit decompression by returning 0.
  The implementation should always write back the number of bytes written to
  dst_buf.

  @param src_buf The source buffer, i.e. data read from the lump.
  @param src_size_ptr
  When called, stores the number of bytes that are available to be
  decompressed from the lump data. The implementation of this callback should
  write back the number of bytes actually consumed.

  @return
  A hint for how much many source bytes should be provided next call.
  Ends decompression if set to 0.
 */
typedef size_t (*mdo_decompressor_decompress_cb) (void *, void *, size_t *,
                                                  const void *, size_t *);

/*! @function mdo_decompressor_finalize_cb
  Callback to clean up a decompressor. Called by mdo_decompressor_delete.
  Intended to be used to free up any allocated resources or otherwise cleanup
  a decompressor implementation.
  @param data User-set data. See mdo_decompressor_create().
 */
typedef void (*mdo_decompressor_finalize_cb) (void *);

/*! @typedef mdo_decompressor_t
  A decompressor implementation. Used to decompress incoming lump data.
 */
typedef struct mdo_decompressor_s mdo_decompressor_t;

/*! @function mdo_decompressor_create
  Creates a decompressor implementation.
  @param new_decompressor The newly created decompressor.
  @param alloc An allocator to use, or NULL for default.
  @param decompress_cb See mdo_decompressor_cb().
  @param finalize_cb See mdo_decompressor_finalize_cb().
  @return 0 on success, non-zero otherwise.
 */
int mdo_decompressor_create (mdo_decompressor_t **, const mdo_allocator_t *,
                             void *, mdo_decompressor_decompress_cb,
                             mdo_decompressor_finalize_cb);

/*! @function mdo_decompressor_delete
  Deletes a decompressor implementation. Calls the finalize callback to free
  up any resources used by the implementation.
  @param decompressor The decompressor to delete.
 */
void mdo_decompressor_delete (mdo_decompressor_t *);

/*! @function mdo_decompressor_decompress
  Decompresses data given a decompressor implementation. See
  mdo_decompressor_cb() for more details.
 */
size_t mdo_decompressor_decompress (const mdo_decompressor_t *, void *,
                                    size_t *, const void *, size_t *);
