# PuttyAlt

**Modern SSH terminal built on PuTTY 0.83** — dark UI, custom-drawn controls, tabs, 233 modules in a single portable EXE.

![Version](https://img.shields.io/badge/version-2.0.1-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Windows%20x64-lightgrey)
![Modules](https://img.shields.io/badge/modules-233-orange)
![Tests](https://img.shields.io/badge/tests-108%20passing-brightgreen)

---

## Screenshots

Dark theme with custom tab bar, sidebar, and terminal:

```
+-----------------------------------------------------+
|  PuttyAlt v2.0.1                            - [] X  |
+----------+----------+------------------------------+
| Session1 | Server2  |  +                            |
+----------+----------+------------------------------+
| SESSIONS |                                          |
| * prod   |  PuttyAlt                                |
|   dev    |  Modern SSH & Terminal Client             |
|          |                                          |
| QUICK    |     Ctrl+N   New Connection              |
| New Sess |     Ctrl+P   Command Palette             |
| Snippets |     Ctrl+F   Search                      |
| SFTP     |     Ctrl+,   Preferences                 |
| Keys     |     F11      Fullscreen                  |
|          |                                          |
+----------+------------------------------------------+
| * Connected | user@host |               v2.0.1     |
+-----------------------------------------------------+
```

## Features

| Category | Highlights |
|----------|-----------|
| **UI** | GitHub Dark theme, custom-drawn controls, DWM dark title bar, double-buffered 120fps rendering |
| **Sessions** | Tabs, groups, tags, sharing, templates, approval workflow, import/export |
| **Terminal** | ANSI parser, 256-color, selection modes, scrollback search, cursor styles |
| **Security** | Credential store (OS entropy), audit trail, role-based access, host verification |
| **Automation** | Lua scripting, expect patterns, pipeline builder, scheduler, REST API, webhooks |
| **Networking** | Tunnel manager, proxy chains, keepalive, bandwidth monitor, connection dashboard |
| **Transfer** | SFTP panel, SCP, Zmodem, SFTP sync (folder watch) |
| **Tools** | Command palette, snippets, macros, bookmarks, cron viewer, regex triggers |

## Quick Start

```bash
# Download portable EXE (no install needed)
wget https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.exe

# Or build from source
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt
x86_64-w64-mingw32-gcc -c -Iinclude -I. -O2 src/*/*.c
x86_64-w64-mingw32-gcc *.o -o puttyalt.exe -mwindows -lgdi32 -ldwmapi -lws2_32 -lcomctl32
```

## Command Line

```
puttyalt [options] [user@]host[:port]

  -P port       Port number (default: 22)
  -l user       Login username
  -i keyfile    Private key file
  -load name    Load saved session
  -portable     Force portable mode
  -config file  Alternate config file
```

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+N` | New connection |
| `Ctrl+P` | Command palette |
| `Ctrl+F` | Find in terminal |
| `Ctrl+,` | Preferences |
| `Ctrl+D` | Duplicate tab |
| `Ctrl+R` | Reconnect |
| `Ctrl+B` | Broadcast mode |
| `Ctrl+\` | Toggle sidebar |
| `F11` | Fullscreen |
| `Ctrl++/-/0` | Zoom in/out/reset |

## Project Structure

```
src/
├── core/          25 modules — GUI, design system, plugins, app lifecycle
├── ui/            42 modules — tabs, toast, tooltip, transitions, sidebar
├── network/       33 modules — connections, proxy, tunnels, keepalive
├── session/       15 modules — session management, groups, tags, sharing
├── security/      15 modules — credentials, keys, audit, roles
├── terminal/      10 modules — emulation, ANSI parser, selection
├── automation/    11 modules — Lua, expect, pipelines, scheduler, webhooks
├── editor/        11 modules — snippets, clipboard, file editing
├── tools/         60 modules — SFTP, logging, bookmarks, diagnostics
└── config/        11 modules — sync, templates, presets, validation

tests/             9 test suites (108 assertions, all passing)
include/           All headers (single include path)
```

## Version History

| Version | Highlights |
|---------|-----------|
| **2.0.1** | UI polish, multi-size icon, command palette, test suite (108/108), project reorganization |
| **2.0.0** | Complete UI rewrite — GitHub Dark, custom controls, DWM dark mode, double-buffered rendering |
| 1.5.0 | Scripting & Automation — Lua engine, expect, pipelines, REST API, webhooks |
| 1.4.0 | Collaboration — audit trail, roles, session sharing, approval workflow |
| 1.3.x | Workflow — session groups, SFTP sync, regex triggers, OS detection |
| 1.2.0 | UX — smart paste, fuzzy search, password generator, multi-clipboard |
| 1.1.0 | Terminal engine — ANSI parser, selection, scrollback, 256-color |

## Windows SmartScreen

First run may trigger SmartScreen — this is normal for unsigned software.

1. Click **"More info"** then **"Run anyway"**

Or in PowerShell: `Unblock-File -Path .\puttyalt.exe`

## Building

Requirements: MinGW-w64 (x86_64-w64-mingw32-gcc)

```bash
# Compile all modules
for f in src/*/*.c; do
  x86_64-w64-mingw32-gcc -c -Iinclude -I. -O2 "$f" -o "build/$(basename $f .c).o"
done

# Link
x86_64-w64-mingw32-gcc build/*.o -o puttyalt.exe \
  -mwindows -lws2_32 -lgdi32 -lcomctl32 -ldwmapi -lcomdlg32 -lm

# Run tests (native)
gcc -Iinclude -I. tests/test_automation.c src/automation/*.c -o test && ./test
```

## License

MIT License — see [LICENSE](LICENSE) for details.

---

**233 modules | 550+ commits | 35,000+ lines of C**
