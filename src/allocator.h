/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include "include/allocator.h"

#include <stddef.h>

typedef void *(*mdo_malloc_cb) (void *, size_t);
typedef void *(*mdo_calloc_cb) (void *, size_t, size_t);
typedef void *(*mdo_realloc_cb) (void *, void *, size_t);
typedef void (*mdo_free_cb) (void *, void *);

struct mdo_allocator_s
{
  void *data;
  mdo_malloc_cb malloc;
  mdo_calloc_cb calloc;
  mdo_realloc_cb realloc;
  mdo_free_cb free;
};
