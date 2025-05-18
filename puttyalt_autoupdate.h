#ifndef PUTTYALT_AUTOUPDATE_H
#define PUTTYALT_AUTOUPDATE_H

#define UPDATE_URL_LEN  512

typedef enum {
    UPDATE_NONE = 0,
    UPDATE_AVAILABLE,
    UPDATE_CHECKING,
    UPDATE_ERROR
} UpdateStatus;

typedef struct UpdateInfo {
    UpdateStatus status;
    char current_version[16];
    char latest_version[16];
    char download_url[UPDATE_URL_LEN];
    char release_notes[1024];
    int check_on_startup;
    int check_interval_hours;
    unsigned long last_check;
} UpdateInfo;

void update_init(UpdateInfo *ui, const char *current_ver);
int  update_check(UpdateInfo *ui);
int  update_is_newer(const char *current, const char *latest);
void update_dismiss(UpdateInfo *ui);

#endif
