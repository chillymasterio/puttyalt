#include "puttyalt_bellctl.h"
#include <string.h>
#include <time.h>

void bell_init(BellController *bc)
{
    memset(bc, 0, sizeof(*bc));
    bc->mode = BELL_VISUAL;
    bc->enabled = 1;
    bc->flash_duration_ms = 150;
    bc->cooldown_ms = 200;
    bc->visual_invert = 1;
    bc->taskbar_flash = 1;
}

void bell_trigger(BellController *bc)
{
    if (!bc->enabled || bc->muted) return;
    unsigned long now = (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
    if (now - bc->last_bell_time < (unsigned long)bc->cooldown_ms) return;
    bc->last_bell_time = now;
    bc->bell_count++;
}

int bell_should_fire(BellController *bc)
{
    if (!bc->enabled || bc->muted) return 0;
    unsigned long now = (unsigned long)(clock() * 1000 / CLOCKS_PER_SEC);
    return (now - bc->last_bell_time) < (unsigned long)bc->flash_duration_ms;
}

void bell_mute(BellController *bc) { bc->muted = 1; }
void bell_unmute(BellController *bc) { bc->muted = 0; }
void bell_set_mode(BellController *bc, BellMode mode) { bc->mode = mode; }
