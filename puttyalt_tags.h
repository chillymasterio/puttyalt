#ifndef PUTTYALT_TAGS_H
#define PUTTYALT_TAGS_H

#define TAG_MAX_PER_SESSION  8
#define TAG_MAX_LEN          32
#define TAG_MAX_TOTAL        64

typedef struct SessionTag {
    char name[TAG_MAX_LEN];
    unsigned int color;      /* RGB packed */
} SessionTag;

typedef struct TagRegistry {
    SessionTag tags[TAG_MAX_TOTAL];
    int count;
} TagRegistry;

void tags_init(TagRegistry *reg);
int  tags_create(TagRegistry *reg, const char *name, unsigned int color);
int  tags_delete(TagRegistry *reg, const char *name);
int  tags_find(const TagRegistry *reg, const char *name);
int  tags_rename(TagRegistry *reg, const char *old_name, const char *new_name);
int  tags_set_color(TagRegistry *reg, const char *name, unsigned int color);

#endif
