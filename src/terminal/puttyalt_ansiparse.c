#include "puttyalt_ansiparse.h"
#include <string.h>
#include <stdio.h>

void ansi_init(AnsiParser *ap)
{
    memset(ap, 0, sizeof(*ap));
    ap->state = ANSI_STATE_GROUND;
}

void ansi_process_sgr(AnsiParser *ap, TermState *ts)
{
    if (ap->param_count == 0) {
        ts->current_fg = ts->default_fg;
        ts->current_bg = ts->default_bg;
        ts->current_attr = 0;
        return;
    }
    for (int i = 0; i < ap->param_count; i++) {
        int p = ap->params[i];
        if (p == 0) { ts->current_fg = ts->default_fg; ts->current_bg = ts->default_bg; ts->current_attr = 0; }
        else if (p == 1) ts->current_attr |= 1;   /* bold */
        else if (p == 3) ts->current_attr |= 4;   /* italic */
        else if (p == 4) ts->current_attr |= 2;   /* underline */
        else if (p == 5) ts->current_attr |= 8;   /* blink */
        else if (p == 7) ts->current_attr |= 16;  /* reverse */
        else if (p == 9) ts->current_attr |= 32;  /* strikethrough */
        else if (p == 22) ts->current_attr &= ~1;
        else if (p == 23) ts->current_attr &= ~4;
        else if (p == 24) ts->current_attr &= ~2;
        else if (p == 25) ts->current_attr &= ~8;
        else if (p == 27) ts->current_attr &= ~16;
        else if (p == 29) ts->current_attr &= ~32;
        else if (p >= 30 && p <= 37) ts->current_fg = (uint8_t)(p - 30);
        else if (p == 38 && i + 2 < ap->param_count && ap->params[i+1] == 5) {
            ts->current_fg = (uint8_t)ap->params[i+2]; i += 2;
        }
        else if (p == 39) ts->current_fg = ts->default_fg;
        else if (p >= 40 && p <= 47) ts->current_bg = (uint8_t)(p - 40);
        else if (p == 48 && i + 2 < ap->param_count && ap->params[i+1] == 5) {
            ts->current_bg = (uint8_t)ap->params[i+2]; i += 2;
        }
        else if (p == 49) ts->current_bg = ts->default_bg;
        else if (p >= 90 && p <= 97) ts->current_fg = (uint8_t)(p - 90 + 8);
        else if (p >= 100 && p <= 107) ts->current_bg = (uint8_t)(p - 100 + 8);
    }
}

void ansi_process_csi(AnsiParser *ap, TermState *ts)
{
    int p1 = ap->param_count > 0 ? ap->params[0] : 0;
    int p2 = ap->param_count > 1 ? ap->params[1] : 0;

    switch (ap->final_char) {
    case 'A': term_set_cursor(ts, ts->cursor_row - (p1 ? p1 : 1), ts->cursor_col); break;
    case 'B': term_set_cursor(ts, ts->cursor_row + (p1 ? p1 : 1), ts->cursor_col); break;
    case 'C': term_set_cursor(ts, ts->cursor_row, ts->cursor_col + (p1 ? p1 : 1)); break;
    case 'D': term_set_cursor(ts, ts->cursor_row, ts->cursor_col - (p1 ? p1 : 1)); break;
    case 'H': case 'f': term_set_cursor(ts, (p1 ? p1 : 1) - 1, (p2 ? p2 : 1) - 1); break;
    case 'J': term_erase_screen(ts, p1); break;
    case 'K': term_erase_line(ts, p1); break;
    case 'L': term_scroll_down(ts, p1 ? p1 : 1); break;
    case 'M': term_scroll_up(ts, p1 ? p1 : 1); break;
    case '@': term_insert_chars(ts, p1 ? p1 : 1); break;
    case 'P': term_delete_chars(ts, p1 ? p1 : 1); break;
    case 'd': term_set_cursor(ts, (p1 ? p1 : 1) - 1, ts->cursor_col); break;
    case 'G': case '`': term_set_cursor(ts, ts->cursor_row, (p1 ? p1 : 1) - 1); break;
    case 'm': ansi_process_sgr(ap, ts); break;
    case 'r':
        ts->scroll_top = (p1 ? p1 : 1) - 1;
        ts->scroll_bottom = (p2 ? p2 : ts->rows) - 1;
        term_set_cursor(ts, 0, 0);
        break;
    case 's': term_save_cursor(ts); break;
    case 'u': term_restore_cursor(ts); break;
    case 'h':
        if (ap->private_mode) {
            if (p1 == 25) ts->cursor_visible = 1;
            else if (p1 == 1049) term_switch_screen(ts, 1);
            else if (p1 == 2004) ts->bracketed_paste = 1;
        } else if (p1 == 4) ts->insert_mode = 1;
        break;
    case 'l':
        if (ap->private_mode) {
            if (p1 == 25) ts->cursor_visible = 0;
            else if (p1 == 1049) term_switch_screen(ts, 0);
            else if (p1 == 2004) ts->bracketed_paste = 0;
        } else if (p1 == 4) ts->insert_mode = 0;
        break;
    }
}

void ansi_process_osc(AnsiParser *ap, TermState *ts)
{
    (void)ts;
    /* OSC 0/1/2: set window title — handled by GUI layer */
    (void)ap;
}

void ansi_feed(AnsiParser *ap, TermState *ts, uint8_t ch)
{
    switch (ap->state) {
    case ANSI_STATE_GROUND:
        if (ch == 0x1B) ap->state = ANSI_STATE_ESC;
        else term_write(ts, &ch, 1);
        break;

    case ANSI_STATE_ESC:
        if (ch == '[') {
            ap->state = ANSI_STATE_CSI;
            ap->param_count = 0;
            ap->private_mode = 0;
            ap->intermediate = 0;
            memset(ap->params, 0, sizeof(ap->params));
        } else if (ch == ']') {
            ap->state = ANSI_STATE_OSC;
            ap->osc_len = 0;
        } else if (ch == 'P') { ap->state = ANSI_STATE_DCS; }
        else if (ch == 'c') { term_reset(ts); ap->state = ANSI_STATE_GROUND; }
        else if (ch == '7') { term_save_cursor(ts); ap->state = ANSI_STATE_GROUND; }
        else if (ch == '8') { term_restore_cursor(ts); ap->state = ANSI_STATE_GROUND; }
        else if (ch == 'M') { term_scroll_down(ts, 1); ap->state = ANSI_STATE_GROUND; }
        else if (ch == 'D') { term_scroll_up(ts, 1); ap->state = ANSI_STATE_GROUND; }
        else ap->state = ANSI_STATE_GROUND;
        break;

    case ANSI_STATE_CSI:
        if (ch == '?') { ap->private_mode = 1; }
        else if (ch >= '0' && ch <= '9') {
            if (ap->param_count == 0) ap->param_count = 1;
            ap->params[ap->param_count - 1] = ap->params[ap->param_count - 1] * 10 + (ch - '0');
        } else if (ch == ';') {
            if (ap->param_count < ANSI_MAX_PARAMS) ap->param_count++;
        } else if (ch >= 0x20 && ch <= 0x2F) {
            ap->intermediate = (char)ch;
        } else if (ch >= 0x40 && ch <= 0x7E) {
            ap->final_char = (char)ch;
            ansi_process_csi(ap, ts);
            ap->state = ANSI_STATE_GROUND;
        } else ap->state = ANSI_STATE_GROUND;
        break;

    case ANSI_STATE_OSC:
        if (ch == 0x07 || ch == 0x1B) {
            ap->osc_buf[ap->osc_len] = '\0';
            ansi_process_osc(ap, ts);
            ap->state = (ch == 0x1B) ? ANSI_STATE_ST : ANSI_STATE_GROUND;
        } else if (ap->osc_len < ANSI_BUF_SIZE - 1) {
            ap->osc_buf[ap->osc_len++] = (char)ch;
        }
        break;

    case ANSI_STATE_DCS:
        if (ch == 0x1B) ap->state = ANSI_STATE_ST;
        break;

    case ANSI_STATE_ST:
        ap->state = ANSI_STATE_GROUND;
        break;
    }
}
