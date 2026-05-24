#ifndef PUTTYALT_SOUND_H
#define PUTTYALT_SOUND_H

typedef enum {
    SND_BELL = 0,
    SND_CONNECT,
    SND_DISCONNECT,
    SND_TRANSFER_DONE,
    SND_ERROR,
    SND_CUSTOM
} SoundEvent;

typedef struct SoundConfig {
    char paths[6][512];  /* one per SoundEvent */
    int volume;          /* 0-100 */
    int enabled;
    int muted;
} SoundConfig;

void sound_init(SoundConfig *sc);
int  sound_play(const SoundConfig *sc, SoundEvent evt);
void sound_set_path(SoundConfig *sc, SoundEvent evt, const char *path);
void sound_mute(SoundConfig *sc, int muted);

#endif
