#include "test_common.h"

#include "src/asset_ctx.c"

Test (asset_ctx, init_and_destroy)
{
  mdo_asset_ctx_t ctx;
  cr_assert_eq (0, mdo_asset_ctx_init (&ctx, cr_allocator ()));
  mdo_asset_ctx_destroy (&ctx);
}
