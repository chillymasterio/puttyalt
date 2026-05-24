#ifndef PUTTYALT_WIZARD_H
#define PUTTYALT_WIZARD_H
#include "puttyalt_gui.h"

typedef enum { WIZ_WELCOME, WIZ_THEME, WIZ_FONT, WIZ_BEHAVIOR, WIZ_DONE } WizardPage;

typedef struct {
    WizardPage current_page;
    int total_pages;
    int completed;
    GUIConfig preview_config;
    int first_run;
} SetupWizard;

void wizard_init(SetupWizard *wiz, GUIConfig *cfg);
int  wizard_is_first_run(void);
int  wizard_next(SetupWizard *wiz);
int  wizard_prev(SetupWizard *wiz);
int  wizard_apply(SetupWizard *wiz, GUIConfig *cfg);
const char *wizard_page_title(WizardPage page);
const char *wizard_page_description(WizardPage page);

#endif
