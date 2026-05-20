#include "puttyalt_cmdpal.h"
#include <string.h>
#include <ctype.h>

static int fuzzy_match(const char *query, const char *text)
{
    if (!query[0]) return 100;
    int score = 0;
    int qi = 0;
    int consecutive = 0;
    int first_match = -1;

    for (int i = 0; text[i] && query[qi]; i++) {
        char tc = (char)tolower((unsigned char)text[i]);
        char qc = (char)tolower((unsigned char)query[qi]);
        if (tc == qc) {
            if (first_match < 0) first_match = i;
            qi++;
            consecutive++;
            score += consecutive * 2;
            /* Bonus for word boundary match */
            if (i == 0 || text[i-1] == ' ' || text[i-1] == '/')
                score += 5;
        } else {
            consecutive = 0;
        }
    }

    if (query[qi]) return 0;  /* didn't match all chars */
    if (first_match == 0) score += 10;  /* prefix match bonus */
    return score;
}

int cmdpal_init(CmdPalState *cp)
{
    memset(cp, 0, sizeof(*cp));
    cp->max_visible = 12;
    return 0;
}

void cmdpal_destroy(CmdPalState *cp)
{
    memset(cp, 0, sizeof(*cp));
}

int cmdpal_add(CmdPalState *cp, const char *label, const char *category,
               const char *shortcut, int cmd_id)
{
    if (cp->item_count >= CMDPAL_MAX_ITEMS) return -1;
    CmdPalItem *item = &cp->items[cp->item_count];
    snprintf(item->label, sizeof(item->label), "%s", label);
    if (category) snprintf(item->category, sizeof(item->category), "%s", category);
    if (shortcut) snprintf(item->shortcut, sizeof(item->shortcut), "%s", shortcut);
    item->command_id = cmd_id;
    item->visible = 1;
    item->score = 0;
    cp->item_count++;
    return cp->item_count - 1;
}

void cmdpal_open(CmdPalState *cp)
{
    cp->active = 1;
    cp->query[0] = '\0';
    cp->query_len = 0;
    cp->selected = 0;
    cp->scroll_offset = 0;
    /* Show all items */
    cp->visible_count = cp->item_count;
    for (int i = 0; i < cp->item_count; i++) {
        cp->items[i].visible = 1;
        cp->items[i].score = 100;
    }
}

void cmdpal_close(CmdPalState *cp)
{
    cp->active = 0;
    cp->query[0] = '\0';
    cp->query_len = 0;
}

void cmdpal_filter(CmdPalState *cp, const char *query)
{
    snprintf(cp->query, sizeof(cp->query), "%s", query);
    cp->query_len = (int)strlen(cp->query);
    cp->visible_count = 0;
    cp->selected = 0;
    cp->scroll_offset = 0;

    for (int i = 0; i < cp->item_count; i++) {
        int score = fuzzy_match(cp->query, cp->items[i].label);
        if (!score && cp->items[i].category[0])
            score = fuzzy_match(cp->query, cp->items[i].category) / 2;
        cp->items[i].score = score;
        cp->items[i].visible = (score > 0);
        if (score > 0) cp->visible_count++;
    }

    /* Sort by score (simple bubble for small N) */
    for (int i = 0; i < cp->item_count - 1; i++) {
        for (int j = i + 1; j < cp->item_count; j++) {
            if (cp->items[j].score > cp->items[i].score) {
                CmdPalItem tmp = cp->items[i];
                cp->items[i] = cp->items[j];
                cp->items[j] = tmp;
            }
        }
    }
}

int cmdpal_select(CmdPalState *cp)
{
    if (!cp->active) return -1;
    int visible_idx = 0;
    for (int i = 0; i < cp->item_count; i++) {
        if (!cp->items[i].visible) continue;
        if (visible_idx == cp->selected) {
            cp->active = 0;
            return cp->items[i].command_id;
        }
        visible_idx++;
    }
    return -1;
}

void cmdpal_move(CmdPalState *cp, int delta)
{
    cp->selected += delta;
    if (cp->selected < 0) cp->selected = cp->visible_count - 1;
    if (cp->selected >= cp->visible_count) cp->selected = 0;

    /* Scroll */
    if (cp->selected < cp->scroll_offset)
        cp->scroll_offset = cp->selected;
    if (cp->selected >= cp->scroll_offset + cp->max_visible)
        cp->scroll_offset = cp->selected - cp->max_visible + 1;
}

int cmdpal_handle_key(CmdPalState *cp, int vk, int shift)
{
    (void)shift;
    if (!cp->active) return 0;

    switch (vk) {
    case 0x1B: /* Escape */
        cmdpal_close(cp);
        return 1;
    case 0x0D: /* Enter */
        return cmdpal_select(cp) >= 0 ? 2 : 1;
    case 0x26: /* Up */
        cmdpal_move(cp, -1);
        return 1;
    case 0x28: /* Down */
        cmdpal_move(cp, 1);
        return 1;
    case 0x08: /* Backspace */
        if (cp->query_len > 0) {
            cp->query[--cp->query_len] = '\0';
            cmdpal_filter(cp, cp->query);
        }
        return 1;
    default:
        if (vk >= 32 && vk < 127 && cp->query_len < CMDPAL_MAX_QUERY - 1) {
            cp->query[cp->query_len++] = (char)vk;
            cp->query[cp->query_len] = '\0';
            cmdpal_filter(cp, cp->query);
            return 1;
        }
        break;
    }
    return 0;
}
