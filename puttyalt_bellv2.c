#include <string.h>
#include "puttyalt_bellv2.h"

void bell_init(BellConfig *bc)
{
    memset(bc, 0, sizeof(*bc));
    bc->mode = BELL_SYSTEM;
    bc->visual_duration_ms = 100;
    bc->suppress_after_ms = 200;
    bc->visual_color = 0xFF4444; /* red flash */
}

int bell_is_suppressed(const BellConfig *bc, unsigned long now)
{
    if (bc->suppress_after_ms <= 0) return 0;
    return (now - bc->last_bell) < (unsigned long)bc->suppress_after_ms;
}

int bell_trigger(BellConfig *bc, unsigned long now)
{
    if (bc->mode == BELL_NONE) return 0;
    if (bell_is_suppressed(bc, now)) return 0;

    bc->last_bell = now;
    bc->total_bells++;

    switch (bc->mode) {
        case BELL_SYSTEM:
            /* Platform beep */
            break;
        case BELL_VISUAL:
            /* Flash terminal background */
            return 1;
        case BELL_TASKBAR:
            /* Flash taskbar button */
            return 2;
        case BELL_WAV:
            /* Play wav file */
            return 3;
        case BELL_NOTIFICATION:
            /* Send desktop notification */
            return 4;
        default:
            break;
    }
    return 0;
}

void bell_set_mode(BellConfig *bc, BellMode mode)
{
    bc->mode = mode;
}

void bell_set_wav(BellConfig *bc, const char *path)
{
    strncpy(bc->wav_path, path, BELL_WAV_PATH_LEN - 1);
    bc->wav_path[BELL_WAV_PATH_LEN - 1] = '\0';
    bc->mode = BELL_WAV;
}
