/*
 * test_workflow.c — Tests for workflow modules:
 *   sessshare, approval, roles, activity, templates
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_sessshare.h"
#include "puttyalt_approval.h"
#include "puttyalt_roles.h"
#include "puttyalt_activity.h"
#include "puttyalt_templates.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else { printf("FAIL\n"); } \
} while(0)

/* ═══════════ Session Sharing Tests ═══════════ */

static int test_sessshare_init(void)
{
    SessionShare ss;
    sessshare_init(&ss);
    assert(ss.state == SHARE_STATE_IDLE);
    assert(ss.port == SESSSHARE_PORT_DEFAULT);
    assert(ss.viewer_count == 0);
    assert(ss.max_viewers == SESSSHARE_MAX_VIEWERS);
    sessshare_destroy(&ss);
    return 1;
}

static int test_sessshare_host_start(void)
{
    SessionShare ss;
    sessshare_init(&ss);
    int r = sessshare_host_start(&ss, 9000, "secret123");
    assert(r == 0);
    assert(ss.state == SHARE_STATE_HOSTING);
    assert(ss.port == 9000);
    assert(strcmp(ss.password, "secret123") == 0);
    sessshare_host_stop(&ss);
    assert(ss.state == SHARE_STATE_IDLE);
    sessshare_destroy(&ss);
    return 1;
}

static int test_sessshare_broadcast(void)
{
    SessionShare ss;
    sessshare_init(&ss);
    sessshare_host_start(&ss, 9000, "pw");
    unsigned char data[] = "Hello viewers";
    int r = sessshare_broadcast_data(&ss, data, (int)strlen((char *)data));
    assert(r == 0);
    assert(ss.total_bytes >= strlen((char *)data));
    sessshare_host_stop(&ss);
    sessshare_destroy(&ss);
    return 1;
}

static int test_sessshare_set_mode(void)
{
    SessionShare ss;
    sessshare_init(&ss);
    sessshare_host_start(&ss, 9000, "pw");
    sessshare_set_mode(&ss, SHARE_MODE_READONLY);
    assert(ss.mode == SHARE_MODE_READONLY);
    sessshare_set_mode(&ss, SHARE_MODE_INTERACTIVE);
    assert(ss.mode == SHARE_MODE_INTERACTIVE);
    sessshare_host_stop(&ss);
    sessshare_destroy(&ss);
    return 1;
}

/* ═══════════ Approval Tests ═══════════ */

static int test_approval_init(void)
{
    ApprovalManager am;
    approval_init(&am);
    assert(am.count == 0);
    assert(am.enabled == 0);
    approval_destroy(&am);
    return 1;
}

static int test_approval_request_grant(void)
{
    ApprovalManager am;
    approval_init(&am);
    am.enabled = 1;
    int idx = approval_request(&am, "dev1", "prod-db.internal", 5432, "Deploy fix");
    assert(idx == 0);
    assert(am.count == 1);
    assert(am.requests[0].status == APPROVAL_PENDING);
    assert(approval_check(&am, "prod-db.internal", 5432) == 0);

    approval_grant(&am, idx, "admin");
    assert(am.requests[0].status == APPROVAL_GRANTED);
    assert(strcmp(am.requests[0].approver, "admin") == 0);
    assert(approval_check(&am, "prod-db.internal", 5432) == 1);
    approval_destroy(&am);
    return 1;
}

static int test_approval_deny(void)
{
    ApprovalManager am;
    approval_init(&am);
    am.enabled = 1;
    int idx = approval_request(&am, "dev2", "secure.host", 22, "Access needed");
    assert(idx == 0);
    approval_deny(&am, idx, "secops");
    assert(am.requests[0].status == APPROVAL_DENIED);
    assert(approval_check(&am, "secure.host", 22) == 0);
    approval_destroy(&am);
    return 1;
}

static int test_approval_pending(void)
{
    ApprovalManager am;
    approval_init(&am);
    am.enabled = 1;
    approval_request(&am, "u1", "host1", 22, "reason1");
    approval_request(&am, "u2", "host2", 22, "reason2");
    assert(am.count == 2);
    assert(am.requests[0].status == APPROVAL_PENDING);
    assert(am.requests[1].status == APPROVAL_PENDING);
    approval_grant(&am, 0, "boss");
    assert(am.requests[0].status == APPROVAL_GRANTED);
    assert(am.requests[1].status == APPROVAL_PENDING);
    approval_destroy(&am);
    return 1;
}

/* ═══════════ Roles Tests ═══════════ */

static int test_role_create(void)
{
    RoleManager rm;
    role_init(&rm);
    int p = role_add_profile(&rm, "Developer", "Dev team access");
    assert(p == 0);
    assert(rm.profile_count == 1);
    assert(strcmp(rm.profiles[0].name, "Developer") == 0);
    role_destroy(&rm);
    return 1;
}

static int test_role_assign_permission(void)
{
    RoleManager rm;
    role_init(&rm);
    int p = role_add_profile(&rm, "Ops", "Operations");
    int r = role_add_rule(&rm, p, RULE_HOST_PATTERN, ROLE_ALLOW, "*.ops.internal", 0, 0);
    assert(r == 0);
    assert(rm.profiles[p].rule_count == 1);
    assert(rm.profiles[p].rules[0].action == ROLE_ALLOW);
    role_destroy(&rm);
    return 1;
}

static int test_role_check_access(void)
{
    RoleManager rm;
    role_init(&rm);
    int p = role_add_profile(&rm, "Restricted", "Limited access");
    role_add_rule(&rm, p, RULE_HOST_PATTERN, ROLE_DENY, "*.prod.*", 0, 0);
    role_activate(&rm, p);

    RoleAction action = role_check_host(&rm, "db.prod.corp", 3306);
    assert(action == ROLE_DENY);
    action = role_check_host(&rm, "dev.staging.corp", 22);
    assert(action == ROLE_ALLOW);
    role_destroy(&rm);
    return 1;
}

static int test_role_multiple_profiles(void)
{
    RoleManager rm;
    role_init(&rm);
    int p1 = role_add_profile(&rm, "Admin", "Full");
    int p2 = role_add_profile(&rm, "Guest", "Read only");
    assert(p1 == 0 && p2 == 1);
    assert(rm.profile_count == 2);
    role_activate(&rm, p2);
    assert(rm.active_profile == p2);
    role_destroy(&rm);
    return 1;
}

/* ═══════════ Activity Tests ═══════════ */

static int test_activity_log(void)
{
    ActivityFeed af;
    activity_init(&af);
    activity_log(&af, ACT_CONNECT, "alice", "server1.com", "SSH connected");
    assert(af.count == 1);
    assert(af.events[0].type == ACT_CONNECT);
    assert(strcmp(af.events[0].user, "alice") == 0);
    activity_destroy(&af);
    return 1;
}

static int test_activity_query(void)
{
    ActivityFeed af;
    activity_init(&af);
    activity_log(&af, ACT_CONNECT, "bob", "host1", "connected");
    activity_log(&af, ACT_COMMAND, "bob", "host1", "ls");
    activity_log(&af, ACT_DISCONNECT, "bob", "host1", "closed");
    assert(af.count == 3);

    ActivityEvent recent[10];
    int n = activity_get_recent(&af, recent, 10, 60);
    assert(n == 3);
    activity_destroy(&af);
    return 1;
}

static int test_activity_clear(void)
{
    ActivityFeed af;
    activity_init(&af);
    activity_log(&af, ACT_INFO, "sys", "local", "started");
    activity_log(&af, ACT_ERROR, "sys", "local", "timeout");
    assert(af.count == 2);
    activity_clear(&af);
    assert(af.count == 0);
    activity_destroy(&af);
    return 1;
}

/* ═══════════ Templates Tests ═══════════ */

static int test_template_create(void)
{
    TemplateStore ts;
    tmpl_init(&ts);
    ConnTemplate t = {0};
    strcpy(t.name, "Production SSH");
    strcpy(t.host_pattern, "*.prod.corp");
    t.port = 22;
    strcpy(t.username, "deploy");
    int r = tmpl_add(&ts, &t);
    assert(r == 0);
    assert(ts.count == 1);
    assert(strcmp(ts.templates[0].name, "Production SSH") == 0);
    tmpl_destroy(&ts);
    return 1;
}

static int test_template_apply(void)
{
    TemplateStore ts;
    tmpl_init(&ts);
    ConnTemplate t = {0};
    strcpy(t.name, "Web Server");
    strcpy(t.host_pattern, "web01.example.com");
    t.port = 2222;
    strcpy(t.username, "webadmin");
    tmpl_add(&ts, &t);

    char host[128], user[64];
    int port;
    int r = tmpl_apply(&ts, 0, host, &port, user, sizeof(host), sizeof(user));
    assert(r == 0);
    assert(strcmp(host, "web01.example.com") == 0);
    assert(port == 2222);
    assert(strcmp(user, "webadmin") == 0);
    tmpl_destroy(&ts);
    return 1;
}

static int test_template_validate_fields(void)
{
    TemplateStore ts;
    tmpl_init(&ts);
    ConnTemplate t = {0};
    strcpy(t.name, "Test Template");
    strcpy(t.host_pattern, "host.example.com");
    t.port = 22;
    strcpy(t.username, "user");
    strcpy(t.description, "A test template");
    strcpy(t.created_by, "admin");
    t.created_at = 1000000;
    int r = tmpl_add(&ts, &t);
    assert(r == 0);
    assert(ts.templates[0].port == 22);
    assert(strcmp(ts.templates[0].created_by, "admin") == 0);
    assert(ts.templates[0].created_at == 1000000);
    tmpl_destroy(&ts);
    return 1;
}

static int test_template_remove(void)
{
    TemplateStore ts;
    tmpl_init(&ts);
    ConnTemplate t1 = {0}, t2 = {0};
    strcpy(t1.name, "First");
    strcpy(t1.host_pattern, "first.host");
    t1.port = 22;
    strcpy(t2.name, "Second");
    strcpy(t2.host_pattern, "second.host");
    t2.port = 443;
    tmpl_add(&ts, &t1);
    tmpl_add(&ts, &t2);
    assert(ts.count == 2);
    tmpl_remove(&ts, 0);
    assert(ts.count == 1);
    assert(strcmp(ts.templates[0].name, "Second") == 0);
    tmpl_destroy(&ts);
    return 1;
}

/* ═══════════ Main ═══════════ */

int main(void)
{
    printf("PuttyAlt Workflow Tests\n");
    printf("=======================\n\n");

    /* Session Sharing */
    printf("[Session Sharing]\n");
    RUN_TEST(sessshare_init);
    RUN_TEST(sessshare_host_start);
    RUN_TEST(sessshare_broadcast);
    RUN_TEST(sessshare_set_mode);

    /* Approval */
    printf("\n[Approval]\n");
    RUN_TEST(approval_init);
    RUN_TEST(approval_request_grant);
    RUN_TEST(approval_deny);
    RUN_TEST(approval_pending);

    /* Roles */
    printf("\n[Roles]\n");
    RUN_TEST(role_create);
    RUN_TEST(role_assign_permission);
    RUN_TEST(role_check_access);
    RUN_TEST(role_multiple_profiles);

    /* Activity */
    printf("\n[Activity]\n");
    RUN_TEST(activity_log);
    RUN_TEST(activity_query);
    RUN_TEST(activity_clear);

    /* Templates */
    printf("\n[Templates]\n");
    RUN_TEST(template_create);
    RUN_TEST(template_apply);
    RUN_TEST(template_validate_fields);
    RUN_TEST(template_remove);

    printf("\n=======================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("=======================\n");

    return tests_passed == tests_run ? 0 : 1;
}
