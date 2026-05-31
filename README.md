# PuttyAlt

<div align="center">

### The SSH client you actually want to use.

**PuTTY was built in 1999. Your workflow has changed. Your tools should too.**

![Version](https://img.shields.io/badge/version-2.0.1-58A6FF?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-3FB950?style=flat-square)
![Platform](https://img.shields.io/badge/Windows%20x64-0D1117?style=flat-square&logo=windows&logoColor=white)
![Modules](https://img.shields.io/badge/350+%20modules-D29922?style=flat-square)
![Tests](https://img.shields.io/badge/108%20tests%20passing-3FB950?style=flat-square)

**[Download v2.0.1](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.msi)** | [Changelog](CHANGELOG.md) | [Roadmap](ROADMAP.md)

</div>

---

## Why PuttyAlt?

| | PuTTY | Other terminals | **PuttyAlt** |
|---|---|---|---|
| Dark mode | No | Some | **Native dark UI, 10 color schemes** |
| Tabs | No | Yes | **Groups, hover preview, drag reorder** |
| Split panes | No | Some | **Binary split tree, h/v, zoom, broadcast** |
| Automation | No | Limited | **Lua, expect, macros, watchdog, cron, plugins** |
| Performance | Good | Varies | **120fps, render cache, dirty-rect redraw** |
| Open source | Yes | Some | **MIT, 40K+ lines of C99** |

> **No Electron. No dependencies. No bloat.**
> Download. Double-click. Connect.

---

## Features

| Category | Highlights |
|----------|-----------|
| **UI** | 10 color schemes (Dracula, Nord, Tokyo Night...), split panes (h/v) with focus nav/zoom, floating panel snap, animations, tab groups, search bar, font scaling, quick-launch palette |
| **Sessions** | Ratings, bookmarks with folders, labels, notes, clone/template, import from PuTTY registry, workspaces, startup restore, quick Ctrl+Tab switcher |
| **Terminal** | Truecolor (24-bit), inline images (Sixel, iTerm2, Kitty), GPU glyph atlas, OSC 8 hyperlinks, OSC 133 prompt marks, ligatures, BiDi, block selection, multi-pane broadcast, scrollback search |
| **Security** | 2FA/TOTP detection, certificate viewer, TOFU host verification, known_hosts parser, audit trail, credential store |
| **Automation** | Lua scripting, expect patterns, macro recording, watchdog alerts, cron scheduler, tmux integration, sandboxed plugin API v2, AI command suggestions, REST API, webhooks |
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
x86_64-w64-mingw32-gcc *.o -o puttyalt.exe -mwindows -lgdi32 -ldwmapi -lws2_32 -lcomctl32
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
├── core/          29 modules — GUI, layout, render cache, dirty-rect, frame pacer
├── ui/            58 modules — tabs, split panes, focus nav, panel snap, scrubber
├── network/       48 modules — SSH, tunnels, proxy, DNS, write batch, latency hist
├── session/       29 modules — bookmarks, workspaces, co-edit, presence, restore
├── security/      26 modules — 2FA, certs, plugin grants, vault, TOFU pin, audit chain
├── terminal/      44 modules — GPU atlas, inline images, hyperlinks, BiDi, allocators
├── automation/    20 modules — Lua, expect, plugin API v2, AI suggestions, cron
├── editor/        12 modules — snippets, clipboard, categories
├── tools/         71 modules — SCP, hex, base64, fuzzy history, asciicast, logging
└── config/        19 modules — profiles, layouts, keybinds, migration, backup

tests/             9 suites (108 assertions, all passing)
```

## Version History

| Version | Highlights |
|---------|-----------|
| **2.2.0** (in progress) | Vulkan/GPU glyph atlas, real-time collaboration (co-edit, presence, annotations), perf (arena/slab allocators, write batching, latency histogram), OSC 8 hyperlinks, OSC 133 prompt marks, BiDi, WebSocket transport, audit signing chain |
| **2.1.0** | Split panes (h/v, focus nav, zoom, broadcast), render cache + dirty-rect redraw, inline images (iTerm2/Kitty), sandboxed plugin API v2, AI command suggestions, workspaces, layout save/restore |
| **2.0.1** | 270+ modules, truecolor, Sixel, 2FA, macro recording, tab preview, watchdog, cipher prefs, command palette, multi-size icon, test suite (108/108), project reorganization |
| **2.0.0** | Complete UI rewrite — dark theme, custom controls, DWM integration, double-buffered rendering |
| 1.5.0 | Scripting & Automation — Lua engine, expect, pipelines, REST API, webhooks |
| 1.4.0 | Collaboration — audit trail, roles, session sharing, approval workflow |

## Design Philosophy

```
Fast.       — Native C99, no runtime, no VM, no framework overhead.
Focused.    — SSH done right. Not a general terminal pretending to do SSH.
Modular.    — 350+ modules organized by function. Extend what you need.
Private.    — Zero telemetry. No cloud. Your keys stay on your machine.
```

## Windows SmartScreen

First launch may show a SmartScreen warning — normal for unsigned open-source software.

Click **"More info"** → **"Run anyway"**.

## License

MIT — see [LICENSE](LICENSE).

---

<div align="center">

**350+ modules | 730+ commits | 48,000+ lines of C99**

Built with care. No bloat. No compromise.

**[Download PuttyAlt](https://github.com/chillymasterio/puttyalt/releases/download/v2.0.1/puttyalt.msi)**

</div>
