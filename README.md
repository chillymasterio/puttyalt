# PuttyAlt

<div align="center">

### The SSH client you actually want to use.

**PuTTY was built in 1999. Your workflow has changed. Your tools should too.**

![Version](https://img.shields.io/badge/version-2.0.2-58A6FF?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-3FB950?style=flat-square)
![Platform](https://img.shields.io/badge/Windows%20x64-0D1117?style=flat-square&logo=windows&logoColor=white)
![Modules](https://img.shields.io/badge/270+%20modules-D29922?style=flat-square)
![Tests](https://img.shields.io/badge/108%20tests%20passing-3FB950?style=flat-square)

**[Download v2.0.2](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.msi)** | [Changelog](CHANGELOG.md) | [Roadmap](ROADMAP.md)

</div>

---

## Why PuttyAlt?

| | PuTTY | Other terminals | **PuttyAlt** |
|---|---|---|---|
| Dark mode | No | Some | **Native dark UI, 10 color schemes** |
| Tabs | No | Yes | **Groups, hover preview, drag reorder** |
| Automation | No | Limited | **Lua, expect, macros, watchdog, cron** |
| Performance | Good | Varies | **120fps double-buffered, truecolor** |
| Open source | Yes | Some | **MIT, 40K+ lines of C99** |

> **No Electron. No dependencies. No bloat.**
> Download. Double-click. Connect.

---

## Features

| Category | Highlights |
|----------|-----------|
| **UI** | 10 color schemes (Dracula, Nord, Tokyo Night...), animations, tab groups, search bar, font scaling, context menu, status line |
| **Sessions** | Ratings, bookmarks with folders, labels, notes, clone/template, import from PuTTY registry, quick Ctrl+Tab switcher |
| **Terminal** | Truecolor (24-bit), Sixel images, ligatures, bracketed paste, mouse tracking (SGR), scrollback search, syntax highlighting |
| **Security** | 2FA/TOTP detection, certificate viewer, TOFU host verification, known_hosts parser, audit trail, credential store |
| **Automation** | Lua scripting, expect patterns, macro recording, watchdog alerts, cron scheduler, tmux integration, REST API, webhooks |
| **Networking** | SSH banner/OS detection, cipher preferences, keep-alive manager, DNS cache, speed test, proxy presets, port forwarding manager |
| **Transfer** | SCP progress bar with ETA, SFTP panel, drag-and-drop upload, Zmodem |
| **Tools** | Clipboard ring, hex dump, base64, regex matching, hostname auto-complete, Unicode/CJK support, structured logging |
| **Config** | Switchable profiles, keyboard remapping, env variables with expansion, theme export/import, config migration, auto-backup |

## Quick Start

```bash
# Download and run
wget https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.msi

# Or build from source
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt
x86_64-w64-mingw32-gcc -c -Iinclude -I. -O2 src/*/*.c
x86_64-w64-mingw32-gcc *.o -o puttyalt.msi -mwindows -lgdi32 -ldwmapi -lws2_32 -lcomctl32
```

## Command Line

```
puttyalt [options] [user@]host[:port]

  -p port       Port number (default: 22)
  -l user       Login username
  -i keyfile    Private key file
  -load name    Load saved session
  -e command    Execute remote command
  -v            Verbose mode
  -config file  Alternate config file
```

## Keyboard Shortcuts

| Shortcut | Action |
|----------|--------|
| `Ctrl+P` | Command palette |
| `Ctrl+T` | New tab |
| `Ctrl+W` | Close tab |
| `Ctrl+Tab` | Quick switch (MRU) |
| `Ctrl+F` | Find in terminal |
| `Ctrl+N` | New connection |
| `Ctrl+D` | Duplicate session |
| `Ctrl+,` | Settings |
| `Ctrl++/-/0` | Zoom in/out/reset |
| `F11` | Fullscreen |

## Project Structure

```
src/
├── core/          25 modules — GUI, layout, plugins, lifecycle
├── ui/            50 modules — tabs, preview, animations, search, drag-drop
├── network/       40 modules — SSH, tunnels, proxy, DNS, cipher prefs
├── session/       22 modules — bookmarks, labels, ratings, clone, import
├── security/      20 modules — 2FA, certs, audit, known_hosts, host verify
├── terminal/      18 modules — truecolor, sixel, ligatures, mouse, paste
├── automation/    15 modules — Lua, expect, macros, watchdog, tmux, cron
├── editor/        11 modules — snippets, clipboard, categories
├── tools/         70 modules — SCP, hex, base64, Unicode, logging, regex
└── config/        16 modules — profiles, keybinds, env, migration, backup

tests/             9 suites (108 assertions, all passing)
```

## Version History

| Version | Highlights |
|---------|-----------|
| **2.0.2** | 270+ modules, truecolor, Sixel, 2FA, macro recording, tab preview, watchdog, cipher prefs |
| **2.0.1** | UI polish, command palette, multi-size icon, test suite (108/108), project reorganization |
| **2.0.0** | Complete UI rewrite — dark theme, custom controls, DWM integration, double-buffered rendering |
| 1.5.0 | Scripting & Automation — Lua engine, expect, pipelines, REST API, webhooks |
| 1.4.0 | Collaboration — audit trail, roles, session sharing, approval workflow |

## Design Philosophy

```
Fast.       — Native C99, no runtime, no VM, no framework overhead.
Focused.    — SSH done right. Not a general terminal pretending to do SSH.
Modular.    — 270+ modules organized by function. Extend what you need.
Private.    — Zero telemetry. No cloud. Your keys stay on your machine.
```

## Windows SmartScreen

First launch may show a SmartScreen warning — normal for unsigned open-source software.

Click **"More info"** → **"Run anyway"**.

## License

MIT — see [LICENSE](LICENSE).

---

<div align="center">

**270+ modules | 650+ commits | 40,000+ lines of C99**

Built with care. No bloat. No compromise.

**[Download PuttyAlt](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.msi)**

</div>
