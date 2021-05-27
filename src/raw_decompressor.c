#include "include/raw_decompressor.h"

#include <string.h>

typedef struct mdo_raw_decompressor_s
{
  mdo_decompressor_t *base;
  const mdo_allocator_t *alloc;
} mdo_raw_decompressor_t;

size_t
mdo_raw_decompressor_decompress (void *data, void *dst_buf,
                                 size_t *dst_size_ptr, const void *src_buf,
                                 size_t *src_size_ptr)
{
  if (*src_size_ptr > *dst_size_ptr)
    return 0;

  memcpy (dst_buf, src_buf, *src_size_ptr);
  *dst_size_ptr = *src_size_ptr;
  return *dst_size_ptr;
}

void
mdo_raw_decompressor_finalize (void *data)
{
  mdo_raw_decompressor_t *rd = (mdo_raw_decompressor_t *)data;
  mdo_allocator_free (rd->alloc, data);
}

int
mdo_raw_decompressor_create (mdo_decompressor_t **decompressor,
                             const mdo_allocator_t *alloc)
{
  *decompressor = NULL;

  if (!alloc)
    alloc = mdo_default_allocator ();

  mdo_raw_decompressor_t *rd
      = mdo_allocator_malloc (alloc, sizeof (mdo_raw_decompressor_t));
  rd->alloc = alloc;

  if (mdo_decompressor_create (decompressor, alloc, rd,
                               mdo_raw_decompressor_decompress,
                               mdo_raw_decompressor_finalize))
    return 1;

  rd->base = *decompressor;

  return 0;
}
