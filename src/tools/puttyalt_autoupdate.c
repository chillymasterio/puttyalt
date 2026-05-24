#include <string.h>
#include <stdlib.h>
#include "puttyalt_autoupdate.h"

void update_init(UpdateInfo *ui, const char *current_ver)
{
    memset(ui, 0, sizeof(*ui));
    strncpy(ui->current_version, current_ver, sizeof(ui->current_version) - 1);
    ui->check_on_startup = 1;
    ui->check_interval_hours = 24;
}

static int parse_version(const char *ver, int *major, int *minor, int *patch)
{
    const char *p = ver;
    if (*p == 'v') p++;
    *major = atoi(p);
    p = strchr(p, '.'); if (!p) return -1; p++;
    *minor = atoi(p);
    p = strchr(p, '.'); if (!p) { *patch = 0; return 0; } p++;
    *patch = atoi(p);
    return 0;
}

int update_is_newer(const char *current, const char *latest)
{
    int cmaj, cmin, cpat, lmaj, lmin, lpat;
    if (parse_version(current, &cmaj, &cmin, &cpat) < 0) return 0;
    if (parse_version(latest, &lmaj, &lmin, &lpat) < 0) return 0;

    if (lmaj > cmaj) return 1;
    if (lmaj == cmaj && lmin > cmin) return 1;
    if (lmaj == cmaj && lmin == cmin && lpat > cpat) return 1;
    return 0;
}

int update_check(UpdateInfo *ui)
{
    ui->status = UPDATE_CHECKING;
    /* In production: HTTP GET to GitHub releases API */
    /* For now, stub — always reports no update */
    ui->status = UPDATE_NONE;
    return 0;
}

void update_dismiss(UpdateInfo *ui)
{
    ui->status = UPDATE_NONE;
}
