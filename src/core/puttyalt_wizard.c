#include "puttyalt_wizard.h"
#include <string.h>
#include <stdio.h>

void wizard_init(SetupWizard *wiz, GUIConfig *cfg)
{
    memset(wiz, 0, sizeof(*wiz));
    wiz->current_page = WIZ_WELCOME;
    wiz->total_pages = 5;
    if (cfg) wiz->preview_config = *cfg;
    wiz->first_run = wizard_is_first_run();
}

int wizard_is_first_run(void)
{
    FILE *f = fopen("puttyalt.conf", "r");
    if (f) { fclose(f); return 0; }
    return 1;
}

int wizard_next(SetupWizard *wiz)
{
    if (wiz->current_page < WIZ_DONE) { wiz->current_page++; return 0; }
    return -1;
}

int wizard_prev(SetupWizard *wiz)
{
    if (wiz->current_page > WIZ_WELCOME) { wiz->current_page--; return 0; }
    return -1;
}

int wizard_apply(SetupWizard *wiz, GUIConfig *cfg)
{
    *cfg = wiz->preview_config;
    wiz->completed = 1;
    return 0;
}

const char *wizard_page_title(WizardPage page)
{
    switch (page) {
    case WIZ_WELCOME: return "Welcome to PuttyAlt";
    case WIZ_THEME: return "Choose Your Theme";
    case WIZ_FONT: return "Select Terminal Font";
    case WIZ_BEHAVIOR: return "Configure Behavior";
    case WIZ_DONE: return "Setup Complete";
    }
    return "";
}

const char *wizard_page_description(WizardPage page)
{
    switch (page) {
    case WIZ_WELCOME: return "Let's set up PuttyAlt to work just the way you like it.";
    case WIZ_THEME: return "Pick a color scheme for the terminal and interface.";
    case WIZ_FONT: return "Choose a monospace font and size for the terminal.";
    case WIZ_BEHAVIOR: return "Configure clipboard, bell, and session behavior.";
    case WIZ_DONE: return "You're all set! Start connecting to your servers.";
    }
    return "";
}
