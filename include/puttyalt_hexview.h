#ifndef PUTTYALT_HEXVIEW_H
#define PUTTYALT_HEXVIEW_H

#define HEX_MAX_DATA    (64 * 1024)
#define HEX_COLS        16
#define HEX_ROWS_PAGE   32

typedef struct {
    unsigned char data[HEX_MAX_DATA];
    int           data_len;
    int           offset;         /* scroll offset in bytes */
    int           cursor;         /* cursor position in data */
    int           selection_start;
    int           selection_end;
    int           ascii_mode;     /* 0=hex, 1=ascii editing */
    int           read_only;
    char          search_hex[128];
    int           last_match;
} HexView;

void hexview_init(HexView *hv);
int  hexview_load(HexView *hv, const unsigned char *buf, int len);
int  hexview_load_file(HexView *hv, const char *path);
void hexview_scroll(HexView *hv, int delta_rows);
void hexview_set_cursor(HexView *hv, int pos);
void hexview_select(HexView *hv, int start, int end);
int  hexview_search(HexView *hv, const char *hex_pattern);
int  hexview_format_line(const HexView *hv, int offset, char *out, int outsz);
int  hexview_save_file(const HexView *hv, const char *path);
void hexview_toggle_mode(HexView *hv);

#endif
