/*! @file
 @copyright Copyright (c) 2021 Marceline Cramer
 SPDX-License-Identifier: MIT
 */

#pragma once

#include "include/allocator.h"

#include <stddef.h>

typedef void *(*mdo_malloc_cb) (size_t);
typedef void *(*mdo_calloc_cb) (size_t, size_t);
typedef void *(*mdo_realloc_cb) (void *, size_t);
typedef void (*mdo_free_cb) (void *);

struct mdo_allocator_s
{
  mdo_malloc_cb malloc;
  mdo_calloc_cb calloc;
  mdo_realloc_cb realloc;
  mdo_free_cb free;
};
