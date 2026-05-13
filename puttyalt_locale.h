#ifndef PUTTYALT_LOCALE_H
#define PUTTYALT_LOCALE_H

#define L10N_MAX_STRINGS  512
#define L10N_MAX_KEY      64
#define L10N_MAX_VALUE    256
#define L10N_MAX_LANGS    16
#define L10N_MAX_LNAME    32

typedef struct {
    char key[L10N_MAX_KEY];
    char value[L10N_MAX_VALUE];
} L10NString;

typedef struct {
    char       code[8];
    char       name[L10N_MAX_LNAME];
    L10NString strings[L10N_MAX_STRINGS];
    int        count;
} L10NLang;

typedef struct {
    L10NLang languages[L10N_MAX_LANGS];
    int      lang_count;
    int      active;
    char     fallback[8];
} Locale;

void locale_init(Locale *loc);
int  locale_add_lang(Locale *loc, const char *code, const char *name);
int  locale_set_string(Locale *loc, int lang, const char *key, const char *value);
const char *locale_get(const Locale *loc, const char *key);
int  locale_set_active(Locale *loc, const char *code);
int  locale_load(Locale *loc, const char *path);
int  locale_save(const Locale *loc, const char *path);
void locale_load_defaults(Locale *loc);

#endif
