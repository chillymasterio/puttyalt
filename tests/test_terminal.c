/*
 * test_terminal.c — Unit tests for terminal emulation modules:
 *   terminal emulator, ANSI parser, selection, scrollback.
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#include "puttyalt_termemu.h"
#include "puttyalt_ansiparse.h"
#include "puttyalt_selection.h"
#include "puttyalt_scrollstate.h"

static int tests_run = 0;
static int tests_passed = 0;

#define TEST(name) do { \
    tests_run++; \
    printf("  TEST: %s ... ", #name); \
} while(0)

#define PASS() do { tests_passed++; printf("PASS\n"); } while(0)
#define FAIL(msg) do { printf("FAIL: %s\n", msg); } while(0)

/* --- Terminal Emulator Tests --- */

static void test_term_init(void)
{
    TEST(term_init);
    TermState ts;
    int rc = term_init(&ts, 24, 80);
    assert(rc == 0);
    assert(ts.rows == 24);
    assert(ts.cols == 80);
    assert(ts.cursor_row == 0);
    assert(ts.cursor_col == 0);
    assert(ts.cursor_visible == 1);
    term_destroy(&ts);
    PASS();
}

static void test_term_write_ascii(void)
{
    TEST(term_write_ascii);
    TermState ts;
    term_init(&ts, 24, 80);

    const uint8_t text[] = "Hello";
    term_write(&ts, text, 5);

    /* Verify characters were placed */
    TermCell *c = term_cell_at(&ts, 0, 0);
    assert(c && c->ch == 'H');
    c = term_cell_at(&ts, 0, 4);
    assert(c && c->ch == 'o');

    term_destroy(&ts);
    PASS();
}

static void test_term_cursor_movement(void)
{
    TEST(term_cursor_movement);
    TermState ts;
    term_init(&ts, 24, 80);

    term_set_cursor(&ts, 5, 10);
    assert(ts.cursor_row == 5);
    assert(ts.cursor_col == 10);

    /* Out of bounds should clamp */
    term_set_cursor(&ts, 100, 200);
    assert(ts.cursor_row <= ts.rows - 1);
    assert(ts.cursor_col <= ts.cols - 1);

    term_destroy(&ts);
    PASS();
}

static void test_term_scroll_up(void)
{
    TEST(term_scroll_up);
    TermState ts;
    term_init(&ts, 24, 80);

    /* Write something on row 0 */
    const uint8_t text[] = "Line0";
    term_write(&ts, text, 5);

    /* Scroll up by 1 - row 0 content moves off screen */
    term_scroll_up(&ts, 1);

    /* Row 0 should now be empty (new blank line scrolled in at bottom) */
    TermCell *c = term_cell_at(&ts, ts.rows - 1, 0);
    assert(c && c->ch == ' ');

    term_destroy(&ts);
    PASS();
}

static void test_term_erase_line(void)
{
    TEST(term_erase_line);
    TermState ts;
    term_init(&ts, 24, 80);

    const uint8_t text[] = "ABCDEFGH";
    term_write(&ts, text, 8);

    term_set_cursor(&ts, 0, 4);
    term_erase_line(&ts, 0); /* erase from cursor to end */

    TermCell *c = term_cell_at(&ts, 0, 4);
    assert(c && c->ch == ' ');

    /* Characters before cursor should remain */
    c = term_cell_at(&ts, 0, 0);
    assert(c && c->ch == 'A');

    term_destroy(&ts);
    PASS();
}

/* --- ANSI Parser Tests --- */

static void test_ansi_init(void)
{
    TEST(ansi_init);
    AnsiParser ap;
    ansi_init(&ap);
    assert(ap.state == ANSI_STATE_GROUND);
    assert(ap.param_count == 0);
    PASS();
}

static void test_ansi_csi_cursor_move(void)
{
    TEST(ansi_csi_cursor_move);
    TermState ts;
    term_init(&ts, 24, 80);
    AnsiParser ap;
    ansi_init(&ap);

    /* ESC [ 5 ; 10 H  = move cursor to row 5, col 10 */
    const uint8_t seq[] = "\033[5;10H";
    for (int i = 0; seq[i]; i++)
        ansi_feed(&ap, &ts, seq[i]);

    /* ANSI uses 1-based, internal is 0-based */
    assert(ts.cursor_row == 4);
    assert(ts.cursor_col == 9);

    term_destroy(&ts);
    PASS();
}

static void test_ansi_sgr_bold(void)
{
    TEST(ansi_sgr_bold);
    TermState ts;
    term_init(&ts, 24, 80);
    AnsiParser ap;
    ansi_init(&ap);

    /* ESC [ 1 m = set bold */
    const uint8_t seq[] = "\033[1m";
    for (int i = 0; seq[i]; i++)
        ansi_feed(&ap, &ts, seq[i]);

    assert(ts.current_attr & 1); /* bold bit */

    term_destroy(&ts);
    PASS();
}

/* --- Selection Tests --- */

static void test_selection_init(void)
{
    TEST(selection_init);
    Selection sel;
    sel_init(&sel);
    assert(sel.active == 0);
    assert(sel.mode == SEL_NONE);
    sel_destroy(&sel);
    PASS();
}

static void test_selection_start_and_update(void)
{
    TEST(selection_start_update);
    Selection sel;
    sel_init(&sel);

    sel_start(&sel, 3, 5, SEL_CHAR);
    assert(sel.active == 1);
    assert(sel.start_row == 3);
    assert(sel.start_col == 5);

    sel_update(&sel, 3, 20);
    assert(sel.end_row == 3);
    assert(sel.end_col == 20);

    sel_clear(&sel);
    assert(sel.active == 0);

    sel_destroy(&sel);
    PASS();
}

static void test_selection_is_selected(void)
{
    TEST(selection_is_selected);
    Selection sel;
    sel_init(&sel);

    sel_start(&sel, 2, 5, SEL_CHAR);
    sel_update(&sel, 2, 15);

    assert(sel_is_selected(&sel, 2, 10) == 1);
    assert(sel_is_selected(&sel, 2, 20) == 0);
    assert(sel_is_selected(&sel, 0, 5) == 0);

    sel_destroy(&sel);
    PASS();
}

/* --- Scrollback Tests --- */

static void test_scroll_init(void)
{
    TEST(scroll_init);
    ScrollState ss;
    scroll_init(&ss);
    assert(ss.scroll_pos == 0);
    assert(ss.visible_lines == 24);
    assert(ss.auto_scroll == 1);
    PASS();
}

static void test_scroll_set_content(void)
{
    TEST(scroll_set_content);
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 100, 24);
    assert(ss.total_lines == 100);
    assert(ss.scroll_max == 76); /* 100 - 24 */
    PASS();
}

static void test_scroll_navigation(void)
{
    TEST(scroll_navigation);
    ScrollState ss;
    scroll_init(&ss);
    scroll_set_content(&ss, 200, 24);

    scroll_to(&ss, 50);
    assert(ss.scroll_pos == 50);

    scroll_by(&ss, 10);
    assert(ss.scroll_pos == 60);

    /* Should clamp at max */
    scroll_to(&ss, 9999);
    assert(ss.scroll_pos == ss.scroll_max);

    /* Should clamp at 0 */
    scroll_to(&ss, -100);
    assert(ss.scroll_pos == 0);
    PASS();
}

/* --- Main --- */

int main(void)
{
    printf("=== Terminal Module Tests ===\n\n");

    printf("[Terminal Emulator]\n");
    test_term_init();
    test_term_write_ascii();
    test_term_cursor_movement();
    test_term_scroll_up();
    test_term_erase_line();

    printf("\n[ANSI Parser]\n");
    test_ansi_init();
    test_ansi_csi_cursor_move();
    test_ansi_sgr_bold();

    printf("\n[Selection]\n");
    test_selection_init();
    test_selection_start_and_update();
    test_selection_is_selected();

    printf("\n[Scrollback]\n");
    test_scroll_init();
    test_scroll_set_content();
    test_scroll_navigation();

    printf("\n=== Results: %d/%d passed ===\n", tests_passed, tests_run);
    return tests_passed == tests_run ? 0 : 1;
}
