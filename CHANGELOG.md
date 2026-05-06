# Changelog

## [0.3.0] — 2026-05-17

### Added
- Split-pane terminal view (horizontal/vertical, up to 4 panes)
- Keyboard macro recording and playback with loop support
- Structured logging v2: JSON, CSV, and text formats with rotation
- Session lock with auto-lock timer and brute-force lockout
- Bandwidth usage monitor with peak tracking
- ZMODEM auto-detection for sz/rz file transfers
- Command auto-completion with Unix command dictionary
- Mouse gesture navigation (right-click drag for tab/action control)

### Fixed
- Crash when closing last tab in group (NULL dereference)
- Buffer overflow in credential store label/username copy
- Missing reconnect_cancel() caused stale timer callbacks
- Include guard placement in puttyalt.h (v0.2 flags were outside guard)

## [0.2.0] — 2026-05-16

### Added
- Tabbed interface (up to 64 sessions, Ctrl+T/Ctrl+W)
- Split-pane SFTP file browser with drag-and-drop
- AES-256-GCM encrypted credential store
- 5 built-in colour themes (Solarized, Dracula, Nord, Monokai, Gruvbox)
- Session import from SSH config and PuTTY registry
- Customisable keyboard shortcuts
- Desktop notification queue
- System proxy auto-detection
- File transfer progress with ETA
- Connection latency sparkline
- Cross-session command history (2048 entries)
- Status bar with configurable sections
- SSH host key pinning (TOFU model)
- ProxyJump / jump host chaining (up to 8 hops)

## [0.1.0] — 2026-05-15

### Added
- Portable mode (INI-based, no registry)
- Auto-reconnect with exponential backoff
- Window geometry persistence
- Session manager with folders and tags
- Command snippet store
- Connection health monitor
- Log rotation
- Clipboard history ring
- Terminal output triggers
- Multi-input broadcast
- SSH key manager
- Terminal scrollback search (Ctrl+Shift+F)
- Environment colour indicator
- Quick connect bar (Ctrl+K)
