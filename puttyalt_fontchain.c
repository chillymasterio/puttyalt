#include "puttyalt_fontchain.h"
#include <string.h>
#include <stdio.h>

void fontchain_init(FontChain *fc)
{
    memset(fc, 0, sizeof(*fc));
    fc->size = 11;
    fc->antialias = 1;
    fc->hinting = 1;
    fc->line_spacing = 1.2f;
    fc->letter_spacing = 0.0f;
    fontchain_add(fc, "Cascadia Code");
    fontchain_add(fc, "Consolas");
    fontchain_add(fc, "Courier New");
}

int fontchain_add(FontChain *fc, const char *name)
{
    if (fc->count >= FONT_CHAIN_MAX) return -1;
    snprintf(fc->names[fc->count], 64, "%s", name);
    fc->count++;
    return 0;
}

void fontchain_remove(FontChain *fc, int index)
{
    if (index < 0 || index >= fc->count) return;
    for (int i = index; i < fc->count - 1; i++)
        memcpy(fc->names[i], fc->names[i + 1], 64);
    fc->count--;
}

void fontchain_set_size(FontChain *fc, int size) { if (size >= 6 && size <= 72) fc->size = size; }
const char *fontchain_get(FontChain *fc, int index) { return (index >= 0 && index < fc->count) ? fc->names[index] : NULL; }

int fontchain_load(FontChain *fc, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[128];
    fc->count = 0;
    while (fgets(line, sizeof(line), f) && fc->count < FONT_CHAIN_MAX) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '#' || line[0] == '\0') continue;
        if (strncmp(line, "size=", 5) == 0) fc->size = atoi(line + 5);
        else if (strncmp(line, "antialias=", 10) == 0) fc->antialias = atoi(line + 10);
        else fontchain_add(fc, line);
    }
    fclose(f);
    return 0;
}

int fontchain_save(const FontChain *fc, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    fprintf(f, "# Font fallback chain\nsize=%d\nantialias=%d\n", fc->size, fc->antialias);
    for (int i = 0; i < fc->count; i++) fprintf(f, "%s\n", fc->names[i]);
    fclose(f);
    return 0;
}
