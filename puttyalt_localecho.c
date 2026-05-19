#include "puttyalt_localecho.h"
#include <string.h>

void localecho_init(LocalEcho *le)
{
    memset(le, 0, sizeof(*le));
    le->auto_detect = 1;
    le->show_indicator = 1;
    le->color = 0x888888;
    le->password_mask = '*';
    le->detect_password_prompt = 1;
}

int localecho_should_echo(LocalEcho *le, const char *last_output)
{
    if (!le->enabled && !le->auto_detect) return 0;
    if (le->enabled) return 1;
    /* auto-detect: if the last output contains password-like prompt, don't echo */
    if (le->detect_password_prompt && last_output) {
        if (strstr(last_output, "assword:") || strstr(last_output, "assphrase:"))
            return 0;
    }
    return le->auto_detect;
}

void localecho_toggle(LocalEcho *le) { le->enabled = !le->enabled; }
char localecho_mask_char(LocalEcho *le) { return le->password_mask; }
