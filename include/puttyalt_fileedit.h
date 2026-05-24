#ifndef PUTTYALT_FILEEDIT_H
#define PUTTYALT_FILEEDIT_H

#define EDIT_MAX_LINES 10000
#define EDIT_MAX_LINE_LEN 4096

typedef struct {
    char path[512];
    char **lines;
    int  line_count;
    int  capacity;
    int  cursor_line;
    int  cursor_col;
    int  modified;
    int  readonly;
    int  is_binary;
    char encoding[16];
    char line_ending[4]; /* "\n" or "\r\n" */
} FileEditor;

void fileedit_init(FileEditor *fe);
void fileedit_free(FileEditor *fe);
int  fileedit_open(FileEditor *fe, const char *content, const char *path);
int  fileedit_insert_char(FileEditor *fe, char c);
int  fileedit_delete_char(FileEditor *fe);
int  fileedit_new_line(FileEditor *fe);
int  fileedit_delete_line(FileEditor *fe, int line);
const char *fileedit_get_line(FileEditor *fe, int line);
int  fileedit_goto(FileEditor *fe, int line, int col);
char *fileedit_serialize(FileEditor *fe, int *out_len);
int  fileedit_find(FileEditor *fe, const char *needle, int *line, int *col);

#endif
