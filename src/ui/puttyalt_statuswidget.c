/* puttyalt_statuswidget.c - Status widget set (clock/conn/encoding indicators). */
#include <string.h>
#include <stdio.h>
#define SW_MAX 12
#define SW_TEXT 32
typedef struct {
    char text[SW_TEXT];
    int widget_type;
    int priority;
    int visible;
} sw_widget;
typedef struct {
    sw_widget widgets[SW_MAX];
    int n;
} StatusWidget;
void statuswidget_init(StatusWidget *s) {
    if (s) memset(s, 0, sizeof(*s));
}
int statuswidget_add(StatusWidget *s, int type, const char *text, int priority) {
    if (!s || s->n >= SW_MAX) return -1;
    sw_widget *w = &s->widgets[s->n];
    w->widget_type = type;
    snprintf(w->text, SW_TEXT, "%s", text ? text : "");
    w->priority = priority;
    w->visible = 1;
    return s->n++;
}
int statuswidget_update(StatusWidget *s, int idx, const char *text) {
    if (!s || idx < 0 || idx >= s->n) return -1;
    snprintf(s->widgets[idx].text, SW_TEXT, "%s", text ? text : "");
    return 0;
}
int statuswidget_render(const StatusWidget *s, char *out, int outlen) {
    if (!s || !out) return -1;
    int pos = 0;
    for (int i = 0; i < s->n && pos < outlen; i++) {
        if (!s->widgets[i].visible) continue;
        pos += snprintf(out + pos, outlen - pos, "%s%s", pos > 0 ? " | " : "", s->widgets[i].text);
    }
    return pos;
}
int statuswidget_count(const StatusWidget *s) {
    return s ? s->n : -1;
}
