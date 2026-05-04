# Changelog

All notable changes to PuttyAlt will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [0.1.0] - 2026-05-15

### Added
- Project setup: README, contributing guidelines, CI/CD pipeline
- GitHub Pages website
- Rebranding from PuTTY to PuttyAlt
- `puttyalt.h`: Centralised version and feature flag header
- CMake security hardening flags (stack protector, FORTIFY_SOURCE, RELRO)
- Portable mode: run from USB with no registry modifications
- Auto-reconnect with exponential backoff (1s-60s, configurable)
- Window geometry save/restore per session
- Session manager with folder hierarchy, tags, and search
- Command snippet store with categories and auto-enter
- Connection health monitor (uptime, bytes, reconnect stats)
- Automatic session log rotation by file size
- Clipboard history ring buffer (32 entries) with search
- Terminal output trigger engine (pattern → alert/exec/log)
- Multi-input broadcast: type once, send to multiple sessions
- SSH key manager with auto-discovery of `.ppk` and OpenSSH keys
- Terminal scrollback search (Ctrl+Shift+F) with hit navigation
- Environment indicator: colour-coded stripes for prod/staging/dev
- Quick-connect URI parser (Ctrl+K) supporting `ssh://user@host:port`

### Based On
- PuTTY 0.83 (2025-02-08) by Simon Tatham
- Includes ML-KEM post-quantum key exchange support
- All upstream security patches applied

## [0.2.0] - 2026-05-16

### Added - Core Features
- **Tabbed interface** — multiple sessions in one window (Ctrl+T/Ctrl+W)
- **SFTP panel** — split-pane file browser with sorting and navigation
- **Credential store** — AES-256-GCM encrypted password storage
- **Theme engine** — 5 built-in schemes (Solarized, Dracula, Nord, Monokai, Gruvbox)
- **Session import** — import from PuTTY registry and SSH config
- **Keyboard shortcuts** — fully customisable bindings with browser-like defaults

### Added - Quality of Life
- **Desktop notifications** — toast alerts for connection events and triggers
- **Proxy auto-detect** — reads system proxy from registry/environment
- **Transfer progress** — speed, ETA, and progress bar for SFTP transfers
- **Latency tracker** — sparkline graph and quality rating in status bar
- **Command history** — cross-session history with prefix search
- **Status bar** — connection info, latency, and transfer status
- **Idle timeout** — per-session connect/idle timeout with warnings
- **Hostname aliases** — short names for frequently-used servers
