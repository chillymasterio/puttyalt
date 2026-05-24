/*
 * puttyalt_sftppanel.h: Side-panel SFTP file browser.
 *
 * Provides a split-pane file browser that shares the SSH connection,
 * allowing drag-and-drop file transfers while using the terminal.
 */

#ifndef PUTTYALT_SFTPPANEL_H
#define PUTTYALT_SFTPPANEL_H

#define SFTP_MAX_PATH  4096
#define SFTP_MAX_FILES 2048

typedef enum {
    SFTP_ENTRY_FILE,
    SFTP_ENTRY_DIR,
    SFTP_ENTRY_SYMLINK
} SftpEntryType;

typedef struct SftpEntry {
    char          name[256];
    char          path[SFTP_MAX_PATH];
    SftpEntryType type;
    long          size;
    long          mtime;
    int           permissions;
} SftpEntry;

typedef struct SftpPanel {
    char      cwd[SFTP_MAX_PATH];
    SftpEntry entries[SFTP_MAX_FILES];
    int       count;
    int       selected;     /* cursor position */
    int       scroll_offset;
    int       visible_rows;
    int       panel_width;  /* pixels */
    int       visible;      /* panel shown/hidden */
    int       sort_by;      /* 0=name, 1=size, 2=date */
    int       sort_desc;
    int       show_hidden;
} SftpPanel;

void sftp_panel_init(SftpPanel *sp);
void sftp_panel_set_cwd(SftpPanel *sp, const char *path);
int  sftp_panel_add_entry(SftpPanel *sp, const SftpEntry *entry);
void sftp_panel_clear(SftpPanel *sp);
void sftp_panel_sort(SftpPanel *sp);
void sftp_panel_cursor_up(SftpPanel *sp);
void sftp_panel_cursor_down(SftpPanel *sp);
const SftpEntry *sftp_panel_current(const SftpPanel *sp);
void sftp_panel_toggle_hidden(SftpPanel *sp);
void sftp_panel_toggle_visible(SftpPanel *sp);

#endif /* PUTTYALT_SFTPPANEL_H */
