#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "puttyalt_sound.h"

void sound_init(SoundConfig *sc)
{
    memset(sc, 0, sizeof(*sc));
    sc->volume = 80;
    sc->enabled = 1;
}

int sound_play(const SoundConfig *sc, SoundEvent evt)
{
    if (!sc->enabled || sc->muted) return 0;
    if (evt < 0 || evt > SND_CUSTOM) return -1;
    if (sc->paths[evt][0] == '\0') return 0;

#ifdef _WIN32
    /* PlaySound(sc->paths[evt], NULL, SND_FILENAME | SND_ASYNC); */
    return 0;
#elif defined(__APPLE__)
    char cmd[600];
    snprintf(cmd, sizeof(cmd), "afplay '%s' &", sc->paths[evt]);
    return system(cmd);
#else
    char cmd[600];
    snprintf(cmd, sizeof(cmd), "paplay '%s' &", sc->paths[evt]);
    return system(cmd);
#endif
}

void sound_set_path(SoundConfig *sc, SoundEvent evt, const char *path)
{
    if (evt >= 0 && evt <= SND_CUSTOM)
        strncpy(sc->paths[evt], path, sizeof(sc->paths[0]) - 1);
}

void sound_mute(SoundConfig *sc, int muted)
{
    sc->muted = muted;
}
