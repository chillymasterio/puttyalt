# Changelog

## [2.0.0] — 2025-05-27

### Changed — Modern UI Rewrite
- Complete GUI rewrite with GitHub Dark design system
- Custom double-buffered rendering for all panels (flicker-free)
- Custom owner-drawn tab bar with close buttons and hover states
- Owner-drawn status bar with connection indicator
- DWM dark title bar integration (Windows 10 1809+)
- New sidebar design with section headers and quick actions
- Centered welcome screen with keyboard shortcut grid
- Removed legacy Win32 toolbar (cleaner, minimal interface)
- All dialogs updated to new color palette
- 120fps render cap with optimized message pump

### Added
- Design system header (puttyalt_design.h) with full token set
- GitHub Dark color palette (backgrounds, surfaces, accents, semantic)
- Typography scale with Segoe UI (interface) and Cascadia Code (terminal)
- 4px spacing grid system
- Rounded rectangle rendering helpers
- Tab management: close tabs, add new, hover feedback
- Connection status dot in status bar

### Improved
- Shell-style terminal prompt when connected
- Status updates use targeted invalidation (not full repaint)
- Tab titles update automatically on connect/disconnect
- Reduced Sleep() from 10ms to 8ms for smoother feel

## [1.3.1] — 2025-05-26

### Added
- First-connect OS detection with distro recognition and suggested snippets
- Connection speed test with latency, jitter, and quality labels
- Tab hover preview with cached output lines
- Session diff viewer with LCS comparison
- Input macro recorder with playback speed control
- Local echo with auto-detection and password masking
- Connection retry policies (default, aggressive, conservative)
- Terminal timestamp annotations with elapsed time

### Fixed
- Smart paste now detects base64-encoded shell pipe attacks
- Session search supports wildcard (*) queries
- Output capture flushes periodically for crash safety
- Inline editor detects and blocks binary file editing

## [1.3.0] — 2025-05-25

### Added — Workflow & Productivity
- Session groups v2 with nested folders and color-coding
- Command history browser with cross-session search
- SFTP sync with local folder watch and exclude patterns
- Output capture with ANSI stripping and timestamps
- Connection dashboard with live status and traffic overview
- Regex trigger engine with notify, log, sound, highlight actions
- Inline file editor with line editing and search
- Cron job viewer with human-readable schedule descriptions
- Smart resize with auto font scaling and grid snapping
- Tab color tags (Production, Staging, Dev, etc.)
- Session export/import to OpenSSH config format

## [1.2.0] — 2025-05-24

### Added — UX & Usability
- Smart paste detection: warns before pasting dangerous commands (rm -rf, sudo, curl|bash)
- Session search with fuzzy matching across names, hosts, and tags
- Password generator with strength meter and entropy calculation
- Drag-and-drop handler for SFTP upload and path paste
- Snippet library with categories, search, and 10 default system commands
- Session notes: per-connection annotations with auto-display on connect
- Connection presets: Default, Development, Production, Presentation profiles
- Notification queue with levels, actions, sound, and popup duration
- Terminal search overlay (Ctrl+F) with highlight-all and case-insensitive mode
- SSH key manager with type detection, agent integration, and host matching
- Auto-complete v2 with command history learning and prefix matching
- Connection timeout profiles: default, fast, patient
- Multi-clipboard with 10 named slots and ring mode
- Remote environment variable viewer with filter and edit
- Quick commands panel with one-click monitoring shortcuts

## [1.1.0] — 2025-05-23

### Added
- Terminal emulation engine with full cell-based rendering
- ANSI escape sequence parser (CSI, SGR, OSC, DCS)
- Text selection: character, word, line, and rectangle modes
- Scrollback ring buffer (configurable, default 20,000 lines)
- Scrollback text search
- 256-color palette manager with xterm compatibility
- Font fallback chain (Cascadia Code → Consolas → Courier New)
- URL detection with protocol-aware matching
- Right-click context menus (terminal, sidebar, tabs)
- Quick connect bar with history
- Session favorites with tags, pinning, and usage stats
- Connection health monitor (latency, packet loss, idle detection)
- Auto-reconnect with exponential backoff and jitter
- SSH jump host chain manager (up to 8 hops)
- UI abstraction layer for future framework migration
- Tooltip system with configurable delay
- Dockable panel layout engine (left/right/top/bottom/center/float)
- Configuration validation and sanitization
- First-run setup wizard
- Session recording and binary replay format
- Color scheme editor with import/export
- Advanced bell controller (visual, system, taskbar, custom)
- Enhanced portable mode with directory structure
- Tab manager with pin, move, close-others, Ctrl+Tab
- Broadcast v2 with target filtering and dangerous command detection

### Improved
- All 40+ menu items now have handlers
- Dark-themed dialogs (Connect, Settings, Find, Theme)
- Toolbar with standard icons
- 16 keyboard accelerators

## [1.0.6] — 2025-05-22

### Added
- Layout manager with named presets
- Macro recorder with hotkey triggers
- Terminal profile manager
- Auto-updater with GitHub integration
- Theme engine with 4 built-in schemes
- Keybinding manager
- SFTP file manager
- Bandwidth monitor
- Proxy chain manager

## [1.0.0] — 2025-05-19
- Complete GUI redesign with Warm Blue theme
- Win32 native application framework
- 100+ feature modules

## [0.4.0] — 2025-05-18
- Workspaces, tunnel manager, plugins, serial port

## [0.3.0] — 2025-05-17
- Split view, macros, logging v2, autocomplete

## [0.2.0] — 2025-05-16
- Tabs, SFTP panel, themes, credential store

## [0.1.0] — 2025-05-14
- Initial release: portable mode, auto-reconnect, snippets, broadcast
