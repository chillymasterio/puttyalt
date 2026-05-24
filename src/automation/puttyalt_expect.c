#include "puttyalt_expect.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

int expect_init(ExpectEngine *ee)
{
    memset(ee, 0, sizeof(*ee));
    ee->global_timeout_ms = 30000;
    ee->log_matches = 1;
    ee->active_script = -1;
    return 0;
}

void expect_destroy(ExpectEngine *ee)
{
    memset(ee, 0, sizeof(*ee));
}

int expect_add_script(ExpectEngine *ee, const char *name)
{
    if (ee->script_count >= EXPECT_MAX_SCRIPTS) return -1;
    ExpectScript *s = &ee->scripts[ee->script_count];
    memset(s, 0, sizeof(*s));
    snprintf(s->name, sizeof(s->name), "%s", name ? name : "unnamed");
    ee->script_count++;
    return ee->script_count - 1;
}

int expect_add_rule(ExpectEngine *ee, int script_idx, ExpectMatchType type,
                    const char *pattern, ExpectAction action,
                    const char *response, int timeout_ms)
{
    if (script_idx < 0 || script_idx >= ee->script_count) return -1;
    ExpectScript *s = &ee->scripts[script_idx];
    if (s->rule_count >= EXPECT_MAX_RULES) return -1;

    ExpectRule *r = &s->rules[s->rule_count];
    memset(r, 0, sizeof(*r));
    r->match_type = type;
    if (pattern) snprintf(r->pattern, sizeof(r->pattern), "%s", pattern);
    r->action = action;
    if (response) snprintf(r->response, sizeof(r->response), "%s", response);
    r->timeout_ms = timeout_ms > 0 ? timeout_ms : ee->global_timeout_ms;
    r->next_rule = s->rule_count + 1;

    s->rule_count++;
    return s->rule_count - 1;
}

int expect_start(ExpectEngine *ee, int script_idx)
{
    if (script_idx < 0 || script_idx >= ee->script_count) return -1;
    ExpectScript *s = &ee->scripts[script_idx];
    s->running = 1;
    s->paused = 0;
    s->current_rule = 0;
    s->started_at = (long)time(NULL);
    s->matches_total = 0;
    ee->active_script = script_idx;
    return 0;
}

int expect_stop(ExpectEngine *ee, int script_idx)
{
    if (script_idx < 0 || script_idx >= ee->script_count) return -1;
    ee->scripts[script_idx].running = 0;
    if (ee->active_script == script_idx)
        ee->active_script = -1;
    return 0;
}

static int match_rule(const ExpectRule *r, const char *data, int len)
{
    switch (r->match_type) {
    case EXPECT_EXACT:
        return len == (int)strlen(r->pattern) &&
               memcmp(data, r->pattern, len) == 0;
    case EXPECT_CONTAINS:
        return len > 0 && strstr(data, r->pattern) != NULL;
    case EXPECT_REGEX:
        /* Simplified: treat as contains */
        return strstr(data, r->pattern) != NULL;
    case EXPECT_TIMEOUT:
        return 0;  /* handled separately */
    }
    return 0;
}

int expect_feed(ExpectEngine *ee, const char *data, int len,
                char *response_out, int response_max)
{
    if (ee->active_script < 0) return 0;
    ExpectScript *s = &ee->scripts[ee->active_script];
    if (!s->running || s->paused) return 0;
    if (s->current_rule >= s->rule_count) {
        s->running = 0;
        return 0;
    }

    ExpectRule *r = &s->rules[s->current_rule];
    if (!match_rule(r, data, len)) return 0;

    /* Match found */
    r->matched_count++;
    s->matches_total++;
    snprintf(s->last_match, sizeof(s->last_match), "%s", r->pattern);

    /* Execute action */
    switch (r->action) {
    case EXPECT_ACTION_SEND:
    case EXPECT_ACTION_SEND_LINE:
        if (response_out && r->response[0]) {
            int rlen = snprintf(response_out, response_max, "%s%s",
                               r->response,
                               r->action == EXPECT_ACTION_SEND_LINE ? "\n" : "");
            s->current_rule = r->next_rule;
            return rlen;
        }
        break;
    case EXPECT_ACTION_WAIT:
        /* Just advance */
        s->current_rule = r->next_rule;
        break;
    case EXPECT_ACTION_LOG:
        s->current_rule = r->next_rule;
        break;
    case EXPECT_ACTION_GOTO:
        s->current_rule = r->next_rule;
        break;
    case EXPECT_ACTION_STOP:
        s->running = 0;
        break;
    }

    return 0;
}

int expect_load_file(ExpectEngine *ee, const char *path)
{
    FILE *f = fopen(path, "r");
    if (!f) return -1;

    char line[1024];
    int script_idx = -1;

    while (fgets(line, sizeof(line), f)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (!line[0] || line[0] == '#') continue;

        if (strncmp(line, "script:", 7) == 0) {
            script_idx = expect_add_script(ee, line + 7);
        } else if (script_idx >= 0 && strncmp(line, "expect:", 7) == 0) {
            char *arrow = strstr(line + 7, " -> ");
            if (arrow) {
                *arrow = '\0';
                expect_add_rule(ee, script_idx, EXPECT_CONTAINS,
                               line + 7, EXPECT_ACTION_SEND_LINE,
                               arrow + 4, 0);
            }
        }
    }
    fclose(f);
    return 0;
}
