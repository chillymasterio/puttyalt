#ifndef PUTTYALT_INI_H
#define PUTTYALT_INI_H

#define INI_MAX_SECTIONS  128
#define INI_MAX_KEYS      32
#define INI_KEY_LEN       64
#define INI_VAL_LEN       512
#define INI_SECTION_LEN   64

typedef struct INIEntry {
    char key[INI_KEY_LEN];
    char value[INI_VAL_LEN];
} INIEntry;

typedef struct INISection {
    char name[INI_SECTION_LEN];
    INIEntry entries[INI_MAX_KEYS];
    int count;
} INISection;

typedef struct INIFile {
    INISection sections[INI_MAX_SECTIONS];
    int num_sections;
    char path[512];
    int modified;
} INIFile;

int  ini_load(INIFile *ini, const char *path);
int  ini_save(const INIFile *ini);
const char *ini_get(const INIFile *ini, const char *section, const char *key);
int  ini_get_int(const INIFile *ini, const char *section, const char *key, int def);
int  ini_set(INIFile *ini, const char *section, const char *key, const char *value);
int  ini_set_int(INIFile *ini, const char *section, const char *key, int value);
int  ini_delete_key(INIFile *ini, const char *section, const char *key);
int  ini_delete_section(INIFile *ini, const char *section);

#endif
