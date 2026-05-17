# Changelog

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
