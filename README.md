# PuttyAlt

<div align="center">

### The SSH client you actually want to use.

**PuTTY was built in 1999. Your workflow has changed. Your tools should too.**

![Version](https://img.shields.io/badge/version-2.0.1-58A6FF?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-3FB950?style=flat-square)
![Platform](https://img.shields.io/badge/Windows%20x64-0D1117?style=flat-square&logo=windows&logoColor=white)
![Modules](https://img.shields.io/badge/233%20modules-D29922?style=flat-square)
![Tests](https://img.shields.io/badge/108%20tests%20passing-3FB950?style=flat-square)

**[Download v2.0.1](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.exe)** | [Changelog](CHANGELOG.md) | [Roadmap](ROADMAP.md)

</div>

---

## Why PuttyAlt?

| | PuTTY | Other terminals | **PuttyAlt** |
|---|---|---|---|
| Dark mode | No | Some | **Native dark UI, DWM title bar** |
| Tabs | No | Yes | **Custom-drawn with hover & close** |
| Automation | No | Limited | **Lua, expect, pipelines, REST API** |
| Portable | Yes | Rarely | **Single EXE, zero install** |
| Performance | Good | Varies | **120fps double-buffered, < 1MB** |
| Open source | Yes | Some | **MIT, 35K lines of C99** |

> **One file. No install. No Electron. No dependencies.**
> Download. Double-click. Connect.

---

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

## Design Philosophy

```
Fast.     — Native C99, no runtime, no VM, no framework overhead.
Focused.  — SSH done right. Not a general terminal pretending to do SSH.
Portable. — One EXE. Works from USB. No registry. No %APPDATA% mess.
Private.  — Zero telemetry. No cloud. Your keys stay on your machine.
```

## Building from Source

```bash
git clone https://github.com/chillymasterio/puttyalt.git && cd puttyalt

# Compile (MinGW-w64)
for f in src/*/*.c; do
  x86_64-w64-mingw32-gcc -c -Iinclude -I. -O2 "$f" -o "build/$(basename $f .c).o"
done

# Link
x86_64-w64-mingw32-gcc build/*.o -o puttyalt.exe \
  -mwindows -lws2_32 -lgdi32 -lcomctl32 -ldwmapi -lcomdlg32 -lm
```

## Windows SmartScreen

First launch may show a SmartScreen warning — normal for unsigned open-source software.

Click **"More info"** → **"Run anyway"**, or: `Unblock-File -Path .\puttyalt.exe`

## License

MIT — see [LICENSE](LICENSE).

---

<div align="center">

**233 modules | 550+ commits | 35,000+ lines of C99**

Built with care. No bloat. No compromise.

**[Download PuttyAlt](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.exe)**

</div>
