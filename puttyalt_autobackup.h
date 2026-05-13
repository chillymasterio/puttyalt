#ifndef PUTTYALT_AUTOBACKUP_H
#define PUTTYALT_AUTOBACKUP_H

#define BK_MAX_BACKUPS  128
#define BK_MAX_PATH     512

typedef enum {
    BK_CONFIG = 0,
    BK_SESSIONS,
    BK_CREDENTIALS,
    BK_KEYS,
    BK_FULL
} BKType;

typedef struct {
    char    path[BK_MAX_PATH];
    BKType  type;
    long    timestamp;
    long    size;
    char    checksum[65];
} BackupEntry;

typedef struct {
    BackupEntry entries[BK_MAX_BACKUPS];
    int         count;
    char        backup_dir[BK_MAX_PATH];
    int         max_backups;
    int         auto_enabled;
    int         interval_minutes;
    long        last_backup;
} AutoBackup;

void backup_init(AutoBackup *ab);
int  backup_set_dir(AutoBackup *ab, const char *dir);
int  backup_create(AutoBackup *ab, const char *src, BKType type);
int  backup_restore(const AutoBackup *ab, int index, const char *dest);
int  backup_remove(AutoBackup *ab, int index);
int  backup_prune(AutoBackup *ab);
int  backup_check_needed(const AutoBackup *ab);
int  backup_load_index(AutoBackup *ab);
int  backup_save_index(const AutoBackup *ab);
int  backup_list(const AutoBackup *ab, BKType type, int *indices, int max);

#endif
