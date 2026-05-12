#include "puttyalt_inputfilter.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void ifpipe_init(FilterPipeline *fp)
{
    memset(fp, 0, sizeof(*fp));
    fp->global_enabled = 1;
}

int ifpipe_add(FilterPipeline *fp, const char *name, const char *pattern,
               const char *replacement, IFDirection dir, IFAction action)
{
    if (fp->count >= IF_MAX_FILTERS) return -1;
    InputFilter *f = &fp->filters[fp->count];
    memset(f, 0, sizeof(*f));
    snprintf(f->name, IF_MAX_NAME, "%s", name);
    snprintf(f->pattern, IF_MAX_PATTERN, "%s", pattern);
    if (replacement) snprintf(f->replacement, IF_MAX_REPLACE, "%s", replacement);
    f->direction = dir;
    f->action = action;
    f->enabled = 1;
    return fp->count++;
}

int ifpipe_remove(FilterPipeline *fp, int index)
{
    if (index < 0 || index >= fp->count) return -1;
    for (int i = index; i < fp->count - 1; i++)
        fp->filters[i] = fp->filters[i + 1];
    fp->count--;
    return 0;
}

int ifpipe_enable(FilterPipeline *fp, int index, int enable)
{
    if (index < 0 || index >= fp->count) return -1;
    fp->filters[index].enabled = enable;
    return 0;
}

static char *strcasestr_impl(const char *haystack, const char *needle)
{
    size_t nlen = strlen(needle);
    if (nlen == 0) return (char *)haystack;
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) == tolower((unsigned char)*needle)) {
            size_t i;
            for (i = 1; i < nlen; i++) {
                if (tolower((unsigned char)haystack[i]) !=
                    tolower((unsigned char)needle[i]))
                    break;
            }
            if (i == nlen) return (char *)haystack;
        }
    }
    return NULL;
}

static int apply_replace(char *buf, int bufsz, const char *pattern,
                         const char *replacement, int ci)
{
    char tmp[4096];
    int changes = 0;
    char *src = buf;
    int pos = 0;
    size_t plen = strlen(pattern);
    size_t rlen = strlen(replacement);

    while (*src && pos < (int)sizeof(tmp) - 1) {
        char *match = ci ? strcasestr_impl(src, pattern) : strstr(src, pattern);
        if (!match) {
            size_t rem = strlen(src);
            if (pos + (int)rem >= (int)sizeof(tmp)) break;
            memcpy(tmp + pos, src, rem);
            pos += (int)rem;
            break;
        }
        size_t prefix = (size_t)(match - src);
        if (pos + (int)prefix + (int)rlen >= (int)sizeof(tmp)) break;
        memcpy(tmp + pos, src, prefix);
        pos += (int)prefix;
        memcpy(tmp + pos, replacement, rlen);
        pos += (int)rlen;
        src = match + plen;
        changes++;
    }
    tmp[pos] = '\0';
    if (changes > 0) snprintf(buf, bufsz, "%s", tmp);
    return changes;
}

int ifpipe_process(FilterPipeline *fp, char *buf, int bufsz, IFDirection dir)
{
    if (!fp->global_enabled) return 0;
    int total = 0;
    for (int i = 0; i < fp->count; i++) {
        InputFilter *f = &fp->filters[i];
        if (!f->enabled) continue;
        if (f->direction != IF_BOTH && f->direction != dir) continue;

        char *match = f->case_insensitive ?
            strcasestr_impl(buf, f->pattern) : strstr(buf, f->pattern);
        if (!match) continue;
        f->match_count++;

        switch (f->action) {
        case IF_REPLACE:
            total += apply_replace(buf, bufsz, f->pattern,
                                   f->replacement, f->case_insensitive);
            break;
        case IF_STRIP:
            total += apply_replace(buf, bufsz, f->pattern, "",
                                   f->case_insensitive);
            break;
        case IF_BLOCK:
            buf[0] = '\0';
            total++;
            return total;
        default:
            total++;
            break;
        }
    }
    return total;
}

int ifpipe_load(FilterPipeline *fp, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return -1;
    ifpipe_init(fp);
    InputFilter *cur = NULL;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (strcmp(line, "[filter]") == 0) {
            if (fp->count >= IF_MAX_FILTERS) break;
            cur = &fp->filters[fp->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            continue;
        }
        if (!cur) continue;
        if (strncmp(line, "name=", 5) == 0)
            snprintf(cur->name, IF_MAX_NAME, "%s", line + 5);
        else if (strncmp(line, "pattern=", 8) == 0)
            snprintf(cur->pattern, IF_MAX_PATTERN, "%s", line + 8);
        else if (strncmp(line, "replace=", 8) == 0)
            snprintf(cur->replacement, IF_MAX_REPLACE, "%s", line + 8);
        else if (strncmp(line, "dir=", 4) == 0)
            cur->direction = atoi(line + 4);
        else if (strncmp(line, "action=", 7) == 0)
            cur->action = atoi(line + 7);
    }
    fclose(f);
    return 0;
}

int ifpipe_save(const FilterPipeline *fp, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < fp->count; i++) {
        const InputFilter *fl = &fp->filters[i];
        fprintf(f, "[filter]\nname=%s\npattern=%s\nreplace=%s\n"
                   "dir=%d\naction=%d\n\n",
                fl->name, fl->pattern, fl->replacement,
                fl->direction, fl->action);
    }
    fclose(f);
    return 0;
}

void ifpipe_reset_stats(FilterPipeline *fp)
{
    for (int i = 0; i < fp->count; i++)
        fp->filters[i].match_count = 0;
}
