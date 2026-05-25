#include <string.h>
#include <stdio.h>
#include <time.h>

#define BACKUP_MAX 64

typedef struct {
    char session_name[128];
    char backup_path[512];
    time_t timestamp;
} SessionBackup;

static SessionBackup g_backups[BACKUP_MAX];
static int g_backup_count = 0;

int sessbackup_create(const char *session_name, const char *config_data)
{
    if (g_backup_count >= BACKUP_MAX) {
        /* Rotate: remove oldest */
        memmove(&g_backups[0], &g_backups[1], sizeof(SessionBackup) * (BACKUP_MAX - 1));
        g_backup_count = BACKUP_MAX - 1;
    }
    SessionBackup *b = &g_backups[g_backup_count];
    snprintf(b->session_name, 128, "%s", session_name);
    b->timestamp = time(NULL);
    snprintf(b->backup_path, 512, "backups/%s_%ld.bak", session_name, (long)b->timestamp);

    FILE *fp = fopen(b->backup_path, "w");
    if (!fp) return -1;
    fputs(config_data, fp);
    fclose(fp);
    g_backup_count++;
    return 0;
}

int sessbackup_list(const char *session_name, char paths[][512], int max)
{
    int n = 0;
    for (int i = g_backup_count - 1; i >= 0 && n < max; i--)
        if (strcmp(g_backups[i].session_name, session_name) == 0)
            snprintf(paths[n++], 512, "%s", g_backups[i].backup_path);
    return n;
}

int sessbackup_count(void) { return g_backup_count; }
