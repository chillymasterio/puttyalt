#ifndef PUTTYALT_CFGVALID_H
#define PUTTYALT_CFGVALID_H
#include "puttyalt_gui.h"

typedef struct {
    int error_count;
    int warning_count;
    char messages[32][256];
    int msg_count;
} ValidationResult;

void cfg_validate(const GUIConfig *cfg, ValidationResult *result);
int  cfg_validate_host(const char *host);
int  cfg_validate_port(int port);
int  cfg_validate_font(const char *name, int size);
int  cfg_validate_color(unsigned int color);
void cfg_sanitize(GUIConfig *cfg);

#endif
