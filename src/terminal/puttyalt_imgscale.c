/* puttyalt_imgscale.c - image scaling for terminal image display (PuttyAlt) */
#include <string.h>
#include <stdio.h>

typedef enum {
    IMGSCALE_NONE = 0,
    IMGSCALE_STRETCH,
    IMGSCALE_CONTAIN,
    IMGSCALE_COVER
} ImgScaleMode;

typedef struct { int cell_w_px; int cell_h_px; } ImgScaleCtx;

typedef struct {
    int src_w_px, src_h_px;   /* source image size */
    int box_cols, box_rows;   /* target box in cells */
    ImgScaleMode mode;
} ImgScaleReq;

typedef struct {
    int out_w_px, out_h_px;   /* scaled pixel size */
    int cols, rows;           /* cell span */
    char desc[64];
} ImgScaleResult;

void imgscale_set_cell(ImgScaleCtx *ctx, int cell_w_px, int cell_h_px)
{
    if (!ctx) return;
    ctx->cell_w_px = (cell_w_px > 0) ? cell_w_px : 1;
    ctx->cell_h_px = (cell_h_px > 0) ? cell_h_px : 1;
}

int imgscale_cells_for_px(const ImgScaleCtx *ctx, int w_px, int h_px,
                          int *cols, int *rows)
{
    if (!ctx || !cols || !rows || ctx->cell_w_px <= 0 || ctx->cell_h_px <= 0)
        return -1;
    if (w_px < 0 || h_px < 0) return -1;
    *cols = (w_px + ctx->cell_w_px - 1) / ctx->cell_w_px;
    *rows = (h_px + ctx->cell_h_px - 1) / ctx->cell_h_px;
    return 0;
}

int imgscale_fit(const ImgScaleCtx *ctx, ImgScaleReq req, ImgScaleResult *out)
{
    int box_w, box_h, ow, oh;
    const char *name;
    if (!ctx || !out || ctx->cell_w_px <= 0 || ctx->cell_h_px <= 0) return -1;
    if (req.src_w_px <= 0 || req.src_h_px <= 0) return -1;
    if (req.box_cols <= 0 || req.box_rows <= 0) return -1;

    box_w = req.box_cols * ctx->cell_w_px;
    box_h = req.box_rows * ctx->cell_h_px;

    switch (req.mode) {
    case IMGSCALE_STRETCH:
        ow = box_w; oh = box_h; name = "stretch";
        break;
    case IMGSCALE_NONE:
        ow = (req.src_w_px < box_w) ? req.src_w_px : box_w;
        oh = (req.src_h_px < box_h) ? req.src_h_px : box_h;
        name = "none";
        break;
    case IMGSCALE_CONTAIN:
    case IMGSCALE_COVER: {
        /* scale by ratio*1000 to avoid float; min for contain, max for cover */
        long rx = (long)box_w * 1000 / req.src_w_px;
        long ry = (long)box_h * 1000 / req.src_h_px;
        int contain = (req.mode == IMGSCALE_CONTAIN);
        long r = contain ? (rx < ry ? rx : ry) : (rx > ry ? rx : ry);
        if (r < 1) r = 1;
        ow = (int)((long)req.src_w_px * r / 1000);
        oh = (int)((long)req.src_h_px * r / 1000);
        if (ow < 1) ow = 1;
        if (oh < 1) oh = 1;
        name = contain ? "contain" : "cover";
        break;
    }
    default:
        return -1;
    }

    out->out_w_px = ow;
    out->out_h_px = oh;
    if (imgscale_cells_for_px(ctx, ow, oh, &out->cols, &out->rows) != 0)
        return -1;
    snprintf(out->desc, sizeof(out->desc), "%s %dx%dpx %dx%dc",
             name, ow, oh, out->cols, out->rows);
    return 0;
}

int imgscale_clamp(ImgScaleResult *r, int max_w_px, int max_h_px)
{
    if (!r || max_w_px <= 0 || max_h_px <= 0) return -1;
    if (r->out_w_px > max_w_px) r->out_w_px = max_w_px;
    if (r->out_h_px > max_h_px) r->out_h_px = max_h_px;
    if (r->out_w_px < 0) r->out_w_px = 0;
    if (r->out_h_px < 0) r->out_h_px = 0;
    return 0;
}

ImgScaleMode imgscale_mode_from_str(const char *s)
{
    if (!s) return IMGSCALE_NONE;
    if (strcmp(s, "stretch") == 0) return IMGSCALE_STRETCH;
    if (strcmp(s, "contain") == 0) return IMGSCALE_CONTAIN;
    if (strcmp(s, "cover") == 0)   return IMGSCALE_COVER;
    return IMGSCALE_NONE;
}
