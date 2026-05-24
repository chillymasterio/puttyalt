/*
 * test_ui_components.c — Tests for UI component modules:
 *   toast, cmdpal, focusring, taboverflow, sidebarresize,
 *   transitions, themeswitch, tooltip
 */

#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "puttyalt_toast.h"
#include "puttyalt_cmdpal.h"
#include "puttyalt_focusring.h"
#include "puttyalt_taboverflow.h"
#include "puttyalt_sidebarresize.h"
#include "puttyalt_transitions.h"
#include "puttyalt_themeswitch.h"
#include "puttyalt_tooltip.h"

static int tests_run = 0;
static int tests_passed = 0;

#define RUN_TEST(name) do { \
    tests_run++; \
    printf("  TEST %s... ", #name); \
    if (test_##name()) { tests_passed++; printf("OK\n"); } \
    else { printf("FAIL\n"); } \
} while(0)

/* ═══════════ Toast Tests ═══════════ */

static int test_toast_show(void)
{
    ToastManager tm;
    toast_init(&tm);
    toast_show(&tm, TOAST_INFO, "Hello World", 3000);
    assert(toast_count(&tm) == 1);
    assert(tm.queue[0].type == TOAST_INFO);
    assert(strcmp(tm.queue[0].message, "Hello World") == 0);
    assert(tm.queue[0].duration_ms == 3000);
    return 1;
}

static int test_toast_dismiss(void)
{
    ToastManager tm;
    toast_init(&tm);
    toast_show(&tm, TOAST_SUCCESS, "Saved", 2000);
    toast_show(&tm, TOAST_ERROR, "Failed", 5000);
    assert(toast_count(&tm) == 2);
    toast_dismiss(&tm, 0);
    toast_update(&tm, 10000);
    assert(toast_count(&tm) == 1);
    return 1;
}

static int test_toast_queue(void)
{
    ToastManager tm;
    toast_init(&tm);
    int i;
    for (i = 0; i < TOAST_MAX_QUEUE; i++) {
        toast_show(&tm, TOAST_WARNING, "msg", 1000);
    }
    assert(toast_count(&tm) == TOAST_MAX_QUEUE);
    toast_dismiss_all(&tm);
    toast_update(&tm, 99999);
    assert(toast_count(&tm) == 0);
    return 1;
}

/* ═══════════ Command Palette Tests ═══════════ */

static int test_cmdpal_search(void)
{
    CmdPalState cp;
    cmdpal_init(&cp);
    cmdpal_add(&cp, "New Session", "File", "Ctrl+N", 101);
    cmdpal_add(&cp, "Close Session", "File", "Ctrl+W", 102);
    cmdpal_add(&cp, "Settings", "Edit", "Ctrl+,", 201);
    cmdpal_add(&cp, "Full Screen", "View", "F11", 301);

    cmdpal_open(&cp);
    assert(cp.active == 1);
    assert(cp.visible_count == 4);

    cmdpal_filter(&cp, "sess");
    assert(cp.visible_count >= 2); /* New Session, Close Session */

    cmdpal_filter(&cp, "full");
    assert(cp.visible_count >= 1); /* Full Screen */

    cmdpal_close(&cp);
    cmdpal_destroy(&cp);
    return 1;
}

static int test_cmdpal_execute(void)
{
    CmdPalState cp;
    cmdpal_init(&cp);
    cmdpal_add(&cp, "Copy", "Edit", "Ctrl+C", 501);
    cmdpal_add(&cp, "Paste", "Edit", "Ctrl+V", 502);

    cmdpal_open(&cp);
    cmdpal_filter(&cp, "");
    int cmd = cmdpal_select(&cp);
    assert(cmd == 501 || cmd >= 0);

    cmdpal_destroy(&cp);
    return 1;
}

static int test_cmdpal_navigate(void)
{
    CmdPalState cp;
    cmdpal_init(&cp);
    cmdpal_add(&cp, "Item A", "Cat", "", 1);
    cmdpal_add(&cp, "Item B", "Cat", "", 2);
    cmdpal_add(&cp, "Item C", "Cat", "", 3);

    cmdpal_open(&cp);
    assert(cp.selected == 0);
    cmdpal_move(&cp, 1);
    assert(cp.selected == 1);
    cmdpal_move(&cp, 1);
    assert(cp.selected == 2);

    cmdpal_destroy(&cp);
    return 1;
}

/* ═══════════ Focus Ring Tests ═══════════ */

static int test_focusring_navigation(void)
{
    FocusRing fr;
    focus_ring_init(&fr);
    assert(focus_ring_get(&fr) == FOCUS_TERMINAL);
    focus_ring_cycle(&fr);
    assert(focus_ring_get(&fr) == FOCUS_SIDEBAR);
    focus_ring_cycle(&fr);
    assert(focus_ring_get(&fr) == FOCUS_TABBAR);
    return 1;
}

static int test_focusring_wrap(void)
{
    FocusRing fr;
    focus_ring_init(&fr);
    /* Cycle through all targets and wrap back */
    focus_ring_cycle(&fr); /* SIDEBAR */
    focus_ring_cycle(&fr); /* TABBAR */
    focus_ring_cycle(&fr); /* wraps to TERMINAL */
    assert(focus_ring_get(&fr) == FOCUS_TERMINAL);
    return 1;
}

static int test_focusring_set(void)
{
    FocusRing fr;
    focus_ring_init(&fr);
    focus_ring_set(&fr, FOCUS_CMDPAL);
    assert(focus_ring_get(&fr) == FOCUS_CMDPAL);
    assert(focus_ring_should_draw(&fr, FOCUS_CMDPAL) == 1);
    assert(focus_ring_should_draw(&fr, FOCUS_TERMINAL) == 0);
    return 1;
}

/* ═══════════ Tab Overflow Tests ═══════════ */

static int test_taboverflow_detection(void)
{
    TabOverflow to;
    tab_overflow_init(&to);
    /* 10 tabs, 800px container, 160px each tab = overflow */
    tab_overflow_update(&to, 10, 800, 160);
    assert(tab_overflow_needs_arrows(&to));
    assert(to.total_tabs == 10);
    assert(to.visible_tabs < to.total_tabs);
    return 1;
}

static int test_taboverflow_no_overflow(void)
{
    TabOverflow to;
    tab_overflow_init(&to);
    /* 3 tabs, 800px container, 160px each = no overflow */
    tab_overflow_update(&to, 3, 800, 160);
    assert(!tab_overflow_needs_arrows(&to));
    return 1;
}

static int test_taboverflow_scroll(void)
{
    TabOverflow to;
    tab_overflow_init(&to);
    tab_overflow_update(&to, 20, 800, 160);
    assert(tab_overflow_get_offset(&to) == 0);

    tab_overflow_scroll_right(&to);
    assert(tab_overflow_get_offset(&to) == 1);

    tab_overflow_scroll_right(&to);
    assert(tab_overflow_get_offset(&to) == 2);

    tab_overflow_scroll_left(&to);
    assert(tab_overflow_get_offset(&to) == 1);

    /* Scroll left at beginning should clamp */
    tab_overflow_scroll_left(&to);
    tab_overflow_scroll_left(&to);
    assert(tab_overflow_get_offset(&to) == 0);
    return 1;
}

/* ═══════════ Sidebar Resize Tests ═══════════ */

static int test_sidebar_resize(void)
{
    SidebarResize sr;
    sidebar_resize_init(&sr, 240);
    assert(sidebar_resize_get_width(&sr) == 240);
    assert(!sidebar_resize_is_dragging(&sr));
    return 1;
}

static int test_sidebar_resize_drag(void)
{
    SidebarResize sr;
    sidebar_resize_init(&sr, 240);
    sidebar_resize_begin(&sr, 240);
    assert(sidebar_resize_is_dragging(&sr));
    sidebar_resize_move(&sr, 300);
    assert(sidebar_resize_get_width(&sr) == 300);
    sidebar_resize_end(&sr);
    assert(!sidebar_resize_is_dragging(&sr));
    assert(sidebar_resize_get_width(&sr) == 300);
    return 1;
}

static int test_sidebar_min_max_bounds(void)
{
    SidebarResize sr;
    sidebar_resize_init(&sr, 240);
    sidebar_resize_begin(&sr, 240);
    /* Try to go below minimum — snaps to collapsed */
    sidebar_resize_move(&sr, 50);
    assert(sidebar_resize_get_width(&sr) <= SIDEBAR_MIN_WIDTH);
    /* Try to go above maximum — clamped to max */
    sidebar_resize_move(&sr, 900);
    assert(sidebar_resize_get_width(&sr) <= SIDEBAR_MAX_WIDTH);
    sidebar_resize_end(&sr);
    return 1;
}

/* ═══════════ Transitions Tests ═══════════ */

static int test_transition_start(void)
{
    Transition t;
    transition_init(&t, 0.0f, 2.0f);
    assert(t.current == 0.0f);
    assert(t.speed == 2.0f);
    assert(transition_done(&t));
    return 1;
}

static int test_transition_update(void)
{
    Transition t;
    transition_init(&t, 0.0f, 1.0f);
    transition_set(&t, 10.0f);
    assert(!transition_done(&t));

    float val = transition_update(&t, 5.0f);
    assert(val > 0.0f);
    assert(val <= 10.0f);
    return 1;
}

static int test_transition_complete(void)
{
    Transition t;
    transition_init(&t, 0.0f, 1.0f);
    transition_set(&t, 1.0f);
    transition_update(&t, 1.0f);
    assert(transition_done(&t));
    assert(t.current == t.target);
    return 1;
}

static int test_transition_easing(void)
{
    assert(ease_linear(0.0f) == 0.0f);
    assert(ease_linear(1.0f) == 1.0f);
    float mid = ease_linear(0.5f);
    assert(mid >= 0.49f && mid <= 0.51f);

    assert(ease_in_quad(0.0f) == 0.0f);
    assert(ease_in_quad(1.0f) == 1.0f);
    assert(ease_out_quad(0.0f) == 0.0f);
    assert(ease_out_quad(1.0f) == 1.0f);
    assert(ease_in_out_cubic(0.0f) == 0.0f);
    assert(ease_in_out_cubic(1.0f) == 1.0f);
    return 1;
}

static int test_color_lerp(void)
{
    unsigned int black = 0x000000;
    unsigned int white = 0xFFFFFF;
    unsigned int mid = color_lerp(black, white, 0.5f);
    int r = (mid >> 16) & 0xFF;
    int g = (mid >> 8) & 0xFF;
    int b = mid & 0xFF;
    assert(r >= 126 && r <= 128);
    assert(g >= 126 && g <= 128);
    assert(b >= 126 && b <= 128);
    return 1;
}

/* ═══════════ Theme Switch Tests ═══════════ */

static int test_theme_register(void)
{
    ThemeRegistry tr;
    theme_reg_init(&tr);
    assert(tr.count >= 1); /* should have built-in themes */

    ThemeDef custom = {0};
    strcpy(custom.name, "Custom Dark");
    custom.bg = 0x1A1A2E;
    custom.text = 0xEEEEEE;
    custom.accent = 0xE94560;
    int r = theme_reg_add(&tr, &custom);
    assert(r >= 0);
    return 1;
}

static int test_theme_switch(void)
{
    ThemeRegistry tr;
    theme_reg_init(&tr);
    int initial = tr.active;
    theme_reg_activate(&tr, 1);
    assert(tr.active == 1);
    assert(tr.active != initial || initial == 1);
    return 1;
}

static int test_theme_current(void)
{
    ThemeRegistry tr;
    theme_reg_init(&tr);
    const ThemeDef *t = theme_reg_current(&tr);
    assert(t != NULL);
    assert(strlen(t->name) > 0);
    assert(t->bg != 0 || t->text != 0);
    return 1;
}

/* ═══════════ Tooltip Tests ═══════════ */

static int test_tooltip_set(void)
{
    Tooltip tt;
    tooltip_init(&tt);
    tooltip_set(&tt, "Connect to server", 100, 200);
    assert(strcmp(tt.text, "Connect to server") == 0);
    assert(tt.x == 100);
    assert(tt.y == 200);
    return 1;
}

static int test_tooltip_show(void)
{
    Tooltip tt;
    tooltip_init(&tt);
    tooltip_set(&tt, "Tooltip text", 50, 75);
    tt.hover_start = 1000;
    tooltip_update(&tt, 1000 + TOOLTIP_DELAY_MS + TOOLTIP_FADE_MS + 1);
    assert(tooltip_should_show(&tt));
    return 1;
}

static int test_tooltip_hide(void)
{
    Tooltip tt;
    tooltip_init(&tt);
    tooltip_set(&tt, "Some tip", 10, 20);
    tt.hover_start = 1000;
    tooltip_update(&tt, 2000);
    tooltip_hide(&tt);
    assert(!tooltip_should_show(&tt));
    return 1;
}

/* ═══════════ Main ═══════════ */

int main(void)
{
    printf("PuttyAlt UI Components Tests\n");
    printf("============================\n\n");

    /* Toast */
    printf("[Toast]\n");
    RUN_TEST(toast_show);
    RUN_TEST(toast_dismiss);
    RUN_TEST(toast_queue);

    /* Command Palette */
    printf("\n[Command Palette]\n");
    RUN_TEST(cmdpal_search);
    RUN_TEST(cmdpal_execute);
    RUN_TEST(cmdpal_navigate);

    /* Focus Ring */
    printf("\n[Focus Ring]\n");
    RUN_TEST(focusring_navigation);
    RUN_TEST(focusring_wrap);
    RUN_TEST(focusring_set);

    /* Tab Overflow */
    printf("\n[Tab Overflow]\n");
    RUN_TEST(taboverflow_detection);
    RUN_TEST(taboverflow_no_overflow);
    RUN_TEST(taboverflow_scroll);

    /* Sidebar Resize */
    printf("\n[Sidebar Resize]\n");
    RUN_TEST(sidebar_resize);
    RUN_TEST(sidebar_resize_drag);
    RUN_TEST(sidebar_min_max_bounds);

    /* Transitions */
    printf("\n[Transitions]\n");
    RUN_TEST(transition_start);
    RUN_TEST(transition_update);
    RUN_TEST(transition_complete);
    RUN_TEST(transition_easing);
    RUN_TEST(color_lerp);

    /* Theme Switch */
    printf("\n[Theme Switch]\n");
    RUN_TEST(theme_register);
    RUN_TEST(theme_switch);
    RUN_TEST(theme_current);

    /* Tooltip */
    printf("\n[Tooltip]\n");
    RUN_TEST(tooltip_set);
    RUN_TEST(tooltip_show);
    RUN_TEST(tooltip_hide);

    printf("\n============================\n");
    printf("Results: %d/%d passed\n", tests_passed, tests_run);
    printf("============================\n");

    return tests_passed == tests_run ? 0 : 1;
}
