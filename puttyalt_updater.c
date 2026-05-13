#include "puttyalt_updater.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void updater_init(Updater *u, const char *current_version)
{
    memset(u, 0, sizeof(*u));
    snprintf(u->current_version, UPD_MAX_VERSION, "%s", current_version);
    snprintf(u->update_channel, sizeof(u->update_channel), "stable");
    snprintf(u->download_url, UPD_MAX_URL,
             "https://github.com/chillymasterio/puttyalt/releases/latest");
    u->auto_check = 1;
    u->check_interval = 24;
}

int updater_is_newer(const char *current, const char *latest)
{
    int c_maj = 0, c_min = 0, c_pat = 0;
    int l_maj = 0, l_min = 0, l_pat = 0;
    sscanf(current, "%d.%d.%d", &c_maj, &c_min, &c_pat);
    sscanf(latest, "%d.%d.%d", &l_maj, &l_min, &l_pat);

    if (l_maj != c_maj) return l_maj > c_maj;
    if (l_min != c_min) return l_min > c_min;
    return l_pat > c_pat;
}

int updater_check(Updater *u)
{
    u->status = UPD_CHECKING;
    u->last_check = (long)time(NULL);

    /* In real implementation:
     * 1. HTTP GET https://api.github.com/repos/chillymasterio/puttyalt/releases/latest
     * 2. Parse JSON for tag_name, body, assets[0].browser_download_url
     * 3. Compare versions
     *
     * For now, simulate check result */
    u->status = UPD_IDLE;
    return 0;
}

int updater_download(Updater *u, const char *dest_path)
{
    if (u->status != UPD_AVAILABLE) return -1;
    u->status = UPD_DOWNLOADING;

    /* Real implementation would use WinHTTP/libcurl */
    (void)dest_path;

    u->status = UPD_READY;
    return 0;
}

int updater_apply(Updater *u)
{
    if (u->status != UPD_READY) return -1;

    /* Real implementation:
     * 1. Rename current exe to .bak
     * 2. Move downloaded exe to current path
     * 3. Remove Zone.Identifier from new exe
     * 4. Restart application */
    return 0;
}

void updater_set_channel(Updater *u, const char *channel)
{
    snprintf(u->update_channel, sizeof(u->update_channel), "%s", channel);
}

void updater_set_auto_check(Updater *u, int enable, int hours)
{
    u->auto_check = enable;
    u->check_interval = hours > 0 ? hours : 24;
}
