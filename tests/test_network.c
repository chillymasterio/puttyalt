/*
 * test_network.c — Unit tests for network modules:
 *   keepalive, proxy chain, connection pool, tunnel monitor.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "../puttyalt_keepalive.h"
#include "../puttyalt_proxychain.h"
#include "../puttyalt_connpool.h"
#include "../puttyalt_tunnelmon.h"

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  TEST: %s ... ", #name); \
} while(0)

#define PASS() do { tests_passed++; printf("PASS\n"); } while(0)

/* --- KeepAlive Tests --- */

static void test_keepalive_init(void)
{
    TEST(keepalive_init);
    KeepAliveManager km;
    ka_init(&km);
    assert(km.count == 0);
    assert(km.global_interval == 0);
    PASS();
}

static void test_keepalive_register(void)
{
    TEST(keepalive_register);
    KeepAliveManager km;
    ka_init(&km);

    int rc = ka_register(&km, 0, 30, KA_SSH_NOOP);
    assert(rc >= 0);
    assert(km.count == 1);
    assert(km.sessions[0].interval_sec == 30);
    assert(km.sessions[0].method == KA_SSH_NOOP);
    assert(km.sessions[0].enabled == 1);
    PASS();
}

static void test_keepalive_unregister(void)
{
    TEST(keepalive_unregister);
    KeepAliveManager km;
    ka_init(&km);
    ka_register(&km, 0, 30, KA_SSH_IGNORE);
    assert(km.count == 1);

    int rc = ka_unregister(&km, 0);
    assert(rc >= 0);
    PASS();
}

static void test_keepalive_global_interval(void)
{
    TEST(keepalive_global_interval);
    KeepAliveManager km;
    ka_init(&km);
    ka_set_global_interval(&km, 60);
    assert(km.global_interval == 60);
    PASS();
}

/* --- Proxy Chain Tests --- */

static void test_proxychain_init(void)
{
    TEST(proxychain_init);
    ProxyChain pc;
    proxychain_init(&pc);
    assert(pc.num_hops == 0);
    PASS();
}

static void test_proxychain_add(void)
{
    TEST(proxychain_add);
    ProxyChain pc;
    proxychain_init(&pc);

    int rc = proxychain_add(&pc, PROXY_SOCKS5, "proxy1.example.com",
                            1080, "user", "pass");
    assert(rc == 0);
    assert(pc.num_hops == 1);
    assert(pc.hops[0].type == PROXY_SOCKS5);
    assert(pc.hops[0].port == 1080);
    assert(strcmp(pc.hops[0].host, "proxy1.example.com") == 0);
    PASS();
}

static void test_proxychain_max_hops(void)
{
    TEST(proxychain_max_hops);
    ProxyChain pc;
    proxychain_init(&pc);

    for (int i = 0; i < PROXY_MAX_CHAIN; i++) {
        int rc = proxychain_add(&pc, PROXY_HTTP, "hop", 8080, NULL, NULL);
        assert(rc == 0);
    }
    /* Should reject beyond max */
    int rc = proxychain_add(&pc, PROXY_HTTP, "extra", 8080, NULL, NULL);
    assert(rc != 0);
    PASS();
}

static void test_proxychain_remove(void)
{
    TEST(proxychain_remove);
    ProxyChain pc;
    proxychain_init(&pc);
    proxychain_add(&pc, PROXY_SOCKS4, "host1", 1080, NULL, NULL);
    proxychain_add(&pc, PROXY_HTTP, "host2", 3128, NULL, NULL);
    assert(pc.num_hops == 2);

    int rc = proxychain_remove(&pc, 0);
    assert(rc == 0);
    assert(pc.num_hops == 1);
    assert(strcmp(pc.hops[0].host, "host2") == 0);
    PASS();
}

static void test_proxychain_clear(void)
{
    TEST(proxychain_clear);
    ProxyChain pc;
    proxychain_init(&pc);
    proxychain_add(&pc, PROXY_SOCKS5, "host", 1080, NULL, NULL);
    proxychain_clear(&pc);
    assert(pc.num_hops == 0);
    PASS();
}

/* --- Connection Pool Tests --- */

static void test_connpool_init(void)
{
    TEST(connpool_init);
    ConnpoolState s;
    memset(&s, 0, sizeof(s));
    int rc = connpool_init(&s);
    assert(rc == 0);
    assert(s.initialized == 1);
    connpool_destroy(&s);
    PASS();
}

static void test_connpool_update(void)
{
    TEST(connpool_update);
    ConnpoolState s;
    memset(&s, 0, sizeof(s));
    connpool_init(&s);

    int rc = connpool_update(&s);
    assert(rc == 0);

    connpool_destroy(&s);
    PASS();
}

/* --- Tunnel Monitor Tests --- */

static void test_tunnelmon_structure(void)
{
    TEST(tunnelmon_structure);
    TunnelMonitor tm;
    memset(&tm, 0, sizeof(tm));
    assert(tm.count == 0);
    assert(tm.total_active == 0);

    /* Simulate adding a tunnel */
    TMTunnel *t = &tm.tunnels[0];
    snprintf(t->label, sizeof(t->label), "db-tunnel");
    snprintf(t->local_bind, sizeof(t->local_bind), "127.0.0.1");
    t->local_port = 5432;
    snprintf(t->remote_host, sizeof(t->remote_host), "db.internal");
    t->remote_port = 5432;
    t->state = TM_STATE_ACTIVE;
    t->auto_reconnect = 1;
    t->max_retries = 5;
    tm.count = 1;

    assert(tm.count == 1);
    assert(t->state == TM_STATE_ACTIVE);
    assert(t->local_port == 5432);
    PASS();
}

static void test_tunnelmon_error_state(void)
{
    TEST(tunnelmon_error_state);
    TunnelMonitor tm;
    memset(&tm, 0, sizeof(tm));

    TMTunnel *t = &tm.tunnels[0];
    t->state = TM_STATE_ERROR;
    snprintf(t->error_msg, sizeof(t->error_msg), "Connection refused");
    t->retry_count = 3;
    tm.count = 1;

    assert(t->state == TM_STATE_ERROR);
    assert(strcmp(t->error_msg, "Connection refused") == 0);
    assert(t->retry_count == 3);
    PASS();
}

/* --- Main --- */

int main(void)
{
    printf("=== Network Module Tests ===\n\n");

    printf("[KeepAlive]\n");
    test_keepalive_init();
    test_keepalive_register();
    test_keepalive_unregister();
    test_keepalive_global_interval();

    printf("\n[Proxy Chain]\n");
    test_proxychain_init();
    test_proxychain_add();
    test_proxychain_max_hops();
    test_proxychain_remove();
    test_proxychain_clear();

    printf("\n[Connection Pool]\n");
    test_connpool_init();
    test_connpool_update();

    printf("\n[Tunnel Monitor]\n");
    test_tunnelmon_structure();
    test_tunnelmon_error_state();

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
