#ifndef PUTTYALT_CMDPALETTE_H
#define PUTTYALT_CMDPALETTE_H

#define CP_MAX_COMMANDS  256
#define CP_MAX_NAME      128
#define CP_MAX_DESC      256
#define CP_MAX_RESULTS   32

typedef struct {
    char  name[CP_MAX_NAME];
    char  description[CP_MAX_DESC];
    char  category[32];
    char  shortcut[32];
    int   id;
    long  use_count;
    int   hidden;
} CPCommand;

typedef struct {
    CPCommand commands[CP_MAX_COMMANDS];
    int       count;
    CPCommand results[CP_MAX_RESULTS];
    int       result_count;
    char      query[CP_MAX_NAME];
    int       selected;
    int       visible;
} CmdPalette;

void cmdpal_init(CmdPalette *cp);
int  cmdpal_register(CmdPalette *cp, const char *name, const char *desc,
                     const char *category, const char *shortcut, int id);
int  cmdpal_search(CmdPalette *cp, const char *query);
int  cmdpal_execute(CmdPalette *cp);
void cmdpal_show(CmdPalette *cp);
void cmdpal_hide(CmdPalette *cp);
void cmdpal_select_next(CmdPalette *cp);
void cmdpal_select_prev(CmdPalette *cp);
void cmdpal_load_defaults(CmdPalette *cp);

#endif
