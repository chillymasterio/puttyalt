# PuttyAlt Roadmap

## Philosophy
PuttyAlt aims to be the most **convenient** SSH client for daily work.
Every feature is measured by: *Does this save the user time or reduce friction?*

---

## v1.1.0 (Released) — Terminal Engine & UI Foundation
- [x] Terminal emulation engine with ANSI parser
- [x] Text selection (char, word, line, rectangle)
- [x] Scrollback ring buffer with search
- [x] 256-color palette, font fallback chain
- [x] URL detection, context menus, quick connect bar
- [x] Session favorites, health monitoring, auto-reconnect
- [x] Jump host chains, UI abstraction layer, dock panels
- [x] Tab manager, broadcast v2, session replay, setup wizard

## v1.2.0 (Released) — UX & Usability
- [x] Smart paste — dangerous command warnings
- [x] Session search — fuzzy matching across names/hosts/tags
- [x] Password generator, drag & drop, snippet library
- [x] Session notes, connection presets (Default/Dev/Prod/Presentation)
- [x] Notification queue, search overlay (Ctrl+F)
- [x] SSH key manager, auto-complete v2, timeout profiles
- [x] Multi-clipboard (10 slots), env viewer, quick commands

## v1.3.0 (Current) — Workflow & Productivity
*Focus: reduce repetitive work, help power users.*

- [x] **Session groups v2** — nested folders, color-coded, cycle-safe nesting
- [x] **Command history browser** — cross-session search with deduplication
- [x] **SFTP sync** — watch local folder, auto-upload, exclude patterns
- [x] **Output capture** — save terminal output with ANSI stripping and timestamps
- [x] **Connection dashboard** — live status, latency, traffic for all sessions
- [x] **Regex triggers v2** — pattern-match output → notify/log/sound/highlight
- [x] **Inline file editor** — line-based editor with search and CR/LF support
- [x] **Cron job viewer** — parse crontab with human-readable schedule descriptions
- [x] **Smart resize** — auto font scaling to fit window, character grid snapping
- [x] **Tab color tags** — Production/Staging/Dev labels with colored dots
- [x] **SSH config export** — export/import sessions to OpenSSH config format

### Still planned for v1.3.x patches
- [x] First-connect suggestions (OS detection)
- [x] Connection speed test on connect
- [x] Tab preview on hover
- [x] Session diff — compare two recordings side-by-side

## v1.4.0 (Next) — Collaboration & Sharing
*Focus: team workflows and compliance.*

### Planned
- [x] **Shared snippet libraries** — sync via git or network share
- [ ] **Session sharing** — read-only terminal stream for pair programming
- [ ] **Audit trail** — compliance-ready log of all sessions and commands
- [ ] **Role-based profiles** — restrict targets per user, enforce presets
- [ ] **Config sync** — export/import all settings as portable profile
- [ ] **Team templates** — shared connection templates with locked fields
- [ ] **Activity feed** — who connected where and when (opt-in)
- [ ] **Approval workflow** — request access to sensitive hosts

## v1.5.0 (Future) — Scripting & Automation
- [ ] **Lua scripting engine** — automate terminal interactions
- [ ] **Expect-like automation** — wait/send patterns for login scripts
- [ ] **Scheduled tasks** — run commands on cron-like schedule per session
- [ ] **Pipeline builder** — chain commands across multiple sessions
- [ ] **REST API** — control PuttyAlt from external tools
- [ ] **Webhook triggers** — fire webhooks on connection events

## v2.0.0 (Released) — Modern UI Rewrite
**Complete UI rewrite with custom rendering engine.**

### Rendering
- [x] Double-buffered GDI rendering (flicker-free)
- [x] 120 FPS message pump with 8ms sleep cap
- [x] ClearType font rendering
- [ ] GPU-accelerated rendering (Direct2D / Vulkan) — planned v2.1
- [ ] Ligature support via HarfBuzz — planned v2.1
- [x] True color (24-bit RGB) — parser ready since v1.1.0
- [ ] Inline images (iTerm2 / Sixel / Kitty graphics) — planned v2.1

### Layout
- [x] Custom owner-drawn tab bar with close buttons
- [x] Custom owner-drawn sidebar with sections
- [x] Custom owner-drawn status bar
- [x] DWM dark title bar integration
- [ ] Split panes (horizontal, vertical) — planned v2.1
- [ ] Floating panels with snap-to-edge — planned v2.1
- [x] Zen mode (fullscreen via F11)

### Design System
- [x] GitHub Dark inspired color palette
- [x] Typography scale (Segoe UI + Cascadia Code)
- [x] 4px spacing grid
- [x] Rounded rectangle primitives
- [x] Semantic color tokens (success, error, warning, info)
- [x] Dark-themed dialogs matching new palette

### Architecture
- [x] Design system header (puttyalt_design.h)
- [x] PaintBuffer double-buffering abstraction
- [x] Custom window classes (tab bar, sidebar, terminal)
- [x] UILayer abstraction (ready since v1.1.0)
- [ ] Plugin API v2 with sandboxed execution — planned v2.1
- [ ] WebSocket-based remote access (optional) — planned v2.1

## v2.1.0 (Vision)
- Local AI command suggestions (no cloud)
- Cloud session sync (encrypted, opt-in)
- Team collaboration (shared terminals)
- Mobile companion (iOS/Android viewer)
- Native Linux packages (deb/rpm/AppImage)
- Native macOS build (Universal Binary)
