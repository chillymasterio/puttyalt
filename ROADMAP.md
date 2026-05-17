# PuttyAlt Roadmap

## Philosophy
PuttyAlt aims to be the most **convenient** SSH client for daily work.
Every feature is measured by: *Does this save the user time or reduce friction?*

---

## v1.1.0 (Released) — Terminal Engine & UI Foundation
- [x] Terminal emulation engine with ANSI parser
- [x] Text selection (char, word, line, rectangle)
- [x] Scrollback ring buffer with search
- [x] 256-color palette manager
- [x] Font fallback chain
- [x] URL detection and clickable links
- [x] Right-click context menus
- [x] Quick connect bar
- [x] Session favorites with tags
- [x] Connection health monitoring
- [x] Auto-reconnect with exponential backoff
- [x] Jump host chain manager
- [x] UI abstraction layer
- [x] Dockable panel layout
- [x] Tab manager with pin/move/close
- [x] Broadcast v2 with filters
- [x] Session recording and replay
- [x] Setup wizard
- [x] Configuration validation

## v1.2.0 (Current) — UX & Usability
*Focus: make every daily task faster and safer.*

- [x] Smart paste — warn before pasting dangerous commands (rm -rf, sudo, curl|bash)
- [x] Session search — fuzzy matching across names, hosts, tags
- [x] Password generator — configurable strength, entropy meter
- [x] Drag & drop — files to SFTP upload or path paste
- [x] Snippet library — categorized commands with search and defaults
- [x] Session notes — per-connection annotations, auto-display on connect
- [x] Connection presets — Default, Dev, Production, Presentation profiles
- [x] Notification queue — leveled alerts with actions and sound control
- [x] Search overlay — Ctrl+F with highlight-all and case toggle
- [x] SSH key manager — auto-detect types, agent integration, host matching
- [x] Auto-complete v2 — history learning, prefix matching, context-aware
- [x] Connection timeouts — fast/default/patient profiles
- [x] Multi-clipboard — 10 slots with ring mode and labels
- [x] Environment viewer — remote env vars with filter and edit
- [x] Quick commands — one-click monitoring shortcuts panel

## v1.3.0 (Next) — Workflow & Productivity
*Focus: reduce repetitive work, help power users.*

### Planned
- [ ] **Session groups v2** — nested folders, color-coded, drag reorder
- [ ] **Command history browser** — full-text search across all sessions
- [ ] **SFTP sync** — watch local folder, auto-upload on change
- [ ] **Regex triggers v2** — pattern-match output, trigger actions (alert, log, script)
- [ ] **Terminal multiplexer v2** — tmux-like splits without server-side tmux
- [ ] **Output capture** — save terminal output to file with timestamp
- [ ] **Diff viewer** — compare two sessions or session recordings side-by-side
- [ ] **Connection dashboard** — overview of all active sessions with health status
- [ ] **Inline file editor** — quick edit of remote files without leaving terminal
- [ ] **Cron job viewer** — list and manage remote crontab entries

### UX Improvements
- [ ] First-connect suggestions (based on OS detection)
- [ ] Session color tags visible in tab bar
- [ ] Connection speed test on first connect
- [ ] Smart resize — adjust font to fit window changes
- [ ] Tab preview on hover
- [ ] Session export to OpenSSH config format

## v1.4.0 (Future) — Collaboration & Sharing
- [ ] **Shared snippets** — team snippet libraries via git sync
- [ ] **Session sharing** — read-only terminal view for pair programming
- [ ] **Audit log** — compliance-ready session audit trail
- [ ] **Role-based access** — restrict connection targets per user profile
- [ ] **Config profiles** — import/export all settings as portable profile

## v2.0.0 (Planned) — Modern UI Rewrite
**The big one.** Complete UI rewrite with a modern rendering engine.

### Rendering
- **GPU-accelerated rendering** — Direct2D/OpenGL/Vulkan backend replacing GDI
- **120 FPS terminal** — smooth cursor blink, scroll, animations
- **Subpixel font rendering** — ClearType/FreeType with gamma correction
- **Ligature support** — full font shaping with HarfBuzz
- **True color** — 24-bit RGB (parser already supports it since v1.1.0)
- **Image protocol** — inline images via iTerm2/Sixel/Kitty graphics

### Layout
- **Split panes** — horizontal, vertical, nested, with drag resize
- **Floating panels** — undockable panels with snap-to-edge
- **Minimap** — scrollback overview like modern code editors
- **Session thumbnails** — live terminal preview in tab bar
- **Zen mode** — distraction-free single-session fullscreen

### Interaction
- **Smooth scrolling** — pixel-level with momentum and touchpad gestures
- **Search overlay** — Ctrl+F with regex, live highlight, match counter
- **Command palette** — fuzzy-search all commands (Ctrl+Shift+P)
- **Inline rename** — double-click tab to rename
- **Drag tabs** — reorder and detach tabs into new windows

### Architecture Changes
- Migrate from Win32 GDI to Direct2D/SDL2 hybrid renderer
- Separate logic layer from rendering (via UILayer, already in v1.1.0)
- Plugin API v2 with sandboxed execution
- WebSocket-based remote access (optional)

### Timeline
- Q3 2025: Renderer prototype + benchmarking
- Q4 2025: Widget toolkit + split panes + smooth scroll
- Q1 2026: Beta with full feature parity
- Q2 2026: Stable v2.0.0 release

## v2.1.0 (Vision)
- AI-powered command suggestions (local LLM, no cloud)
- Cloud session sync (encrypted, opt-in)
- Team collaboration mode (shared terminals)
- Mobile companion app (iOS/Android session viewer)
- Native Linux package (deb/rpm/AppImage)
- Native macOS build (Universal Binary)
