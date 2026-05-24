#ifndef PUTTYALT_CMDPAL_H
#define PUTTYALT_CMDPAL_H

#define CMDPAL_MAX_ITEMS    256
#define CMDPAL_MAX_QUERY    128
#define CMDPAL_MAX_LABEL    64
#define CMDPAL_MAX_CATEGORY 32

typedef struct {
    char label[CMDPAL_MAX_LABEL];
    char category[CMDPAL_MAX_CATEGORY];
    char shortcut[24];
    int command_id;
    int score;
    int visible;
} CmdPalItem;

typedef struct {
    CmdPalItem items[CMDPAL_MAX_ITEMS];
    int item_count;
    char query[CMDPAL_MAX_QUERY];
    int query_len;
    int selected;
    int visible_count;
    int active;
    int scroll_offset;
    int max_visible;
} CmdPalState;

int  cmdpal_init(CmdPalState *cp);
void cmdpal_destroy(CmdPalState *cp);
int  cmdpal_add(CmdPalState *cp, const char *label, const char *category,
                const char *shortcut, int cmd_id);
void cmdpal_open(CmdPalState *cp);
void cmdpal_close(CmdPalState *cp);
void cmdpal_filter(CmdPalState *cp, const char *query);
int  cmdpal_select(CmdPalState *cp);
void cmdpal_move(CmdPalState *cp, int delta);
int  cmdpal_handle_key(CmdPalState *cp, int vk, int shift);

#endif
