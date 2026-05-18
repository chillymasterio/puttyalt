# PuttyAlt

**Enhanced SSH terminal based on PuTTY 0.83** — tabs, macros, SFTP, themes, plugins, and 100+ features in a portable EXE.

![Version](https://img.shields.io/badge/version-1.2.0-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Platform](https://img.shields.io/badge/platform-Windows%20|%20Linux%20|%20macOS-lightgrey)

## Features

| Category | Features |
|----------|----------|
| **Sessions** | Tabs, groups, bookmarks, workspaces, templates, import/export |
| **Terminal** | Split view, multiplexer, 256-color palette, themes, zoom, renderer |
| **Transfer** | Built-in SFTP panel, SCP, Zmodem |
| **Security** | Credential store, SSH agent, host verification, code signing |
| **Automation** | Macros, snippets, triggers, scripts (shell/python/lua), scheduler |
| **Networking** | Tunnel manager, proxy chains, keep-alive, serial port, Telnet |
| **Monitoring** | Connection profiler, session monitor, bandwidth stats, latency |
| **UI** | Warm blue theme, sidebar, toolbar, system tray, fullscreen |

## Windows SmartScreen Notice

When you first run PuttyAlt, Windows SmartScreen may show a warning — this is normal for any new unsigned software. PuttyAlt is fully open-source and safe to run.

**How to launch:**

1. Double-click `puttyalt.exe`
2. If SmartScreen appears, click **"More info"**
3. Click **"Run anyway"**

**Alternative methods:**

```powershell
# PowerShell — remove download block
Unblock-File -Path .\puttyalt.exe

# Or right-click the file → Properties → check "Unblock" → OK
```

> **Why does this happen?**
> Windows SmartScreen flags executables that don't have an EV code signing certificate ($300+/year). Since PuttyAlt is a free open-source project, we don't have one yet. The source code is fully available for review — you can build the EXE yourself from source if you prefer.

## Quick Start

```bash
# Download
wget https://github.com/chillymasterio/puttyalt/releases/download/v1.0.6/puttyalt.exe

# Or build from source
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt && mkdir build && cd build
cmake .. && make -j$(nproc)
```

## Command Line

```
puttyalt [options] [user@]host[:port]

  -P port       Port number (default: 22)
  -l user       Login username
  -i keyfile    Private key file
  -load name    Load saved session
  -telnet       Telnet protocol
  -serial       Serial connection
  -portable     Force portable mode
  -config file  Alternate config file
```

## Version History

| Version | Highlights |
|---------|-----------|
| **1.0.0** | GUI redesign (warm blue), multiplexer, script manager, session monitor, SSH agent, SSH config parser, code signing, 100+ modules |
| 0.4.0 | Workspaces, tunnels, plugins, serial, 40+ new modules |
| 0.3.0 | Split view, macros, logging v2, autocomplete |
| 0.2.0 | Tabs, SFTP, themes, credential store |
| 0.1.0 | Portable mode, reconnect, snippets, broadcast |

## License

MIT License — see [LICENSE](LICENSE) for details.

### v1.0.2 Modules
| Module | Description |
|--------|-------------|
| Hex Viewer | Binary data inspection with search and edit |
| Fingerprint Manager | SSH host key verification and trust store |
| Port Forwarding | Local, remote, and dynamic SOCKS tunnels |
| Tab Completion | Frequency-ranked command/path suggestions |
| Accessibility | Screen reader support, high contrast themes |
| Rate Limiter | Token bucket connection throttling |
| Chat Log | Chat-style session log with HTML export |

### v1.0.5 Modules
| Module | Description |
|--------|-------------|
| Network Scanner | CIDR range scanning with service detection |
| Terminal Effects | CRT distortion, scanlines, phosphor glow |
| Localization | i18n support for en, ru, de, zh, ja |
| Tunnel Monitor | SSH tunnel status with auto-reconnect |
| Command Palette | Fuzzy search command launcher (Ctrl+Shift+P) |
| Performance Monitor | Real-time metrics with threshold alerts |

**Total: 205+ source modules across 420+ commits**

### v1.0.6 (Latest)
- Layout manager with named presets and pane persistence
- Macro recorder/player with looping and hotkey triggers
- Terminal profile manager (Default, Compact, Widescreen, Presentation)
- Auto-updater with GitHub releases integration
- Enhanced status bar with sections and live updates
- SmartScreen bypass improvements: WinVerifyTrust, auto Zone.Identifier cleanup
- Code signing scripts for Windows (PowerShell + batch)
- Theme engine with 4 built-in color schemes (Warm Blue, Ocean Dark, Midnight Green, Classic)
- Customizable keybinding manager
- SFTP file manager with transfer queue
- Bandwidth monitor with per-session tracking
- Proxy chain manager (SOCKS4/5, HTTP CONNECT)
- Hex viewer for binary data inspection
- SSH host fingerprint trust manager
- Port forwarding manager (local/remote/dynamic)
- Tab completion engine with frequency ranking
- Accessibility: screen reader, high contrast themes, visual bell
- Token bucket rate limiter
- Chat-style session log with HTML export

---

## Coming Soon: PuttyAlt v2.0

> **A complete UI rewrite is in development.**

PuttyAlt v2.0 will feature a modern, GPU-accelerated rendering engine replacing the current Win32 GDI backend. Expect smooth animations, ligature support, true 24-bit color, split panes, a command palette, and much more.

See [ROADMAP.md](ROADMAP.md) for the full plan.

### v1.1.0 Highlights (Current)

| Module | Description |
|--------|-------------|
| Terminal Engine | Full cell-based emulation with ANSI parser |
| Text Selection | Char, word, line, and rectangle selection modes |
| Scrollback Buffer | Ring buffer with configurable size and search |
| Color Manager | xterm-256 palette with scheme editor |
| Font Chain | Fallback chain with antialiasing control |
| URL Detection | Clickable links with protocol matching |
| Context Menus | Right-click menus for terminal, sidebar, tabs |
| Quick Connect | Connect bar with history and user@host parsing |
| Favorites | Session favorites with tags and usage tracking |
| Health Monitor | Latency, packet loss, idle detection |
| Auto-Reconnect | Exponential backoff with jitter |
| Jump Hosts | Multi-hop SSH proxy chains (up to 8 hops) |
| UI Layer | Abstraction for future framework migration |
| Dock Panels | Left/right/top/bottom/center/float layout |
| Tab Manager | Pin, move, close-others, Ctrl+Tab cycling |
| Broadcast v2 | Target filtering, dangerous command detection |
| Session Replay | Binary recording format with playback controls |
| Setup Wizard | First-run configuration wizard |
| Bell Controller | Visual flash, system, taskbar, custom sound |

**Total: 205+ source modules across 420+ commits**
