#ifndef PUTTYALT_MEMPOOL_H
#define PUTTYALT_MEMPOOL_H

#include <stddef.h>

#define POOL_BLOCK_SIZE  (64 * 1024)  /* 64 KB blocks */
#define POOL_MAX_BLOCKS  256

typedef struct PoolBlock {
    unsigned char *data;
    size_t used;
    size_t capacity;
} PoolBlock;

typedef struct MemPool {
    PoolBlock blocks[POOL_MAX_BLOCKS];
    int num_blocks;
    size_t total_allocated;
    size_t total_used;
} MemPool;

void  pool_init(MemPool *pool);
void  pool_destroy(MemPool *pool);
void *pool_alloc(MemPool *pool, size_t size);
void  pool_reset(MemPool *pool);
size_t pool_used(const MemPool *pool);
size_t pool_allocated(const MemPool *pool);

#endif
