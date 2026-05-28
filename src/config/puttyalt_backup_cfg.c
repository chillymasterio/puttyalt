#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_CFG_BACKUPS 16
typedef struct { char path[512]; time_t created; int size; } CfgBackup;
static CfgBackup g_backups[MAX_CFG_BACKUPS]; static int g_backup_count = 0;
int cfg_backup_create(const char *src_path, const char *backup_dir) {
    if (g_backup_count >= MAX_CFG_BACKUPS) { memmove(&g_backups[0], &g_backups[1], sizeof(CfgBackup)*(MAX_CFG_BACKUPS-1)); g_backup_count--; }
    char ts[32]; time_t now = time(NULL); strftime(ts, 32, "%Y%m%d_%H%M%S", localtime(&now));
    CfgBackup *b = &g_backups[g_backup_count]; memset(b, 0, sizeof(*b));
    snprintf(b->path, 512, "%s/config_%s.bak", backup_dir, ts); b->created = now;
    FILE *src = fopen(src_path, "r"); if (!src) return -1;
    FILE *dst = fopen(b->path, "w"); if (!dst) { fclose(src); return -1; }
    char buf[4096]; size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) { fwrite(buf, 1, n, dst); b->size += (int)n; }
    fclose(src); fclose(dst); g_backup_count++; return 0;
}
int cfg_backup_count(void) { return g_backup_count; }
int cfg_backup_restore(int idx, const char *dest_path) {
    if (idx<0 || idx>=g_backup_count) return -1;
    FILE *src = fopen(g_backups[idx].path, "r"); if (!src) return -1;
    FILE *dst = fopen(dest_path, "w"); if (!dst) { fclose(src); return -1; }
    char buf[4096]; size_t n;
    while ((n = fread(buf, 1, sizeof(buf), src)) > 0) fwrite(buf, 1, n, dst);
    fclose(src); fclose(dst); return 0;
}
