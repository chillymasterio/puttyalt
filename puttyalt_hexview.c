#include "puttyalt_hexview.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

void hexview_init(HexView *hv)
{
    memset(hv, 0, sizeof(*hv));
    hv->last_match = -1;
    hv->selection_start = -1;
    hv->selection_end = -1;
}

int hexview_load(HexView *hv, const unsigned char *buf, int len)
{
    if (len > HEX_MAX_DATA) len = HEX_MAX_DATA;
    memcpy(hv->data, buf, len);
    hv->data_len = len;
    hv->offset = 0;
    hv->cursor = 0;
    return 0;
}

int hexview_load_file(HexView *hv, const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) return -1;
    hv->data_len = (int)fread(hv->data, 1, HEX_MAX_DATA, f);
    fclose(f);
    hv->offset = 0;
    hv->cursor = 0;
    return 0;
}

void hexview_scroll(HexView *hv, int delta_rows)
{
    int new_off = hv->offset + delta_rows * HEX_COLS;
    if (new_off < 0) new_off = 0;
    if (new_off >= hv->data_len) new_off = (hv->data_len / HEX_COLS) * HEX_COLS;
    hv->offset = new_off;
}

void hexview_set_cursor(HexView *hv, int pos)
{
    if (pos < 0) pos = 0;
    if (pos >= hv->data_len) pos = hv->data_len - 1;
    hv->cursor = pos;
}

void hexview_select(HexView *hv, int start, int end)
{
    hv->selection_start = start;
    hv->selection_end = end;
}

int hexview_search(HexView *hv, const char *hex_pattern)
{
    unsigned char needle[64];
    int nlen = 0;
    const char *p = hex_pattern;
    while (*p && nlen < 64) {
        while (*p == ' ') p++;
        if (!*p) break;
        char tmp[3] = {p[0], p[1] ? p[1] : '0', 0};
        needle[nlen++] = (unsigned char)strtol(tmp, NULL, 16);
        p += (p[1] ? 2 : 1);
    }
    if (nlen == 0) return -1;

    int start = hv->last_match >= 0 ? hv->last_match + 1 : 0;
    for (int i = start; i <= hv->data_len - nlen; i++) {
        if (memcmp(hv->data + i, needle, nlen) == 0) {
            hv->last_match = i;
            hv->cursor = i;
            return i;
        }
    }
    /* Wrap around */
    for (int i = 0; i < start && i <= hv->data_len - nlen; i++) {
        if (memcmp(hv->data + i, needle, nlen) == 0) {
            hv->last_match = i;
            hv->cursor = i;
            return i;
        }
    }
    return -1;
}

int hexview_format_line(const HexView *hv, int offset, char *out, int outsz)
{
    if (offset >= hv->data_len || outsz < 80) return -1;
    int pos = 0;
    pos += snprintf(out + pos, outsz - pos, "%08X  ", offset);
    for (int i = 0; i < HEX_COLS; i++) {
        if (offset + i < hv->data_len)
            pos += snprintf(out + pos, outsz - pos, "%02X ", hv->data[offset + i]);
        else
            pos += snprintf(out + pos, outsz - pos, "   ");
        if (i == 7) out[pos++] = ' ';
    }
    pos += snprintf(out + pos, outsz - pos, " |");
    for (int i = 0; i < HEX_COLS && offset + i < hv->data_len; i++) {
        unsigned char c = hv->data[offset + i];
        out[pos++] = isprint(c) ? (char)c : '.';
    }
    out[pos++] = '|';
    out[pos] = '\0';
    return pos;
}

int hexview_save_file(const HexView *hv, const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f) return -1;
    fwrite(hv->data, 1, hv->data_len, f);
    fclose(f);
    return 0;
}

void hexview_toggle_mode(HexView *hv)
{
    hv->ascii_mode = !hv->ascii_mode;
}
