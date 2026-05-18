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
- [ ] First-connect suggestions (OS detection)
- [ ] Connection speed test on connect
- [ ] Tab preview on hover
- [ ] Session diff — compare two recordings side-by-side

## v1.4.0 (Next) — Collaboration & Sharing
*Focus: team workflows and compliance.*

### Planned
- [ ] **Shared snippet libraries** — sync via git or network share
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

## v2.0.0 (Planned) — Modern UI Rewrite
**Complete UI rewrite with modern rendering engine.**

### Rendering
- GPU-accelerated rendering (Direct2D / OpenGL / Vulkan)
- 120 FPS terminal with smooth animations
- Subpixel font rendering with gamma correction
- Ligature support via HarfBuzz
- True color (24-bit RGB) — parser ready since v1.1.0
- Inline images (iTerm2 / Sixel / Kitty graphics)

### Layout
- Split panes (horizontal, vertical, nested, drag resize)
- Floating panels with snap-to-edge
- Minimap (scrollback overview)
- Session thumbnails in tab bar
- Zen mode (distraction-free fullscreen)

### Interaction
- Smooth scrolling with momentum and touchpad gestures
- Search overlay with regex and live highlight
- Command palette (Ctrl+Shift+P)
- Inline tab rename (double-click)
- Drag tabs between windows

### Architecture
- Direct2D / SDL2 hybrid renderer
- UILayer abstraction (ready since v1.1.0)
- Plugin API v2 with sandboxed execution
- WebSocket-based remote access (optional)

### Timeline
- Q3 2025: Renderer prototype
- Q4 2025: Widget toolkit + split panes
- Q1 2026: Beta with feature parity
- Q2 2026: Stable v2.0.0

## v2.1.0 (Vision)
- Local AI command suggestions (no cloud)
- Cloud session sync (encrypted, opt-in)
- Team collaboration (shared terminals)
- Mobile companion (iOS/Android viewer)
- Native Linux packages (deb/rpm/AppImage)
- Native macOS build (Universal Binary)
