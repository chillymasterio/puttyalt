# PuttyAlt

[![Build](https://github.com/chillymasterio/puttyalt/actions/workflows/build.yml/badge.svg)](https://github.com/chillymasterio/puttyalt/actions/workflows/build.yml)
[![Release](https://img.shields.io/github/v/release/chillymasterio/puttyalt)](https://github.com/chillymasterio/puttyalt/releases)
[![License](https://img.shields.io/github/license/chillymasterio/puttyalt)](LICENSE)

**A modern, feature-rich fork of PuTTY** — the classic SSH and telnet client — with tabbed sessions, split-pane terminals, session management, macros, ZMODEM transfers, and 50+ quality-of-life enhancements for sysadmins and DevOps engineers.

## Key Features

| Category | Features |
|----------|----------|
| **Sessions** | Tabbed interface, workspaces, session templates, bookmarks, tags |
| **Terminal** | Split-pane view, scrollback search, keyboard macros, auto-complete |
| **Transfers** | SFTP panel, SCP integration, ZMODEM auto-detection |
| **Security** | Encrypted credential store, host key pinning, session lock, agent forwarding control |
| **Networking** | SSH tunnel manager, proxy chain, jump host support, keep-alive |
| **UI** | 5 built-in themes, color picker, font manager, mouse gestures |
| **Protocol** | Telnet negotiation, serial port, X11 forwarding |
| **Developer** | Plugin API, event bus, IPC, structured logging, performance profiler |

## Quick Start

```bash
git clone https://github.com/chillymasterio/puttyalt.git
cd puttyalt
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

### Portable Mode

Copy `puttyalt.ini.example` to `puttyalt.ini` next to the executable.
All settings stored in the INI file — no registry access needed.

## Module Architecture

PuttyAlt is built as a modular library (`libputtyalt`) on top of PuTTY 0.83.
Each feature is a self-contained `puttyalt_*.c`/`.h` pair with a feature flag
in `puttyalt.h`.

See [ARCHITECTURE.md](ARCHITECTURE.md) for the full module reference.

## Versioning

| Version | Highlights |
|---------|-----------|
| **0.4.0** | Workspaces, SSH tunnels, serial port, plugin system, INI parser, 256-color palette |
| **0.3.0** | Split view, macros, ZMODEM, auto-complete, mouse gestures |
| **0.2.0** | Tabs, SFTP panel, credential store, themes, key pinning |
| **0.1.0** | Portable mode, auto-reconnect, session manager, snippets |

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for development setup and code style.

## Security

See [SECURITY.md](SECURITY.md) for our vulnerability disclosure policy.

## License

PuttyAlt is released under the MIT License. Based on [PuTTY 0.83](https://www.chiark.greenend.org.uk/~sgtatham/putty/) by Simon Tatham.
