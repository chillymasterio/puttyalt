#include <string.h>
#include <stdlib.h>
#include "puttyalt_sharedclip.h"

void sclip_init(SharedClipboard *sc)
{
    memset(sc, 0, sizeof(*sc));
    sc->sync_enabled = 1;
}

void sclip_free(SharedClipboard *sc)
{
    for (int i = 0; i < CLIP_MAX_ENTRIES; i++) {
        free(sc->entries[i].data);
        sc->entries[i].data = NULL;
    }
    sc->count = 0;
    sc->head = 0;
}

int sclip_push(SharedClipboard *sc, const char *data, int len, int session)
{
    if (!data || len <= 0 || len > CLIP_MAX_SIZE) return -1;

    /* Free old entry at head if ring is full */
    ClipEntry *e = &sc->entries[sc->head];
    free(e->data);

    e->data = malloc(len + 1);
    if (!e->data) return -1;
    memcpy(e->data, data, len);
    e->data[len] = '\0';
    e->len = len;
    e->from_session = session;

    sc->head = (sc->head + 1) % CLIP_MAX_ENTRIES;
    if (sc->count < CLIP_MAX_ENTRIES) sc->count++;
    return 0;
}

const ClipEntry *sclip_peek(const SharedClipboard *sc)
{
    if (sc->count == 0) return NULL;
    int last = (sc->head - 1 + CLIP_MAX_ENTRIES) % CLIP_MAX_ENTRIES;
    return &sc->entries[last];
}

const ClipEntry *sclip_get(const SharedClipboard *sc, int index)
{
    if (index < 0 || index >= sc->count) return NULL;
    int actual = (sc->head - 1 - index + CLIP_MAX_ENTRIES * 2) % CLIP_MAX_ENTRIES;
    return &sc->entries[actual];
}

int sclip_clear(SharedClipboard *sc)
{
    sclip_free(sc);
    return 0;
}
