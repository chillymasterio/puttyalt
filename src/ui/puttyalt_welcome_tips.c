#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
static const char *tips[] = {
    "Press Ctrl+P to open the command palette",
    "Drag tabs to reorder them",
    "Right-click the terminal for quick actions",
    "Use Ctrl+Shift+C/V for copy/paste in terminal",
    "Double-click a word to select it",
    "Press F11 for fullscreen mode",
    "Ctrl+Plus/Minus to zoom in/out",
    "Type 'puttyalt user@host' for quick connect",
    "Sessions are auto-saved on exit",
    "Use labels to organize your sessions",
    "Press Ctrl+F to search terminal scrollback",
    "Enable timestamps in Settings > Terminal",
};
#define TIP_COUNT (int)(sizeof(tips)/sizeof(tips[0]))
const char *welcome_tip_random(void) {
    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }
    return tips[rand() % TIP_COUNT];
}
const char *welcome_tip_get(int idx) { return (idx >= 0 && idx < TIP_COUNT) ? tips[idx] : NULL; }
int welcome_tip_count(void) { return TIP_COUNT; }
