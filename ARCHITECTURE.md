# PuttyAlt Architecture

## Module Layout

All PuttyAlt-specific code lives in `puttyalt_*.c` / `puttyalt_*.h` files
at the repository root. This keeps upstream PuTTY code unmodified for easy
rebasing when new PuTTY versions are released.

## Module Categories

### Core UI
| Module | Purpose |
|--------|---------|
| `puttyalt_tabs` | Tab management and switching |
| `puttyalt_sftppanel` | Split-pane SFTP file browser |
| `puttyalt_statusbar` | Bottom status bar |
| `puttyalt_theme` | Colour scheme engine |
| `puttyalt_shortcuts` | Keyboard shortcut mapping |
| `puttyalt_notify` | Desktop toast notifications |

### Session Management
| Module | Purpose |
|--------|---------|
| `puttyalt_sessions` | Folder hierarchy and tags |
| `puttyalt_import` | Import from PuTTY/SSH config |
| `puttyalt_credstore` | Encrypted password storage |
| `puttyalt_aliases` | Hostname shortcut resolver |
| `puttyalt_quickconnect` | URI parser for Ctrl+K bar |

### Connection
| Module | Purpose |
|--------|---------|
| `puttyalt_reconnect` | Auto-reconnect with backoff |
| `puttyalt_timeout` | Connect/idle timeout config |
| `puttyalt_proxydetect` | System proxy auto-detection |
| `puttyalt_latency` | RTT tracking and sparklines |
| `puttyalt_health` | Uptime and transfer stats |

### Terminal Enhancements
| Module | Purpose |
|--------|---------|
| `puttyalt_termsearch` | Scrollback text search |
| `puttyalt_triggers` | Pattern-based alerts |
| `puttyalt_broadcast` | Multi-input to many sessions |
| `puttyalt_cliphistory` | Clipboard ring buffer |
| `puttyalt_cmdhistory` | Cross-session command history |
| `puttyalt_envindicator` | Prod/staging colour stripes |

### Infrastructure
| Module | Purpose |
|--------|---------|
| `puttyalt_portable` | Registry-free portable mode |
| `puttyalt_geometry` | Window position save/restore |
| `puttyalt_logrotate` | Automatic log file rotation |
| `puttyalt_snippets` | Command snippet library |
| `puttyalt_keymanager` | SSH key auto-discovery |
| `puttyalt_transfer` | File transfer progress |
| `puttyalt_agentfwd` | SSH agent forwarding control |
| `puttyalt_export` | Session export to SSH config/JSON |
| `puttyalt_auditlog` | Security audit logging |

## Build System

All modules are compiled into a single static library (`libputtyalt`) via
CMake and linked into the final executables. Feature flags in `puttyalt.h`
control which modules are active at compile time.

## Design Principles

1. **Minimal upstream changes** — never modify `terminal.c`, `window.c`,
   etc. directly; hook in via callbacks
2. **C99** — match PuTTY's language standard
3. **No external dependencies** — only Win32 API and POSIX
4. **Portable** — every module compiles on Windows (MSVC/MinGW) and Linux
5. **Secure** — use `smemclr()` for secrets, validate all inputs

## v0.3.0 Modules

### puttyalt_splitview (Split-Pane Terminal)
- Horizontal and vertical splits, up to 4 panes
- Each pane can host an independent session
- Resize redistributes space evenly

### puttyalt_macro (Keyboard Macros)
- Record keystroke sequences with timing
- Playback with configurable repeat count or infinite loop
- Up to 16 named macro slots

### puttyalt_loggingv2 (Structured Logging)
- JSON, CSV, and plain text output formats
- Severity levels (TRACE through FATAL)
- Automatic rotation by size with retention policy

### puttyalt_sesslock (Session Lock)
- Password-protect active sessions
- Auto-lock after idle time
- Brute-force lockout (5 attempts / 60s)

### puttyalt_bandwidth (Bandwidth Monitor)
- Real-time RX/TX rate calculation
- 60-second rolling history with peak tracking

### puttyalt_zmodem (ZMODEM Transfer)
- Auto-detects ZMODEM initiation in terminal output
- Delegates to rz/sz for protocol handling

### puttyalt_autocomplete (Command Completion)
- Built-in dictionary of 60+ Unix commands
- Prefix matching, learns from history

### puttyalt_gesture (Mouse Gestures)
- Right-button drag recognition (8 gesture types)
- Configurable action bindings

## v0.4.0 Module Summary

v0.4.0 adds 40+ modules across infrastructure (INI, strbuf, mempool, base64,
escape parser, validation, perf timer), networking (tunnels, connpool, diag,
proxy chain, keepalive, telnet, serial), sessions (workspaces, templates,
bookmarks, tags, migration, scheduler, startup), security (host verify, keygen,
agent fwd, x11 fwd), UI (palette, colorpicker, fontmgr, layouts, bell,
imgproto, url handler, find/replace, sound), data (scp, export, scrollcomp,
shared clipboard, stats), and extensibility (plugin, eventbus, ipc, triggers v2,
autoupdate). Total: 80+ source files.

## v1.0.0 — GUI, Multiplexer & Monitoring

### GUI Layer
- `puttyalt_gui.h/.c` — Win32 native GUI framework, warm blue theme
- `puttyalt_dialogs.h/.c` — Connection, settings, and tool dialogs
- `puttyalt_tray.h/.c` — System tray icon with notifications
- `puttyalt_renderer.h/.c` — Cell-based terminal renderer

### Session Management
- `puttyalt_sessgroup.h/.c` — Session groups with color coding
- `puttyalt_sshconfig.h/.c` — SSH config file parser/writer

### Monitoring & Profiling
- `puttyalt_connprof.h/.c` — Connection profiler with jitter analysis
- `puttyalt_monitor.h/.c` — Alert-based session monitor
- `puttyalt_logengine.h/.c` — Multi-format structured logging

### Automation & Tools
- `puttyalt_scriptmgr.h/.c` — Multi-language script runner
- `puttyalt_mux.h/.c` — Terminal multiplexer (split panes)
- `puttyalt_sshagent.h/.c` — SSH agent key manager
- `puttyalt_clipmgr.h/.c` — Advanced clipboard manager

### Security
- `puttyalt_codesign.h/.c` — Code signing and SmartScreen trust

### Build
- `main.c` — GUI entry point (WinMain / main)
- `puttyalt.rc` — Windows resources (icon, manifest, version)
- `res/puttyalt.manifest` — DPI, compatibility, trust manifest
- `res/puttyalt.ico` — Application icon
