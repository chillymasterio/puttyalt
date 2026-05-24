/*
 * puttyalt_portable.c: Portable mode configuration path support.
 *
 * When a file named "puttyalt.ini" exists in the current working
 * directory, PuttyAlt runs in portable mode: all configuration
 * is stored in files alongside the binary rather than in the system
 * config directories.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "puttyalt.h"

#ifndef _WIN32
#include <unistd.h>
#include <limits.h>
#include <libgen.h>
#endif

static int portable_mode_detected = -1;
static char portable_dir[4096];

#ifdef _WIN32
#define PATH_SEP '\\'
#else
#define PATH_SEP '/'
#endif

int puttyalt_is_portable(void)
{
    if (portable_mode_detected >= 0)
        return portable_mode_detected;

    portable_dir[0] = '\0';

    /* Check current working directory for portability marker */
    FILE *f = fopen("puttyalt.ini", "r");
    if (f) {
        fclose(f);
        snprintf(portable_dir, sizeof(portable_dir), ".");
        portable_mode_detected = 1;
        return 1;
    }

#ifndef _WIN32
    /* On Unix, also check directory of the executable */
    char linkpath[4096];
    ssize_t len = readlink("/proc/self/exe", linkpath, sizeof(linkpath) - 1);
    if (len > 0) {
        linkpath[len] = '\0';
        char *dir = dirname(linkpath);
        snprintf(portable_dir, sizeof(portable_dir), "%s", dir);

        char ini_path[4096];
        snprintf(ini_path, sizeof(ini_path), "%s/puttyalt.ini", dir);
        f = fopen(ini_path, "r");
        if (f) {
            fclose(f);
            portable_mode_detected = 1;
            return 1;
        }
    }
#endif

    portable_mode_detected = 0;
    return 0;
}

const char *puttyalt_portable_dir(void)
{
    if (!puttyalt_is_portable())
        return NULL;
    return portable_dir;
}

const char *puttyalt_portable_path(const char *filename)
{
    static char path[4096];
    const char *dir = puttyalt_portable_dir();
    if (!dir)
        return NULL;
    snprintf(path, sizeof(path), "%s%c%s", dir, PATH_SEP, filename);
    return path;
}
