/*
 * puttyalt_portable.h: Portable mode — run from USB with no registry.
 *
 * When "puttyalt.ini" exists beside the executable, all config is
 * stored in files in that directory instead of the system registry.
 */

#ifndef PUTTYALT_PORTABLE_H
#define PUTTYALT_PORTABLE_H

/* Check if running in portable mode */
int puttyalt_is_portable(void);

/* Get the portable config directory, or NULL if not portable */
const char *puttyalt_portable_dir(void);

/* Build a path relative to the portable directory.
 * Returns pointer to a static buffer, or NULL if not portable. */
const char *puttyalt_portable_path(const char *filename);

#endif /* PUTTYALT_PORTABLE_H */
