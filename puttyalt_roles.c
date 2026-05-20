#include "puttyalt_roles.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int pattern_match(const char *pattern, const char *text)
{
    /* Simple wildcard matching: * matches any, ? matches one */
    while (*pattern && *text) {
        if (*pattern == '*') {
            pattern++;
            if (!*pattern) return 1;
            while (*text) {
                if (pattern_match(pattern, text)) return 1;
                text++;
            }
            return 0;
        }
        if (*pattern == '?' || *pattern == *text) {
            pattern++;
            text++;
        } else {
            return 0;
        }
    }
    while (*pattern == '*') pattern++;
    return !*pattern && !*text;
}

int role_init(RoleManager *rm)
{
    memset(rm, 0, sizeof(*rm));
    rm->enforcement = 1;  /* warn by default */
    return 0;
}

void role_destroy(RoleManager *rm)
{
    memset(rm, 0, sizeof(*rm));
}

int role_add_profile(RoleManager *rm, const char *name, const char *desc)
{
    if (rm->profile_count >= ROLE_MAX_PROFILES) return -1;
    RoleProfile *p = &rm->profiles[rm->profile_count];
    memset(p, 0, sizeof(*p));
    snprintf(p->name, sizeof(p->name), "%s", name);
    if (desc) snprintf(p->description, sizeof(p->description), "%s", desc);
    p->active = 1;
    rm->profile_count++;
    return rm->profile_count - 1;
}

int role_add_rule(RoleManager *rm, int profile_idx, RuleType type,
                  RoleAction action, const char *pattern, int min, int max)
{
    if (profile_idx < 0 || profile_idx >= rm->profile_count) return -1;
    RoleProfile *p = &rm->profiles[profile_idx];
    if (p->rule_count >= ROLE_MAX_RULES) return -1;
    RoleRule *r = &p->rules[p->rule_count];
    r->type = type;
    r->action = action;
    if (pattern) snprintf(r->pattern, sizeof(r->pattern), "%s", pattern);
    r->value_min = min;
    r->value_max = max;
    p->rule_count++;
    return p->rule_count - 1;
}

RoleAction role_check_host(const RoleManager *rm, const char *host, int port)
{
    if (!rm->enforcement || rm->active_profile < 0) return ROLE_ALLOW;
    const RoleProfile *p = &rm->profiles[rm->active_profile];
    if (!p->active) return ROLE_ALLOW;

    for (int i = 0; i < p->rule_count; i++) {
        const RoleRule *r = &p->rules[i];
        if (r->type == RULE_HOST_PATTERN) {
            if (pattern_match(r->pattern, host))
                return r->action;
        } else if (r->type == RULE_PORT_RANGE) {
            if (port >= r->value_min && port <= r->value_max)
                return r->action;
        }
    }
    return ROLE_ALLOW;
}

RoleAction role_check_command(const RoleManager *rm, const char *cmd)
{
    if (!rm->enforcement || rm->active_profile < 0) return ROLE_ALLOW;
    const RoleProfile *p = &rm->profiles[rm->active_profile];
    if (!p->active) return ROLE_ALLOW;

    for (int i = 0; i < p->rule_count; i++) {
        const RoleRule *r = &p->rules[i];
        if (r->type == RULE_COMMAND_FILTER) {
            if (pattern_match(r->pattern, cmd))
                return r->action;
        }
    }
    return ROLE_ALLOW;
}

int role_activate(RoleManager *rm, int profile_idx)
{
    if (profile_idx < 0 || profile_idx >= rm->profile_count) return -1;
    rm->active_profile = profile_idx;
    return 0;
}

int role_load(RoleManager *rm, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;
    char line[512];
    int cur = -1;
    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (line[0] == '[') {
            char *end = strchr(line, ']');
            if (end) {
                *end = '\0';
                cur = role_add_profile(rm, line + 1, NULL);
            }
        } else if (cur >= 0 && strncmp(line, "deny=", 5) == 0) {
            role_add_rule(rm, cur, RULE_HOST_PATTERN, ROLE_DENY, line + 5, 0, 0);
        } else if (cur >= 0 && strncmp(line, "warn=", 5) == 0) {
            role_add_rule(rm, cur, RULE_HOST_PATTERN, ROLE_WARN, line + 5, 0, 0);
        } else if (cur >= 0 && strncmp(line, "blockcmd=", 9) == 0) {
            role_add_rule(rm, cur, RULE_COMMAND_FILTER, ROLE_DENY, line + 9, 0, 0);
        }
    }
    fclose(f);
    return 0;
}

int role_save(const RoleManager *rm, const char *path)
{
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    for (int i = 0; i < rm->profile_count; i++) {
        const RoleProfile *p = &rm->profiles[i];
        fprintf(f, "[%s]\n", p->name);
        if (p->description[0])
            fprintf(f, "# %s\n", p->description);
        for (int j = 0; j < p->rule_count; j++) {
            const RoleRule *r = &p->rules[j];
            const char *prefix = r->action == ROLE_DENY ? "deny" :
                                 r->action == ROLE_WARN ? "warn" : "allow";
            if (r->type == RULE_HOST_PATTERN)
                fprintf(f, "%s=%s\n", prefix, r->pattern);
            else if (r->type == RULE_COMMAND_FILTER)
                fprintf(f, "blockcmd=%s\n", r->pattern);
        }
        fprintf(f, "\n");
    }
    fclose(f);
    return 0;
}
