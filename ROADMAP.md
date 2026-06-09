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
