/*
 * puttyalt_envindicator.c: Visual environment indicator implementation.
 */

#include "puttyalt_envindicator.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void envind_init(EnvIndicator *ei)
{
    memset(ei, 0, sizeof(*ei));
    ei->stripe_height = 4;
    ei->show_label = 1;
    ei->position = 0;  /* top */
}

int envind_add_rule(EnvIndicator *ei, const EnvRule *rule)
{
    if (ei->count >= MAX_ENV_RULES)
        return -1;
    memcpy(&ei->rules[ei->count], rule, sizeof(EnvRule));
    return ei->count++;
}

int envind_remove_rule(EnvIndicator *ei, int index)
{
    if (index < 0 || index >= ei->count)
        return -1;
    for (int i = index; i < ei->count - 1; i++)
        ei->rules[i] = ei->rules[i + 1];
    ei->count--;
    return 0;
}

/*
 * Case-insensitive substring match.
 */
static int contains_ci(const char *haystack, const char *needle)
{
    if (!haystack || !needle || !*needle)
        return 0;

    size_t nlen = strlen(needle);
    for (; *haystack; haystack++) {
        if (tolower((unsigned char)*haystack) ==
            tolower((unsigned char)*needle)) {
            size_t i;
            for (i = 1; i < nlen; i++) {
                if (tolower((unsigned char)haystack[i]) !=
                    tolower((unsigned char)needle[i]))
                    break;
            }
            if (i == nlen)
                return 1;
        }
    }
    return 0;
}

const EnvRule *envind_evaluate(const EnvIndicator *ei,
                               const char *hostname,
                               const char *session_name,
                               const char *tag,
                               int port)
{
    for (int i = 0; i < ei->count; i++) {
        const EnvRule *r = &ei->rules[i];
        if (!r->enabled)
            continue;

        int matched = 0;
        switch (r->match_type) {
        case ENV_MATCH_HOSTNAME:
            matched = contains_ci(hostname, r->pattern);
            break;
        case ENV_MATCH_SESSION_NAME:
            matched = contains_ci(session_name, r->pattern);
            break;
        case ENV_MATCH_TAG:
            matched = contains_ci(tag, r->pattern);
            break;
        case ENV_MATCH_PORT:
            {
                int rport = atoi(r->pattern);
                matched = (port == rport);
            }
            break;
        }

        if (matched)
            return r;
    }
    return NULL;
}

void envind_add_defaults(EnvIndicator *ei)
{
    EnvRule prod = {0};
    snprintf(prod.pattern, sizeof(prod.pattern), "prod");
    prod.match_type = ENV_MATCH_HOSTNAME;
    snprintf(prod.label, sizeof(prod.label), "PRODUCTION");
    prod.colour = (EnvColour){220, 40, 40};
    prod.blink = 1;
    prod.enabled = 1;
    envind_add_rule(ei, &prod);

    EnvRule staging = {0};
    snprintf(staging.pattern, sizeof(staging.pattern), "staging");
    staging.match_type = ENV_MATCH_HOSTNAME;
    snprintf(staging.label, sizeof(staging.label), "STAGING");
    staging.colour = (EnvColour){230, 180, 30};
    staging.blink = 0;
    staging.enabled = 1;
    envind_add_rule(ei, &staging);

    EnvRule dev = {0};
    snprintf(dev.pattern, sizeof(dev.pattern), "dev");
    dev.match_type = ENV_MATCH_HOSTNAME;
    snprintf(dev.label, sizeof(dev.label), "DEVELOPMENT");
    dev.colour = (EnvColour){40, 180, 60};
    dev.blink = 0;
    dev.enabled = 1;
    envind_add_rule(ei, &dev);
}

int envind_load(EnvIndicator *ei, const char *path)
{
    FILE *f = fopen(path, "r");
    char line[1024];
    EnvRule *cur = NULL;

    if (!f) return -1;

    envind_init(ei);

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';

        if (strcmp(line, "[env-rule]") == 0) {
            if (ei->count >= MAX_ENV_RULES) break;
            cur = &ei->rules[ei->count++];
            memset(cur, 0, sizeof(*cur));
            cur->enabled = 1;
            continue;
        }
        if (!cur) continue;

        if (strncmp(line, "pattern=", 8) == 0)
            snprintf(cur->pattern, sizeof(cur->pattern), "%s", line + 8);
        else if (strncmp(line, "match=", 6) == 0)
            cur->match_type = atoi(line + 6);
        else if (strncmp(line, "label=", 6) == 0)
            snprintf(cur->label, sizeof(cur->label), "%s", line + 6);
        else if (strncmp(line, "colour=", 7) == 0) {
            unsigned int r, g, b;
            if (sscanf(line + 7, "%u,%u,%u", &r, &g, &b) == 3) {
                cur->colour.r = (unsigned char)r;
                cur->colour.g = (unsigned char)g;
                cur->colour.b = (unsigned char)b;
            }
        }
        else if (strncmp(line, "blink=", 6) == 0)
            cur->blink = atoi(line + 6);
        else if (strncmp(line, "enabled=", 8) == 0)
            cur->enabled = atoi(line + 8);
    }

    fclose(f);
    return 0;
}

int envind_save(const EnvIndicator *ei, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;

    for (int i = 0; i < ei->count; i++) {
        const EnvRule *r = &ei->rules[i];
        fprintf(f, "[env-rule]\n");
        fprintf(f, "pattern=%s\n", r->pattern);
        fprintf(f, "match=%d\n", r->match_type);
        fprintf(f, "label=%s\n", r->label);
        fprintf(f, "colour=%u,%u,%u\n", r->colour.r, r->colour.g, r->colour.b);
        fprintf(f, "blink=%d\n", r->blink);
        fprintf(f, "enabled=%d\n", r->enabled);
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}
