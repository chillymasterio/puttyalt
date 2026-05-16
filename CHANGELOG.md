# Changelog

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
- Interactive terminal (welcome screen + connected state)
- Dynamic sidebar with session info
- Status bar with live session data
- Tab text updates on connect/disconnect
- Version shown in terminal welcome

## [1.0.6] — 2025-05-22

### Added
- Layout manager with named presets
- Macro recorder with hotkey triggers
- Terminal profile manager
- Auto-updater with GitHub integration
- Enhanced status bar
- Theme engine with 4 built-in schemes
- Keybinding manager
- SFTP file manager
- Bandwidth monitor
- Proxy chain manager

## [1.0.0] — 2025-05-19

### Added
- Complete GUI redesign with Warm Blue theme
- Win32 native application framework
- Tab control, sidebar, toolbar, status bar
- Session multiplexer
- Script manager
- Connection profiler
- SSH agent forwarding
- SSH config parser
- 100+ feature modules

## [0.4.0] — 2025-05-18
- Workspaces, tunnel manager, plugins, serial port
- 40+ new modules

## [0.3.0] — 2025-05-17
- Split view, macros, logging v2, autocomplete

## [0.2.0] — 2025-05-16
- Tabs, SFTP panel, themes, credential store

## [0.1.0] — 2025-05-14
- Initial release: portable mode, auto-reconnect, snippets, broadcast
