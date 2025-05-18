#include <string.h>
#include "puttyalt_tags.h"

void tags_init(TagRegistry *reg)
{
    memset(reg, 0, sizeof(*reg));
}

int tags_create(TagRegistry *reg, const char *name, unsigned int color)
{
    if (reg->count >= TAG_MAX_TOTAL) return -1;
    if (tags_find(reg, name) >= 0) return -1;
    SessionTag *t = &reg->tags[reg->count];
    strncpy(t->name, name, TAG_MAX_LEN - 1);
    t->color = color;
    return reg->count++;
}

int tags_delete(TagRegistry *reg, const char *name)
{
    int idx = tags_find(reg, name);
    if (idx < 0) return -1;
    for (int i = idx; i < reg->count - 1; i++)
        reg->tags[i] = reg->tags[i + 1];
    reg->count--;
    return 0;
}

int tags_find(const TagRegistry *reg, const char *name)
{
    for (int i = 0; i < reg->count; i++)
        if (strcmp(reg->tags[i].name, name) == 0) return i;
    return -1;
}

int tags_rename(TagRegistry *reg, const char *old_name, const char *new_name)
{
    int idx = tags_find(reg, old_name);
    if (idx < 0) return -1;
    if (tags_find(reg, new_name) >= 0) return -1;
    strncpy(reg->tags[idx].name, new_name, TAG_MAX_LEN - 1);
    reg->tags[idx].name[TAG_MAX_LEN - 1] = '\0';
    return 0;
}

int tags_set_color(TagRegistry *reg, const char *name, unsigned int color)
{
    int idx = tags_find(reg, name);
    if (idx < 0) return -1;
    reg->tags[idx].color = color;
    return 0;
}
