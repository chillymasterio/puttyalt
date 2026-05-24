#include <string.h>
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

    /* Sound playback is handled by the GUI layer using
     * platform-native APIs:
     * - Windows: PlaySound() via winmm
     * - macOS: NSSound / AudioToolbox
     * - Linux: PulseAudio client API
     * This avoids spawning shell processes. */
    return 0;
}

void sound_set_path(SoundConfig *sc, SoundEvent evt, const char *path)
{
    if (evt >= 0 && evt <= SND_CUSTOM)
        snprintf(sc->paths[evt], sizeof(sc->paths[0]), "%s", path);
}

void sound_mute(SoundConfig *sc, int muted)
{
    sc->muted = muted;
}
