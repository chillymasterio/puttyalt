/*
 * puttyalt_geometry.c: Save and restore window position and size.
 *
 * PuTTY always opens windows at the default position. PuttyAlt
 * remembers where you put each session window and restores it on
 * next launch. Geometry is stored per-session in a simple text file.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y;          /* window position */
    int width, height; /* window size in pixels */
    int cols, rows;    /* terminal dimensions */
    int maximized;     /* 1 if window was maximized */
} WindowGeometry;

/*
 * Save window geometry for a named session.
 * File format: x y width height cols rows maximized
 */
int geometry_save(const char *dir, const char *session_name,
                  const WindowGeometry *geom)
{
    char path[4096];
    char safe_name[256];
    size_t i, j;

    /* Sanitise session name for use as filename */
    for (i = 0, j = 0; session_name[i] && j < sizeof(safe_name) - 1; i++) {
        char c = session_name[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.') {
            safe_name[j++] = c;
        } else {
            safe_name[j++] = '_';
        }
    }
    safe_name[j] = '\0';

    snprintf(path, sizeof(path), "%s/geometry_%s.dat", dir, safe_name);

    FILE *f = fopen(path, "w");
    if (!f)
        return -1;

    fprintf(f, "%d %d %d %d %d %d %d\n",
            geom->x, geom->y,
            geom->width, geom->height,
            geom->cols, geom->rows,
            geom->maximized);

    fclose(f);
    return 0;
}

/*
 * Load window geometry for a named session.
 * Returns 0 on success, -1 if no saved geometry exists.
 */
int geometry_load(const char *dir, const char *session_name,
                  WindowGeometry *geom)
{
    char path[4096];
    char safe_name[256];
    size_t i, j;

    for (i = 0, j = 0; session_name[i] && j < sizeof(safe_name) - 1; i++) {
        char c = session_name[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
            (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.') {
            safe_name[j++] = c;
        } else {
            safe_name[j++] = '_';
        }
    }
    safe_name[j] = '\0';

    snprintf(path, sizeof(path), "%s/geometry_%s.dat", dir, safe_name);

    FILE *f = fopen(path, "r");
    if (!f)
        return -1;

    memset(geom, 0, sizeof(*geom));
    int n = fscanf(f, "%d %d %d %d %d %d %d",
                   &geom->x, &geom->y,
                   &geom->width, &geom->height,
                   &geom->cols, &geom->rows,
                   &geom->maximized);
    fclose(f);

    return (n == 7) ? 0 : -1;
}

/*
 * Clamp geometry to screen bounds.
 * Ensures the window title bar is always reachable.
 */
void geometry_clamp(WindowGeometry *geom,
                    int screen_w, int screen_h)
{
    /* Ensure at least 100px of the window is visible */
    if (geom->x + geom->width < 100)
        geom->x = 0;
    if (geom->y < 0)
        geom->y = 0;
    if (geom->x > screen_w - 100)
        geom->x = screen_w - 100;
    if (geom->y > screen_h - 50)
        geom->y = screen_h - 50;

    /* Minimum reasonable terminal size */
    if (geom->cols < 40) geom->cols = 80;
    if (geom->rows < 10) geom->rows = 24;
}
