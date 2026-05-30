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

## v1.4.0 (Released) — Collaboration & Sharing
*Focus: team workflows and compliance.*
- [x] **Shared snippet libraries** — sync via git or network share
- [x] **Session sharing** — read-only terminal stream for pair programming
- [x] **Audit trail** — compliance-ready log of all sessions and commands
- [x] **Role-based profiles** — restrict targets per user, enforce presets
- [x] **Config sync** — export/import all settings as portable profile
- [x] **Team templates** — shared connection templates with locked fields
- [x] **Activity feed** — who connected where and when (opt-in)
- [x] **Approval workflow** — request access to sensitive hosts

## v1.5.0 (Released) — Scripting & Automation
- [x] **Lua scripting engine** — automate terminal interactions
- [x] **Expect-like automation** — wait/send patterns for login scripts
- [x] **Scheduled tasks** — run commands on cron-like schedule per session
- [x] **Pipeline builder** — chain commands across multiple sessions
- [x] **REST API** — control PuttyAlt from external tools
- [x] **Webhook triggers** — fire webhooks on connection events

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

## v2.1.0 (In Progress) — Performance & Extensibility
*Focus: modern rendering, plugin ecosystem, and cloud features.*

### Split panes
- [x] **Split pane support** — binary split tree, horizontal and vertical splitting
- [x] **Pane focus navigation** — directional (h/j/k/l) and cyclic focus moves
- [x] **Pane resize** — divider drag-resize with per-pane minimum-size clamps
- [x] **Pane zoom** — toggle one pane fullscreen with geometry restore
- [x] **Multi-pane broadcast** — fan keystrokes to all panes in a group

### Rendering
- [x] **Direct2D backend abstraction** — hardware path with software fallback
- [x] **Glyph-run render cache** — LRU cache to skip re-rasterizing cells
- [x] **Dirty-rectangle tracking** — coalesced partial redraw
- [x] **Frame pacer** — target-FPS budget with skip logic
- [ ] **Ligature support** — HarfBuzz integration for programming fonts

### Inline images
- [x] **iTerm2 protocol** — OSC 1337 inline image parser
- [x] **Kitty graphics protocol** — APC `_G` sequence parser
- [x] **Image cell placement** — grid anchoring and overlap resolution
- [x] **Image scaling** — aspect-preserving cell-fit geometry
- [x] **Graphics memory budget** — byte ledger with LRU eviction
- [x] **OSC/APC router** — dispatch table isolating image detection

### Plugin API v2 (sandboxed)
- [x] **Manifest parser** — INI-style plugin descriptor with capabilities
- [x] **Capability grants** — manifest → permission set enforcement
- [x] **Sandboxed message bus** — capability-checked, rate/size-limited
- [x] **Lifecycle state machine** — load/enable/disable/unload transitions
- [x] **Plugin registry** — id/name-indexed loaded-plugin table
- [x] **Hook dispatch** — priority-ordered event delivery to plugins

### Cloud sync
- [x] **Encrypted envelope codec** — versioned, checksummed sync payloads
- [x] **Conflict resolution** — three-way merge, last-writer-wins
- [ ] **Cloud transport** — encrypted, opt-in remote synchronization

### AI command suggestions
- [x] **Weighted ranker** — prefix + recency + frequency scoring
- [x] **Decaying frequency model** — time-decayed command usage stats
- [x] **Fuzzy history index** — subsequence matching over history
- [x] **Suggestion cache** — TTL-keyed result cache

### Workspace & layout
- [x] **Layout save/restore** — named window/pane layout store
- [x] **Workspace manager** — group sessions, active-workspace switching
- [x] **Floating panel snap** — snap-to-edge magnetism
- [x] **Session restore on startup** — replay open sessions on launch
- [x] **Quick-launch entries** — palette command registry
