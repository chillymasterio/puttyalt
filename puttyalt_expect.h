#ifndef PUTTYALT_EXPECT_H
#define PUTTYALT_EXPECT_H

#define EXPECT_MAX_RULES   64
#define EXPECT_MAX_PATTERN 512
#define EXPECT_MAX_RESPONSE 1024

typedef enum {
    EXPECT_ACT_SEND,
    EXPECT_ACT_SEND_LINE,
    EXPECT_ACT_WAIT,
    EXPECT_ACT_LOG,
    EXPECT_ACT_BREAK
} ExpectAction;

typedef struct {
    char pattern[EXPECT_MAX_PATTERN];
    char response[EXPECT_MAX_RESPONSE];
    ExpectAction action;
    int timeout_ms;
    int case_sensitive;
    int once;           /* fire only once */
    int fired;
} ExpectRule;

typedef struct {
    ExpectRule rules[EXPECT_MAX_RULES];
    int count;
    int active;
    int current_rule;
    char buffer[4096];  /* accumulation buffer */
    int buf_len;
    int global_timeout_ms;
} ExpectEngine;

void expect_init(ExpectEngine *e);
int  expect_add_rule(ExpectEngine *e, const char *pattern,
                     const char *response, ExpectAction action);
int  expect_feed(ExpectEngine *e, const char *data, int len,
                 char *send_out, int max_out);
void expect_reset(ExpectEngine *e);
void expect_start(ExpectEngine *e);
void expect_stop(ExpectEngine *e);

#endif
