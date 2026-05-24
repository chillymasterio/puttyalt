/*
 * puttyalt_timeout.c: Connection and idle timeout implementation.
 */

#include "puttyalt_timeout.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void timeout_config_init(TimeoutConfig *tc)
{
    memset(tc, 0, sizeof(*tc));
    tc->connect_timeout_s = 30;
    tc->idle_timeout_s = 0;       /* disabled */
    tc->warn_before_idle = 60;    /* warn 60s before */
    tc->keepalive_interval = 0;   /* use PuTTY default */
}

int timeout_check_idle(const TimeoutConfig *tc, long last_activity, long now)
{
    if (tc->idle_timeout_s <= 0)
        return -1;

    long elapsed = now - last_activity;
    long remaining = tc->idle_timeout_s - elapsed;

    if (remaining <= 0)
        return 0;
    return (int)remaining;
}

int timeout_should_warn(const TimeoutConfig *tc, long last_activity, long now)
{
    if (tc->idle_timeout_s <= 0 || tc->warn_before_idle <= 0)
        return 0;

    long elapsed = now - last_activity;
    long warn_at = tc->idle_timeout_s - tc->warn_before_idle;

    if (warn_at < 0)
        warn_at = 0;

    return (elapsed >= warn_at && elapsed < tc->idle_timeout_s);
}

int timeout_load(TimeoutConfig *tc, const char *path, const char *session)
{
    FILE *f = fopen(path, "r");
    char line[512];
    int in_section = 0;

    if (!f) return -1;

    timeout_config_init(tc);

    char section_header[256];
    snprintf(section_header, sizeof(section_header), "[%s]", session);

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (line[0] == '[') {
            in_section = (strcmp(line, section_header) == 0);
            continue;
        }
        if (!in_section)
            continue;

        if (strncmp(line, "connect_timeout=", 16) == 0)
            tc->connect_timeout_s = atoi(line + 16);
        else if (strncmp(line, "idle_timeout=", 13) == 0)
            tc->idle_timeout_s = atoi(line + 13);
        else if (strncmp(line, "warn_before_idle=", 17) == 0)
            tc->warn_before_idle = atoi(line + 17);
        else if (strncmp(line, "keepalive=", 10) == 0)
            tc->keepalive_interval = atoi(line + 10);
    }

    fclose(f);
    return 0;
}

int timeout_save(const TimeoutConfig *tc, const char *path, const char *session)
{
    /* Append-style: writes a new section block.
     * A proper implementation would do in-place editing. */
    FILE *f = fopen(path, "a");
    if (!f) return -1;

    fprintf(f, "[%s]\n", session);
    fprintf(f, "connect_timeout=%d\n", tc->connect_timeout_s);
    fprintf(f, "idle_timeout=%d\n", tc->idle_timeout_s);
    fprintf(f, "warn_before_idle=%d\n", tc->warn_before_idle);
    fprintf(f, "keepalive=%d\n", tc->keepalive_interval);
    fprintf(f, "\n");

    fclose(f);
    return 0;
}
