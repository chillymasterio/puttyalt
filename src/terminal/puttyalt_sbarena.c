/*
 * puttyalt_sbarena.c - Scrollback arena allocator for PuttyAlt.
 *
 * Bump/arena allocator tailored for scrollback lines: memory is carved from
 * a fixed pool of fixed-size blocks, and whole blocks are released at once
 * when scrollback is trimmed, avoiding per-line malloc/free churn.
 *
 * Self-contained C99 library module with static state. No project headers,
 * no windows.h. All functions are prefixed sbarena_. A clock value is taken
 * as the now_ms parameter; no time-of-day calls are made.
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SBARENA_BLOCKS     64
#define SBARENA_BLOCK_SIZE 8192
#define SBARENA_ALIGN      8

typedef struct SbArenaBlock {
    unsigned char data[SBARENA_BLOCK_SIZE];
    int           used;     /* bytes consumed from this block        */
    unsigned char active;   /* 1 if block is in service              */
    uint64_t      stamp_ms; /* now_ms of the block's last allocation */
} SbArenaBlock;

typedef struct SbArena {
    SbArenaBlock blocks[SBARENA_BLOCKS];
    int          cur;          /* index of current fill block, -1 if none */
    uint64_t     total_bytes;  /* lifetime bytes requested                */
    uint64_t     total_allocs; /* lifetime allocation count               */
    uint64_t     releases;     /* lifetime blocks reclaimed               */
    int          oom;          /* count of failed allocations             */
} SbArena;

static SbArena g_sbarena;

static int sbarena_round_up(int n)
{
    int rem = n % SBARENA_ALIGN;
    return rem ? n + (SBARENA_ALIGN - rem) : n;
}

int sbarena_init(void)
{
    memset(&g_sbarena, 0, sizeof(g_sbarena));
    g_sbarena.cur = -1;
    return 0;
}

/* Allocate `size` bytes (8-byte aligned) from the arena, stamping the
 * chosen block with now_ms. Returns NULL on failure; allocations larger
 * than a block, or when the pool is exhausted, are rejected. */
void *sbarena_alloc(int size, uint64_t now_ms)
{
    int need;
    SbArenaBlock *b;
    void *p;
    if (size <= 0)
        return NULL;
    need = sbarena_round_up(size);
    if (need > SBARENA_BLOCK_SIZE) {
        g_sbarena.oom++;
        return NULL;
    }
    if (g_sbarena.cur < 0 ||
        g_sbarena.blocks[g_sbarena.cur].used + need > SBARENA_BLOCK_SIZE) {
        int i, slot = -1;
        for (i = 0; i < SBARENA_BLOCKS; i++) {
            if (!g_sbarena.blocks[i].active) { slot = i; break; }
        }
        if (slot < 0) {
            g_sbarena.oom++;
            return NULL;
        }
        g_sbarena.blocks[slot].active = 1;
        g_sbarena.blocks[slot].used = 0;
        g_sbarena.cur = slot;
    }
    b = &g_sbarena.blocks[g_sbarena.cur];
    p = &b->data[b->used];
    b->used += need;
    b->stamp_ms = now_ms;
    g_sbarena.total_bytes += (uint64_t)need;
    g_sbarena.total_allocs++;
    return p;
}

/* Copy a NUL-terminated string into the arena. Returns NULL on failure. */
char *sbarena_strdup(const char *s, uint64_t now_ms)
{
    int len;
    char *p;
    if (!s)
        return NULL;
    len = (int)strlen(s) + 1;
    p = (char *)sbarena_alloc(len, now_ms);
    if (!p)
        return NULL;
    memcpy(p, s, (size_t)len);
    return p;
}

/* Release every active block (except the current fill block) whose last
 * allocation predates older_than_ms, reclaiming whole blocks at once.
 * Returns the number of blocks freed, or -1 on error. */
int sbarena_release(uint64_t older_than_ms)
{
    int i, freed = 0;
    for (i = 0; i < SBARENA_BLOCKS; i++) {
        SbArenaBlock *b = &g_sbarena.blocks[i];
        if (b->active && i != g_sbarena.cur && b->stamp_ms < older_than_ms) {
            b->active = 0;
            b->used = 0;
            b->stamp_ms = 0;
            g_sbarena.releases++;
            freed++;
        }
    }
    return freed;
}

/* Reset the whole arena, freeing every block for reuse. */
void sbarena_reset(void)
{
    int i;
    for (i = 0; i < SBARENA_BLOCKS; i++) {
        g_sbarena.blocks[i].active = 0;
        g_sbarena.blocks[i].used = 0;
        g_sbarena.blocks[i].stamp_ms = 0;
    }
    g_sbarena.cur = -1;
    g_sbarena.releases++;
}

/* Report fill statistics into buf. Returns bytes written (excluding the
 * NUL terminator), or -1 on error or truncation. */
int sbarena_stats(char *buf, int bufsize)
{
    int i, active = 0, n;
    uint64_t used = 0;
    if (!buf || bufsize <= 0)
        return -1;
    for (i = 0; i < SBARENA_BLOCKS; i++) {
        if (g_sbarena.blocks[i].active) {
            active++;
            used += (uint64_t)g_sbarena.blocks[i].used;
        }
    }
    n = snprintf(buf, (size_t)bufsize,
                 "sbarena: %d/%d blk, %llu B live, %llu allocs, rel=%llu, oom=%d",
                 active, SBARENA_BLOCKS, (unsigned long long)used,
                 (unsigned long long)g_sbarena.total_allocs,
                 (unsigned long long)g_sbarena.releases, g_sbarena.oom);
    if (n < 0 || n >= bufsize)
        return -1;
    return n;
}
