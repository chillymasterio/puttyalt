/*
 * puttyalt_portable.c: Portable mode configuration path support.
 *
 * When a file named "puttyalt.ini" exists in the same directory as
 * the executable, PuttyAlt runs in portable mode: all configuration
 * is stored in files alongside the binary rather than in the system
 * registry or ~/.putty.
 *
 * This allows running PuttyAlt from a USB drive with no host
 * system modifications.
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

#ifdef _WIN32
#include <windows.h>
#endif

static int portable_mode_detected = -1;  /* -1 = not checked yet */
static char portable_dir[4096];

/*
 * Detect whether we're in portable mode by checking for
 * "puttyalt.ini" next to the executable.
 */
int puttyalt_is_portable(void)
{
    if (portable_mode_detected >= 0)
        return portable_mode_detected;

    portable_dir[0] = '\0';

#ifdef _WIN32
    {
        char exepath[MAX_PATH];
        DWORD len = GetModuleFileNameA(NULL, exepath, sizeof(exepath));
        if (len > 0 && len < sizeof(exepath)) {
            /* Strip filename to get directory */
            char *slash = strrchr(exepath, '\\');
            if (slash) *slash = '\0';
            snprintf(portable_dir, sizeof(portable_dir), "%s", exepath);
        }
    }
#else
    {
        char linkpath[4096];
        ssize_t len = readlink("/proc/self/exe", linkpath, sizeof(linkpath) - 1);
        if (len > 0) {
            linkpath[len] = '\0';
            char *dir = dirname(linkpath);
            snprintf(portable_dir, sizeof(portable_dir), "%s", dir);
        }
    }
#endif

    if (portable_dir[0] != '\0') {
        char ini_path[4096];
        snprintf(ini_path, sizeof(ini_path), "%s%cputtyalt.ini",
                 portable_dir,
#ifdef _WIN32
                 '\\'
#else
                 '/'
#endif
                 );
        FILE *f = fopen(ini_path, "r");
        if (f) {
            fclose(f);
            portable_mode_detected = 1;
            return 1;
        }
    }

    portable_mode_detected = 0;
    return 0;
}

/*
 * Return the portable configuration directory, or NULL if not
 * in portable mode.
 */
const char *puttyalt_portable_dir(void)
{
    if (!puttyalt_is_portable())
        return NULL;
    return portable_dir;
}

/*
 * Build a path to a file within the portable configuration
 * directory.  Returns a pointer to a static buffer.
 */
const char *puttyalt_portable_path(const char *filename)
{
    static char path[4096];
    const char *dir = puttyalt_portable_dir();
    if (!dir)
        return NULL;
    snprintf(path, sizeof(path), "%s%c%s", dir,
#ifdef _WIN32
             '\\'
#else
             '/'
#endif
             , filename);
    return path;
}
