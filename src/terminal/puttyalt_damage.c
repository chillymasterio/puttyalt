/* puttyalt_damage.c - Track dirty cell regions for redraw.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define DMG_ROWS 128
typedef struct { int dirty[DMG_ROWS]; int rows; int any; } DamageMap;
void dmg_init(DamageMap *d, int rows) {
    if (!d) return;
    d->rows = (rows > 0 && rows <= DMG_ROWS) ? rows : DMG_ROWS;
    memset(d->dirty, 0, sizeof d->dirty);
    d->any = 0;
}
void dmg_mark(DamageMap *d, int row) {
    if (!d || row < 0 || row >= d->rows) return;
    d->dirty[row] = 1; d->any = 1;
}
void dmg_mark_all(DamageMap *d) {
    if (!d) return;
    for (int i = 0; i < d->rows; i++) d->dirty[i] = 1;
    d->any = 1;
}
int dmg_is_dirty(const DamageMap *d, int row) {
    return d && row >= 0 && row < d->rows && d->dirty[row];
}
void dmg_clear(DamageMap *d) { if (d) { memset(d->dirty, 0, sizeof d->dirty); d->any = 0; } }
int dmg_count(const DamageMap *d) {
    if (!d) return 0;
    int c = 0;
    for (int i = 0; i < d->rows; i++) if (d->dirty[i]) c++;
    return c;
}
