#include "puttyalt_toast.h"
#include <string.h>
#include <stdio.h>

void toast_init(ToastManager *tm)
{
    memset(tm, 0, sizeof(*tm));
    tm->position = 0;  /* top-right */
    tm->margin = 12;
    tm->width = 280;
    tm->height = 48;
}

void toast_show(ToastManager *tm, ToastType type, const char *msg, int duration_ms)
{
    if (tm->count >= TOAST_MAX_QUEUE) {
        /* Remove oldest */
        for (int i = 0; i < tm->count - 1; i++)
            tm->queue[i] = tm->queue[i + 1];
        tm->count--;
    }

    Toast *t = &tm->queue[tm->count];
    memset(t, 0, sizeof(*t));
    t->type = type;
    snprintf(t->message, sizeof(t->message), "%s", msg);
    t->duration_ms = duration_ms > 0 ? duration_ms : 3000;
    t->created_at = 0;  /* set on first update */
    t->opacity = 1.0f;
    tm->count++;
}

void toast_update(ToastManager *tm, long now_ms)
{
    int i = 0;
    while (i < tm->count) {
        Toast *t = &tm->queue[i];
        if (t->created_at == 0) t->created_at = now_ms;

        long elapsed = now_ms - t->created_at;
        if (elapsed > t->duration_ms || t->dismissed) {
            /* Fade out */
            long fade_start = t->duration_ms - 300;
            if (elapsed > t->duration_ms + 300 || t->dismissed) {
                /* Remove */
                for (int j = i; j < tm->count - 1; j++)
                    tm->queue[j] = tm->queue[j + 1];
                tm->count--;
                continue;
            } else if (elapsed > fade_start) {
                t->opacity = 1.0f - (float)(elapsed - fade_start) / 300.0f;
            }
        }
        i++;
    }
}

void toast_dismiss(ToastManager *tm, int idx)
{
    if (idx >= 0 && idx < tm->count)
        tm->queue[idx].dismissed = 1;
}

void toast_dismiss_all(ToastManager *tm)
{
    for (int i = 0; i < tm->count; i++)
        tm->queue[i].dismissed = 1;
}

int toast_count(const ToastManager *tm)
{
    return tm->count;
}
