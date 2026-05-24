#ifndef PUTTYALT_LOCALECHO_H
#define PUTTYALT_LOCALECHO_H

typedef struct {
    int  enabled;
    int  auto_detect; /* auto-enable when server doesn't echo */
    int  show_indicator; /* show [LOCAL ECHO] in status bar */
    int  color; /* color for locally echoed text */
    char password_mask; /* char to show for password fields */
    int  detect_password_prompt;
} LocalEcho;

void localecho_init(LocalEcho *le);
int  localecho_should_echo(LocalEcho *le, const char *last_output);
void localecho_toggle(LocalEcho *le);
char localecho_mask_char(LocalEcho *le);

#endif
