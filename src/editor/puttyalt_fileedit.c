#include <stdio.h>
#include "puttyalt_fileedit.h"
#include <string.h>
#include <stdlib.h>

void fileedit_init(FileEditor *fe)
{
    memset(fe, 0, sizeof(*fe));
    snprintf(fe->encoding, sizeof(fe->encoding), "UTF-8");
    snprintf(fe->line_ending, sizeof(fe->line_ending), "\n");
}

void fileedit_free(FileEditor *fe)
{
    if (fe->lines) {
        for (int i = 0; i < fe->line_count; i++) free(fe->lines[i]);
        free(fe->lines);
    }
    memset(fe, 0, sizeof(*fe));
}

int fileedit_open(FileEditor *fe, const char *content, const char *path)
{
    fileedit_free(fe);
    fileedit_init(fe);
    snprintf(fe->path, sizeof(fe->path), "%s", path ? path : "untitled");
    fe->capacity = 256;
    fe->lines = (char **)calloc(fe->capacity, sizeof(char *));
    if (!fe->lines) return -1;

    /* detect binary content */
    int binary_bytes = 0;
    for (int i = 0; content && i < 512 && content[i]; i++)
        if ((unsigned char)content[i] < 0x09 || ((unsigned char)content[i] > 0x0d && (unsigned char)content[i] < 0x20)) binary_bytes++;
    if (binary_bytes > 16) { fe->is_binary = 1; fe->readonly = 1; }
    if (!content || !content[0]) {
        fe->lines[0] = (char *)calloc(1, 1);
        fe->line_count = 1;
        return 0;
    }

    const char *p = content;
    while (*p && fe->line_count < EDIT_MAX_LINES) {
        const char *eol = strchr(p, '\n');
        int len = eol ? (int)(eol - p) : (int)strlen(p);
        if (len > EDIT_MAX_LINE_LEN) len = EDIT_MAX_LINE_LEN;
        if (fe->line_count >= fe->capacity) {
            fe->capacity *= 2;
            fe->lines = (char **)realloc(fe->lines, fe->capacity * sizeof(char *));
        }
        fe->lines[fe->line_count] = (char *)malloc(len + 1);
        memcpy(fe->lines[fe->line_count], p, len);
        fe->lines[fe->line_count][len] = '\0';
        /* strip CR */
        if (len > 0 && fe->lines[fe->line_count][len-1] == '\r') {
            fe->lines[fe->line_count][len-1] = '\0';
            snprintf(fe->line_ending, sizeof(fe->line_ending), "\r\n");
        }
        fe->line_count++;
        p = eol ? eol + 1 : p + len;
    }
    if (fe->line_count == 0) {
        fe->lines[0] = (char *)calloc(1, 1);
        fe->line_count = 1;
    }
    return 0;
}

int fileedit_insert_char(FileEditor *fe, char c)
{
    if (fe->readonly || fe->cursor_line >= fe->line_count) return -1;
    char *old = fe->lines[fe->cursor_line];
    int len = (int)strlen(old);
    if (fe->cursor_col > len) fe->cursor_col = len;
    char *newl = (char *)malloc(len + 2);
    if (!newl) return -1;
    memcpy(newl, old, fe->cursor_col);
    newl[fe->cursor_col] = c;
    memcpy(newl + fe->cursor_col + 1, old + fe->cursor_col, len - fe->cursor_col + 1);
    free(old);
    fe->lines[fe->cursor_line] = newl;
    fe->cursor_col++;
    fe->modified = 1;
    return 0;
}

int fileedit_delete_char(FileEditor *fe)
{
    if (fe->readonly || fe->cursor_line >= fe->line_count) return -1;
    char *line = fe->lines[fe->cursor_line];
    int len = (int)strlen(line);
    if (fe->cursor_col <= 0) return -1;
    memmove(line + fe->cursor_col - 1, line + fe->cursor_col, len - fe->cursor_col + 1);
    fe->cursor_col--;
    fe->modified = 1;
    return 0;
}

int fileedit_new_line(FileEditor *fe)
{
    if (fe->readonly || fe->line_count >= EDIT_MAX_LINES) return -1;
    if (fe->line_count >= fe->capacity) {
        fe->capacity *= 2;
        fe->lines = (char **)realloc(fe->lines, fe->capacity * sizeof(char *));
    }
    /* split current line at cursor */
    char *old = fe->lines[fe->cursor_line];
    int col = fe->cursor_col;
    int len = (int)strlen(old);
    char *rest = (char *)malloc(len - col + 1);
    memcpy(rest, old + col, len - col + 1);
    old[col] = '\0';
    /* shift lines down */
    memmove(&fe->lines[fe->cursor_line + 2], &fe->lines[fe->cursor_line + 1],
            (fe->line_count - fe->cursor_line - 1) * sizeof(char *));
    fe->lines[fe->cursor_line + 1] = rest;
    fe->line_count++;
    fe->cursor_line++;
    fe->cursor_col = 0;
    fe->modified = 1;
    return 0;
}

int fileedit_delete_line(FileEditor *fe, int line)
{
    if (fe->readonly || line < 0 || line >= fe->line_count || fe->line_count <= 1) return -1;
    free(fe->lines[line]);
    memmove(&fe->lines[line], &fe->lines[line+1], (fe->line_count - line - 1) * sizeof(char *));
    fe->line_count--;
    if (fe->cursor_line >= fe->line_count) fe->cursor_line = fe->line_count - 1;
    fe->modified = 1;
    return 0;
}

const char *fileedit_get_line(FileEditor *fe, int line)
{
    return (line >= 0 && line < fe->line_count) ? fe->lines[line] : NULL;
}

int fileedit_goto(FileEditor *fe, int line, int col)
{
    if (line >= 0 && line < fe->line_count) fe->cursor_line = line;
    if (col >= 0) fe->cursor_col = col;
    return 0;
}

char *fileedit_serialize(FileEditor *fe, int *out_len)
{
    int total = 0, elen = (int)strlen(fe->line_ending);
    for (int i = 0; i < fe->line_count; i++) total += (int)strlen(fe->lines[i]) + elen;
    char *buf = (char *)malloc(total + 1);
    if (!buf) return NULL;
    int pos = 0;
    for (int i = 0; i < fe->line_count; i++) {
        int len = (int)strlen(fe->lines[i]);
        memcpy(buf + pos, fe->lines[i], len); pos += len;
        if (i < fe->line_count - 1) { memcpy(buf + pos, fe->line_ending, elen); pos += elen; }
    }
    buf[pos] = '\0';
    if (out_len) *out_len = pos;
    return buf;
}

int fileedit_find(FileEditor *fe, const char *needle, int *rline, int *rcol)
{
    for (int i = fe->cursor_line; i < fe->line_count; i++) {
        int start = (i == fe->cursor_line) ? fe->cursor_col + 1 : 0;
        const char *found = strstr(fe->lines[i] + start, needle);
        if (found) {
            *rline = i;
            *rcol = (int)(found - fe->lines[i]);
            return 1;
        }
    }
    return 0;
}
