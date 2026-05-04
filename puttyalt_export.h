/*
 * puttyalt_export.h: Export sessions to various formats.
 *
 * Export saved sessions to SSH config format, JSON, or CSV
 * for backup and migration purposes.
 */

#ifndef PUTTYALT_EXPORT_H
#define PUTTYALT_EXPORT_H

typedef enum {
    EXPORT_SSH_CONFIG,
    EXPORT_JSON,
    EXPORT_CSV
} ExportFormat;

int export_sessions(const char *output_path, ExportFormat format);
int export_single_session(const char *session_name, const char *output_path,
                          ExportFormat format);

#endif /* PUTTYALT_EXPORT_H */
