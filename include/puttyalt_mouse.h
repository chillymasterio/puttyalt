/*
 * puttyalt_mouse.h: Enhanced mouse behaviour configuration.
 *
 * Copy on select, middle-click paste, right-click context menu,
 * URL detection and click-to-open.
 */

#ifndef PUTTYALT_MOUSE_H
#define PUTTYALT_MOUSE_H

typedef struct MouseConfig {
    int copy_on_select;      /* auto-copy selected text */
    int paste_on_middle;     /* middle click = paste */
    int paste_on_right;      /* right click = paste (vs context menu) */
    int url_detect;          /* highlight URLs in terminal */
    int url_click_open;      /* Ctrl+click opens URL in browser */
    int scroll_lines;        /* lines per scroll wheel notch */
    int select_word_chars[256]; /* char class for word selection */
} MouseConfig;

void mouse_config_init(MouseConfig *mc);
int  mouse_is_url_char(const MouseConfig *mc, int ch);

#endif /* PUTTYALT_MOUSE_H */
