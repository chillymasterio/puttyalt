#include "puttyalt_autobackup.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void backup_init(AutoBackup *ab)
{
    memset(ab, 0, sizeof(*ab));
    ab->max_backups = 50;
    ab->auto_enabled = 1;
    ab->interval_minutes = 60;
}

int backup_set_dir(AutoBackup *ab, const char *dir)
{
    snprintf(ab->backup_dir, BK_MAX_PATH, "%s", dir);
    return 0;
}

int backup_create(AutoBackup *ab, const char *src, BKType type)
{
    if (ab->count >= BK_MAX_BACKUPS) backup_prune(ab);
    if (ab->count >= BK_MAX_BACKUPS) return -1;

    FILE *sf = fopen(src, "rb");
    if (!sf) return -1;

    /* Determine size */
    fseek(sf, 0, SEEK_END);
    long size = ftell(sf);
    fseek(sf, 0, SEEK_SET);

    /* Generate backup filename */
    long now = (long)time(NULL);
    char bkpath[BK_MAX_PATH];
    static const char *type_names[] = {"config","sessions","creds","keys","full"};
    snprintf(bkpath, BK_MAX_PATH, "%s/%s_%ld.bak",
             ab->backup_dir, type_names[type], now);

    /* Copy file */
    FILE *df = fopen(bkpath, "wb");
    if (!df) { fclose(sf); return -1; }
    char buf[4096];
    size_t n;
    unsigned long hash = 5381;
    while ((n = fread(buf, 1, sizeof(buf), sf)) > 0) {
        fwrite(buf, 1, n, df);
        for (size_t i = 0; i < n; i++)
            hash = hash * 33 + (unsigned char)buf[i];
    }
    fclose(sf);
    fclose(df);

    BackupEntry *e = &ab->entries[ab->count];
    memset(e, 0, sizeof(*e));
    snprintf(e->path, BK_MAX_PATH, "%s", bkpath);
    e->type = type;
    e->timestamp = now;
    e->size = size;
    snprintf(e->checksum, sizeof(e->checksum), "%016lx", hash);
    ab->count++;
    ab->last_backup = now;
    return ab->count - 1;
}

int backup_restore(const AutoBackup *ab, int index, const char *dest)
{
    if (index < 0 || index >= ab->count) return -1;
    FILE *sf = fopen(ab->entries[index].path, "rb");
    if (!sf) return -1;
    FILE *df = fopen(dest, "wb");
    if (!df) { fclose(sf); return -1; }
    char buf[4096];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), sf)) > 0)
        fwrite(buf, 1, n, df);
    fclose(sf);
    fclose(df);
    return 0;
}

int backup_remove(AutoBackup *ab, int index)
{
    if (index < 0 || index >= ab->count) return -1;
    remove(ab->entries[index].path);
    for (int i = index; i < ab->count - 1; i++)
        ab->entries[i] = ab->entries[i + 1];
    ab->count--;
    return 0;
}

int backup_prune(AutoBackup *ab)
{
    int removed = 0;
    while (ab->count > ab->max_backups) {
        /* Remove oldest */
        int oldest = 0;
        for (int i = 1; i < ab->count; i++)
            if (ab->entries[i].timestamp < ab->entries[oldest].timestamp)
                oldest = i;
        backup_remove(ab, oldest);
        removed++;
    }
    return removed;
}

int backup_check_needed(const AutoBackup *ab)
{
    if (!ab->auto_enabled) return 0;
    long now = (long)time(NULL);
    return (now - ab->last_backup) >= (ab->interval_minutes * 60);
}

int backup_load_index(AutoBackup *ab)
{
    char path[BK_MAX_PATH];
    snprintf(path, BK_MAX_PATH, "%s/backup.idx", ab->backup_dir);
    FILE *f = fopen(path, "r");
    char line[1024];
    if (!f) return -1;
    ab->count = 0;
    while (fgets(line, sizeof(line), f) && ab->count < BK_MAX_BACKUPS) {
        BackupEntry *e = &ab->entries[ab->count];
        if (sscanf(line, "%511s %d %ld %ld %64s",
                   e->path, (int*)&e->type, &e->timestamp, &e->size, e->checksum) == 5)
            ab->count++;
    }
    fclose(f);
    return ab->count;
}

int backup_save_index(const AutoBackup *ab)
{
    char path[BK_MAX_PATH];
    snprintf(path, BK_MAX_PATH, "%s/backup.idx", ab->backup_dir);
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < ab->count; i++) {
        const BackupEntry *e = &ab->entries[i];
        fprintf(f, "%s %d %ld %ld %s\n",
                e->path, e->type, e->timestamp, e->size, e->checksum);
    }
    fclose(f);
    return 0;
}

int backup_list(const AutoBackup *ab, BKType type, int *indices, int max)
{
    int n = 0;
    for (int i = 0; i < ab->count && n < max; i++)
        if (ab->entries[i].type == type || type == BK_FULL)
            indices[n++] = i;
    return n;
}
