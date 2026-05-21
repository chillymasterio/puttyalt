#include "puttyalt_sesstags.h"
#include <string.h>
#include <stdio.h>

int tags_init(TagStore *ts) { memset(ts, 0, sizeof(*ts)); return 0; }

static SessionTags *find_session(TagStore *ts, const char *id)
{
    for (int i = 0; i < ts->count; i++)
        if (strcmp(ts->sessions[i].session_id, id) == 0)
            return &ts->sessions[i];
    if (ts->count >= TAG_MAX_TOTAL) return NULL;
    SessionTags *s = &ts->sessions[ts->count++];
    memset(s, 0, sizeof(*s));
    snprintf(s->session_id, 64, "%s", id);
    return s;
}

int tags_add(TagStore *ts, const char *session_id, const char *tag)
{
    SessionTags *s = find_session(ts, session_id);
    if (!s || s->tag_count >= TAG_MAX_PER_SESSION) return -1;
    snprintf(s->tags[s->tag_count++], TAG_MAX_LEN, "%s", tag);
    /* Track unique tags */
    for (int i = 0; i < ts->all_tag_count; i++)
        if (strcmp(ts->all_tags[i], tag) == 0) return 0;
    if (ts->all_tag_count < TAG_MAX_TOTAL)
        snprintf(ts->all_tags[ts->all_tag_count++], TAG_MAX_LEN, "%s", tag);
    return 0;
}

int tags_remove(TagStore *ts, const char *session_id, const char *tag)
{
    SessionTags *s = find_session(ts, session_id);
    if (!s) return -1;
    for (int i = 0; i < s->tag_count; i++) {
        if (strcmp(s->tags[i], tag) == 0) {
            for (int j = i; j < s->tag_count - 1; j++)
                memcpy(s->tags[j], s->tags[j+1], TAG_MAX_LEN);
            s->tag_count--;
            return 0;
        }
    }
    return -1;
}

int tags_filter(const TagStore *ts, const char *tag, char (*results)[64], int max)
{
    int found = 0;
    for (int i = 0; i < ts->count && found < max; i++) {
        for (int j = 0; j < ts->sessions[i].tag_count; j++) {
            if (strcmp(ts->sessions[i].tags[j], tag) == 0) {
                memcpy(results[found++], ts->sessions[i].session_id, 64);
                break;
            }
        }
    }
    return found;
}

int tags_save(const TagStore *ts, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ts->count; i++) {
        const SessionTags *s = &ts->sessions[i];
        fprintf(f, "%s:", s->session_id);
        for (int j = 0; j < s->tag_count; j++)
            fprintf(f, "%s%s", s->tags[j], j < s->tag_count - 1 ? "," : "");
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}

int tags_load(TagStore *ts, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char *colon = strchr(line, ':');
        if (!colon) continue;
        *colon++ = '\0';
        size_t len = strlen(colon);
        while (len > 0 && (colon[len-1] == '\n' || colon[len-1] == '\r'))
            colon[--len] = '\0';
        char *tok = colon;
        while (*tok) {
            char *comma = strchr(tok, ',');
            if (comma) *comma = '\0';
            if (*tok) tags_add(ts, line, tok);
            if (comma) tok = comma + 1; else break;
        }
    }
    fclose(f);
    return 0;
}

void tags_destroy(TagStore *ts) { memset(ts, 0, sizeof(*ts)); }
