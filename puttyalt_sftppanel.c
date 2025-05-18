/*
 * puttyalt_sftppanel.c: SFTP panel implementation.
 */

#include "puttyalt_sftppanel.h"
#include <string.h>
#include <stdlib.h>

void sftp_panel_init(SftpPanel *sp)
{
    memset(sp, 0, sizeof(*sp));
    snprintf(sp->cwd, sizeof(sp->cwd), "/");
    sp->visible = 0;
    sp->panel_width = 280;
    sp->visible_rows = 30;
    sp->show_hidden = 0;
}

void sftp_panel_set_cwd(SftpPanel *sp, const char *path)
{
    if (path)
        snprintf(sp->cwd, sizeof(sp->cwd), "%s", path);
}

int sftp_panel_add_entry(SftpPanel *sp, const SftpEntry *entry)
{
    if (sp->count >= SFTP_MAX_FILES)
        return -1;
    if (!sp->show_hidden && entry->name[0] == '.')
        return -1;
    memcpy(&sp->entries[sp->count], entry, sizeof(SftpEntry));
    return sp->count++;
}

void sftp_panel_clear(SftpPanel *sp)
{
    sp->count = 0;
    sp->selected = 0;
    sp->scroll_offset = 0;
}

static int cmp_name(const void *a, const void *b)
{
    const SftpEntry *ea = (const SftpEntry *)a;
    const SftpEntry *eb = (const SftpEntry *)b;
    /* Dirs first */
    if (ea->type == SFTP_ENTRY_DIR && eb->type != SFTP_ENTRY_DIR) return -1;
    if (ea->type != SFTP_ENTRY_DIR && eb->type == SFTP_ENTRY_DIR) return 1;
    return strcmp(ea->name, eb->name);
}

static int cmp_size(const void *a, const void *b)
{
    const SftpEntry *ea = (const SftpEntry *)a;
    const SftpEntry *eb = (const SftpEntry *)b;
    if (ea->type == SFTP_ENTRY_DIR && eb->type != SFTP_ENTRY_DIR) return -1;
    if (ea->type != SFTP_ENTRY_DIR && eb->type == SFTP_ENTRY_DIR) return 1;
    return (eb->size > ea->size) - (eb->size < ea->size);
}

static int cmp_date(const void *a, const void *b)
{
    const SftpEntry *ea = (const SftpEntry *)a;
    const SftpEntry *eb = (const SftpEntry *)b;
    return (int)(eb->mtime - ea->mtime);
}

void sftp_panel_sort(SftpPanel *sp)
{
    if (sp->count <= 1)
        return; /* nothing to sort, avoid qsort on empty array */

    int (*cmp)(const void *, const void *) = cmp_name;
    if (sp->sort_by == 1) cmp = cmp_size;
    else if (sp->sort_by == 2) cmp = cmp_date;
    qsort(sp->entries, sp->count, sizeof(SftpEntry), cmp);
}

void sftp_panel_cursor_up(SftpPanel *sp)
{
    if (sp->selected > 0) {
        sp->selected--;
        if (sp->selected < sp->scroll_offset)
            sp->scroll_offset = sp->selected;
    }
}

void sftp_panel_cursor_down(SftpPanel *sp)
{
    if (sp->selected < sp->count - 1) {
        sp->selected++;
        if (sp->selected >= sp->scroll_offset + sp->visible_rows)
            sp->scroll_offset = sp->selected - sp->visible_rows + 1;
    }
}

const SftpEntry *sftp_panel_current(const SftpPanel *sp)
{
    if (sp->count == 0 || sp->selected < 0 || sp->selected >= sp->count)
        return NULL;
    return &sp->entries[sp->selected];
}

void sftp_panel_toggle_hidden(SftpPanel *sp)
{
    sp->show_hidden = !sp->show_hidden;
}

void sftp_panel_toggle_visible(SftpPanel *sp)
{
    sp->visible = !sp->visible;
}
