/* puttyalt_inputmode.c - Input mode manager (insert/replace/vi/normal). */
#include <string.h>
enum im_mode { IM_INSERT = 0, IM_REPLACE = 1, IM_NORMAL = 2, IM_VISUAL = 3, IM_COMMAND = 4 };
typedef struct {
    int mode;
    int prev_mode;
    int vi_enabled;
    char pending[8];
    int pending_len;
} InputMode;
void inputmode_init(InputMode *m, int vi_enabled) {
    if (!m) return;
    memset(m, 0, sizeof(*m));
    m->vi_enabled = vi_enabled ? 1 : 0;
    m->mode = vi_enabled ? IM_NORMAL : IM_INSERT;
}
int inputmode_set(InputMode *m, int mode) {
    if (!m) return -1;
    m->prev_mode = m->mode;
    m->mode = mode;
    m->pending_len = 0;
    return 0;
}
int inputmode_feed(InputMode *m, char ch) {
    if (!m) return -1;
    if (m->mode == IM_NORMAL && m->vi_enabled) {
        switch (ch) {
            case 'i': inputmode_set(m, IM_INSERT); return 1;
            case 'R': inputmode_set(m, IM_REPLACE); return 1;
            case 'v': inputmode_set(m, IM_VISUAL); return 1;
            case ':': inputmode_set(m, IM_COMMAND); return 1;
            default: break;
        }
        if (m->pending_len < 7) m->pending[m->pending_len++] = ch;
        return 0;
    }
    if (ch == 27 && m->vi_enabled) {
        inputmode_set(m, IM_NORMAL);
        return 1;
    }
    return 0;
}
int inputmode_current(const InputMode *m) {
    return m ? m->mode : -1;
}
int inputmode_is_editing(const InputMode *m) {
    return (m && (m->mode == IM_INSERT || m->mode == IM_REPLACE)) ? 1 : 0;
}
