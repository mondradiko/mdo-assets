#include "test_common.h"

static const mdo_allocator_t s_cr_allocator = { .malloc = cr_malloc,
                                                .calloc = cr_calloc,
                                                .realloc = cr_realloc,
                                                .free = cr_free };

const mdo_allocator_t *
cr_allocator ()
{
  // return &s_cr_allocator;
  return NULL;
}
