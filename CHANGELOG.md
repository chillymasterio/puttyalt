# Changelog

## [1.0.0] — 2026-05-21

### Major Release — GUI Redesign & Full Feature Set

#### New Features
- **GUI Redesign**: Warm blue minimalist theme with sidebar panel, zoom controls
- **Terminal Multiplexer**: Split panes, focus cycling, equalize layout
- **Script Manager**: Run shell/Python/Lua/Expect scripts on session triggers
- **Connection Profiler**: Real-time latency tracking, jitter analysis, CSV export
- **Session Monitor**: Metric-based alerts with configurable rules and actions
- **SSH Agent Manager**: Key lifecycle management, auto-add, lifetime control
- **Clipboard Manager v2**: Ring buffer with search, deduplication, pinned entries
- **Log Engine v3**: Multi-format output (raw/text/HTML/CSV/JSON) with rotation
- **Session Groups**: Organize sessions into colored collapsible folders
- **Terminal Renderer**: Cell-based grid rendering with dirty region tracking
- **SSH Config Parser**: Full ~/.ssh/config read/write support
- **Code Signing**: SmartScreen trust, Zone.Identifier removal, Authenticode support

#### Improvements
- Windows manifest: OS compatibility declarations, long path support, segment heap
- Resource file: proper version info for SmartScreen trust
- Default font changed to Cascadia Code
- Sidebar: session list, bookmarks, snippets panels
- Zoom: Ctrl++/Ctrl+-/Ctrl+0
- Scrollback increased to 20000 lines default
- Menu reorganization with keyboard shortcuts throughout

#### Bug Fixes
- tabs: set active to -1 when all tabs closed
- snippets: null check on category parameter
- macro: tighter slot bounds checking
- IPC: reset fd to -1 after server stop
- workspace: null-guard save parameters
- validate: reject hostnames starting with dash
- credstore: zero sensitive buffers after use
- hostverify: use snprintf for null termination
- escparse: return 0 for incomplete sequences
- serial: validate baud rate range (50-4M)
- connpool: add forward declaration for eviction
- telnet: prevent option negotiation overflow

## [0.4.0] — 2026-05-20

### Workspaces, Tunnels, Plugins & 40+ New Modules
(see previous changelog entries)

## [0.3.0] — 2026-05-18

### Split View, Macros & Autocomplete

## [0.2.0] — 2026-05-17

### Tabs, SFTP & Themes

## [0.1.0] — 2026-05-16

### Initial Release — Portable mode, reconnect, snippets

## [1.0.2] - 2025-05-22

### Added
- Hex viewer for binary data inspection with hex/ASCII toggle
- SSH host fingerprint manager with trust levels and known_hosts export
- Port forwarding manager supporting local, remote, and dynamic SOCKS tunnels
- Tab completion engine with frequency-ranked suggestions and wordlists
- Accessibility module: screen reader, high contrast themes, visual bell
- Token bucket rate limiter for connection throttling
- Chat-style session log with HTML export, filtering, and markers

### Fixed
- Missing stdio.h include in search highlight module
- Missing stdlib.h include in fingerprint module
- Missing stdio.h include in rate limiter module
