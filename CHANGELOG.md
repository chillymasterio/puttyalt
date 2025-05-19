# Changelog

## [0.4.0] — 2026-05-19

### Added
- Workspaces, SSH tunnel manager, serial port backend
- Find & replace, session tags, scrollback export (HTML/JSON)
- Connection pooling, URL handler, SCP integration
- Network diagnostics, session templates, color picker
- Font manager, startup commands, layout presets
- Session migration (PuTTY/KiTTY/MobaXterm/SSH config)
- Plugin system, event bus, IPC, auto-update checker
- Bookmarks, shared clipboard, keep-alive manager
- Enhanced bell, agent forwarding v2, X11 forwarding
- Command scheduler, Telnet negotiation, performance profiler
- Memory pool, string builder, INI parser, trigger engine v2
- Key generator, host verification DB, image protocol detection
- Session statistics, proxy chain, 256-color palette
- Scrollback RLE compression, base64, escape parser
- Input validation, notification sounds
- .editorconfig, .gitattributes, SECURITY.md, CONTRIBUTING.md

### Fixed
- SFTP panel crash on empty sort, theme diagnostics
- Snippet empty name rejection, split view div-by-zero
- Macro bounds check, session lock timing side-channel
- Bandwidth precision, URL handler option injection
- IPC socket race, workspace null username, JSON newline escape
- Serial port is_open check, credential store stack zeroing

## [0.3.0] — 2026-05-17
### Added
- Split-pane view, keyboard macros, structured logging v2
- Session lock, bandwidth monitor, ZMODEM, auto-complete, gestures
### Fixed
- Tab close NULL deref, credstore overflow, reconnect timer, include guard

## [0.2.0] — 2026-05-16
### Added
- Tabs, SFTP panel, credential store, themes, import, shortcuts
- Notifications, proxy detect, transfer progress, latency, command history
- Status bar, timeouts, aliases, host key pinning, jump hosts

## [0.1.0] — 2026-05-15
### Added
- Portable mode, auto-reconnect, geometry, session manager, snippets
- Health monitor, log rotation, clipboard history, triggers, broadcast
- Key manager, scrollback search, environment indicator, quick connect
