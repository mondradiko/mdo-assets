/*! @file
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include <stddef.h>

/*! @typedef mdo_allocator_t
  @brief A general-purpose dynamic memory allocator. Different implementations
  can be used for different use-cases.
 */
typedef struct mdo_allocator_s mdo_allocator_t;

/*! @function mdo_allocator_malloc
  @brief Allocates a block of memory from an allocator.
  @param alloc The allocator to allocate from.
  @param size The size of the memory to allocate.
  @return A pointer to the freshly allocated memory, or NULL on failure.
 */
void *mdo_allocator_malloc (const mdo_allocator_t *, size_t);

/*! @function mdo_allocator_calloc
  @brief Allocates an array of elements from an allocator.
  @param alloc The allocator to allocate from.
  @param num The number of elements to allocate.
  @param size The size of each element.
  @return A pointer to the freshly allocated array, or NULL on failure.
 */
void *mdo_allocator_calloc (const mdo_allocator_t *, size_t, size_t);

/*! @function mdo_allocator_realloc
  @brief Re-allocates memory from an allocator.
  @param alloc The allocator to re-allocate from.
  @param memory A pointer to the original memory.
  @param size The new size of the data.
  @return A pointer to the re-allocated data, or NULL on failure.
 */
void *mdo_allocator_realloc (const mdo_allocator_t *, void *, size_t);

/*! @function mdo_allocator_free
  @brief Frees memory from an allocator.
  @param alloc The allocator to free from.
  @param memory The memory to free.
 */
void mdo_allocator_free (const mdo_allocator_t *, void *);

/*! @function mdo_default_allocator
  @brief Retrieves the default allocator. For now, uses stdlib malloc(),
  free(), etc.
  @return A handle to the global default allocator.
 */
const mdo_allocator_t *mdo_default_allocator ();
