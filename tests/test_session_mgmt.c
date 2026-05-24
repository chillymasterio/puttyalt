/*
 * test_session_mgmt.c — Tests for session management modules:
 *   sesstags, sessorder, validate, scrollstate, cursor, tabnav
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_sesstags.h"
#include "puttyalt_sessorder.h"
#include "puttyalt_validate.h"
#include "puttyalt_scrollstate.h"
#include "puttyalt_cursor.h"
#include "puttyalt_tabnav.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else { printf("FAIL\n"); } \
} while(0)

/* ═══════════ Session Tags Tests ════════���══ */

static int test_tag_add(void)
{
    TagStore ts;
    tags_init(&ts);
    int r = tags_add(&ts, "session1", "production");
    assert(r == 0);
    r = tags_add(&ts, "session1", "linux");
    assert(r == 0);
    r = tags_add(&ts, "session2", "staging");
    assert(r == 0);
    tags_destroy(&ts);
    return 1;
}

static int test_tag_remove(void)
{
    TagStore ts;
    tags_init(&ts);
    tags_add(&ts, "s1", "web");
    tags_add(&ts, "s1", "prod");
    int r = tags_remove(&ts, "s1", "web");
    assert(r == 0);
    char results[10][64];
    int n = tags_filter(&ts, "web", results, 10);
    assert(n == 0);
    n = tags_filter(&ts, "prod", results, 10);
    assert(n == 1);
    tags_destroy(&ts);
    return 1;
}

static int test_tag_filter(void)
{
    TagStore ts;
    tags_init(&ts);
    tags_add(&ts, "s1", "database");
    tags_add(&ts, "s2", "database");
    tags_add(&ts, "s3", "webserver");
    tags_add(&ts, "s4", "database");

    char results[10][64];
    int n = tags_filter(&ts, "database", results, 10);
    assert(n == 3);
    n = tags_filter(&ts, "webserver", results, 10);
    assert(n == 1);
    n = tags_filter(&ts, "nonexistent", results, 10);
    assert(n == 0);
    tags_destroy(&ts);
    return 1;
}

/* ═══��═══════ Session Order Tests ��══════════ */

static int test_sessorder_reorder(void)
{
    SessionOrder so;
    sessorder_init(&so);
    sessorder_add(&so, "sess_a", 0);
    sessorder_add(&so, "sess_b", 1);
    sessorder_add(&so, "sess_c", 2);
    assert(so.count == 3);

    int r = sessorder_move(&so, "sess_c", 0);
    assert(r == 0);
    assert(sessorder_get_pos(&so, "sess_c") == 0);
    sessorder_destroy(&so);
    return 1;
}

static int test_sessorder_move_up_down(void)
{
    SessionOrder so;
    sessorder_init(&so);
    sessorder_add(&so, "first", 0);
    sessorder_add(&so, "second", 1);
    sessorder_add(&so, "third", 2);

    /* Move "third" up to position 1 */
    sessorder_move(&so, "third", 1);
    assert(sessorder_get_pos(&so, "third") == 1);

    /* Move "first" down to position 2 */
    sessorder_move(&so, "first", 2);
    assert(sessorder_get_pos(&so, "first") == 2);
    sessorder_destroy(&so);
    return 1;
}

static int test_sessorder_remove(void)
{
    SessionOrder so;
    sessorder_init(&so);
    sessorder_add(&so, "a", 0);
    sessorder_add(&so, "b", 1);
    sessorder_add(&so, "c", 2);
    assert(so.count == 3);

    int r = sessorder_remove(&so, "b");
    assert(r == 0);
    assert(so.count == 2);
    sessorder_destroy(&so);
    return 1;
}

/* ═══════════ Validate Tests ���══════════ */

static int test_validate_hostname(void)
{
    assert(validate_hostname("example.com") == VALID_OK);
    assert(validate_hostname("sub.domain.org") == VALID_OK);
    assert(validate_hostname("192.168.1.1") == VALID_OK);
    assert(validate_hostname("") == VALID_EMPTY);
    assert(validate_hostname("bad..host") == VALID_INVALID_FORMAT);
    return 1;
}

static int test_validate_port_range(void)
{
    int port;
    assert(validate_port("22", &port) == VALID_OK);
    assert(port == 22);
    assert(validate_port("443", &port) == VALID_OK);
    assert(port == 443);
    assert(validate_port("65535", &port) == VALID_OK);
    assert(port == 65535);
    assert(validate_port("0", &port) == VALID_OUT_OF_RANGE);
    assert(validate_port("99999", &port) == VALID_OUT_OF_RANGE);
    assert(validate_port("-1", &port) == VALID_OUT_OF_RANGE);
    return 1;
}

static int test_validate_config_check(void)
{
    assert(validate_username("admin") == VALID_OK);
    assert(validate_username("user_name") == VALID_OK);
    assert(validate_ip4("192.168.1.1") == VALID_OK);
    assert(validate_ip4("10.0.0.1") == VALID_OK);
    assert(validate_ip4("999.0.0.1") == VALID_OUT_OF_RANGE);
    assert(validate_ip4("not.an.ip.addr") == VALID_OUT_OF_RANGE);
    return 1;
}

/* ══���════════ Scroll State Tests ═══════════ */

static int test_scroll_position(void)
{
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 500, 24);
    assert(ss.total_lines == 500);
    assert(ss.visible_lines == 24);
    assert(ss.scroll_max == 476);
    assert(ss.scroll_pos == 0);
    return 1;
}

static int test_scroll_page_up_down(void)
{
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 1000, 25);

    /* Page down */
    scroll_by(&ss, 25);
    assert(ss.scroll_pos == 25);

    /* Page down again */
    scroll_by(&ss, 25);
    assert(ss.scroll_pos == 50);

    /* Page up */
    scroll_by(&ss, -25);
    assert(ss.scroll_pos == 25);

    /* Scroll to bottom */
    scroll_to_bottom(&ss);
    assert(scroll_at_bottom(&ss));
    assert(ss.scroll_pos == ss.scroll_max);

    /* Scroll to top */
    scroll_to_top(&ss);
    assert(ss.scroll_pos == 0);
    return 1;
}

static int test_scroll_clamp(void)
{
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 100, 24);

    /* Try scrolling past bottom */
    scroll_to(&ss, 9999);
    assert(ss.scroll_pos <= ss.scroll_max);

    /* Try scrolling past top */
    scroll_to(&ss, -100);
    assert(ss.scroll_pos >= 0);
    return 1;
}

/* ═══════════ Cursor Tests ═══════════ */

static int test_cursor_blink(void)
{
    CursorState cs;
    cursor_init(&cs, CURSOR_BLOCK);
    assert(cs.blink == 1);
    assert(cs.blink_rate_ms > 0);

    cursor_set_blink(&cs, 0, 0);
    assert(cs.blink == 0);

    cursor_set_blink(&cs, 1, 600);
    assert(cs.blink == 1);
    assert(cs.blink_rate_ms == 600);
    return 1;
}

static int test_cursor_shape(void)
{
    CursorState cs;
    int rx, ry, rw, rh;

    cursor_init(&cs, CURSOR_BLOCK);
    cursor_get_rect(&cs, 0, 0, 8, 16, &rx, &ry, &rw, &rh);
    assert(rw == 8 && rh == 16);

    cursor_set_style(&cs, CURSOR_BAR);
    cursor_get_rect(&cs, 0, 0, 8, 16, &rx, &ry, &rw, &rh);
    assert(rw == 2); /* bar is thin */
    assert(rh == 16);

    cursor_set_style(&cs, CURSOR_UNDERLINE);
    cursor_get_rect(&cs, 0, 0, 8, 16, &rx, &ry, &rw, &rh);
    assert(rw == 8);
    assert(rh <= 3); /* underline is short */
    return 1;
}

static int test_cursor_visibility(void)
{
    CursorState cs;
    cursor_init(&cs, CURSOR_BLOCK);
    assert(cursor_is_visible(&cs));
    cs.visible = 0;
    assert(!cursor_is_visible(&cs));
    cs.visible = 1;
    assert(cursor_is_visible(&cs));
    return 1;
}

/* ═══════════ Tab Navigation Tests ═══════════ */

static int test_tabnav_switch(void)
{
    TabNav tn;
    tabnav_init(&tn);
    tabnav_set_active(&tn, 0, 5);
    assert(tn.active == 0);
    assert(tn.total == 5);

    int idx = tabnav_goto(&tn, 3);
    assert(idx == 3);
    assert(tn.active == 3);
    return 1;
}

static int test_tabnav_cycle(void)
{
    TabNav tn;
    tabnav_init(&tn);
    tabnav_set_active(&tn, 0, 4);

    assert(tabnav_next(&tn) == 1);
    assert(tabnav_next(&tn) == 2);
    assert(tabnav_next(&tn) == 3);
    assert(tabnav_next(&tn) == 0); /* wraps */

    assert(tabnav_prev(&tn) == 3); /* wraps backwards */
    assert(tabnav_prev(&tn) == 2);
    return 1;
}

static int test_tabnav_close(void)
{
    TabNav tn;
    tabnav_init(&tn);
    tabnav_set_active(&tn, 2, 5);
    int r = tabnav_close_current(&tn);
    assert(r >= 0);
    assert(tn.total == 4);
    return 1;
}

/* ═══════════ Main ═══════���═══ */

int main(void)
{
    printf("PuttyAlt Session Management Tests\n");
    printf("==================================\n\n");

    /* Session Tags */
    printf("[Session Tags]\n");
    RUN_TEST(tag_add);
    RUN_TEST(tag_remove);
    RUN_TEST(tag_filter);

    /* Session Order */
    printf("\n[Session Order]\n");
    RUN_TEST(sessorder_reorder);
    RUN_TEST(sessorder_move_up_down);
    RUN_TEST(sessorder_remove);

    /* Validate */
    printf("\n[Validate]\n");
    RUN_TEST(validate_hostname);
    RUN_TEST(validate_port_range);
    RUN_TEST(validate_config_check);

    /* Scroll State */
    printf("\n[Scroll State]\n");
    RUN_TEST(scroll_position);
    RUN_TEST(scroll_page_up_down);
    RUN_TEST(scroll_clamp);

    /* Cursor */
    printf("\n[Cursor]\n");
    RUN_TEST(cursor_blink);
    RUN_TEST(cursor_shape);
    RUN_TEST(cursor_visibility);

    /* Tab Navigation */
    printf("\n[Tab Navigation]\n");
    RUN_TEST(tabnav_switch);
    RUN_TEST(tabnav_cycle);
    RUN_TEST(tabnav_close);

    printf("\n==================================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("==================================\n");

    return tests_passed == tests_run ? 0 : 1;
}
