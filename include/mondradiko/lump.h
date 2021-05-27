/*! @file lump.h
  @copyright Copyright (c) 2021 Marceline Cramer
  SPDX-License-Identifier: MIT
 */

#pragma once

#include <stdint.h> /* for uint64_t */

#include "mondradiko/asset_ctx.h"
#include "mondradiko/decompressor.h"

/** @typedef mdo_lump_t
  Asset "lumps" contain a compressed array of assets, and are stored on
  and loaded from disk.
 */
typedef struct mdo_lump_s mdo_lump_t;

/*! @function mdo_lump_create
  @brief Creates a lump strucure, used to load lump data into memory.
  @param lump The lump to create.
  @param ctx The asset context to use for shared state.
  @return 0 on success, non-zero otherwise.
 */
int mdo_lump_create (mdo_lump_t **, mdo_asset_ctx_t *);

/*! @function mdo_lump_load_from_file
  Begins loading a lump from a file on disk.
  @param lump The lump to load into.
  @param filename The path to the file to load.
  @param contents_size The expected size of the decompressed lump contents.
  @return 0 on success, non-zero otherwise.
 */
int mdo_lump_load_from_file (mdo_lump_t *, mdo_decompressor_t *, const char *,
                             size_t);

/*! @function mdo_lump_load_from_memory
  Begins loading a lump directly from memory.
  @param lump The lump to load into.
  @param src_data The memory of the lump.
  @param src_size The size of the source memory.
  @param contents_size The expected size of the decompressed lump contents.
  @return 0 on success, non-zero otherwise.
 */
int mdo_lump_load_from_memory (mdo_lump_t *, mdo_decompressor_t *, void *,
                               size_t, size_t);

/*! @function mdo_lump_flush
  @brief Blocks until a lump is finished loading.
  @param lump The lump to flush.
 */
void mdo_lump_flush (mdo_lump_t *);

/*! @function mdo_lump_delete
  @brief Deletes a lump, after canceling all queued jobs and flushing.
  @param lump The lump to destroy.
  @return The time that the lump spent active, in nanoseconds.
 */
uint64_t mdo_lump_delete (mdo_lump_t *);

/*! @function mdo_lump_hash
  @brief Hashes the contents of a lump. Flushes if necessary.
  @param lump The lump to hash.
  @return A 64-bit hash of the lump data, currently calculated with xxHash.
 */
uint64_t mdo_lump_hash (mdo_lump_t *);
