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

## v3.0.0 (Released) — New Era
*A flagship release: AI copilot, zero-trust security, distributed sessions, GPU compositor.*

### AI & automation
- [x] **AI copilot** — turn history + context builder
- [x] **Autonomous agent** — plan/act/observe loop
- [x] **Pipeline v2** — DAG stages with dependency gating
- [x] **Scenario runner** — scripted expect/send with timeouts
- [x] **Command runner** — batch execution with aggregation
- [x] **Event stream** — typed subscribers with replay buffer

### Security (zero-trust)
- [x] **Zero-trust policy engine** — per-action allow/deny
- [x] **MFA flow** — TOTP/push/WebAuthn state machine
- [x] **Secrets vault v2** — envelope-encrypted entries
- [x] **Certificate pinning** — SPKI hash allowlist

### Distributed & cloud
- [x] **Distributed sessions** — leader/follower quorum replication
- [x] **Session sync v2** — CRDT last-write-wins registers
- [x] **Settings sync** — conflict-aware push/pull
- [x] **Multipath transport** — per-path scheduling + failover
- [x] **Connection pool v2** — health-based reuse
- [x] **DoH resolver** — DNS-over-HTTPS cache with TTL

### Rendering & terminal
- [x] **Layer compositor** — z-ordered surfaces, damage merge
- [x] **GPU frame graph** — pass ordering + barriers
- [x] **Text shaper** — cluster grouping + ligatures
- [x] **Terminal state v2** — mode bitmap + cursor stack
- [x] **Theme engine v3** — derived color tokens

### Workflow & UX
- [x] **SSH multiplexer** — virtual terminals over one connection
- [x] **Workspace v2** — layout + activation order
- [x] **Command palette v2** — fuzzy scoring + recent boost
- [x] **Dashboard v2** — widget grid with metric bindings
- [x] **Notification center v2** — grouped + actionable
- [x] **Session recorder v2** — timed frames with seek index

### Observability
- [x] **Metrics** — counter/gauge/histogram
- [x] **Span tracer** — parent-linked timing
- [x] **Feature flags** — rollout-percentage gating

## v3.1.0 (In Progress) — Future-Proof: PQ Crypto, WASM & Edge
*Focus: post-quantum security, WASM plugins, and constrained networks.*

### Modern cryptography
- [x] **Post-quantum KEM** — ML-KEM and hybrid negotiation
- [x] **HKDF key derivation** — extract/expand chain
- [x] **TLS info parser** — version/cipher/PFS detection
- [x] **Auth attempt log** — lockout detection

### WASM plugin platform
- [x] **WASM runtime** — fuel-metered module execution
- [x] **Host-call dispatch** — capability-gated host functions

### Edge & networking
- [x] **Edge/IoT profiles** — low-power/lossy/metered tuning
- [x] **NAT hole-punching** — STUN-style candidate pairs
- [x] **Multi-hop proxy chains** — SOCKS/HTTP/SSH hops
- [x] **Quick-connect parser** — user@host:port/path?opts

### Terminal modernization
- [x] **Unicode 15.1** — emoji ZWJ + variation selectors
- [x] **Synchronized output** — DEC 2026 frame batching
- [x] **Kitty keyboard** — CSI u protocol codec
- [x] **DEC mode registry** — DECSET/DECRST tracking
- [x] **Truecolor quantizer** — cached 256/16 fallback

### Productivity & UX
- [x] **Command palette categories** — grouped actions + keybinds
- [x] **Session folder tree** — collapsible hierarchy
- [x] **Session icons** — OS-detected badges
- [x] **Status icon FSM** — latency-aware signal bars
- [x] **Tab scroll** — overflow chevrons
- [x] **Minimap v2** — density buckets + viewport marker
- [x] **Inverted search index** — token postings over scrollback
- [x] **Clipboard sync ring** — deduplicated history

### Automation & config
- [x] **Lifecycle hook chain** — pre/post connect/disconnect
- [x] **Job scheduler** — cron-like next-run
- [x] **Retry queue** — backoff + max attempts
- [x] **Rate limiter v2** — sliding window
- [x] **File sync conflicts** — bidirectional detector
- [x] **Backup scheduler** — rotation policy
- [x] **Env profiles** — dev/staging/prod variable sets
- [x] **Schema validation** — typed field constraints
- [x] **Layered defaults** — built-in < global < session
- [x] **Log export** — JSONL/CSV/plain
- [x] **Streaming base64** — partial-block codec

## v3.2.0 (In Progress) — Enterprise, Compliance & Dev Tooling
*Focus: enterprise governance, compliance, and power-user tooling.*

### Compliance & governance
- [x] **Compliance framework** — PCI/SOC2/HIPAA/GDPR rule sets
- [x] **Tamper-evident recording** — hash-chained session log
- [x] **RBAC matrix** — roles x permissions with inheritance
- [x] **Secret leak scanner** — outbound credential detection
- [x] **Command approval** — privileged-op workflow
- [x] **Audit query engine** — filter by user/time/severity
- [x] **Org policy files** — enforced/locked settings
- [x] **Session vault binding** — per-session TTL unlock
- [x] **OIDC/SSO claims** — identity token parser
- [x] **Session quotas** — time/bytes/cmd limits
- [x] **Alert rules** — threshold + duration engine

### Terminal graphics
- [x] **Sixel v2** — palette registers + raster attributes
- [x] **Grapheme clusters** — UAX #29 boundary detection
- [x] **Extended underlines** — curly/dotted/dashed + color
- [x] **Title stack** — XTPUSH/XTPOPTITLE

### Smart networking
- [x] **DNSSEC validation** — chain-of-trust signaling
- [x] **Port scanner** — open/closed/filtered aggregation
- [x] **GeoIP cache** — region + latency preference
- [x] **Bandwidth graph** — up/down ring buffer

### Developer tooling
- [x] **Three-way merge** — conflict markers
- [x] **CSV parser** — RFC 4180 quoting
- [x] **Hex editor** — byte buffer view model
- [x] **Tree view** — generic expand/select model
- [x] **State machine** — automation flow engine
- [x] **Runbook executor** — rollback markers
- [x] **Metrics push** — Prometheus/StatsD/Influx

### UX & productivity
- [x] **Tab groups v2** — color labels + collapse
- [x] **Theme picker v2** — favorites ordering
- [x] **Input hints** — history ghost-text prediction
- [x] **Keymap v2** — chord sequences + contexts
- [x] **Batch operations** — bulk session actions
- [x] **Webhook v2** — retry + signing

## v3.3.0 (In Progress) — Intelligent Ops & Polish
*Focus: ML-assisted operations, session intelligence, and refinement.*

### AI/ML-assisted operations
- [x] **Anomaly detector** — z-score over rolling window
- [x] **Command predictor** — Markov bigram model
- [x] **Smart retry** — adaptive strategy learning
- [x] **Log clustering** — template extraction (Drain-lite)
- [x] **Command safety** — risk scoring + blocking
- [x] **Smart copy** — content type auto-detection

### Session intelligence
- [x] **Session scoring** — latency/error/uptime composite grade
- [x] **Activity timeline** — event markers + range query
- [x] **Connection graph** — topology + BFS reachability
- [x] **Scoped share links** — tokens with expiry + scope
- [x] **Session export v2** — JSON/YAML/sshconfig/PuTTY

### Transfer & networking
- [x] **Rsync delta sync** — rolling + strong checksums
- [x] **Archive detection** — tar/zip/gz/bz2/xz/7z
- [x] **Load balancer** — round-robin/least-conn/latency
- [x] **Image render tracking** — inline image placement

### Developer tooling
- [x] **JSONPath-lite** — dot-notation extraction
- [x] **Cron expression matcher** — full 5-field
- [x] **Pipe filter chain** — grep/head transformations
- [x] **Secret references** — env/vault/file/keychain
- [x] **CSV export** — RFC-correct quoting
- [x] **Profile inheritance** — connection profile manager
- [x] **Snippet library v2** — tags + fuzzy search

### UX polish
- [x] **Quick action menu** — context-aware suggestions
- [x] **Search panel** — result navigation
- [x] **Pane borders** — active highlight
- [x] **Scrollback marks** — quick navigation
- [x] **Command help** — inline synopsis lookup
- [x] **Progress bar** — ETA + rate display
- [x] **Tooltip v2** — delayed rich tooltips
- [x] **Settings UI** — categories + search
- [x] **Charset designation** — G0-G3 + DEC graphics
- [x] **Workflow v2** — conditional branches

## v3.4.0 (In Progress) — Protocol Depth & Refinement
*Focus: SSH protocol completeness, transport tuning, and UI refinement.*

### SSH protocol
- [x] **Rekey policy** — by bytes/time (RFC 4253)
- [x] **Compression negotiation** — zlib/zlib@openssh with ratio
- [x] **Port forwarding v2** — local/remote/dynamic SOCKS
- [x] **Keepalive v3** — adaptive interval + dead detection
- [x] **Host key trust v2** — rotation acceptance
- [x] **SSHFP verification** — DNS fingerprint records (RFC 4255)
- [x] **SOCKS5 client** — handshake state machine

### Transport & performance
- [x] **Throughput meter** — EWMA smoothing + peak
- [x] **IP range/CIDR** — matching + private detection

### Terminal
- [x] **OSC 10/11/12** — font/cursor color query
- [x] **Mouse protocol v2** — SGR pixel mode
- [x] **Rectangular selection** — block selection model
- [x] **Line wrap state** — DECAWM pending-wrap
- [x] **Macro playback** — timed replay with loop

### UI & UX
- [x] **Status line v2** — powerline segments
- [x] **Context action bar** — state-driven buttons
- [x] **Color wheel** — HSV picker
- [x] **Animation easing** — cubic/bounce curves
- [x] **Word-wrap engine** — hyphenation hints

### Sessions & config
- [x] **Session tags v2** — AND/OR filter expressions
- [x] **Session notes v2** — pinned markdown notes
- [x] **Session groups v3** — nesting + drag reorder
- [x] **Session fuzzy filter** — name/host/tag
- [x] **Per-host memory** — last-used settings
- [x] **Config 3-way merge** — sync conflict resolution
- [x] **Task scheduler v2** — priorities + dependencies

### Tooling & parsing
- [x] **Expect v2** — send queue
- [x] **Trigger v3** — action chains
- [x] **YAML parser** — flat key/value
- [x] **TAR header parser** — ustar entries
- [x] **UUID v4** — generate + validate
- [x] **File picker** — multi-select + filter
- [x] **Diff statistics** — additions/deletions

## v3.5.0 (In Progress) — Terminal Depth & Hardware Auth
*Focus: complete terminal emulation, hardware security keys, protocol parsers.*

### Terminal emulation
- [x] **VT220 conformance** — DECSCL conformance levels
- [x] **DCS parser** — device control strings (Sixel/UDK/DECRQSS)
- [x] **APC handler** — application program commands (Kitty)
- [x] **Tab stops** — HTS/TBC stop management
- [x] **Box drawing** — DEC line-draw to Unicode
- [x] **Cell attributes** — SGR packing/unpacking
- [x] **Cursor shape** — DECSCUSR styles + blink
- [x] **Dirty-line tracking** — partial redraw buffer
- [x] **Text reflow** — rewrap on resize
- [x] **Key sequences** — terminfo-lite escape mapper

### Hardware & security
- [x] **PKCS#11** — smartcard/HSM slot descriptor
- [x] **FIDO2/U2F** — security key authentication
- [x] **SSH tunnels** — lifecycle with auto-restart

### Protocols
- [x] **HTTPS CONNECT proxy** — tunnel handshake
- [x] **SFTP attributes** — permissions/times parser
- [x] **SCP v2** — protocol message parser
- [x] **REST endpoints** — method+path router
- [x] **Lua API registry** — function binding table

### UI primitives
- [x] **Tab list** — reorder/close/activate model
- [x] **Split resize** — interactive divider drag
- [x] **Modal dialogs** — focus trap + result
- [x] **Scrollbar** — geometry + thumb hit testing

### Utilities
- [x] **Glob matching** — *, ?, [...] patterns
- [x] **INI parser** — sectioned config
- [x] **Word completion** — buffer vocabulary
- [x] **Digest** — fast cache-key hashing
- [x] **Recent connections** — frequency + LRU
- [x] **Network interfaces** — bind preference
- [x] **Time formatting** — relative/ISO/clock
- [x] **Session statistics** — bytes/duration/cmds
- [x] **Event hooks** — priority dispatcher
- [x] **Command chains** — &&/||/; operators

## v3.6.0 (In Progress) — Editor, Search & Polish
*Focus: text editing, search/data-viz, integrations, and test hardening.*

### Editor & search
- [x] **Gap buffer** — efficient text editing primitive
- [x] **Undo/redo stack** — editor operation history
- [x] **Regex engine v2** — anchors, classes, quantifiers
- [x] **Fuzzy matcher v2** — fzf-style scoring
- [x] **Search highlighting** — match ranges + current
- [x] **Word completion** — buffer vocabulary

### Data visualization
- [x] **Sparklines** — Unicode block charts
- [x] **Gauge widget** — color-zoned meter
- [x] **ASCII tables** — column-aligned formatter
- [x] **Byte formatting** — SI/binary/grouped

### Integrations
- [x] **WebSocket client** — RFC 6455 frame codec
- [x] **DNS resolution** — A/AAAA/CNAME handler
- [x] **Plugin host runtime** — lifecycle + sandbox
- [x] **Theme loader** — iTerm/WT JSON format
- [x] **Script loader** — dependency resolution
- [x] **Hash verification** — file integrity check

### Sessions & config
- [x] **Bookmarks v2** — folders + favorites
- [x] **Session metadata** — custom key/value
- [x] **Session restore v2** — window layout persistence
- [x] **Settings store** — change tracking
- [x] **Bandwidth monitor** — per-session aggregation
- [x] **Batch execution** — concurrency-limited
- [x] **Task pool** — priority worker pool

### Terminal & UI
- [x] **Scrollback v2** — ring buffer with markers
- [x] **Dynamic palette** — OSC 4 color set/query
- [x] **Color scheme v2** — bright palette
- [x] **Clipboard formatting** — ANSI strip + EOL
- [x] **Progress modal** — long-operation feedback
- [x] **Rich tooltip** — multi-line + key hints
- [x] **Breadcrumb v2** — clickable segments
- [x] **Keymap editor** — conflict detection
- [x] **Input validation** — hostname/port/email

### Quality
- [x] **Test fixes** — hostverify API, terminal blanks, keepalive (38/38 pass)

## v3.7.0 (In Progress) — i18n, Accessibility & Quality
*Focus: internationalization, accessibility, profiling, and verification tooling.*

### Internationalization & accessibility
- [x] **i18n string table** — locale fallback
- [x] **Locale formatting** — number/date by locale
- [x] **Accessibility v2** — focus order + ARIA roles
- [x] **BiDi segmentation** — LTR/RTL run detection
- [x] **UI zoom** — font steps + DPI scaling

### Networking
- [x] **IPv6 parsing** — address + classification
- [x] **Connection state machine** — validated lifecycle
- [x] **Reconnect v3** — backoff + jitter cap
- [x] **Connection history v2** — reliability stats
- [x] **Rate calculator v2** — smoothed ETA

### Terminal & editor
- [x] **OSC parser v2** — numeric + string params
- [x] **Escape filter** — strip/sanitize control codes
- [x] **Syntax highlighting** — keyword/string/comment tokenizer
- [x] **Character counter** — chars/words/lines (UTF-8)
- [x] **Output throttle** — flood prevention
- [x] **Ring buffer v2** — overflow tracking

### Tools & automation
- [x] **Function profiler** — call counts + timing
- [x] **Script debugger** — breakpoints + watch
- [x] **JSON builder** — object/array emission
- [x] **Levenshtein distance** — typo suggestions
- [x] **Command tokenizer** — quotes/escapes/pipes
- [x] **Conditional variables** — automation expansion
- [x] **Config import** — PuTTY/OpenSSH/JSON
- [x] **Settings export v3** — portable archive
- [x] **Macro editor** — insert/delete/reorder

### UI & sessions
- [x] **Audit log v2** — severity levels
- [x] **Radial menu** — pie context geometry
- [x] **Notification toasts** — positioned queue
- [x] **Session grid** — tile layout
- [x] **Session search v2** — multi-field ranking
- [x] **Split layout v2** — nested binary tree

### Quality & tooling
- [x] **verify.sh** — local quality gate (compile + link + dup-symbol + tests)
- [x] **Warning cleanup** — zero-warning test build
- [x] **Test suite** — 38/38 passing

## v3.8.0 (In Progress) — Crypto Helpers, UTF-8 & Widgets
*Focus: encoding/crypto primitives, UTF-8 correctness, UI widgets.*

### Crypto & encoding
- [x] **Base32** — RFC 4648 encode/decode
- [x] **TOTP** — code computation model
- [x] **CRC32** — IEEE checksum
- [x] **UTF-8** — encode/decode/validate/strlen
- [x] **URL parser** — scheme/host/port/path/query
- [x] **Hex dump v2** — offset + ASCII gutter

### Terminal
- [x] **ANSI 256-color** — SGR sequence parser
- [x] **Text selection v2** — char/word/line/block
- [x] **Wrap indent** — continuation tracking
- [x] **Cursor blink** — timing + visibility

### UI widgets
- [x] **List view** — selection/scroll/filter
- [x] **Dropdown** — keyboard navigation
- [x] **Spinner** — loading animation
- [x] **Diff view** — side-by-side layout

### Sessions & config
- [x] **Connection preset v2** — parameter overrides
- [x] **Session clone v2** — field selection
- [x] **Session pinning** — priority order
- [x] **Config migration v2** — version chain
- [x] **Key binding map** — modifier combos
- [x] **State manager** — persistence flags

### Networking & automation
- [x] **Packet log** — SSH type counts
- [x] **Bandwidth limit v2** — burst allowance
- [x] **Protocol negotiation** — SSH banner exchange
- [x] **Trigger matcher** — multi-pattern
- [x] **Replay macro** — speed control
- [x] **Schedule rules** — interval/daily/weekly
- [x] **Action queue** — undo support
- [x] **Key store** — agent integration
- [x] **Memory pool** — fixed-block allocator
- [x] **Shell quoting** — POSIX/Windows
- [x] **File type detection** — extension + magic

### Tooling
- [x] **verify.sh v2** — now tracks -Wall warning count

## v3.9.0 (In Progress) — Data, Layout & Quality
*Focus: data structures, layout systems, parsing, and warning cleanup.*

### Data & parsing
- [x] **JSON merge patch** — RFC 7386
- [x] **Text diff** — line-based LCS
- [x] **Time parsing** — ISO 8601 / relative
- [x] **Option parser** — getopt-like CLI
- [x] **String utilities** — trim/split/replace
- [x] **Path utilities** — basename/dirname/join
- [x] **logfmt** — structured key=value

### Layout & UI
- [x] **Grid layout** — rows x cols with spans
- [x] **Dockable panels** — left/right/top/bottom/float
- [x] **Menu bar** — submenus + accelerators
- [x] **Status widgets** — composable indicators
- [x] **Progress ring** — circular segments
- [x] **Theme variables** — cascading resolution

### Terminal
- [x] **Resize handler** — cursor clamp + reflow
- [x] **Scroll region v2** — DECSLRM margins
- [x] **Character map** — codepage/ACS translation
- [x] **Input mode** — insert/replace/vi/normal

### Networking & security
- [x] **Connection pool v3** — idle timeout + warmup
- [x] **DNS cache v2** — negative caching
- [x] **Address pool** — health-based selection
- [x] **TLS verification** — certificate chain
- [x] **Password policy** — length/complexity

### Automation & infra
- [x] **Event bus v2** — topic wildcards
- [x] **Pipeline executor** — stage chaining
- [x] **Job queue v2** — dependency ordering
- [x] **Task dependency graph** — topological sort
- [x] **Condition checker** — automation guards
- [x] **Retry timer** — fixed/linear/exp/fibonacci
- [x] **LRU cache v2** — TTL + size eviction
- [x] **Session template v2** — variable substitution
- [x] **Session bookmarks** — in-scroll marks
- [x] **Form validation** — field rules

### Quality
- [x] **Warning cleanup** — 474 -> 101 across-tree (-78%)

## v3.10.0 (In Progress) — Data Structures, Codecs & Safety
*Focus: core data structures, encoding, automation safety, UI primitives.*

### Data structures & codecs
- [x] **Ring queue** — fixed-capacity FIFO
- [x] **Bit set** — popcount + union
- [x] **base64url** — RFC 4648 URL-safe
- [x] **Key fingerprint** — SHA256 base64 / MD5 hex
- [x] **CSV writer** — proper escaping
- [x] **Tokenizer** — custom delimiters
- [x] **Number formatting** — ordinal/roman/percent
- [x] **Humanize** — count/duration/relative time

### Terminal
- [x] **Cell diff tracker** — minimal screen updates
- [x] **Terminal mode flags** — DECSET handling
- [x] **OSC status** — title/icon/working-dir
- [x] **Scroll animation** — ease-out interpolation

### Networking
- [x] **Keepalive v4** — RTT-adaptive intervals
- [x] **Proxy auth** — basic/digest/NTLM
- [x] **MTU discovery** — path probing
- [x] **DoH client** — DNS-over-HTTPS

### Automation safety
- [x] **Flow control** — if/while/break state
- [x] **Loop guard** — runaway protection
- [x] **Workflow validation** — cycle/orphan detection
- [x] **Retry policy** — circuit breaker
- [x] **Script context** — variables + return

### Sessions & config
- [x] **Session history** — command dedup + search
- [x] **MRU v2** — frequency weighting
- [x] **Session groups v4** — color tags + counts
- [x] **Env expansion** — $VAR / ${VAR}
- [x] **Setting validation** — type coercion
- [x] **Profile diff** — added/changed/removed
- [x] **Certificate expiry** — monitoring + warnings

### UI primitives
- [x] **Tree list** — expand/collapse + indent
- [x] **Toast manager** — stacking + auto-dismiss
- [x] **Tab context menu** — per-tab actions
- [x] **Splitter bar** — drag handling

### Quality
- [x] **Warning cleanup** — 101 -> 52 (-90% across session)

## v3.11.0 (In Progress) — Text Processing, Parsing & Codecs
*Focus: string utilities, parsers, encoders, terminal and UI helpers.*

### Text & string
- [x] **Trim/pad** — ltrim/rtrim/trim helpers
- [x] **Slugify** — label to URL slug
- [x] **Word wrap** — greedy column wrapping
- [x] **Levenshtein ratio** — similarity 0..100
- [x] **CSV parse** — quoted-field record parser
- [x] **Columnize** — aligned multi-column layout

### Codecs & checksums
- [x] **Base32** — RFC 4648 encode/decode
- [x] **URL encode** — percent-encode/decode
- [x] **RLE** — run-length encode/decode
- [x] **Checksum8** — XOR / additive / Fletcher-16
- [x] **Shell quote** — POSIX-safe argument quoting

### Networking
- [x] **CIDR v4** — parse + range matching
- [x] **MAC address** — parse/format/multicast
- [x] **Port range** — "22,80,8000-8010" spec parser

### Terminal
- [x] **SGR parse** — color/attr escape parsing
- [x] **Tab stops** — DECST tracking
- [x] **Char width** — East-Asian/combining width
- [x] **Cursor save** — DECSC save/restore stack

### Security
- [x] **Password entropy** — bits estimate + label
- [x] **Host key fingerprint** — MD5 colon-hex format

### Config & sessions
- [x] **INI writer** — section/key serializer
- [x] **Glob v2** — *?[set] matcher
- [x] **Keymap parse** — "Ctrl+Shift+P" chords
- [x] **Tag index** — session-by-tag lookup
- [x] **Export HTML** — session log to HTML

### Automation & UI
- [x] **Template engine** — {{var}} rendering
- [x] **Cooldown gate** — per-key debounce
- [x] **Rate limit v2** — token bucket
- [x] **Progress bar** — text progress rendering
- [x] **Spinner** — animation frames
- [x] **Time format** — durations and timestamps

## v3.12.0 (In Progress) — Numeric, Data Structures & Scheduling
*Focus: math accumulators, containers, scheduling, rendering math.*

### Numeric & math
- [x] **Stat accumulator** — streaming mean/variance/stddev
- [x] **Fixed-point** — 16.16 arithmetic
- [x] **Moving average** — windowed mean + EMA
- [x] **Quantizer** — histogram binning
- [x] **CRC-16/CCITT** — checksum + streaming update
- [x] **Varint** — LEB128 codec

### Data structures
- [x] **Min-heap** — binary heap of ints
- [x] **LRU cache** — fixed-size eviction
- [x] **Bit field** — set/test/popcount
- [x] **Interval** — overlap/union/merge
- [x] **Job queue** — FIFO with status tracking

### Scheduling & automation
- [x] **Scheduler v2** — earliest-deadline-first
- [x] **Backoff** — exponential with jitter
- [x] **State machine v2** — transition table

### Networking
- [x] **Bandwidth meter** — throughput + bit-rate format
- [x] **DNS cache v2** — TTL resolution cache
- [x] **HTTP range** — Range header build/parse
- [x] **IPv6 format** — compress/expand addresses

### Terminal & UI
- [x] **Palette 256** — index to RGB + nearest
- [x] **Line wrap v2** — soft-wrap reflow math
- [x] **Blink timer** — cursor blink phase
- [x] **Menu tree** — hierarchical menu model
- [x] **Scrollbar v2** — thumb geometry

### Security & config
- [x] **TOTP window** — time-step validation
- [x] **Redact** — secret masking in logs
- [x] **Env file** — .env KEY=VALUE parser
- [x] **Unit parse** — human sizes/durations
- [x] **Color parse** — #RRGGBB / rgb()

### Sessions
- [x] **Session sort** — by name/recent/rating
- [x] **Recent files** — MRU file tracker

## v3.13.0 (In Progress) — Protocol Parsers & Terminal Sequences
*Focus: wire-protocol parsing, escape-sequence handling, auth safety.*

### Protocol parsing
- [x] **SSH banner** — version-exchange parser
- [x] **HTTP status** — code reason/class/retryable
- [x] **Cookie jar** — Set-Cookie parse + store
- [x] **WebSocket frame** — header encode/decode
- [x] **Proxy URL** — scheme/auth/host/port parse
- [x] **JSON Pointer** — RFC 6901 path resolution
- [x] **TOML value** — scalar value parser

### Terminal sequences
- [x] **CSI parse v2** — control-sequence params
- [x] **OSC parse** — title/hyperlink/clipboard
- [x] **DEC special graphics** — box-drawing map
- [x] **Mouse events** — SGR encode/decode
- [x] **ANSI length** — visible width + strip

### Security & auth
- [x] **JWT decode** — segment split/inspect
- [x] **Cert subject** — X.509 DN field extract
- [x] **Auth limiter** — lockout after failures
- [x] **Redact** (see also v3.12.0)

### Automation
- [x] **Expect parser** — send/expect rules
- [x] **Cron field** — schedule field matcher
- [x] **Macro record** — keystroke record/replay
- [x] **Trigger match** — output-line rules

### Tools & UI
- [x] **Diff hunk** — unified-diff headers
- [x] **SemVer** — parse + compare
- [x] **Number base** — base 2-36 conversion
- [x] **KV store** — in-memory string store
- [x] **Hex view** — hex+ASCII dump rows
- [x] **Dialog fit** — centered geometry
- [x] **Text field** — single-line edit model
- [x] **Session filter** — key:value predicates
- [x] **Path normalize** — collapse . / .. segments

## v3.14.0 (In Progress) — Rendering, Layout & Statistics
*Focus: redraw tracking, layout math, codecs, statistical helpers.*

### Rendering & terminal
- [x] **Damage map** — dirty-row redraw tracking
- [x] **Selection span** — normalize/contains math
- [x] **Reflow v2** — scrollback segment reflow
- [x] **Grid copy/scroll** — rectangular region scroll

### Layout & UI
- [x] **Layout grid** — uniform gapped cells
- [x] **Flexbox** — weighted space distribution
- [x] **Anchor position** — corner/center anchoring
- [x] **Tooltip** — on-screen placement
- [x] **Badge** — notification count format
- [x] **Sparkline v2** — unicode tier mapping

### Codecs & text
- [x] **JSON escape** — string escape/unescape
- [x] **String split v2** — delimiter splitting
- [x] **Ascii85** — encode/decode
- [x] **Random name** — deterministic readable ids
- [x] **Time parse** — ISO-8601 + epoch day

### Statistics
- [x] **Percentile** — interpolated p50/p90/p99
- [x] **Fuzzy score** — subsequence match scoring

### Networking
- [x] **ETag** — generate + If-None-Match compare
- [x] **Retry-After** — header value parsing
- [x] **FTP codes** — reply classification

### Automation & config
- [x] **Pipeline v2** — staged command chains
- [x] **Watchdog v2** — heartbeat timeout
- [x] **Debounce v2** — quiet-period debounce
- [x] **Merge config** — layered overrides
- [x] **Session clone** — config clone with overrides
- [x] **Permission check** — access-control bits

### Quality
- [x] **Warning cleanup** — eliminated all misleading-indentation (51 -> 25)

## v3.15.0 (In Progress) — Compression, Codecs & Bit I/O
*Focus: compression primitives, encoders, bit-level I/O, protocol parsing.*

### Compression & checksums
- [x] **LZ77** — sliding-window longest match
- [x] **Huffman lengths** — canonical code lengths
- [x] **Delta encode** — successive-delta + zigzag
- [x] **Adler-32** — checksum + streaming update

### Bit & byte I/O
- [x] **Bit reader** — MSB-first bit extraction
- [x] **Bit writer** — MSB-first bit packing
- [x] **Hex codec** — hex string encode/decode

### Protocol parsing
- [x] **Chunked decode** — HTTP chunk sizes
- [x] **Content-Type** — mime/charset/boundary
- [x] **Query params** — URL query-string parse
- [x] **TLS version** — protocol classification

### Terminal
- [x] **Scroll region** — DECSTBM management
- [x] **Unicode fold** — ASCII/Latin-1 case folding

### Config & sessions
- [x] **Dot path** — dotted-key navigation
- [x] **Defaults** — default + override resolution
- [x] **Session stats** — usage aggregation

### Automation & UI
- [x] **Conditional** — comparison evaluation
- [x] **Throttle v2** — N-per-window limiting
- [x] **List view v2** — selection + scroll
- [x] **Status format** — status-bar segments

### Tools
- [x] **Roman numerals** — to/from conversion
- [x] **EMA v2** — exponential moving average
- [x] **Salt generator** — deterministic stretch

## v3.16.0 (In Progress) — Geometry, Graphics & Containers
*Focus: 2D math, color/graphics helpers, core containers, networking math.*

### Geometry & graphics
- [x] **Rect ops** — intersect/union/contains
- [x] **Easing** — quad/cubic interpolation
- [x] **Color mix** — blend/lighten/contrast
- [x] **Vec2** — 2D integer vector math
- [x] **Bresenham line** — point generation
- [x] **Clamp/remap** — range mapping + wrap
- [x] **Box drawing** — frame glyph selection
- [x] **Gradient** — color gradient fill

### Containers
- [x] **Circular buffer** — ring byte buffer
- [x] **Int stack** — bounded LIFO
- [x] **Event counter** — per-category counts

### Networking
- [x] **Subnet** — netmask/network/broadcast
- [x] **TCP state** — connection state names
- [x] **Keepalive calc** — adaptive intervals

### Automation & config
- [x] **Retry table** — per-key retry tracking
- [x] **Sequence runner** — indexed step runner
- [x] **Schema check** — value validation

### Sessions & UI
- [x] **Quick connect** — user@host:port parse
- [x] **Tab strip** — ordering + active tab
- [x] **Table format** — aligned text tables

### Security & tools
- [x] **Fingerprint compare** — normalized compare
- [x] **Base58** — Bitcoin-alphabet encode

## v3.17.0 (In Progress) — String Algorithms & Search
*Focus: substring search, string metrics, tokenizing, autocomplete.*

### Search algorithms
- [x] **KMP** — Knuth-Morris-Pratt search
- [x] **Boyer-Moore** — Horspool search + count
- [x] **LCS** — longest common subsequence/substring
- [x] **Trie** — prefix trie for autocomplete

### String metrics & transforms
- [x] **String metrics** — Jaro + Hamming
- [x] **Soundex** — phonetic encoding
- [x] **Replace** — replace-all + count
- [x] **Title case** — title/snake conversion
- [x] **Word count** — chars/words/lines

### Parsing & tokenizing
- [x] **Arg parse v2** — flags + positionals
- [x] **Token stream** — script lexer
- [x] **Number parse v2** — signed + percent
- [x] **User-Agent** — build/parse/bot detect

### Config & automation
- [x] **Inherit chain** — config inheritance
- [x] **Variable scope** — scoped resolution
- [x] **Schema check** (see v3.16.0)

### Sessions, UI & terminal
- [x] **Session match** — fuzzy quick-switch
- [x] **Key hints** — shortcut hint bars
- [x] **Breadcrumb** — navigation trails
- [x] **Wrap cursor** — deferred line-end wrap
- [x] **Grid map** — 2D coordinate mapping

### Security
- [x] **Password policy v2** — composition rules

## v3.18.0 (In Progress) — Protocols, Terminal Attributes & Safety
*Focus: HTTP/DNS/SOCKS handling, cell attributes, command safety.*

### Networking & protocols
- [x] **HTTP headers** — parse/store/lookup
- [x] **Request line** — method/path/version + status
- [x] **SOCKS5** — greeting/connect packet build
- [x] **DNS name** — label encode/decode
- [x] **IP class** — private/loopback/multicast scope
- [x] **MIME type** — extension guessing
- [x] **User-Agent** (see v3.17.0)

### Terminal
- [x] **Cell attributes** — pack/unpack fg/bg/flags
- [x] **VT params** — parse + default values
- [x] **Alt buffer** — alternate-screen switching

### Automation & safety
- [x] **State track** — named flags/counters
- [x] **Guardrail** — dangerous-command blocking
- [x] **Script loop** — bounded loop control
- [x] **Session key** — derive + rotate

### Config & sessions
- [x] **Profile merge** — field-precedence merge
- [x] **Wildcard match** — hostname patterns
- [x] **Connection timer** — per-session durations

### UI & tools
- [x] **Context menu v2** — build + navigate
- [x] **Notification stack** — toast expiry
- [x] **IPv4 sort** — sort + dedup
- [x] **Ratio** — reduce + format fractions

## v3.19.0 (In Progress) — Sessions, Scheduling & Audit
*Focus: bookmarks, scheduling, job graphs, audit chains, UI state.*

### Sessions & bookmarks
- [x] **Bookmark tree** — hierarchical folders
- [x] **Session export** — OpenSSH config format
- [x] **Favorites v2** — usage-ranked favorites
- [x] **Connection timer** (see v3.18.0)

### Scheduling & jobs
- [x] **Schedule rule** — daily/weekly/interval
- [x] **Job graph** — topological dependency order
- [x] **Retry budget** — windowed retry limit
- [x] **Dry run** — track apply vs simulate

### Config & themes
- [x] **Config lint** — common-mistake detection
- [x] **Backup rotate** — numbered backups
- [x] **Theme parse v2** — color definitions

### Security & networking
- [x] **Audit line** — tamper-evident chain hash
- [x] **Latency stat** — avg/max/jitter tracking

### UI & terminal
- [x] **Search bar v2** — incremental search state
- [x] **Panel resize** — constrained divider drag
- [x] **Scrollback index** — fast line access
- [x] **Color picker** — HSV/RGB conversion

### Tools
- [x] **Pretty bytes** — human-readable sizes
- [x] **List dedup** — order-preserving dedup
- [x] **Time ago v2** — relative descriptions
- [x] **CSV quote** — field escaping

## v3.20.0 (In Progress) — Numeric Algorithms & Hashing
*Focus: integer math, hashing, PRNG, color quantization.*

### Integer math
- [x] **Integer sqrt** — isqrt + power-of-2 checks
- [x] **Primes** — primality + next-prime + factors
- [x] **GCD/LCM** — including extended GCD
- [x] **Modular exp** — powmod + mulmod
- [x] **Math util** — factorial/fibonacci/binomial

### Hashing & PRNG
- [x] **FNV-1a** — 32/64-bit hash
- [x] **MurmurHash3** — 32-bit hash
- [x] **Xorshift** — fast PRNG + bounded/double

### Networking
- [x] **IPv6 class** — scope classification
- [x] **HTTP date** — IMF-fixdate format/parse

### Terminal & rendering
- [x] **Color quantize** — truecolor to 256/16
- [x] **Graphemes** — cluster boundary detection

### Config & automation
- [x] **Duration config** — compound "1h30m" parse
- [x] **Expression calc** — integer arithmetic eval

### UI & sessions
- [x] **Sort indicator** — column/direction state
- [x] **Virtual scroll** — windowed list math
- [x] **Session tags v2** — colored labels

### Security
- [x] **OTP window** — HOTP counter verification

## v3.21.0 (In Progress) — Text Encoding & Internationalization
*Focus: Unicode codecs, transliteration, i18n, message catalogs.*

### Unicode codecs
- [x] **UTF-8 codec** — encode/decode/strlen
- [x] **UTF-16 codec** — surrogate pair handling
- [x] **BOM** — byte-order-mark detect/strip
- [x] **Latin-1** — ASCII transliteration

### i18n & localization
- [x] **Plural** — English + Slavic plural rules
- [x] **Message catalog** — key/value translations
- [x] **Number locale** — digit grouping
- [x] **i18n format** — {n} placeholder interpolation

### Text transforms
- [x] **Case convert** — upper/lower/swap
- [x] **HTML entities** — encode/decode
- [x] **Text align** — left/right/center

### Networking & security
- [x] **Punycode** — IDNA ACE detection
- [x] **Unicode safe** — bidi/zero-width detection

### UI, config & sessions
- [x] **Char picker** — special-char categories
- [x] **Keyboard layout** — shift/unshift mapping
- [x] **History search** — filtered command search

## v3.22.0 (In Progress) — Data Structures II
*Focus: maps, queues, sets, allocators, sliding windows.*

### Maps & sets
- [x] **Hash map v2** — open-addressing string map
- [x] **Sorted set** — binary-search insert
- [x] **Range set** — integer range membership
- [x] **Bloom filter** — probabilistic membership
- [x] **Union-find** — disjoint set with path compression

### Queues & lists
- [x] **Deque v2** — double-ended queue
- [x] **Priority queue** — explicit-priority heap
- [x] **Linked list v2** — array-backed list
- [x] **Action queue** — dedup + priority dequeue

### Allocators & windows
- [x] **Object pool** — fixed-slot allocator
- [x] **Session pool** — connection-slot reuse
- [x] **Sliding window** — sequence-number tracking

### State & UI
- [x] **FSM v2** — named-state machine
- [x] **Tree model** — expandable tree

### Terminal & config
- [x] **Grapheme width** — run width + fit
- [x] **Layered config** — priority-layer resolution

## v3.23.0 (In Progress) — Transport, HTTP & Multiplexing
*Focus: congestion control, HTTP caching/auth, SSH channels, rate limits.*

### Transport
- [x] **TCP window** — congestion window estimation
- [x] **Sliding window** (see v3.22.0)
- [x] **Port knock** — knock-sequence validation
- [x] **GeoIP** — IP-range region lookup

### HTTP
- [x] **HTTP cache** — Cache-Control freshness
- [x] **HTTP auth v2** — Basic + Bearer headers
- [x] **API rate limit** — header tracking + backoff
- [x] **Retry jitter** — decorrelated/full jitter

### SSH & tunnels
- [x] **SSH channels** — multiplexing window tracking
- [x] **Tunnel map** — local/remote forward mapping
- [x] **WebSocket mask** — payload masking

### Automation, config & security
- [x] **Webhook format** — JSON event payloads
- [x] **Proxy config** — env parse + no_proxy
- [x] **Escape filter** — strip unsafe OSC sequences

### Sessions & UI
- [x] **Connection history** — per-host success rate
- [x] **Status icon** — state to icon/color

## v3.24.0 (In Progress) — Rendering & Animation
*Focus: redraw coalescing, glyph caching, frame pacing, animation.*

### Rendering pipeline
- [x] **Dirty rect** — redraw region coalescing
- [x] **Glyph cache** — LRU rasterization cache
- [x] **Frame pacer** — target-FPS budget
- [x] **Cursor shape** — DECSCUSR handling
- [x] **Sixel parse** — graphics dimension scan

### Animation & motion
- [x] **Anim frame** — frame-based interpolation
- [x] **Blink state** — blink attribute timing
- [x] **Scroll momentum** — kinetic scrolling

### Terminal text
- [x] **Wrap log** — continuation markers
- [x] **Tab expand** — tabs to spaces + visual col

### UI
- [x] **Focus ring** — traversal order
- [x] **Minimap** — scrollbar overview mapping
- [x] **Font config** — font spec parse/format

### Automation & sessions
- [x] **Output match** — expectation tracking
- [x] **Replay buffer** — session output buffering

## v3.25.0 (In Progress) — Cryptography & Access Control
*Focus: hashing cores, key derivation, secret handling, sandbox policy.*

### Cryptographic primitives
- [x] **SHA-256 core** — block transform
- [x] **HMAC util** — ipad/opad key padding
- [x] **Key derive** — iterated KDF mixing
- [x] **Constant-time eq** — timing-safe compare
- [x] **Nonce** — generate + replay tracking

### Secrets & passwords
- [x] **Password strength v2** — variety + sequence scoring
- [x] **Secret mask** — sensitive key/value masking
- [x] **Secret store v2** — keystream obfuscation
- [x] **Credential cache** — TTL-expiring cache

### Certificates & TLS
- [x] **Cert validity** — date-window checks
- [x] **TLS cipher** — suite strength rating

### Access control
- [x] **Permission bits** — rwx format/parse/octal
- [x] **Sandbox policy** — capability grants

### UI
- [x] **Lock indicator** — caps/num/scroll state

## v3.26.0 (In Progress) — Workflow, Scheduling & Config
*Focus: DAG validation, cron timing, config diffing, builders.*

### Workflow & scheduling
- [x] **Workflow DAG** — cycle detection
- [x] **Cron next** — next fire-time computation
- [x] **Task timer** — execution time measurement
- [x] **Step retry** — per-step retry limits
- [x] **Script error** — error tracking/formatting

### Config & environment
- [x] **YAML scalar** — typed value parsing
- [x] **Config watch** — change detection
- [x] **Setting diff v2** — change counting
- [x] **Env merge** — variable set merging

### UI & tools
- [x] **Command palette v2** — fuzzy filter/rank
- [x] **Quick switch v2** — MRU ordering
- [x] **Condition chain** — AND/OR evaluation
- [x] **String builder** — growable buffer
- [x] **CSV reader** — streaming row reader

### Sessions
- [x] **Session notes** — attach + search

## v3.27.0 (In Progress) — Observability & Text Utilities
*Focus: metrics, logging, diagnostics, text formatting, networking helpers.*

### Metrics & diagnostics
- [x] **Histogram v2** — bucketed percentiles
- [x] **Counter v2** — monotonic + rate
- [x] **Gauge v2** — min/max tracking
- [x] **Stopwatch v2** — lap timing
- [x] **Metric aggregate** — windowed avg/min/max
- [x] **Health check** — component states
- [x] **Moving median** — approximate median

### Logging
- [x] **Logfmt** — key=value structured logs
- [x] **Log level** — parse/filter levels
- [x] **Ring log** — recent-line buffer
- [x] **Alert dedup** — windowed suppression

### Text utilities
- [x] **Text indent** — indent/dedent blocks
- [x] **String repeat** — repeat/reverse
- [x] **Truncate v2** — ellipsis + middle
- [x] **Wrap text v2** — line-count estimation

### Networking
- [x] **URL build** — component assembly
- [x] **Host/port** — split/join (IPv6 aware)
- [x] **IPv4 increment** — address iteration
- [x] **HTTP method** — safe/idempotent/body
- [x] **Status category** — code classification

### Terminal
- [x] **Bell control** — rate limiting
- [x] **Title stack** — XTPUSHTITLE save/restore
- [x] **OSC 52 build** — clipboard sequences
- [x] **Mode toggle** — DEC private mode tracking
- [x] **Cursor report** — CPR parse/build

### UI, config, sessions & security
- [x] **Match highlight** — fuzzy match positions
- [x] **Tab title** — truncated titles
- [x] **Key chord v2** — parse/format
- [x] **Bool text** — boolean parsing
- [x] **Path join** — join/basename/extension
- [x] **List parse v2** — delimited lists
- [x] **PIN generator** — generate + weakness check
- [x] **Token bucket v2** — security throttling
- [x] **Session rate** — quality scoring
- [x] **Reconnect v2** — attempt scheduling

## v3.28.0 (In Progress) — Math & Geometry Utilities
- [x] **anglecvt** — Angle unit conversions (deg/rad/grad)
- [x] **basexx** — Convert integers to arbitrary base strings
- [x] **lerpset** — Interpolation helpers (linear/smooth/step)
- [x] **roundto** — Round numbers to steps and significant digits
- [x] **portname** — Well-known port to service name
- [x] **scrollmark** — Track scroll position markers
- [x] **statusflash** — Timed status message flashing
- [x] **csvschema** — Validate CSV rows against a column schema
- [x] **jitterbo** — Exponential backoff with proportional jitter
- [x] **hashcmp** — Compare hash digests safely

## v3.29.0 (In Progress) — Encoding & Parsing
- [x] **base16** — Uppercase/lowercase hex with grouping
- [x] **quotedprint** — Quoted-Printable encode/decode
- [x] **csvrow** — Build a CSV row from fields with escaping
- [x] **kvparse2** — Parse key=value with comment/section support
- [x] **urihost** — Extract host/port/path from a URI
- [x] **c0names** — Names for C0 control characters
- [x] **inputmask** — Apply input masks to text fields
- [x] **envsubst** — Substitute $VAR references in strings
- [x] **tokenmask** — Mask tokens showing only edges
- [x] **idlewatch** — Track session idle time and warnings
