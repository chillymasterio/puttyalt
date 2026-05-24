#include <string.h>
#include <stdlib.h>
#include "puttyalt_mempool.h"

void pool_init(MemPool *pool)
{
    memset(pool, 0, sizeof(*pool));
}

static int pool_add_block(MemPool *pool, size_t min_size)
{
    if (pool->num_blocks >= POOL_MAX_BLOCKS) return -1;
    size_t cap = min_size > POOL_BLOCK_SIZE ? min_size : POOL_BLOCK_SIZE;
    PoolBlock *b = &pool->blocks[pool->num_blocks];
    b->data = (unsigned char *)malloc(cap);
    if (!b->data) return -1;
    b->used = 0;
    b->capacity = cap;
    pool->num_blocks++;
    pool->total_allocated += cap;
    return pool->num_blocks - 1;
}

void *pool_alloc(MemPool *pool, size_t size)
{
    if (size == 0) return NULL;

    /* Align to 8 bytes */
    size = (size + 7) & ~(size_t)7;

    /* Try current block */
    if (pool->num_blocks > 0) {
        PoolBlock *b = &pool->blocks[pool->num_blocks - 1];
        if (b->used + size <= b->capacity) {
            void *ptr = b->data + b->used;
            b->used += size;
            pool->total_used += size;
            return ptr;
        }
    }

    /* Need new block */
    int idx = pool_add_block(pool, size);
    if (idx < 0) return NULL;

    PoolBlock *b = &pool->blocks[idx];
    void *ptr = b->data + b->used;
    b->used += size;
    pool->total_used += size;
    return ptr;
}

void pool_reset(MemPool *pool)
{
    for (int i = 0; i < pool->num_blocks; i++)
        pool->blocks[i].used = 0;
    pool->total_used = 0;
}

void pool_destroy(MemPool *pool)
{
    for (int i = 0; i < pool->num_blocks; i++) {
        free(pool->blocks[i].data);
        pool->blocks[i].data = NULL;
    }
    pool->num_blocks = 0;
    pool->total_allocated = 0;
    pool->total_used = 0;
}

size_t pool_used(const MemPool *pool)
{
    return pool->total_used;
}

size_t pool_allocated(const MemPool *pool)
{
    return pool->total_allocated;
}
