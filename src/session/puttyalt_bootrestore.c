/* puttyalt_bootrestore.c - PuttyAlt session restore on startup.
 *
 * Records the set of open sessions at shutdown and replays them on the
 * next launch, honoring a restore-enabled flag. Self-contained module.
 */
#include <string.h>
#include <stdio.h>

#define BOOTREST_MAX_SESSIONS 64
#define BOOTREST_NAME_LEN     128

typedef struct BootRestore {
    int  enabled;                                       /* restore on/off */
    int  count;                                         /* recorded sessions */
    int  cursor;                                        /* replay position */
    char names[BOOTREST_MAX_SESSIONS][BOOTREST_NAME_LEN];
} BootRestore;

void bootrest_init(BootRestore *br)
{
    if (!br) return;
    br->enabled = 0;
    br->count = 0;
    br->cursor = 0;
    memset(br->names, 0, sizeof br->names);
}

void bootrest_set_enabled(BootRestore *br, int enabled)
{
    if (!br) return;
    br->enabled = enabled ? 1 : 0;
}

/* Record one open session name. Rejects null/empty/oversized names and
 * silently ignores duplicates. Returns 0 on success, -1 on error. */
int bootrest_record(BootRestore *br, const char *session_name)
{
    int i;
    size_t len;

    if (!br || !session_name) return -1;
    len = strlen(session_name);
    if (len == 0 || len >= BOOTREST_NAME_LEN) return -1;
    if (br->count >= BOOTREST_MAX_SESSIONS) return -1;

    for (i = 0; i < br->count; i++) {
        if (strcmp(br->names[i], session_name) == 0) return 0;
    }
    memcpy(br->names[br->count], session_name, len + 1);
    br->count++;
    return 0;
}

/* Serialize as "enabled\ncount\nname0\nname1\n..." into out (cap bytes).
 * Returns bytes written (excluding NUL) on success, -1 on error/overflow. */
int bootrest_serialize(const BootRestore *br, char *out, int cap)
{
    int i, n, total = 0;

    if (!br || !out || cap <= 0) return -1;

    n = snprintf(out, (size_t)cap, "%d\n%d\n", br->enabled, br->count);
    if (n < 0 || n >= cap) return -1;
    total = n;

    for (i = 0; i < br->count; i++) {
        n = snprintf(out + total, (size_t)(cap - total), "%s\n", br->names[i]);
        if (n < 0 || n >= cap - total) return -1;
        total += n;
    }
    return total;
}

/* Parse text produced by bootrest_serialize. Resets br first.
 * Returns 0 on success, -1 on malformed input. */
int bootrest_parse(BootRestore *br, const char *buf)
{
    int enabled = 0, count = 0, consumed = 0, got;
    const char *p;
    char line[BOOTREST_NAME_LEN];

    if (!br || !buf) return -1;

    if (sscanf(buf, "%d%n", &enabled, &consumed) != 1) return -1;
    p = buf + consumed;
    if (*p != '\n') return -1;
    p++;
    if (sscanf(p, "%d%n", &count, &consumed) != 1) return -1;
    p += consumed;
    if (*p != '\n') return -1;
    p++;
    if (count < 0 || count > BOOTREST_MAX_SESSIONS) return -1;

    bootrest_init(br);
    br->enabled = enabled ? 1 : 0;

    for (got = 0; got < count; got++) {
        const char *nl = strchr(p, '\n');
        size_t len = nl ? (size_t)(nl - p) : strlen(p);
        if (len == 0 || len >= BOOTREST_NAME_LEN) return -1;
        memcpy(line, p, len);
        line[len] = '\0';
        memcpy(br->names[got], line, len + 1);
        br->count++;
        if (!nl) break;
        p = nl + 1;
    }
    if (br->count != count) return -1;
    return 0;
}

/* Pop the next session to restore into out (cap bytes). Yields nothing when
 * disabled. Returns 1 if a name was produced, 0 when exhausted, -1 on error. */
int bootrest_next(BootRestore *br, char *out, int cap)
{
    size_t len;

    if (!br || !out || cap <= 0) return -1;
    if (!br->enabled) return 0;
    if (br->cursor >= br->count) return 0;

    len = strlen(br->names[br->cursor]);
    if (len >= (size_t)cap) return -1;
    memcpy(out, br->names[br->cursor], len + 1);
    br->cursor++;
    return 1;
}
