#include "puttyalt_locale.h"
#include <stdio.h>
#include <string.h>

void locale_init(Locale *loc)
{
    memset(loc, 0, sizeof(*loc));
    loc->active = -1;
    snprintf(loc->fallback, sizeof(loc->fallback), "en");
}

int locale_add_lang(Locale *loc, const char *code, const char *name)
{
    if (loc->lang_count >= L10N_MAX_LANGS) return -1;
    L10NLang *l = &loc->languages[loc->lang_count];
    memset(l, 0, sizeof(*l));
    snprintf(l->code, sizeof(l->code), "%s", code);
    snprintf(l->name, L10N_MAX_LNAME, "%s", name);
    return loc->lang_count++;
}

static int find_lang(const Locale *loc, const char *code)
{
    for (int i = 0; i < loc->lang_count; i++)
        if (strcmp(loc->languages[i].code, code) == 0) return i;
    return -1;
}

int locale_set_string(Locale *loc, int lang, const char *key, const char *value)
{
    if (lang < 0 || lang >= loc->lang_count) return -1;
    L10NLang *l = &loc->languages[lang];
    /* Update existing */
    for (int i = 0; i < l->count; i++) {
        if (strcmp(l->strings[i].key, key) == 0) {
            snprintf(l->strings[i].value, L10N_MAX_VALUE, "%s", value);
            return i;
        }
    }
    if (l->count >= L10N_MAX_STRINGS) return -1;
    L10NString *s = &l->strings[l->count];
    snprintf(s->key, L10N_MAX_KEY, "%s", key);
    snprintf(s->value, L10N_MAX_VALUE, "%s", value);
    return l->count++;
}

const char *locale_get(const Locale *loc, const char *key)
{
    int lang = loc->active;
    if (lang < 0) lang = find_lang(loc, loc->fallback);
    if (lang < 0) return key;

    const L10NLang *l = &loc->languages[lang];
    for (int i = 0; i < l->count; i++)
        if (strcmp(l->strings[i].key, key) == 0) return l->strings[i].value;

    /* Fallback */
    if (lang != find_lang(loc, loc->fallback)) {
        int fb = find_lang(loc, loc->fallback);
        if (fb >= 0) {
            const L10NLang *fl = &loc->languages[fb];
            for (int i = 0; i < fl->count; i++)
                if (strcmp(fl->strings[i].key, key) == 0) return fl->strings[i].value;
        }
    }
    return key;
}

int locale_set_active(Locale *loc, const char *code)
{
    int idx = find_lang(loc, code);
    if (idx < 0) return -1;
    loc->active = idx;
    return 0;
}

void locale_load_defaults(Locale *loc)
{
    int en = locale_add_lang(loc, "en", "English");
    locale_set_string(loc, en, "app.title", "PuttyAlt - SSH Terminal");
    locale_set_string(loc, en, "menu.file", "File");
    locale_set_string(loc, en, "menu.edit", "Edit");
    locale_set_string(loc, en, "menu.view", "View");
    locale_set_string(loc, en, "menu.session", "Session");
    locale_set_string(loc, en, "menu.tools", "Tools");
    locale_set_string(loc, en, "menu.help", "Help");
    locale_set_string(loc, en, "btn.connect", "Connect");
    locale_set_string(loc, en, "btn.disconnect", "Disconnect");
    locale_set_string(loc, en, "btn.save", "Save");
    locale_set_string(loc, en, "btn.cancel", "Cancel");
    locale_set_string(loc, en, "status.connected", "Connected to %s");
    locale_set_string(loc, en, "status.disconnected", "Disconnected");

    int ru = locale_add_lang(loc, "ru", "Russian");
    locale_set_string(loc, ru, "app.title", "PuttyAlt - SSH Terminal");
    locale_set_string(loc, ru, "menu.file", "Файл");
    locale_set_string(loc, ru, "menu.edit", "Редактировать");
    locale_set_string(loc, ru, "menu.view", "Вид");
    locale_set_string(loc, ru, "menu.session", "Сессия");
    locale_set_string(loc, ru, "menu.tools", "Инструменты");
    locale_set_string(loc, ru, "menu.help", "Справка");
    locale_set_string(loc, ru, "btn.connect", "Подключить");
    locale_set_string(loc, ru, "btn.disconnect", "Отключить");
    locale_set_string(loc, ru, "btn.save", "Сохранить");
    locale_set_string(loc, ru, "btn.cancel", "Отмена");

    int de = locale_add_lang(loc, "de", "German");
    locale_set_string(loc, de, "menu.file", "Datei");
    locale_set_string(loc, de, "menu.edit", "Bearbeiten");
    locale_set_string(loc, de, "menu.view", "Ansicht");
    locale_set_string(loc, de, "btn.connect", "Verbinden");
    locale_set_string(loc, de, "btn.disconnect", "Trennen");

    int zh = locale_add_lang(loc, "zh", "Chinese");
    locale_set_string(loc, zh, "menu.file", "文件");
    locale_set_string(loc, zh, "menu.edit", "编辑");
    locale_set_string(loc, zh, "menu.view", "视图");
    locale_set_string(loc, zh, "btn.connect", "连接");

    int ja = locale_add_lang(loc, "ja", "Japanese");
    locale_set_string(loc, ja, "menu.file", "ファイル");
    locale_set_string(loc, ja, "menu.edit", "編集");
    locale_set_string(loc, ja, "btn.connect", "接続");

    loc->active = 0;
}

int locale_load(Locale *loc, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[512];
    if (!f) return -1;
    locale_init(loc);
    int cur_lang = -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) line[--len] = '\0';
        if (strncmp(line, "[lang:", 6) == 0) {
            char *end = strchr(line + 6, ']');
            if (end) *end = '\0';
            cur_lang = find_lang(loc, line + 6);
            if (cur_lang < 0) cur_lang = locale_add_lang(loc, line + 6, line + 6);
        } else if (cur_lang >= 0 && strchr(line, '=')) {
            char *eq = strchr(line, '=');
            *eq = '\0';
            locale_set_string(loc, cur_lang, line, eq + 1);
        }
    }
    fclose(f);
    return 0;
}

int locale_save(const Locale *loc, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < loc->lang_count; i++) {
        const L10NLang *l = &loc->languages[i];
        fprintf(f, "[lang:%s]\n", l->code);
        for (int j = 0; j < l->count; j++)
            fprintf(f, "%s=%s\n", l->strings[j].key, l->strings[j].value);
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
