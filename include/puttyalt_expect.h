#ifndef PUTTYALT_EXPECT_H
#define PUTTYALT_EXPECT_H

#define EXPECT_MAX_RULES    128
#define EXPECT_MAX_PATTERN  256
#define EXPECT_MAX_RESPONSE 512
#define EXPECT_MAX_SCRIPTS  32

typedef enum {
    EXPECT_EXACT,
    EXPECT_CONTAINS,
    EXPECT_REGEX,
    EXPECT_TIMEOUT
} ExpectMatchType;

typedef enum {
    EXPECT_ACTION_SEND,
    EXPECT_ACTION_SEND_LINE,
    EXPECT_ACTION_WAIT,
    EXPECT_ACTION_LOG,
    EXPECT_ACTION_GOTO,
    EXPECT_ACTION_STOP
} ExpectAction;

typedef struct {
    ExpectMatchType match_type;
    char pattern[EXPECT_MAX_PATTERN];
    ExpectAction action;
    char response[EXPECT_MAX_RESPONSE];
    int timeout_ms;
    int next_rule;
    int repeat_count;
    int matched_count;
} ExpectRule;

typedef struct {
    char name[64];
    ExpectRule rules[EXPECT_MAX_RULES];
    int rule_count;
    int current_rule;
    int running;
    int paused;
    long started_at;
    int matches_total;
    char last_match[EXPECT_MAX_PATTERN];
} ExpectScript;

typedef struct {
    ExpectScript scripts[EXPECT_MAX_SCRIPTS];
    int script_count;
    int active_script;
    int global_timeout_ms;
    int log_matches;
} ExpectEngine;

int  expect_init(ExpectEngine *ee);
void expect_destroy(ExpectEngine *ee);
int  expect_add_script(ExpectEngine *ee, const char *name);
int  expect_add_rule(ExpectEngine *ee, int script_idx, ExpectMatchType type,
                     const char *pattern, ExpectAction action,
                     const char *response, int timeout_ms);
int  expect_start(ExpectEngine *ee, int script_idx);
int  expect_stop(ExpectEngine *ee, int script_idx);
int  expect_feed(ExpectEngine *ee, const char *data, int len,
                 char *response_out, int response_max);
int  expect_load_file(ExpectEngine *ee, const char *path);

#endif
