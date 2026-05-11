# PuttyAlt

**Enhanced SSH terminal based on PuTTY 0.83** — tabs, macros, SFTP, themes, plugins, and 100+ features in a portable EXE.

![Version](https://img.shields.io/badge/version-1.0.0-blue)
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

## Quick Start

```bash
# Download
wget https://github.com/chillymasterio/puttyalt/releases/download/v1.0.0/puttyalt.exe

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
