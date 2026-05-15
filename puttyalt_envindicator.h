/*
 * puttyalt_envindicator.h: Visual environment indicator.
 *
 * Adds a coloured stripe at the top/bottom of the terminal window
 * to clearly distinguish production, staging, and development
 * sessions.  Helps prevent "oops I ran that on prod" moments.
 */

#ifndef PUTTYALT_ENVINDI_H
#define PUTTYALT_ENVINDI_H

#define MAX_ENV_RULES  32
#define MAX_ENV_LABEL  64

typedef struct EnvColour {
    unsigned char r, g, b;
} EnvColour;

typedef enum {
    ENV_MATCH_HOSTNAME,     /* match against hostname */
    ENV_MATCH_SESSION_NAME, /* match against session name */
    ENV_MATCH_TAG,          /* match against session tag */
    ENV_MATCH_PORT          /* match against port number */
} EnvMatchType;

typedef struct EnvRule {
    char         pattern[128];      /* substring to match */
    EnvMatchType match_type;
    char         label[MAX_ENV_LABEL]; /* e.g. "PRODUCTION" */
    EnvColour    colour;            /* stripe colour */
    int          blink;             /* 1 = blink the stripe for extra warning */
    int          enabled;
} EnvRule;

typedef struct EnvIndicator {
    EnvRule rules[MAX_ENV_RULES];
    int     count;
    int     show_label;     /* overlay text label on the stripe */
    int     stripe_height;  /* pixels, default 4 */
    int     position;       /* 0 = top, 1 = bottom, 2 = both */
} EnvIndicator;

/* Initialise with sensible defaults */
void envind_init(EnvIndicator *ei);

/* Add a rule.  Returns index or -1. */
int envind_add_rule(EnvIndicator *ei, const EnvRule *rule);

/* Remove a rule by index */
int envind_remove_rule(EnvIndicator *ei, int index);

/* Evaluate a session against all rules.
 * Returns pointer to the first matching rule, or NULL. */
const EnvRule *envind_evaluate(const EnvIndicator *ei,
                               const char *hostname,
                               const char *session_name,
                               const char *tag,
                               int port);

/* Load / save rules from config file */
int envind_load(EnvIndicator *ei, const char *path);
int envind_save(const EnvIndicator *ei, const char *path);

/* Add built-in default rules (prod=red, staging=yellow, dev=green) */
void envind_add_defaults(EnvIndicator *ei);

#endif /* PUTTYALT_ENVINDI_H */
