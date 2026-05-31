/* puttyalt_asciicast.c - record terminal output to asciinema v2 .cast files */
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define ACAST_MAX_FRAMES 4096
#define ACAST_DATA_MAX   512
#define ACAST_TITLE_MAX  128

typedef struct {
    uint64_t at_ms;                 /* absolute timestamp of this frame */
    uint32_t len;                   /* bytes used in data */
    char     data[ACAST_DATA_MAX];  /* raw output chunk */
} AcastFrame;

typedef struct {
    AcastFrame frames[ACAST_MAX_FRAMES];
    uint32_t   frame_count;         /* number of recorded frames */
    uint64_t   start_ms;            /* session start timestamp */
    uint16_t   width, height;       /* terminal geometry */
    int        recording;           /* 1 while capturing */
    char       title[ACAST_TITLE_MAX];
} AcastRecorder;

int acast_init(AcastRecorder *r, uint16_t width, uint16_t height, const char *title)
{
    if (!r) return -1;
    memset(r, 0, sizeof(*r));
    r->width = width ? width : 80;
    r->height = height ? height : 24;
    if (title)
        snprintf(r->title, sizeof(r->title), "%s", title);
    return 0;
}

void acast_free(AcastRecorder *r)
{
    if (r) memset(r, 0, sizeof(*r));
}

int acast_start(AcastRecorder *r, uint64_t now_ms)
{
    if (!r || r->recording) return -1;
    r->frame_count = 0;
    r->start_ms = now_ms;
    r->recording = 1;
    return 0;
}

int acast_stop(AcastRecorder *r)
{
    if (!r) return -1;
    r->recording = 0;
    return 0;
}

int acast_record(AcastRecorder *r, uint64_t now_ms, const char *data, uint32_t len)
{
    AcastFrame *f;
    if (!r || !data || !r->recording) return -1;
    if (r->frame_count >= ACAST_MAX_FRAMES) return -1;
    if (len > ACAST_DATA_MAX) len = ACAST_DATA_MAX;
    if (now_ms < r->start_ms) now_ms = r->start_ms;
    f = &r->frames[r->frame_count];
    f->at_ms = now_ms;
    f->len = len;
    memcpy(f->data, data, len);
    r->frame_count++;
    return 0;
}

/* Escape in[0..in_len) into out for JSON string context. Returns bytes or -1. */
int acast_json_escape(const char *in, uint32_t in_len, char *out, uint32_t out_sz)
{
    uint32_t i, o = 0;
    if (!in || !out || out_sz == 0) return -1;
    for (i = 0; i < in_len; i++) {
        unsigned char c = (unsigned char)in[i];
        char esc[8];
        uint32_t n;
        switch (c) {
            case '"':  memcpy(esc, "\\\"", 2); n = 2; break;
            case '\\': memcpy(esc, "\\\\", 2); n = 2; break;
            case '\n': memcpy(esc, "\\n", 2);  n = 2; break;
            case '\r': memcpy(esc, "\\r", 2);  n = 2; break;
            case '\t': memcpy(esc, "\\t", 2);  n = 2; break;
            case '\b': memcpy(esc, "\\b", 2);  n = 2; break;
            case '\f': memcpy(esc, "\\f", 2);  n = 2; break;
            default:
                if (c < 0x20)
                    n = (uint32_t)snprintf(esc, sizeof(esc), "\\u%04x", c);
                else { esc[0] = (char)c; n = 1; }
                break;
        }
        if (o + n >= out_sz) return -1;
        memcpy(out + o, esc, n);
        o += n;
    }
    out[o] = '\0';
    return (int)o;
}

/* Total recorded session duration in seconds. */
double acast_duration(const AcastRecorder *r)
{
    if (!r || r->frame_count == 0) return 0.0;
    return (double)(r->frames[r->frame_count - 1].at_ms - r->start_ms) / 1000.0;
}

int acast_export(const AcastRecorder *r, const char *path)
{
    FILE *fp;
    uint32_t i;
    if (!r || !path) return -1;
    fp = fopen(path, "wb");
    if (!fp) return -1;
    if (fprintf(fp, "{\"version\": 2, \"width\": %u, \"height\": %u",
                (unsigned)r->width, (unsigned)r->height) < 0)
        goto fail;
    if (r->title[0]) {
        char esc[ACAST_TITLE_MAX * 6 + 1];
        if (acast_json_escape(r->title, (uint32_t)strlen(r->title),
                              esc, sizeof(esc)) >= 0)
            fprintf(fp, ", \"title\": \"%s\"", esc);
    }
    if (fprintf(fp, "}\n") < 0) goto fail;
    for (i = 0; i < r->frame_count; i++) {
        const AcastFrame *f = &r->frames[i];
        char esc[ACAST_DATA_MAX * 6 + 1];
        double t = (double)(f->at_ms - r->start_ms) / 1000.0;
        if (acast_json_escape(f->data, f->len, esc, sizeof(esc)) < 0) goto fail;
        if (fprintf(fp, "[%.6f, \"o\", \"%s\"]\n", t, esc) < 0) goto fail;
    }
    fclose(fp);
    return 0;
fail:
    fclose(fp);
    return -1;
}
