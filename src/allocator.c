/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#include "allocator.h"

#include <stdlib.h>

void *
mdo_allocator_malloc (const mdo_allocator_t *alloc, size_t size)
{
  return alloc->malloc (alloc->data, size);
}

void *
mdo_allocator_calloc (const mdo_allocator_t *alloc, size_t num, size_t size)
{
  return alloc->calloc (alloc->data, num, size);
}

void *
mdo_allocator_realloc (const mdo_allocator_t *alloc, void *memory, size_t size)
{
  return alloc->realloc (alloc->data, memory, size);
}

void
mdo_allocator_free (const mdo_allocator_t *alloc, void *memory)
{
  alloc->free (alloc->data, memory);
}

void *
mdo_std_malloc (void *data, size_t size)
{
  return malloc (size);
}

void *
mdo_std_calloc (void *data, size_t num, size_t size)
{
  return calloc (num, size);
}

void *
mdo_std_realloc (void *data, void *memory, size_t size)
{
  return realloc (memory, size);
}

void
mdo_std_free (void *data, void *memory)
{
  free (memory);
}

static const mdo_allocator_t s_default_allocator
    = { .malloc = mdo_std_malloc,
        .calloc = mdo_std_calloc,
        .realloc = mdo_std_realloc,
        .free = mdo_std_free };

const mdo_allocator_t *
mdo_default_allocator ()
{
  return &s_default_allocator;
}
