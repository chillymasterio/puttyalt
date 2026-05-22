/*
 * puttyalt_design.h — Design System v2.0
 * Modern dark theme inspired by GitHub Dark / VS Code / Warp Terminal.
 * All rendering uses these constants for visual consistency.
 */

#ifndef PUTTYALT_DESIGN_H
#define PUTTYALT_DESIGN_H

/* ═══════════════════════════════════════════
 *  Color Palette — GitHub Dark Dimmed
 * ═══════════════════════════════════════════ */

/* Backgrounds */
#define DS_BG              0x0D1117   /* Main background */
#define DS_SURFACE         0x161B22   /* Cards, panels, sidebar */
#define DS_SURFACE_HOVER   0x1C2128   /* Hover state on surfaces */
#define DS_SURFACE_ACTIVE  0x21262D   /* Active/pressed state */
#define DS_OVERLAY         0x1C2128   /* Popups, command palette */

/* Borders */
#define DS_BORDER          0x30363D   /* Standard border */
#define DS_BORDER_SUBTLE   0x21262D   /* Subtle separators */
#define DS_BORDER_FOCUS    0x58A6FF   /* Focus ring */

/* Text */
#define DS_TEXT            0xE6EDF3   /* Primary text */
#define DS_TEXT_SECONDARY  0x8B949E   /* Secondary / dimmed */
#define DS_TEXT_MUTED      0x484F58   /* Muted / disabled */
#define DS_TEXT_LINK       0x58A6FF   /* Links / clickable */

/* Accent colors */
#define DS_ACCENT          0x58A6FF   /* Primary accent (blue) */
#define DS_ACCENT_HOVER    0x79C0FF   /* Accent hover */
#define DS_ACCENT_MUTED    0x388BFD   /* Accent dimmed */

/* Semantic colors */
#define DS_SUCCESS         0x3FB950   /* Connected, success */
#define DS_ERROR           0xF85149   /* Error, disconnect */
#define DS_WARNING         0xD29922   /* Warning */
#define DS_INFO            0x58A6FF   /* Info (same as accent) */

/* Terminal specific */
#define DS_TERM_BG         0x0D1117   /* Terminal background */
#define DS_TERM_FG         0xE6EDF3   /* Terminal foreground */
#define DS_TERM_CURSOR     0x58A6FF   /* Cursor color */
#define DS_TERM_SELECTION  0x264F78   /* Selection highlight */

/* Tab bar */
#define DS_TAB_BG          0x010409   /* Tab bar background */
#define DS_TAB_ACTIVE      0x0D1117   /* Active tab */
#define DS_TAB_HOVER       0x161B22   /* Tab hover */
#define DS_TAB_BORDER      0x30363D   /* Tab separator */

/* Sidebar */
#define DS_SIDEBAR_BG      0x010409   /* Sidebar background */
#define DS_SIDEBAR_ITEM    0x161B22   /* Item hover */
#define DS_SIDEBAR_ACTIVE  0x1F6FEB   /* Active item accent */

/* Status bar */
#define DS_STATUS_BG       0x010409   /* Status background */
#define DS_STATUS_FG       0x8B949E   /* Status text */

/* ═══════════════════════════════════════════
 *  Spacing (base unit: 4px)
 * ═══════════════════════════════════════════ */

#define DS_SPACE_XS   4
#define DS_SPACE_SM   8
#define DS_SPACE_MD   12
#define DS_SPACE_LG   16
#define DS_SPACE_XL   24
#define DS_SPACE_XXL  32

/* ═══════════════════════════════════════════
 *  Typography
 * ═══════════════════════════════════════════ */

#define DS_FONT_UI          "Segoe UI"
#define DS_FONT_MONO        "Cascadia Code"
#define DS_FONT_FALLBACK    "Consolas"

#define DS_FONT_SIZE_XS     10
#define DS_FONT_SIZE_SM     11
#define DS_FONT_SIZE_MD     12
#define DS_FONT_SIZE_LG     14
#define DS_FONT_SIZE_XL     16
#define DS_FONT_SIZE_TITLE  18

/* ═══════════════════════════════════════════
 *  Layout
 * ═══════════════════════════════════════════ */

#define DS_SIDEBAR_WIDTH    240
#define DS_TABBAR_HEIGHT    36
#define DS_TOOLBAR_HEIGHT   0     /* Hidden by default in v2.0 */
#define DS_STATUSBAR_HEIGHT 24
#define DS_BORDER_RADIUS    6
#define DS_TAB_HEIGHT       34
#define DS_TAB_MIN_WIDTH    120
#define DS_TAB_MAX_WIDTH    200
#define DS_TAB_CLOSE_SIZE   16

/* ═══════════════════════════════════════════
 *  Animation (milliseconds)
 * ═══════════════════════════════════════════ */

#define DS_ANIM_FAST    100
#define DS_ANIM_NORMAL  150
#define DS_ANIM_SLOW    300

/* ═══════════════════════════════════════════
 *  DWM Dark Mode (Windows 10 1809+)
 *  Note: actual constants are in dwmapi.h or
 *  defined locally in puttyalt_gui.c
 * ═══════════════════════════════════════════ */

#define DS_DWMWA_DARK_MODE   20
#define DS_DWMWA_CAPTION     35
#define DS_DWMWA_BORDER      34

/* Welcome screen layout */
#define DS_WELCOME_TITLE_SIZE  28
#define DS_WELCOME_SUB_SIZE    14
#define DS_WELCOME_KEY_COL     120

#endif /* PUTTYALT_DESIGN_H */
