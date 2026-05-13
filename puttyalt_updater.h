#ifndef PUTTYALT_UPDATER_H
#define PUTTYALT_UPDATER_H

#define UPD_MAX_URL       512
#define UPD_MAX_VERSION   32
#define UPD_MAX_NOTES     2048

typedef enum {
    UPD_IDLE = 0,
    UPD_CHECKING,
    UPD_AVAILABLE,
    UPD_DOWNLOADING,
    UPD_READY,
    UPD_ERROR
} UpdStatus;

typedef struct {
    UpdStatus  status;
    char       current_version[UPD_MAX_VERSION];
    char       latest_version[UPD_MAX_VERSION];
    char       download_url[UPD_MAX_URL];
    char       release_notes[UPD_MAX_NOTES];
    long       download_size;
    long       downloaded;
    int        auto_check;
    int        check_interval;   /* hours */
    long       last_check;
    char       update_channel[32]; /* stable, beta, nightly */
} Updater;

void updater_init(Updater *u, const char *current_version);
int  updater_check(Updater *u);
int  updater_download(Updater *u, const char *dest_path);
int  updater_apply(Updater *u);
int  updater_is_newer(const char *current, const char *latest);
void updater_set_channel(Updater *u, const char *channel);
void updater_set_auto_check(Updater *u, int enable, int hours);

#endif
