# PuttyAlt Architecture

## Module Organization

```
puttyalt/
├── Core
│   ├── puttyalt.h          — Version, branding, feature flags
│   ├── main.c              — Entry point (WinMain / main)
│   └── puttyalt_config.c   — INI config parser
│
├── GUI Framework
│   ├── puttyalt_gui.h/c    — Main window, menu, toolbar
│   ├── puttyalt_dialogs.h/c — Dialog windows (Connect, Settings, etc.)
│   ├── puttyalt_uilayer.h/c — UI abstraction layer
│   └── puttyalt_ctxmenu.h/c — Right-click context menus
│
├── Terminal Engine (v1.1.0)
│   ├── puttyalt_termemu.h/c  — Terminal state, cells, cursor
│   ├── puttyalt_ansiparse.h/c — ANSI/VT escape sequences
│   ├── puttyalt_selection.h/c — Text selection modes
│   ├── puttyalt_ringbuf.h/c   — Scrollback ring buffer
│   └── puttyalt_colormgr.h/c  — 256-color palette
│
├── Session Management
│   ├── puttyalt_favorites.h/c   — Session favorites store
│   ├── puttyalt_tabmgr.h/c      — Tab management
│   ├── puttyalt_quickbar.h/c    — Quick connect bar
│   ├── puttyalt_broadcast_v2.h/c — Multi-session broadcast
│   └── puttyalt_sessreplay.h/c  — Session recording
│
├── Connection
│   ├── puttyalt_autoreconn.h/c — Auto-reconnect engine
│   ├── puttyalt_healthchk.h/c  — Connection health monitor
│   └── puttyalt_jumpchain.h/c  — Jump host chains
│
└── UI Components
    ├── puttyalt_dockpanel.h/c  — Dockable panel layout
    ├── puttyalt_tooltip.h/c    — Tooltip system
    ├── puttyalt_fontchain.h/c  — Font fallback
    ├── puttyalt_bellctl.h/c    — Bell controller
    └── puttyalt_wizard.h/c     — Setup wizard
```

## Design Principles

1. **Single EXE** — No DLL dependencies, static linking
2. **Portable** — Zero registry, settings in local files
3. **Layered** — UI abstracted from logic for framework migration
4. **Cross-platform stubs** — All modules compile on Unix (as stubs)
5. **AV-clean** — No suspicious Win32 APIs (LoadLibrary, ShellExecute, etc.)

## v1.2.0 — UX Modules

```
├── puttyalt_smartpaste.h/c     — Dangerous paste detection
├── puttyalt_sesssearch.h/c     — Fuzzy session search
├── puttyalt_passgen.h/c        — Password generator
├── puttyalt_dragdrop.h/c       — File drag & drop
├── puttyalt_snippetlib.h/c     — Categorized snippet library
├── puttyalt_sessnote.h/c       — Session annotations
├── puttyalt_presets.h/c        — Connection presets
├── puttyalt_notifqueue.h/c     — Notification queue
├── puttyalt_searchoverlay.h/c  — Terminal search overlay
├── puttyalt_sshkeymgr.h/c      — SSH key manager
├── puttyalt_autocomplete_v2.h/c — Context-aware autocomplete
├── puttyalt_conntimeout.h/c    — Timeout profiles
├── puttyalt_multiclip.h/c      — Multi-clipboard (10 slots)
├── puttyalt_envview.h/c        — Environment viewer
└── puttyalt_quickcmd.h/c       — Quick command panel
```

## v1.3.0 — Workflow Modules

```
├── puttyalt_sessgroup_v2.h/c   — Nested session groups
├── puttyalt_cmdhist_v2.h/c     — Cross-session command history
├── puttyalt_sftpsync.h/c       — SFTP folder sync
├── puttyalt_outputcap.h/c      — Output capture
├── puttyalt_conndash.h/c       — Connection dashboard
├── puttyalt_regtrig.h/c        — Regex trigger engine
├── puttyalt_fileedit.h/c       — Inline file editor
├── puttyalt_cronview.h/c       — Cron job viewer
├── puttyalt_smartresize.h/c    — Smart window resize
├── puttyalt_tabcolors.h/c      — Tab color tags
└── puttyalt_sshexport.h/c      — OpenSSH config export
```
