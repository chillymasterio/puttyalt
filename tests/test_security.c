/*
 * test_security.c — Unit tests for security-related modules:
 *   credential store, rate limiter, input filter, host verification.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_credstore.h"
#include "puttyalt_ratelimit.h"
#include "puttyalt_inputfilter.h"
#include "puttyalt_hostverify.h"

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  TEST: %s ... ", #name); \
} while(0)

#define PASS() do { tests_passed++; printf("PASS\n"); } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

/* --- Credential Store Tests --- */

static void test_credstore_init(void)
{
    TEST(credstore_init);
    CredStore cs;
    credstore_init(&cs);
    assert(cs.count == 0);
    assert(cs.locked == 1);
    PASS();
}

static void test_credstore_add_and_find(void)
{
    TEST(credstore_add_and_find);
    CredStore cs;
    credstore_init(&cs);
    credstore_unlock(&cs, "master123");

    int idx = credstore_add(&cs, "server1", "admin", "pass123", "master123");
    assert(idx >= 0);
    assert(cs.count == 1);

    int found = credstore_find(&cs, "server1");
    assert(found == 0);

    int not_found = credstore_find(&cs, "nonexist");
    assert(not_found < 0);
    PASS();
}

static void test_credstore_remove(void)
{
    TEST(credstore_remove);
    CredStore cs;
    credstore_init(&cs);
    credstore_unlock(&cs, "master123");
    credstore_add(&cs, "srv", "user", "pw", "master123");
    assert(cs.count == 1);

    int rc = credstore_remove(&cs, 0);
    assert(rc == 0);
    assert(cs.count == 0);
    PASS();
}

/* --- Rate Limiter Tests --- */

static void test_ratelimit_init(void)
{
    TEST(ratelimit_init);
    RateLimiter rl;
    rl_init(&rl);
    assert(rl.count == 0);
    assert(rl.global_enabled == 1);
    PASS();
}

static void test_ratelimit_bucket(void)
{
    TEST(ratelimit_add_and_acquire);
    RateLimiter rl;
    rl_init(&rl);

    int idx = rl_add_bucket(&rl, "api", 10, 1);
    assert(idx >= 0);
    assert(rl.count == 1);

    /* Should succeed - tokens available */
    int ok = rl_try_acquire(&rl, "api", 1);
    assert(ok == 1);

    /* Drain all tokens */
    for (int i = 0; i < 9; i++)
        rl_try_acquire(&rl, "api", 1);

    /* Should fail - no tokens left */
    int denied = rl_try_acquire(&rl, "api", 1);
    assert(denied == 0);
    PASS();
}

static void test_ratelimit_find(void)
{
    TEST(ratelimit_find);
    RateLimiter rl;
    rl_init(&rl);
    rl_add_bucket(&rl, "login", 5, 1);

    int idx = rl_find(&rl, "login");
    assert(idx >= 0);

    int bad = rl_find(&rl, "missing");
    assert(bad < 0);
    PASS();
}

/* --- Input Filter Tests --- */

static void test_inputfilter_init(void)
{
    TEST(inputfilter_init);
    FilterPipeline fp;
    memset(&fp, 0, sizeof(fp));
    assert(fp.count == 0);
    PASS();
}

static void test_inputfilter_structure(void)
{
    TEST(inputfilter_structure);
    InputFilter f;
    memset(&f, 0, sizeof(f));
    snprintf(f.name, IF_MAX_NAME, "block_rm_rf");
    snprintf(f.pattern, IF_MAX_PATTERN, "rm -rf /");
    f.direction = IF_INPUT;
    f.action = IF_BLOCK;
    f.enabled = 1;

    assert(strcmp(f.name, "block_rm_rf") == 0);
    assert(f.action == IF_BLOCK);
    PASS();
}

/* --- Host Verification Tests --- */

static void test_hostverify_empty_db(void)
{
    TEST(hostverify_empty_db);
    KnownHostDB db;
    memset(&db, 0, sizeof(db));

    HostVerifyStatus st = hv_check(&db, "example.com", 22,
                                   "SHA256:abc123");
    assert(st == HV_UNKNOWN);
    PASS();
}

static void test_hostverify_add_and_check(void)
{
    TEST(hostverify_add_and_check);
    KnownHostDB db;
    memset(&db, 0, sizeof(db));

    int rc = hv_add(&db, "server.test", 22, "ssh-ed25519",
                    "SHA256:testfp123", 1000);
    assert(rc == 0);
    assert(db.count == 1);

    HostVerifyStatus st = hv_check(&db, "server.test", 22,
                                   "SHA256:testfp123");
    assert(st == HV_TRUSTED);

    /* Different fingerprint = changed */
    HostVerifyStatus changed = hv_check(&db, "server.test", 22,
                                        "SHA256:different");
    assert(changed == HV_CHANGED);
    PASS();
}

static void test_hostverify_remove(void)
{
    TEST(hostverify_remove);
    KnownHostDB db;
    memset(&db, 0, sizeof(db));
    hv_add(&db, "host1", 22, "rsa", "fp1", 1000);
    assert(db.count == 1);

    int rc = hv_remove(&db, "host1", 22);
    assert(rc == 0);
    assert(db.count == 0);
    PASS();
}

/* --- Main --- */

int main(void)
{
    printf("=== Security Module Tests ===\n\n");

    printf("[Credential Store]\n");
    test_credstore_init();
    test_credstore_add_and_find();
    test_credstore_remove();

    printf("\n[Rate Limiter]\n");
    test_ratelimit_init();
    test_ratelimit_bucket();
    test_ratelimit_find();

    printf("\n[Input Filter]\n");
    test_inputfilter_init();
    test_inputfilter_structure();

    printf("\n[Host Verification]\n");
    test_hostverify_empty_db();
    test_hostverify_add_and_check();
    test_hostverify_remove();

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
