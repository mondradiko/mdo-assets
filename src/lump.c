#include "include/lump.h"
#include "include/allocator.h"
#include "include/asset_ctx.h"
#include "include/decompressor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <xxhash.h>

#include "include/raw_decompressor.h"

struct mdo_lump_s
{
  mdo_asset_ctx_t *ctx;

  uv_mutex_t mutex;

  uint64_t begin_time;
  int is_loading;

  uv_buf_t iov;
  size_t buf_size;
  char *buf_data;

  uv_fs_t open_req;
  uv_fs_t read_req;
  uv_fs_t close_req;
  uv_work_t data_work;

  size_t contents_size;
  char *contents_data;

  mdo_decompressor_t *decompressor;
  size_t dst_cursor;
};

static void mdo_lump_on_open (uv_fs_t *);
static void mdo_lump_on_read (uv_fs_t *);

static void
mdo_lump_on_data (uv_work_t *req)
{
  mdo_lump_t *lump = (mdo_lump_t *)uv_req_get_data ((uv_req_t *)req);
  uv_mutex_lock (&lump->mutex);

  void *dst_buf = lump->contents_data + lump->dst_cursor;
  size_t dst_size = lump->contents_size - lump->dst_cursor;
  size_t src_size = lump->read_req.result;

  size_t read_hint = mdo_decompressor_decompress (
      lump->decompressor, dst_buf, &dst_size, lump->buf_data, &src_size);

  lump->dst_cursor += dst_size;

  if (read_hint == 0)
    {
      fprintf (stderr, "compression implementation aborted\n");
      lump->is_loading = 0;
    }
  else if (lump->dst_cursor > lump->contents_size)
    {
      fprintf (stderr, "compression overflow; aborting\n");
      lump->is_loading = 0;
    }
  else if (src_size > lump->buf_size)
    {
      fprintf (stderr, "nonsensical src_size; aborting\n");
      lump->is_loading = 0;
    }
  else if (src_size == 0)
    {
      fprintf (stderr, "compression overflow; aborting\n");
      lump->is_loading = 0;
    }

  uv_mutex_unlock (&lump->mutex);
}

static void
mdo_lump_after_data (uv_work_t *req, int status)
{
  mdo_lump_t *lump = (mdo_lump_t *)uv_req_get_data ((uv_req_t *)req);
  uv_mutex_lock (&lump->mutex);

  uv_fs_read (mdo_asset_ctx_get_loop (lump->ctx), &lump->read_req,
              lump->open_req.result, &lump->iov, 1, -1, mdo_lump_on_read);

  uv_mutex_unlock (&lump->mutex);
}

static void
mdo_lump_on_open (uv_fs_t *req)
{
  mdo_lump_t *lump = (mdo_lump_t *)uv_req_get_data ((uv_req_t *)req);
  uv_mutex_lock (&lump->mutex);

  if (req->result < 0)
    {
      fprintf (stderr, "error opening file: %s\n", uv_strerror (req->result));
      lump->is_loading = 0;
    }
  else
    {
      lump->begin_time = uv_hrtime ();
      uv_fs_read (mdo_asset_ctx_get_loop (lump->ctx), &lump->read_req,
                  req->result, &lump->iov, 1, -1, mdo_lump_on_read);
    }

  uv_mutex_unlock (&lump->mutex);
}

static void
mdo_lump_on_read (uv_fs_t *req)
{
  mdo_lump_t *lump = (mdo_lump_t *)uv_req_get_data ((uv_req_t *)req);
  uv_mutex_lock (&lump->mutex);

  if (req->result < 0)
    {
      fprintf (stderr, "error reading file: %s\n", uv_strerror (req->result));
      lump->is_loading = 0;
    }
  else if (req->result == 0)
    {
      lump->is_loading = 0;
      uv_fs_close (mdo_asset_ctx_get_loop (lump->ctx), &lump->close_req,
                   lump->open_req.result, NULL);
    }
  else if (lump->is_loading)
    {
      uv_queue_work (mdo_asset_ctx_get_loop (lump->ctx), &lump->data_work,
                     mdo_lump_on_data, mdo_lump_after_data);
    }

  uv_mutex_unlock (&lump->mutex);
}

int
mdo_lump_create (mdo_lump_t **lump, mdo_asset_ctx_t *ctx)
{
  const mdo_allocator_t *alloc = mdo_asset_ctx_get_alloc (ctx);

  mdo_lump_t *new_lump = mdo_allocator_malloc (alloc, sizeof (mdo_lump_t));
  *lump = new_lump;

  new_lump->ctx = ctx;

  new_lump->is_loading = 0;
  new_lump->buf_data = NULL;
  new_lump->contents_data = NULL;

  uv_req_set_data ((uv_req_t *)&new_lump->open_req, new_lump);
  uv_req_set_data ((uv_req_t *)&new_lump->read_req, new_lump);
  uv_req_set_data ((uv_req_t *)&new_lump->close_req, new_lump);

  uv_req_set_data ((uv_req_t *)&new_lump->data_work, new_lump);

  uv_mutex_init (&new_lump->mutex);

  return 0;
}

int
mdo_lump_load_from_file (mdo_lump_t *lump, mdo_decompressor_t *decompressor,
                         const char *filename, size_t contents_size)
{
  const mdo_allocator_t *alloc = mdo_asset_ctx_get_alloc (lump->ctx);

  lump->is_loading = 1;
  lump->decompressor = decompressor;

  lump->buf_size = 96 * 1024;
  lump->buf_data = mdo_allocator_malloc (alloc, lump->buf_size);
  lump->iov = uv_buf_init (lump->buf_data, lump->buf_size);

  lump->contents_size = contents_size;
  lump->contents_data = mdo_allocator_malloc (alloc, contents_size);
  lump->dst_cursor = 0;

  lump->begin_time = uv_hrtime ();

  int result = uv_fs_open (mdo_asset_ctx_get_loop (lump->ctx), &lump->open_req,
                           filename, O_RDONLY, O_ASYNC | O_NONBLOCK,
                           mdo_lump_on_open);

  if (result)
    {
      fprintf (stderr, "error opening file: %s\n", uv_strerror (result));
      lump->is_loading = 0;
      return 1;
    }

  uv_run (mdo_asset_ctx_get_loop (lump->ctx), UV_RUN_ONCE);

  return 0;
}

void
mdo_lump_flush (mdo_lump_t *lump)
{
  uv_run (mdo_asset_ctx_get_loop (lump->ctx), UV_RUN_DEFAULT);

  if (lump->is_loading)
    {
      uv_mutex_lock (&lump->mutex);
      while (lump->is_loading)
        {
          uv_mutex_unlock (&lump->mutex);
          fprintf (stderr, "flushing lump state\n");
          uv_sleep (10);
          uv_mutex_lock (&lump->mutex);
        }

      uv_mutex_unlock (&lump->mutex);
    }
}

uint64_t
mdo_lump_delete (mdo_lump_t *lump)
{
  if (!lump)
    return 0;

  mdo_lump_flush (lump);

  uint64_t end_time = uv_hrtime ();
  uint64_t elapsed = end_time - lump->begin_time;

  mdo_decompressor_delete (lump->decompressor);
  uv_mutex_destroy (&lump->mutex);

  const mdo_allocator_t *alloc = mdo_asset_ctx_get_alloc (lump->ctx);

  if (lump->buf_data)
    mdo_allocator_free (alloc, lump->buf_data);

  if (lump->contents_data)
    mdo_allocator_free (alloc, lump->contents_data);

  mdo_allocator_free (alloc, lump);
  return elapsed;
}

uint64_t
mdo_lump_hash (mdo_lump_t *lump)
{
  mdo_lump_flush (lump);
  return XXH64 (lump->contents_data, lump->dst_cursor, 0);
}
