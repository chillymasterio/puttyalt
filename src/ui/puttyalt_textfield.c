/* puttyalt_textfield.c - Single-line text field edit model.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define TF_MAX 256
typedef struct { char buf[TF_MAX]; int len; int cursor; } TextField;
void tf2_init(TextField *t) { if (t) { t->buf[0]=0; t->len=0; t->cursor=0; } }
int tf2_insert(TextField *t, char c) {
    if (!t || t->len >= TF_MAX - 1) return -1;
    memmove(t->buf + t->cursor + 1, t->buf + t->cursor, t->len - t->cursor);
    t->buf[t->cursor] = c;
    t->len++; t->cursor++;
    t->buf[t->len] = 0;
    return 0;
}
int tf2_backspace(TextField *t) {
    if (!t || t->cursor == 0) return -1;
    memmove(t->buf + t->cursor - 1, t->buf + t->cursor, t->len - t->cursor);
    t->len--; t->cursor--;
    t->buf[t->len] = 0;
    return 0;
}
void tf2_left(TextField *t) { if (t && t->cursor > 0) t->cursor--; }
void tf2_right(TextField *t) { if (t && t->cursor < t->len) t->cursor++; }
void tf2_home(TextField *t) { if (t) t->cursor = 0; }
void tf2_end(TextField *t) { if (t) t->cursor = t->len; }
