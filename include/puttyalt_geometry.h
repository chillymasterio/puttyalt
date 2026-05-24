/*
 * puttyalt_geometry.h: Window geometry save/restore.
 *
 * Remembers the position, size, and maximized state of each session's
 * window so it opens in the same place next time.
 */

#ifndef PUTTYALT_GEOMETRY_H
#define PUTTYALT_GEOMETRY_H

typedef struct WindowGeometry {
    int x, y;
    int width, height;
    int cols, rows;
    int maximized;
} WindowGeometry;

/* Save geometry to config file (INI format) */
int geometry_save(const char *session_name, const WindowGeometry *g,
                  const char *config_dir);

/* Load geometry from config file.  Returns 0 on success. */
int geometry_load(const char *session_name, WindowGeometry *g,
                  const char *config_dir);

/* Clamp geometry to fit within the given screen bounds */
void geometry_clamp(WindowGeometry *g, int screen_w, int screen_h);

#endif /* PUTTYALT_GEOMETRY_H */
