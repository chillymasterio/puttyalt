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
