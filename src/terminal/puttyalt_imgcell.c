/*
 * puttyalt_imgcell.c - Image cell placement tracking for PuttyAlt.
 * Maps image ids to anchor row/col cell rectangles on the terminal grid
 * and resolves overlaps on scroll or reflow. Self-contained module.
 */

#include <string.h>
#include <stdio.h>

#define IMGCELL_MAX 64   /* maximum simultaneous image placements */

/* One image placement anchored at (row,col) spanning cols x rows cells. */
typedef struct ImgCellPlacement {
    int img_id;          /* image identifier; -1 when slot is free */
    int row;             /* anchor row (top), may go negative on scroll */
    int col;             /* anchor column (left) */
    int cols;            /* width in cells, > 0 */
    int rows;            /* height in cells, > 0 */
    char tag[24];        /* formatted "id:RxC" tag for diagnostics */
} ImgCellPlacement;

typedef struct ImgCellMap {
    ImgCellPlacement slots[IMGCELL_MAX];
    int count;           /* number of occupied slots */
} ImgCellMap;

void imgcell_init(ImgCellMap *map)
{
    int i;
    if (!map)
        return;
    memset(map, 0, sizeof(*map));
    for (i = 0; i < IMGCELL_MAX; i++)
        map->slots[i].img_id = -1;
    map->count = 0;
}

int imgcell_place(ImgCellMap *map, int img_id, int row, int col,
                  int cols, int rows)
{
    int i, free_idx = -1;
    if (!map || img_id < 0 || col < 0 || cols <= 0 || rows <= 0)
        return -1;
    /* Reuse an existing slot for the same id (re-placement). */
    for (i = 0; i < IMGCELL_MAX; i++) {
        if (map->slots[i].img_id == img_id) {
            free_idx = i;
            break;
        }
        if (free_idx < 0 && map->slots[i].img_id < 0)
            free_idx = i;
    }
    if (free_idx < 0)
        return -1;   /* table full */
    if (map->slots[free_idx].img_id < 0)
        map->count++;
    map->slots[free_idx].img_id = img_id;
    map->slots[free_idx].row = row;
    map->slots[free_idx].col = col;
    map->slots[free_idx].cols = cols;
    map->slots[free_idx].rows = rows;
    snprintf(map->slots[free_idx].tag, sizeof(map->slots[free_idx].tag),
             "%d:%dx%d", img_id, rows, cols);
    return 0;
}

int imgcell_at(const ImgCellMap *map, int row, int col)
{
    int i;
    if (!map || col < 0)
        return -1;
    for (i = 0; i < IMGCELL_MAX; i++) {
        const ImgCellPlacement *p = &map->slots[i];
        if (p->img_id < 0)
            continue;
        if (row >= p->row && row < p->row + p->rows &&
            col >= p->col && col < p->col + p->cols)
            return p->img_id;
    }
    return -1;
}

void imgcell_scroll(ImgCellMap *map, int lines)
{
    int i;
    if (!map || lines == 0)
        return;
    for (i = 0; i < IMGCELL_MAX; i++) {
        ImgCellPlacement *p = &map->slots[i];
        if (p->img_id < 0)
            continue;
        p->row -= lines;
        /* Drop placements scrolled entirely off the top of the grid. */
        if (p->row + p->rows <= 0) {
            p->img_id = -1;
            if (map->count > 0)
                map->count--;
        }
    }
}

int imgcell_remove(ImgCellMap *map, int img_id)
{
    int i;
    if (!map || img_id < 0)
        return -1;
    for (i = 0; i < IMGCELL_MAX; i++) {
        if (map->slots[i].img_id == img_id) {
            map->slots[i].img_id = -1;
            if (map->count > 0)
                map->count--;
            return 0;
        }
    }
    return -1;
}

void imgcell_clear(ImgCellMap *map)
{
    imgcell_init(map);
}
