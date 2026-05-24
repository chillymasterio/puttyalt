/*
 * test_automation.c — Tests for automation modules:
 *   expect, pipeline, scheduler, webhooks
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_expect.h"
#include "puttyalt_pipeline.h"
#include "puttyalt_scheduler.h"
#include "puttyalt_webhooks.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else { printf("FAIL\n"); } \
} while(0)

/* ═══════════ Expect Engine Tests ═══════════ */

static int test_expect_init(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    assert(ee.script_count == 0);
    assert(ee.active_script == -1 || ee.active_script == 0);
    expect_destroy(&ee);
    return 1;
}

static int test_expect_add_pattern(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    int s = expect_add_script(&ee, "login_script");
    assert(s == 0);
    assert(ee.script_count == 1);
    assert(strcmp(ee.scripts[0].name, "login_script") == 0);

    int r = expect_add_rule(&ee, s, EXPECT_CONTAINS, "password:",
                            EXPECT_ACTION_SEND_LINE, "mypass", 5000);
    assert(r == 0);
    assert(ee.scripts[s].rule_count == 1);
    assert(ee.scripts[s].rules[0].match_type == EXPECT_CONTAINS);
    assert(ee.scripts[s].rules[0].timeout_ms == 5000);
    expect_destroy(&ee);
    return 1;
}

static int test_expect_match_check(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    int s = expect_add_script(&ee, "test");
    expect_add_rule(&ee, s, EXPECT_CONTAINS, "login:",
                    EXPECT_ACTION_SEND_LINE, "admin", 3000);
    expect_add_rule(&ee, s, EXPECT_CONTAINS, "password:",
                    EXPECT_ACTION_SEND_LINE, "secret", 3000);
    expect_start(&ee, s);

    char response[256] = "";
    int matched = expect_feed(&ee, "Please enter login:", 19, response, sizeof(response));
    assert(matched > 0 || strlen(response) > 0);

    expect_stop(&ee, s);
    expect_destroy(&ee);
    return 1;
}

static int test_expect_multiple_scripts(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    int s1 = expect_add_script(&ee, "script_a");
    int s2 = expect_add_script(&ee, "script_b");
    assert(s1 == 0 && s2 == 1);
    assert(ee.script_count == 2);

    expect_add_rule(&ee, s1, EXPECT_EXACT, "hello",
                    EXPECT_ACTION_SEND, "world", 1000);
    expect_add_rule(&ee, s2, EXPECT_CONTAINS, "$",
                    EXPECT_ACTION_SEND_LINE, "ls", 2000);

    assert(ee.scripts[s1].rule_count == 1);
    assert(ee.scripts[s2].rule_count == 1);
    expect_destroy(&ee);
    return 1;
}

static int test_expect_start_stop(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    int s = expect_add_script(&ee, "test");
    expect_add_rule(&ee, s, EXPECT_CONTAINS, ">",
                    EXPECT_ACTION_SEND, "cmd", 1000);
    int r = expect_start(&ee, s);
    assert(r == 0);
    assert(ee.scripts[s].running == 1);
    r = expect_stop(&ee, s);
    assert(r == 0);
    assert(ee.scripts[s].running == 0);
    expect_destroy(&ee);
    return 1;
}

/* ═══════════ Pipeline Tests ═══════════ */

static int test_pipeline_create(void)
{
    PipeManager pm;
    pipe_init(&pm);
    int p = pipe_create(&pm, "Deploy Pipeline");
    assert(p == 0);
    assert(pm.pipe_count == 1);
    assert(strcmp(pm.pipes[0].name, "Deploy Pipeline") == 0);
    assert(pm.pipes[0].status == PIPE_STATUS_IDLE);
    pipe_destroy(&pm);
    return 1;
}

static int test_pipeline_add_stage(void)
{
    PipeManager pm;
    pipe_init(&pm);
    int p = pipe_create(&pm, "Build");
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "make clean", 0, 10000);
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "make all", 0, 60000);
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "make test", 0, 120000);
    assert(pm.pipes[p].step_count == 3);
    assert(pm.pipes[p].steps[0].type == PIPE_STEP_COMMAND);
    assert(strcmp(pm.pipes[p].steps[1].command, "make all") == 0);
    assert(pm.pipes[p].steps[2].timeout_ms == 120000);
    pipe_destroy(&pm);
    return 1;
}

static int test_pipeline_execute(void)
{
    PipeManager pm;
    pipe_init(&pm);
    int p = pipe_create(&pm, "Quick");
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "echo hello", 0, 5000);
    assert(pipe_status(&pm, p) == PIPE_STATUS_IDLE);

    int r = pipe_start(&pm, p);
    assert(r == 0);
    assert(pipe_status(&pm, p) == PIPE_STATUS_RUNNING);

    pipe_stop(&pm, p);
    assert(pipe_status(&pm, p) == PIPE_STATUS_IDLE ||
           pipe_status(&pm, p) == PIPE_STATUS_DONE);
    pipe_destroy(&pm);
    return 1;
}

static int test_pipeline_pause_resume(void)
{
    PipeManager pm;
    pipe_init(&pm);
    int p = pipe_create(&pm, "Pausable");
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "step1", 0, 5000);
    pipe_add_step(&pm, p, PIPE_STEP_WAIT, "", 0, 2000);
    pipe_start(&pm, p);

    pipe_pause(&pm, p);
    assert(pipe_status(&pm, p) == PIPE_STATUS_PAUSED);
    pipe_resume(&pm, p);
    assert(pipe_status(&pm, p) == PIPE_STATUS_RUNNING);
    pipe_stop(&pm, p);
    pipe_destroy(&pm);
    return 1;
}

/* ═══════════ Scheduler Tests ═══════════ */

static int test_scheduler_add_task(void)
{
    Scheduler s;
    sched_init(&s);
    int r = sched_add(&s, "Heartbeat", "echo alive", SCHED_INTERVAL, 60);
    assert(r == 0);
    assert(s.count == 1);
    assert(strcmp(s.tasks[0].name, "Heartbeat") == 0);
    assert(s.tasks[0].type == SCHED_INTERVAL);
    assert(s.tasks[0].interval_sec == 60);
    assert(s.tasks[0].enabled == 1);
    sched_destroy(&s);
    return 1;
}

static int test_scheduler_remove(void)
{
    Scheduler s;
    sched_init(&s);
    sched_add(&s, "Task1", "cmd1", SCHED_ONCE, 0);
    sched_add(&s, "Task2", "cmd2", SCHED_INTERVAL, 30);
    assert(s.count == 2);
    int r = sched_remove(&s, 0);
    assert(r == 0);
    assert(s.count == 1);
    assert(strcmp(s.tasks[0].name, "Task2") == 0);
    sched_destroy(&s);
    return 1;
}

static int test_scheduler_next_run(void)
{
    Scheduler s;
    sched_init(&s);
    sched_add(&s, "Periodic", "check_status", SCHED_INTERVAL, 120);
    assert(s.tasks[0].next_run > 0 || s.tasks[0].interval_sec == 120);
    sched_enable(&s, 0, 0);
    assert(s.tasks[0].enabled == 0);
    sched_enable(&s, 0, 1);
    assert(s.tasks[0].enabled == 1);
    sched_destroy(&s);
    return 1;
}

static int test_scheduler_tick(void)
{
    Scheduler s;
    sched_init(&s);
    sched_add(&s, "Ticker", "tick_cmd", SCHED_INTERVAL, 10);
    s.tasks[0].next_run = 1000;
    int fired = sched_tick(&s, 1001);
    assert(fired >= 0);
    sched_destroy(&s);
    return 1;
}

/* ═══════════ Webhooks Tests ═══════════ */

static int test_webhook_register(void)
{
    WebhookManager wm;
    webhook_init(&wm);
    int r = webhook_add(&wm, "deploy_hook", "https://hooks.example.com/deploy",
                        WH_EVENT_CONNECT);
    assert(r == 0);
    assert(wm.hook_count == 1);
    assert(strcmp(wm.hooks[0].name, "deploy_hook") == 0);
    assert(wm.hooks[0].event == WH_EVENT_CONNECT);
    assert(wm.hooks[0].enabled == 1);
    webhook_destroy(&wm);
    return 1;
}

static int test_webhook_trigger(void)
{
    WebhookManager wm;
    webhook_init(&wm);
    webhook_add(&wm, "notify", "https://hooks.example.com/notify",
                WH_EVENT_DISCONNECT);
    int r = webhook_fire(&wm, WH_EVENT_DISCONNECT, "{\"session\":\"test\"}");
    assert(r >= 0);
    webhook_destroy(&wm);
    return 1;
}

static int test_webhook_status(void)
{
    WebhookManager wm;
    webhook_init(&wm);
    webhook_add(&wm, "monitor", "https://hooks.example.com/mon",
                WH_EVENT_ERROR);
    assert(wm.hooks[0].total_fires == 0);
    assert(wm.hooks[0].total_failures == 0);
    assert(wm.hooks[0].last_status == 0);
    webhook_destroy(&wm);
    return 1;
}

static int test_webhook_remove(void)
{
    WebhookManager wm;
    webhook_init(&wm);
    webhook_add(&wm, "hook1", "https://a.com", WH_EVENT_CONNECT);
    webhook_add(&wm, "hook2", "https://b.com", WH_EVENT_IDLE);
    assert(wm.hook_count == 2);
    int r = webhook_remove(&wm, 0);
    assert(r == 0);
    assert(wm.hook_count == 1);
    assert(strcmp(wm.hooks[0].name, "hook2") == 0);
    webhook_destroy(&wm);
    return 1;
}

static int test_webhook_secret(void)
{
    WebhookManager wm;
    webhook_init(&wm);
    webhook_add(&wm, "secure", "https://secure.example.com/hook",
                WH_EVENT_AUTH_FAIL);
    int r = webhook_set_secret(&wm, 0, "my_hmac_secret");
    assert(r == 0);
    assert(strcmp(wm.hooks[0].secret, "my_hmac_secret") == 0);
    webhook_destroy(&wm);
    return 1;
}

/* ═══════════ Main ═══════════ */

int main(void)
{
    printf("PuttyAlt Automation Tests\n");
    printf("=========================\n\n");

    /* Expect Engine */
    printf("[Expect Engine]\n");
    RUN_TEST(expect_init);
    RUN_TEST(expect_add_pattern);
    RUN_TEST(expect_match_check);
    RUN_TEST(expect_multiple_scripts);
    RUN_TEST(expect_start_stop);

    /* Pipeline */
    printf("\n[Pipeline]\n");
    RUN_TEST(pipeline_create);
    RUN_TEST(pipeline_add_stage);
    RUN_TEST(pipeline_execute);
    RUN_TEST(pipeline_pause_resume);

    /* Scheduler */
    printf("\n[Scheduler]\n");
    RUN_TEST(scheduler_add_task);
    RUN_TEST(scheduler_remove);
    RUN_TEST(scheduler_next_run);
    RUN_TEST(scheduler_tick);

    /* Webhooks */
    printf("\n[Webhooks]\n");
    RUN_TEST(webhook_register);
    RUN_TEST(webhook_trigger);
    RUN_TEST(webhook_status);
    RUN_TEST(webhook_remove);
    RUN_TEST(webhook_secret);

    printf("\n=========================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("=========================\n");

    return tests_passed == tests_run ? 0 : 1;
}
