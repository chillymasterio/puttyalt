#include <string.h>
#include <stdio.h>
#include <time.h>
#define MAX_ACTIONS 512
typedef struct { char input[512]; int delay_ms; time_t recorded_at; } MacroAction;
typedef struct { char name[64]; MacroAction actions[MAX_ACTIONS]; int count; int recording; int playing; } MacroRecording;
static MacroRecording g_macro; static int g_inited = 0;
void macro_init(void) { memset(&g_macro, 0, sizeof(g_macro)); g_inited = 1; }
void macro_start_recording(const char *name) { g_macro.count = 0; g_macro.recording = 1; snprintf(g_macro.name, 64, "%s", name); }
void macro_stop_recording(void) { g_macro.recording = 0; }
int macro_record_input(const char *input, int delay) {
    if (!g_macro.recording || g_macro.count >= MAX_ACTIONS) return -1;
    MacroAction *a = &g_macro.actions[g_macro.count++];
    snprintf(a->input, 512, "%s", input); a->delay_ms = delay; a->recorded_at = time(NULL); return 0;
}
int macro_action_count(void) { return g_macro.count; }
int macro_is_recording(void) { return g_macro.recording; }
int macro_is_playing(void) { return g_macro.playing; }
const char *macro_get_name(void) { return g_macro.name; }
