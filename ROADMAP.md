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
- [x] GPU-accelerated rendering (Direct2D backend v2.1, Vulkan descriptor v2.2)
- [x] Ligature support (shaping table, v2.2)
- [x] True color (24-bit RGB) — parser ready since v1.1.0
- [x] Inline images (Sixel, iTerm2, Kitty graphics) — v2.1

### Layout
- [x] Custom owner-drawn tab bar with close buttons
- [x] Custom owner-drawn sidebar with sections
- [x] Custom owner-drawn status bar
- [x] DWM dark title bar integration
- [x] Split panes (horizontal, vertical) — v2.1
- [x] Floating panels with snap-to-edge — v2.1
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
- [x] Plugin API v2 with sandboxed execution — v2.1
- [x] WebSocket-based remote access (frame codec, v2.2)

## v2.1.0 (Released) — Performance & Extensibility
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
- [x] **Ligature support** — HarfBuzz integration for programming fonts

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
- [x] **Cloud transport** — WebSocket frame codec + opt-in sync state machine

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

## v2.2.0 (In Progress) — GPU, Collaboration & Performance
*Focus: GPU-class rendering, real-time collaboration, and throughput.*

### GPU rendering
- [x] **Vulkan backend descriptor** — device/queue/swapchain abstraction
- [x] **GPU glyph atlas** — shelf-packed texture atlas with LRU eviction
- [x] **Damage-region compositor** — partial frame recomposition
- [x] **Ligature shaping table** — programming-font glyph substitution
- [x] **Subpixel text positioning** — fractional advance accumulation
- [x] **VSync / tear control** — present-mode policy
- [x] **Render command queue** — ring buffer for the render thread

### Collaboration
- [x] **Co-edit cursor tracking** — shared-session cursor presence
- [x] **Operational-transform buffer** — conflict-free shared input
- [x] **Presence roster** — who's connected to a shared session
- [x] **Share permission tiers** — view / comment / control
- [x] **Annotation overlay** — shared highlights and notes
- [x] **Asciicast exporter** — record session to asciicast v2
- [x] **Replay scrubber** — timeline scrubbing for playback

### Performance
- [x] **Scrollback arena** — bump allocator for scrollback
- [x] **Cell slab pool** — fixed-size slab allocator for cells
- [x] **Lazy reflow scheduler** — defer reflow until visible
- [x] **Input coalescing** — merge rapid keystrokes
- [x] **Write batching** — coalesce output writes
- [x] **Latency histogram** — percentile RTT tracking
- [x] **Adaptive read sizing** — grow/shrink read buffer to throughput

### Advanced terminal
- [x] **OSC 8 hyperlinks** — clickable hyperlink escape parser
- [x] **OSC 133 prompt marks** — semantic prompt boundaries
- [x] **Command tracking** — shell-integration command spans
- [x] **Block selection** — rectangular selection model
- [x] **Line-mode undo** — undo/redo ring for line edits
- [x] **Grapheme width table** — cluster width + segmentation
- [x] **BiDi run segmentation** — directional run splitting
- [x] **Tab-stop manager** — custom tab stops

### Cloud transport & security
- [x] **WebSocket frame codec** — remote-access transport framing
- [x] **Cloud-sync state machine** — opt-in sync orchestration
- [x] **Key rotation schedule** — periodic key rollover
- [x] **Vault keyring** — secrets vault abstraction
- [x] **Credential scoping** — per-session credential limits
- [x] **TOFU pin store** — host-key pinning with expiry
- [x] **Audit signing chain** — tamper-evident audit log

## v2.3.0 (In Progress) — Mobile, Accessibility & Modern Protocols
*Focus: touch devices, accessibility, and next-gen transports.*

### Mobile & touch
- [x] **Touch gestures** — tap, double-tap, swipe, pinch, long-press
- [x] **On-screen keyboard** — virtual keyboard layout model
- [x] **Virtual scroll** — windowed rendering for huge lists
- [x] **Zoom/pan** — image preview transform

### Accessibility
- [x] **Screen-reader queue** — prioritized announce queue
- [x] **High-contrast theme** — WCAG contrast-ratio adapter
- [x] **Breadcrumb trail** — navigable folder path
- [x] **East-Asian width** — wide/combining char classifier

### Modern protocols
- [x] **Mosh roaming** — UDP roaming session state
- [x] **SSH3 (over QUIC)** — connection + stream multiplex
- [x] **Happy Eyeballs** — dual-stack connect racer (RFC 8305)
- [x] **Socket stats** — rtt, retransmit, window tracking
- [x] **Proxy auto-config** — host pattern -> proxy matcher
- [x] **Connection retry** — jittered exponential backoff

### Terminal & rendering
- [x] **256-color palette** — xterm palette to RGB
- [x] **Reflow cache** — cached wrap break-points per width
- [x] **Alternate screen** — alt-buffer state + cursor save

### Productivity
- [x] **Session MRU** — recent sessions with pinning
- [x] **Per-session color** — deterministic accent assignment
- [x] **Session templates** — field locking + instantiation
- [x] **Snippet expansion** — placeholder substitution
- [x] **Grep filter** — live include/exclude output filter
- [x] **Toast queue** — TTL notification queue
- [x] **Clipboard format detect** — URL/path/json/code/number
- [x] **Local analytics** — privacy-first usage counters
- [x] **Task queue** — priority background jobs

### Config & utilities
- [x] **KV config store** — typed key/value store
- [x] **Profile export** — portable INI blob
- [x] **Migration runner** — versioned config steps
- [x] **Duration parser** — human duration <-> ms
- [x] **Byte formatting** — human-readable sizes/rates
- [x] **Mini JSON** — flat value extractor
- [x] **Script scope vars** — automation scope stack

### Security
- [x] **Passphrase strength** — entropy-bit estimator
- [x] **Key format detect** — SSH public-key type
- [x] **Conditional triggers** — match -> action with cooldown

## v2.4.0 (In Progress) — AI Assist, Multiplexing & Cloud
*Focus: AI-assisted workflows, connection multiplexing, and cloud profiles.*

### AI assist
- [x] **Prompt context builder** — recent cmds + cwd + errors
- [x] **Completion cache** — TTL-keyed AI response cache
- [x] **Command explainer** — flag/pipe/danger heuristics
- [x] **Sequence planner** — multi-step command plan with confirm gates

### Connection multiplexing
- [x] **Mux channel table** — ControlMaster-style channel registry
- [x] **Channel flow window** — per-channel window accounting
- [x] **KEX negotiation** — algorithm intersection
- [x] **Auth method order** — negotiation + attempt tracking
- [x] **QUIC flow control** — stream credit accounting
- [x] **Session graph** — jump-host dependency topology

### Cloud & sharing
- [x] **Cloud profile sync** — revision + dirty-set metadata
- [x] **Shared session roster** — participants with role tiers
- [x] **Host key rotation** — RFC 8308 rotation tracking

### Advanced SFTP
- [x] **Transfer queue** — progress + retry tracking
- [x] **Resumable transfers** — offset + checksum window
- [x] **Directory mirror** — local/remote diff sets
- [x] **Bandwidth throttle** — token-bucket rate limit
- [x] **Rate + ETA** — sliding-window calculator
- [x] **Path completion** — remote tab-completion

### Terminal
- [x] **CSI parser** — parameter + intermediate + final
- [x] **Scroll region** — DECSTBM margins + origin mode
- [x] **OSC 52 clipboard** — set/query parser
- [x] **OSC 9;4 progress** — taskbar progress parser

### Productivity
- [x] **Command bar** — input model with history
- [x] **Split layouts** — grid/columns/main-stack presets
- [x] **Status segments** — priority-drop layout
- [x] **Log search** — indexed ring-buffer search
- [x] **Color diff** — side-by-side line classifier
- [x] **Diff hunk parser** — unified-diff headers
- [x] **Workflow steps** — multi-host execution

### Config & resilience
- [x] **Hot reload** — mtime + content-hash watcher
- [x] **Setting diff** — changed-key computation
- [x] **Theme validation** — required keys + hex checks
- [x] **Retry context** — per-error-class policies
