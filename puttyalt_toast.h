#ifndef PUTTYALT_TOAST_H
#define PUTTYALT_TOAST_H

#define TOAST_MAX_QUEUE   8
#define TOAST_MAX_MSG     128

typedef enum {
    TOAST_INFO,
    TOAST_SUCCESS,
    TOAST_WARNING,
    TOAST_ERROR
} ToastType;

typedef struct {
    char message[TOAST_MAX_MSG];
    ToastType type;
    int duration_ms;
    long created_at;
    float opacity;
    int dismissed;
} Toast;

typedef struct {
    Toast queue[TOAST_MAX_QUEUE];
    int count;
    int position;  /* 0=top-right, 1=bottom-right, 2=bottom-center */
    int margin;
    int width;
    int height;
} ToastManager;

void toast_init(ToastManager *tm);
void toast_show(ToastManager *tm, ToastType type, const char *msg, int duration_ms);
void toast_update(ToastManager *tm, long now_ms);
void toast_dismiss(ToastManager *tm, int idx);
void toast_dismiss_all(ToastManager *tm);
int  toast_count(const ToastManager *tm);

#endif
