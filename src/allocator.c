/*! @file
 @copyright Copyright (c) 2021 Marceline Cramer
 SPDX-License-Identifier: MIT
 */

#include "allocator.h"

#include <stdlib.h>

static const mdo_allocator_t s_default_allocator
    = { .malloc = malloc, .calloc = calloc, .realloc = realloc, .free = free };

const mdo_allocator_t *
mdo_default_allocator ()
{
  return &s_default_allocator;
}
