/*
 * test_modules.c — Compile-time validation of all PuttyAlt modules.
 * Verifies headers are self-contained and APIs are callable.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

/* Core */
#include "puttyalt.h"
#include "puttyalt_design.h"

/* v1.0 modules */
#include "puttyalt_gui.h"
#include "puttyalt_dialogs.h"
#include "puttyalt_credstore.h"
#include "puttyalt_passgen.h"
#include "puttyalt_hotkeymgr.h"
#include "puttyalt_snippetlib.h"

/* v1.2 modules */
#include "puttyalt_smartpaste.h"
#include "puttyalt_sesssearch.h"
#include "puttyalt_dragdrop.h"
#include "puttyalt_sshkeymgr.h"
#include "puttyalt_multiclip.h"
#include "puttyalt_quickcmd.h"

/* v1.3 modules */
#include "puttyalt_osdetect.h"
#include "puttyalt_speedtest.h"
#include "puttyalt_retrypolicy.h"
#include "puttyalt_timestamp.h"

/* v2.0 modules */
#include "puttyalt_snipsync.h"
#include "puttyalt_audit.h"
#include "puttyalt_transitions.h"
#include "puttyalt_cmdpal.h"
#include "puttyalt_sessshare.h"
#include "puttyalt_roles.h"
#include "puttyalt_cfgsync.h"
#include "puttyalt_luaengine.h"
#include "puttyalt_expect.h"
#include "puttyalt_webhooks.h"
#include "puttyalt_pipeline.h"
#include "puttyalt_restapi.h"
#include "puttyalt_taboverflow.h"
#include "puttyalt_sidebarresize.h"
#include "puttyalt_cursor.h"
#include "puttyalt_paths.h"
#include "puttyalt_focusring.h"
#include "puttyalt_validate.h"
#include "puttyalt_toast.h"
#include "puttyalt_scrollstate.h"
#include "puttyalt_tooltip.h"
#include "puttyalt_tabnav.h"
#include "puttyalt_sessorder.h"
#include "puttyalt_uptime.h"
#include "puttyalt_activity.h"
#include "puttyalt_templates.h"
#include "puttyalt_approval.h"
#include "puttyalt_scheduler.h"
#include "puttyalt_shortcutoverlay.h"
#include "puttyalt_themeswitch.h"
#include "puttyalt_sesstags.h"

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else printf("FAIL\n"); \
} while(0)

/* ═══════════════ Test Functions ═══════════════ */

static int test_design_constants(void)
{
    assert(DS_BG == 0x0D1117);
    assert(DS_ACCENT == 0x58A6FF);
    assert(DS_TEXT == 0xE6EDF3);
    assert(DS_SIDEBAR_WIDTH == 240);
    assert(DS_TABBAR_HEIGHT == 36);
    assert(DS_SPACE_MD == 12);
    return 1;
}

static int test_version(void)
{
    assert(PUTTYALT_VERSION_MAJOR == 2);
    assert(PUTTYALT_VERSION_MINOR == 0);
    assert(strcmp(PUTTYALT_NAME, "PuttyAlt") == 0);
    return 1;
}

static int test_gui_config(void)
{
    GUIConfig cfg;
    gui_config_defaults(&cfg);
    assert(cfg.width == GUI_DEFAULT_WIDTH);
    assert(cfg.height == GUI_DEFAULT_HEIGHT);
    assert(cfg.font_size == DS_FONT_SIZE_MD);
    assert(cfg.sidebar_visible == 1);
    assert(cfg.cursor_style == 2);
    assert(cfg.copy_on_select == 1);
    return 1;
}

static int test_transitions(void)
{
    Transition t;
    transition_init(&t, 0.0f, 1.0f);
    assert(transition_done(&t));
    transition_set(&t, 1.0f);
    assert(!transition_done(&t));
    transition_update(&t, 1.0f);
    assert(transition_done(&t));

    /* Color lerp */
    unsigned int c = color_lerp(0x000000, 0xFFFFFF, 0.5f);
    int r = (c >> 16) & 0xFF;
    assert(r >= 126 && r <= 128);  /* approximately 127 */
    return 1;
}

static int test_cmdpal(void)
{
    CmdPalState cp;
    cmdpal_init(&cp);
    cmdpal_add(&cp, "New Session", "File", "Ctrl+N", 1001);
    cmdpal_add(&cp, "Find", "Edit", "Ctrl+F", 2004);
    cmdpal_add(&cp, "Full Screen", "View", "F11", 4001);
    assert(cp.item_count == 3);

    cmdpal_open(&cp);
    assert(cp.active == 1);
    assert(cp.visible_count == 3);

    cmdpal_filter(&cp, "ful");
    assert(cp.visible_count >= 1);  /* "Full Screen" should match */

    cmdpal_close(&cp);
    assert(cp.active == 0);
    cmdpal_destroy(&cp);
    return 1;
}

static int test_roles(void)
{
    RoleManager rm;
    role_init(&rm);
    int p = role_add_profile(&rm, "Admin", "Full access");
    assert(p == 0);
    role_add_rule(&rm, p, RULE_HOST_PATTERN, ROLE_DENY, "*.prod.internal", 0, 0);
    role_activate(&rm, p);

    RoleAction r = role_check_host(&rm, "db1.prod.internal", 22);
    assert(r == ROLE_DENY);
    r = role_check_host(&rm, "dev.staging.local", 22);
    assert(r == ROLE_ALLOW);

    role_destroy(&rm);
    return 1;
}

static int test_validate(void)
{
    int port;
    assert(validate_hostname("example.com") == VALID_OK);
    assert(validate_hostname("") == VALID_EMPTY);
    assert(validate_hostname("bad..host") == VALID_INVALID_FORMAT);
    assert(validate_port("22", &port) == VALID_OK && port == 22);
    assert(validate_port("0", &port) == VALID_OUT_OF_RANGE);
    assert(validate_port("99999", &port) == VALID_OUT_OF_RANGE);
    assert(validate_username("admin") == VALID_OK);
    assert(validate_ip4("192.168.1.1") == VALID_OK);
    assert(validate_ip4("999.0.0.1") == VALID_OUT_OF_RANGE);
    return 1;
}

static int test_toast(void)
{
    ToastManager tm;
    toast_init(&tm);
    toast_show(&tm, TOAST_SUCCESS, "Connected!", 3000);
    toast_show(&tm, TOAST_ERROR, "Connection failed", 5000);
    assert(toast_count(&tm) == 2);
    toast_dismiss_all(&tm);
    toast_update(&tm, 10000);
    assert(toast_count(&tm) == 0);
    return 1;
}

static int test_scroll(void)
{
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 1000, 24);
    assert(ss.scroll_max == 976);
    scroll_to_bottom(&ss);
    assert(scroll_at_bottom(&ss));
    scroll_by(&ss, -10);
    assert(!scroll_at_bottom(&ss));
    scroll_to_top(&ss);
    assert(ss.scroll_pos == 0);
    return 1;
}

static int test_cursor(void)
{
    CursorState cs;
    cursor_init(&cs, CURSOR_BAR);
    assert(cs.style == CURSOR_BAR);
    assert(cs.blink == 1);
    assert(cursor_is_visible(&cs));

    int rx, ry, rw, rh;
    cursor_get_rect(&cs, 100, 200, 8, 16, &rx, &ry, &rw, &rh);
    assert(rx == 100 && ry == 200);
    assert(rw == 2);  /* bar thickness */
    assert(rh == 16);

    cursor_set_style(&cs, CURSOR_BLOCK);
    cursor_get_rect(&cs, 100, 200, 8, 16, &rx, &ry, &rw, &rh);
    assert(rw == 8 && rh == 16);
    return 1;
}

static int test_tabnav(void)
{
    TabNav tn;
    tabnav_init(&tn);
    tabnav_set_active(&tn, 0, 5);
    assert(tabnav_next(&tn) == 1);
    assert(tabnav_next(&tn) == 2);
    assert(tabnav_prev(&tn) == 1);
    assert(tabnav_goto(&tn, 4) == 4);
    assert(tabnav_next(&tn) == 0);  /* wraps */
    return 1;
}

static int test_uptime(void)
{
    UptimeTimer ut;
    uptime_init(&ut);
    uptime_start(&ut);
    assert(ut.running == 1);
    char buf[64];
    uptime_format(&ut, buf, sizeof(buf));
    assert(strlen(buf) > 0);
    uptime_stop(&ut);
    assert(ut.running == 0);
    return 1;
}

static int test_sessshare(void)
{
    SessionShare ss;
    sessshare_init(&ss);
    assert(ss.port == SESSSHARE_PORT_DEFAULT);
    int r = sessshare_host_start(&ss, 9000, "secret");
    assert(r == 0);
    assert(ss.state == SHARE_STATE_HOSTING);
    sessshare_host_stop(&ss);
    assert(ss.state == SHARE_STATE_IDLE);
    sessshare_destroy(&ss);
    return 1;
}

static int test_expect(void)
{
    ExpectEngine ee;
    expect_init(&ee);
    int s = expect_add_script(&ee, "login");
    assert(s == 0);
    expect_add_rule(&ee, s, EXPECT_CONTAINS, "password:",
                    EXPECT_ACTION_SEND_LINE, "mypass", 5000);
    expect_start(&ee, s);
    assert(ee.active_script == 0);

    char response[256] = "";
    expect_feed(&ee, "Enter password:", 15, response, sizeof(response));
    assert(strlen(response) > 0);
    expect_destroy(&ee);
    return 1;
}

static int test_pipeline(void)
{
    PipeManager pm;
    pipe_init(&pm);
    int p = pipe_create(&pm, "Deploy");
    assert(p == 0);
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "apt update", 0, 30000);
    pipe_add_step(&pm, p, PIPE_STEP_COMMAND, "apt upgrade -y", 0, 60000);
    assert(pm.pipes[p].step_count == 2);
    assert(pipe_start(&pm, p) == 0);
    assert(pipe_status(&pm, p) == PIPE_STATUS_RUNNING);
    pipe_stop(&pm, p);
    assert(pipe_status(&pm, p) == PIPE_STATUS_IDLE);
    pipe_destroy(&pm);
    return 1;
}

static int test_scheduler(void)
{
    Scheduler s;
    sched_init(&s);
    sched_add(&s, "Heartbeat", "echo alive", SCHED_INTERVAL, 60);
    assert(s.count == 1);
    assert(s.tasks[0].enabled == 1);
    sched_enable(&s, 0, 0);
    assert(s.tasks[0].enabled == 0);
    sched_destroy(&s);
    return 1;
}

static int test_themeswitch(void)
{
    ThemeRegistry tr;
    theme_reg_init(&tr);
    assert(tr.count == 4);
    const ThemeDef *t = theme_reg_current(&tr);
    assert(t->bg == DS_BG);  /* GitHub Dark is default */
    theme_reg_activate(&tr, 1);  /* Monokai */
    t = theme_reg_current(&tr);
    assert(t->bg == 0x272822);
    return 1;
}

static int test_tags(void)
{
    TagStore ts;
    tags_init(&ts);
    tags_add(&ts, "session1", "production");
    tags_add(&ts, "session1", "linux");
    tags_add(&ts, "session2", "production");
    char results[10][64];
    int n = tags_filter(&ts, "production", results, 10);
    assert(n == 2);
    tags_remove(&ts, "session1", "production");
    n = tags_filter(&ts, "production", results, 10);
    assert(n == 1);
    tags_destroy(&ts);
    return 1;
}

static int test_approval(void)
{
    ApprovalManager am;
    approval_init(&am);
    am.enabled = 1;
    int idx = approval_request(&am, "user1", "prod-db.internal", 5432, "Deploy");
    assert(idx == 0);
    assert(approval_check(&am, "prod-db.internal", 5432) == 0);  /* not yet approved */
    approval_grant(&am, idx, "admin");
    assert(approval_check(&am, "prod-db.internal", 5432) == 1);  /* approved */
    approval_destroy(&am);
    return 1;
}

static int test_activity(void)
{
    ActivityFeed af;
    activity_init(&af);
    activity_log(&af, ACT_CONNECT, "user", "host.com", "SSH connected");
    activity_log(&af, ACT_COMMAND, "user", "host.com", "ls -la");
    assert(af.count == 2);
    ActivityEvent recent[10];
    int n = activity_get_recent(&af, recent, 10, 60);
    assert(n == 2);
    activity_clear(&af);
    assert(af.count == 0);
    activity_destroy(&af);
    return 1;
}

static int test_restapi(void)
{
    RestServer rs;
    rest_init(&rs, 8080);
    assert(rs.port == 8080);
    rest_add_route(&rs, REST_GET, "/status", NULL, 0);
    assert(rs.route_count == 1);
    rest_start(&rs);
    char resp[256];
    int status = rest_process_request(&rs, REST_GET, "/status", NULL, resp, sizeof(resp));
    assert(status == 200);
    status = rest_process_request(&rs, REST_GET, "/notfound", NULL, resp, sizeof(resp));
    assert(status == 404);
    rest_stop(&rs);
    rest_destroy(&rs);
    return 1;
}

static int test_passgen(void)
{
    PassGenConfig cfg;
    cfg.length = 16;
    cfg.use_upper = 1;
    cfg.use_lower = 1;
    cfg.use_digits = 1;
    cfg.use_symbols = 0;
    cfg.avoid_ambiguous = 0;

    char buf[64];
    int len = passgen_generate(&cfg, buf, sizeof(buf));
    assert(len == 16);
    assert(strlen(buf) == 16);

    /* Check strength */
    int strength = passgen_strength(buf);
    assert(strength > 0);
    return 1;
}

static int test_focusring(void)
{
    FocusRing fr;
    focus_ring_init(&fr);
    assert(focus_ring_get(&fr) == FOCUS_TERMINAL);
    focus_ring_cycle(&fr);
    assert(focus_ring_get(&fr) == FOCUS_SIDEBAR);
    focus_ring_cycle(&fr);
    assert(focus_ring_get(&fr) == FOCUS_TABBAR);
    focus_ring_cycle(&fr);
    assert(focus_ring_get(&fr) == FOCUS_TERMINAL);
    assert(focus_ring_should_draw(&fr, FOCUS_TERMINAL) == 1);
    assert(focus_ring_should_draw(&fr, FOCUS_SIDEBAR) == 0);
    return 1;
}

static int test_taboverflow(void)
{
    TabOverflow to;
    tab_overflow_init(&to);
    tab_overflow_update(&to, 20, 800, 160);  /* 20 tabs, 800px, 160px each */
    assert(tab_overflow_needs_arrows(&to));
    assert(to.visible_tabs == 4);  /* 800/164 ≈ 4 */
    tab_overflow_scroll_right(&to);
    assert(tab_overflow_get_offset(&to) == 1);
    return 1;
}

static int test_sidebar_resize(void)
{
    SidebarResize sr;
    sidebar_resize_init(&sr, 240);
    assert(sidebar_resize_get_width(&sr) == 240);
    sidebar_resize_begin(&sr, 240);
    assert(sidebar_resize_is_dragging(&sr));
    sidebar_resize_move(&sr, 300);
    assert(sidebar_resize_get_width(&sr) == 300);
    sidebar_resize_end(&sr);
    assert(!sidebar_resize_is_dragging(&sr));
    return 1;
}

/* ═══════════════ Main ═══════════════ */

int main(void)
{
    printf("PuttyAlt Module Tests\n");
    printf("=====================\n\n");

    TEST(design_constants);
    TEST(version);
    TEST(gui_config);
    TEST(transitions);
    TEST(cmdpal);
    TEST(roles);
    TEST(validate);
    TEST(toast);
    TEST(scroll);
    TEST(cursor);
    TEST(tabnav);
    TEST(uptime);
    TEST(sessshare);
    TEST(expect);
    TEST(pipeline);
    TEST(scheduler);
    TEST(themeswitch);
    TEST(tags);
    TEST(approval);
    TEST(activity);
    TEST(restapi);
    TEST(passgen);
    TEST(focusring);
    TEST(taboverflow);
    TEST(sidebar_resize);

    printf("\n=====================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("=====================\n");

    return tests_passed == tests_run ? 0 : 1;
}
