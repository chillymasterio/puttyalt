/* PuttyAlt Plugin API v2 - INI-style manifest parser/validator (sandbox). */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define PM_NAME_LEN   64
#define PM_VER_LEN    32
#define PM_ENTRY_LEN  128
#define PM_CAP_LEN    32
#define PM_MAX_CAPS   16

typedef struct {
    char name[PM_NAME_LEN];
    char version[PM_VER_LEN];
    char entry[PM_ENTRY_LEN];
    char caps[PM_MAX_CAPS][PM_CAP_LEN];
    int  cap_count;
} PMManifest;

void pmanifest_reset(PMManifest *m)
{
    if (m) memset(m, 0, sizeof(*m));
}

static void pm_copy_trim(char *dst, int cap, const char *src, int n)
{
    while (n > 0 && (src[0] == ' ' || src[0] == '\t')) { src++; n--; }
    while (n > 0 && (src[n - 1] == ' ' || src[n - 1] == '\t' ||
                     src[n - 1] == '\r')) n--;
    if (n >= cap) n = cap - 1;
    if (n < 0) n = 0;
    memcpy(dst, src, (size_t)n);
    dst[n] = '\0';
}

int pmanifest_parse(const char *text, PMManifest *out)
{
    if (!text || !out) return -1;
    pmanifest_reset(out);
    const char *p = text;
    while (*p) {
        const char *eol = p;
        while (*eol && *eol != '\n') eol++;
        int linelen = (int)(eol - p);
        char key[32], val[256];
        const char *eq = memchr(p, '=', (size_t)linelen);
        if (p[0] != '#' && p[0] != ';' && eq) {
            pm_copy_trim(key, sizeof(key), p, (int)(eq - p));
            pm_copy_trim(val, sizeof(val), eq + 1, (int)(eol - (eq + 1)));
            if (!strcmp(key, "name"))
                strncpy(out->name, val, PM_NAME_LEN - 1);
            else if (!strcmp(key, "version"))
                strncpy(out->version, val, PM_VER_LEN - 1);
            else if (!strcmp(key, "entry"))
                strncpy(out->entry, val, PM_ENTRY_LEN - 1);
            else if (!strcmp(key, "capabilities") || !strcmp(key, "caps")) {
                char *tok = val;
                while (*tok && out->cap_count < PM_MAX_CAPS) {
                    char *comma = strchr(tok, ',');
                    int tn = comma ? (int)(comma - tok) : (int)strlen(tok);
                    char tmp[PM_CAP_LEN];
                    pm_copy_trim(tmp, sizeof(tmp), tok, tn);
                    if (tmp[0])
                        strncpy(out->caps[out->cap_count++], tmp, PM_CAP_LEN - 1);
                    if (!comma) break;
                    tok = comma + 1;
                }
            }
        }
        p = (*eol == '\n') ? eol + 1 : eol;
    }
    return 0;
}

int pmanifest_validate(const PMManifest *m, char *err, int errlen)
{
    if (!m) return -1;
    const char *msg = NULL;
    if (m->name[0] == '\0') msg = "missing name";
    else if (m->version[0] == '\0') msg = "missing version";
    else if (m->entry[0] == '\0') msg = "missing entry";
    else if (m->cap_count < 0 || m->cap_count > PM_MAX_CAPS)
        msg = "capability count out of range";
    if (msg) {
        if (err && errlen > 0) snprintf(err, (size_t)errlen, "%s", msg);
        return -1;
    }
    if (err && errlen > 0) err[0] = '\0';
    return 0;
}

const char *pmanifest_get_field(const PMManifest *m, const char *key)
{
    if (!m || !key) return NULL;
    if (!strcmp(key, "name"))    return m->name;
    if (!strcmp(key, "version")) return m->version;
    if (!strcmp(key, "entry"))   return m->entry;
    return NULL;
}

int pmanifest_caps_csv(const PMManifest *m, char *buf, int len)
{
    if (!m || !buf || len <= 0) return -1;
    buf[0] = '\0';
    int off = 0;
    for (int i = 0; i < m->cap_count; i++) {
        int n = snprintf(buf + off, (size_t)(len - off), "%s%s",
                         i ? "," : "", m->caps[i]);
        if (n < 0 || n >= len - off) { buf[off] = '\0'; return -1; }
        off += n;
    }
    return off;
}
