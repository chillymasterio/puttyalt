/*
 * test_config.c — Tests for configuration modules:
 *   cfgsync, passgen, audit, uptime
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_cfgsync.h"
#include "puttyalt_passgen.h"
#include "puttyalt_audit.h"
#include "puttyalt_uptime.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else { printf("FAIL\n"); } \
} while(0)

/* ═══════════ Config Sync Tests ═══════════ */

static int test_cfgsync_init(void)
{
    CfgSyncState cs;
    int r = cfgsync_init(&cs);
    assert(r == 0);
    assert(cs.section_count == 0);
    assert(cs.encrypt == 0);
    cfgsync_destroy(&cs);
    return 1;
}

static int test_cfgsync_export(void)
{
    CfgSyncState cs;
    cfgsync_init(&cs);
    cfgsync_add_section(&cs, CFGSYNC_EXPORT_SESSIONS);
    cfgsync_add_section(&cs, CFGSYNC_EXPORT_APPEARANCE);
    assert(cs.section_count == 2);

    int r = cfgsync_export(&cs, "test_export.json");
    assert(r == 0);
    assert(strcmp(cs.export_path, "test_export.json") == 0);
    cfgsync_destroy(&cs);
    return 1;
}

static int test_cfgsync_import(void)
{
    CfgSyncState cs;
    cfgsync_init(&cs);
    int r = cfgsync_import(&cs, "test_import.json");
    /* File doesn't exist, so import returns -1 */
    assert(r == -1);
    cfgsync_destroy(&cs);
    return 1;
}

static int test_cfgsync_diff(void)
{
    CfgSyncState cs;
    cfgsync_init(&cs);
    cfgsync_add_section(&cs, CFGSYNC_EXPORT_ALL);
    assert(cs.section_count == 1);
    assert(cs.sections[0] == CFGSYNC_EXPORT_ALL);

    cfgsync_add_section(&cs, CFGSYNC_EXPORT_KEYS);
    assert(cs.section_count == 2);
    cfgsync_destroy(&cs);
    return 1;
}

/* ═══════════ Password Generator Tests ═══════════ */

static int test_passgen_generate(void)
{
    PassGenConfig cfg;
    passgen_default(&cfg);
    cfg.length = 20;

    char buf[64];
    int len = passgen_generate(&cfg, buf, sizeof(buf));
    assert(len == 20);
    assert((int)strlen(buf) == 20);
    return 1;
}

static int test_passgen_strength(void)
{
    int s1 = passgen_strength("abc");
    int s2 = passgen_strength("Abc123!@#xyz");
    int s3 = passgen_strength("aB3$kL9!mN2@pQ5&rT8*");
    assert(s1 < s2);
    assert(s2 <= s3);
    assert(s1 >= 0 && s1 <= 100);
    assert(s3 >= 0 && s3 <= 100);
    return 1;
}

static int test_passgen_entropy(void)
{
    PassGenConfig cfg;
    passgen_default(&cfg);
    cfg.length = 16;
    cfg.use_upper = 1;
    cfg.use_lower = 1;
    cfg.use_digits = 1;
    cfg.use_symbols = 0;

    double e = passgen_entropy(&cfg);
    assert(e > 0.0);

    /* Adding symbols should increase entropy */
    cfg.use_symbols = 1;
    double e2 = passgen_entropy(&cfg);
    assert(e2 > e);
    return 1;
}

static int test_passgen_config_options(void)
{
    PassGenConfig cfg;
    passgen_default(&cfg);
    assert(cfg.use_upper == 1);
    assert(cfg.use_lower == 1);
    assert(cfg.use_digits == 1);
    assert(cfg.length > 0);

    /* Generate with only digits */
    cfg.use_upper = 0;
    cfg.use_lower = 0;
    cfg.use_digits = 1;
    cfg.use_symbols = 0;
    cfg.length = 10;

    char buf[32];
    passgen_generate(&cfg, buf, sizeof(buf));
    assert((int)strlen(buf) == 10);

    /* Verify all chars are digits */
    int i;
    for (i = 0; i < 10; i++) {
        assert(buf[i] >= '0' && buf[i] <= '9');
    }
    return 1;
}

/* ═══════════ Audit Tests ═══════════ */

static int test_audit_log_entry(void)
{
    AuditTrail at;
    audit_init(&at, NULL);
    int r = audit_log(&at, AUDIT_CONNECT, "admin", "server1.com", 22,
                      "SSH connection established");
    assert(r == 0);
    assert(at.count == 1);
    assert(at.entries[0].type == AUDIT_CONNECT);
    assert(strcmp(at.entries[0].user, "admin") == 0);
    assert(strcmp(at.entries[0].host, "server1.com") == 0);
    assert(at.entries[0].port == 22);
    audit_destroy(&at);
    return 1;
}

static int test_audit_search(void)
{
    AuditTrail at;
    audit_init(&at, NULL);
    audit_log(&at, AUDIT_CONNECT, "alice", "host1.com", 22, "connected");
    audit_log(&at, AUDIT_COMMAND, "alice", "host1.com", 22, "ls -la");
    audit_log(&at, AUDIT_CONNECT, "bob", "host2.com", 22, "connected");
    audit_log(&at, AUDIT_DISCONNECT, "alice", "host1.com", 22, "closed");

    AuditEntry results[10];
    int n = audit_search(&at, "alice", results, 10);
    assert(n >= 2); /* at least the connect and disconnect */
    audit_destroy(&at);
    return 1;
}

static int test_audit_rotate(void)
{
    AuditTrail at;
    audit_init(&at, NULL);
    int i;
    for (i = 0; i < 50; i++) {
        audit_log(&at, AUDIT_COMMAND, "user", "host", 22, "command");
    }
    assert(at.count == 50);

    /* Rotate should work without crashing */
    int r = audit_rotate_log(&at, 1024);
    assert(r == 0);
    audit_destroy(&at);
    return 1;
}

static int test_audit_multiple_types(void)
{
    AuditTrail at;
    audit_init(&at, NULL);
    audit_log(&at, AUDIT_CONNECT, "u", "h", 22, "");
    audit_log(&at, AUDIT_AUTH_SUCCESS, "u", "h", 22, "key auth");
    audit_log(&at, AUDIT_COMMAND, "u", "h", 22, "whoami");
    audit_log(&at, AUDIT_FILE_TRANSFER, "u", "h", 22, "upload file.txt");
    audit_log(&at, AUDIT_DISCONNECT, "u", "h", 22, "");
    assert(at.count == 5);
    assert(at.entries[0].type == AUDIT_CONNECT);
    assert(at.entries[1].type == AUDIT_AUTH_SUCCESS);
    assert(at.entries[3].type == AUDIT_FILE_TRANSFER);
    audit_destroy(&at);
    return 1;
}

/* ═══════════ Uptime Tests ═══════════ */

static int test_uptime_start(void)
{
    UptimeTimer ut;
    uptime_init(&ut);
    assert(ut.running == 0);
    int r = uptime_start(&ut);
    assert(r == 0);
    assert(ut.running == 1);
    uptime_stop(&ut);
    return 1;
}

static int test_uptime_elapsed(void)
{
    UptimeTimer ut;
    uptime_init(&ut);
    uptime_start(&ut);
    long e = uptime_elapsed(&ut);
    assert(e >= 0);
    uptime_stop(&ut);
    return 1;
}

static int test_uptime_format(void)
{
    UptimeTimer ut;
    uptime_init(&ut);
    uptime_start(&ut);
    char buf[64];
    uptime_format(&ut, buf, sizeof(buf));
    assert(strlen(buf) > 0);
    uptime_stop(&ut);
    return 1;
}

static int test_uptime_pause_resume(void)
{
    UptimeTimer ut;
    uptime_init(&ut);
    uptime_start(&ut);
    assert(ut.running == 1);

    int r = uptime_pause(&ut);
    assert(r == 0);
    assert(ut.paused == 1);

    r = uptime_resume(&ut);
    assert(r == 0);
    assert(ut.paused == 0);
    assert(ut.running == 1);
    uptime_stop(&ut);
    return 1;
}

/* ═══════════ Main ═══════════ */

int main(void)
{
    printf("PuttyAlt Config Tests\n");
    printf("=====================\n\n");

    /* Config Sync */
    printf("[Config Sync]\n");
    RUN_TEST(cfgsync_init);
    RUN_TEST(cfgsync_export);
    RUN_TEST(cfgsync_import);
    RUN_TEST(cfgsync_diff);

    /* Password Generator */
    printf("\n[Password Generator]\n");
    RUN_TEST(passgen_generate);
    RUN_TEST(passgen_strength);
    RUN_TEST(passgen_entropy);
    RUN_TEST(passgen_config_options);

    /* Audit */
    printf("\n[Audit]\n");
    RUN_TEST(audit_log_entry);
    RUN_TEST(audit_search);
    RUN_TEST(audit_rotate);
    RUN_TEST(audit_multiple_types);

    /* Uptime */
    printf("\n[Uptime]\n");
    RUN_TEST(uptime_start);
    RUN_TEST(uptime_elapsed);
    RUN_TEST(uptime_format);
    RUN_TEST(uptime_pause_resume);

    printf("\n=====================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("=====================\n");

    return tests_passed == tests_run ? 0 : 1;
}
