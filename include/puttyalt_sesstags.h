#ifndef PUTTYALT_SESSTAGS_H
#define PUTTYALT_SESSTAGS_H
#define TAG_MAX_PER_SESSION 8
#define TAG_MAX_LEN 32
#define TAG_MAX_TOTAL 128

typedef struct {
    char tags[TAG_MAX_PER_SESSION][TAG_MAX_LEN];
    int tag_count;
    char session_id[64];
} SessionTags;

typedef struct {
    SessionTags sessions[TAG_MAX_TOTAL];
    int count;
    char all_tags[TAG_MAX_TOTAL][TAG_MAX_LEN];
    int all_tag_count;
} TagStore;

int  tags_init(TagStore *ts);
int  tags_add(TagStore *ts, const char *session_id, const char *tag);
int  tags_remove(TagStore *ts, const char *session_id, const char *tag);
int  tags_filter(const TagStore *ts, const char *tag, char (*results)[64], int max);
int  tags_save(const TagStore *ts, const char *path);
int  tags_load(TagStore *ts, const char *path);
void tags_destroy(TagStore *ts);
#endif
