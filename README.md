# PuttyAlt

<div align="center">

### The SSH client you actually want to use.

**PuTTY was built in 1999. Your workflow has changed. Your tools should too.**

![Version](https://img.shields.io/badge/version-3.0.0-58A6FF?style=flat-square)
![License](https://img.shields.io/badge/license-MIT-3FB950?style=flat-square)
![Platform](https://img.shields.io/badge/Windows%20x64-0D1117?style=flat-square&logo=windows&logoColor=white)
![Modules](https://img.shields.io/badge/1120+%20modules-D29922?style=flat-square)
![Tests](https://img.shields.io/badge/38%20tests%20passing-3FB950?style=flat-square)

**[Download v3.0.0](https://github.com/chillymasterio/puttyalt/releases/download/v3.0.0/puttyalt.msi)** | [Changelog](CHANGELOG.md) | [Roadmap](ROADMAP.md)

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
wget https://github.com/chillymasterio/puttyalt/releases/download/v3.0.0/puttyalt.msi

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
| **3.47.0** (in progress) | UTF-8 ellipsize, word-wrap, pad/title/short-number/quote helpers, hash-line separator, cell alignment, 8 modules |
| **3.48.0** (in progress) | ISO 8601 date, month/weekday lookup, duration buckets, interval overlap, cron slot match, session uptime, jittered backoff, 8 modules |
| **3.49.0** (in progress) | Ascii85, wrapped base64, run-length hex, UTF-8 clip / UTF-16 length, form-urlencode, CSI encoder, OSC builder, 8 modules |
| **3.50.0** (in progress) | Hostname validator, port clamp parser, IPv6 zone-id, CIDR contains, TLS ClientHello SNI, URL scheme, query param, SOCKS5 auth pick, 8 modules |
| **3.51.0** (in progress) | Session ranking, label dedup, alias cycle detection, flag-set parser, KV merge, default-int clamp, profile-name sanitizer, snippet key hash, 8 modules |
| **3.52.0** (in progress) | SGR/CPR builders, dirty-row coalescer, scrollbar geometry, bracketed-paste terminator, focus event decoder, OSC title parser, pane label, 8 modules |
| **3.53.0** (in progress) | Retry table, full cron matcher, job signature, phase re-entrance guard, fixed-window rate limit, quiet-period alert dedup, task pause/resume, progress formatter, 8 modules |
| **3.54.0** (in progress) | Constant-time zeroize, password reset FSM, session PIN with lockout, TLS cipher scorer, secret span redactor, credential lease, HMAC key derive, signed-token verify, 8 modules |
| **3.55.0** (in progress) | Bit set, small string map, uint32 ring buffer, ordered set, sliding-window median, named counter registry, top-N floats, longest-run tracker, 8 modules |
| **3.56.0** (in progress) | Checkbox-group, integer slider, color bar, chip bar, notification stack, sidebar hint fade, quick-pick, tab hover preview, 8 modules |
| **3.57.0** (in progress) | Log sampler, logfmt fields, hex dump v4, error context chain, trace spans, Prometheus metric line, log level parser, panic printer, 8 modules |
| **3.58.0** (in progress) | CSV column extractor, INI unquote, TOML dotted key, YAML flat map, JSON string unescape, semver compare, arg-choice matcher, host:port splitter, 8 modules |
| **3.59.0** (in progress) | Clamp, lerp, binary GCD, integer sqrt, hash-combine, CRC-16/X.25, Fletcher-32, small modular exp, 8 modules |
| **3.60.0** (in progress) | Extension extractor, POSIX path normalize, path join, relative path, glob-lite, portable filename sanitizer, temp/backup name builders, 8 modules |
| **3.27.0** (in progress) | Histogram percentiles, rate counters, gauges, stopwatch laps, logfmt, ring log, alert dedup, health checks, moving median, text indent/truncate, URL build, host:port (IPv6), HTTP method classification, bell control, title stack, OSC 52 clipboard, PIN generator, token bucket, session quality scoring, 35 modules |
| **3.26.0** (in progress) | Workflow DAG cycle detection, cron next-fire, task timing, per-step retry, YAML scalar parse, config-change watch, setting diff v2, env merge, command palette v2 (fuzzy), MRU quick-switch, condition chains, string builder, streaming CSV reader, session notes, 15 modules |
| **3.25.0** (in progress) | SHA-256 block transform, HMAC padding, iterated KDF, constant-time compare, nonce replay tracking, password strength v2, secret masking, secret store, credential cache, cert validity window, TLS cipher rating, permission bits, sandbox capability policy, 14 modules |
| **3.24.0** (in progress) | Dirty-rect coalescing, LRU glyph cache, frame pacer, cursor shape (DECSCUSR), Sixel dimension parse, frame animation, blink timing, scroll momentum, wrap log, tab expand, focus ring, minimap, font config, output match, replay buffer, 15 modules |
| **3.23.0** (in progress) | TCP congestion window, port knocking, GeoIP lookup, HTTP cache freshness, Basic/Bearer auth, API rate-limit tracking, decorrelated retry jitter, SSH channel multiplexing, tunnel map, WebSocket masking, webhook payloads, proxy config, escape filter, connection history, 15 modules |
| **3.22.0** (in progress) | Hash map v2 (open addressing), sorted set, range set, bloom filter, union-find, deque v2, priority queue, array linked list, object pool, sliding window, FSM v2, tree model, grapheme width, layered config, action queue, 16 modules |
| **3.21.0** (in progress) | UTF-8/UTF-16 codecs, BOM detection, Latin-1 transliteration, plural rules (EN/Slavic), message catalog, number grouping, HTML entities, text alignment, Punycode/IDNA, Trojan-source bidi detection, keyboard layout, history search, 16 modules |
| **3.20.0** (in progress) | Integer sqrt, primality/factors, extended GCD, modular exp, FNV-1a, MurmurHash3, xorshift PRNG, IPv6 classification, HTTP date, color quantization (truecolor->256/16), grapheme clusters, expression calculator, virtual scroll, HOTP window, 18 modules |
| **3.19.0** (in progress) | Bookmark tree, OpenSSH export, favorites v2, schedule rules, job dependency graph, retry budget, dry-run tracking, config lint, backup rotation, theme parse v2, audit chain hash, latency/jitter stat, search bar v2, panel resize, scrollback index, HSV color picker, pretty bytes, 20 modules |
| **3.18.0** (in progress) | HTTP headers, request line, SOCKS5, DNS name codec, IP classification, MIME types, cell attributes, VT params, alt buffer, state track, guardrail (dangerous-command block), script loop, session key rotation, profile merge, wildcard match, context menu v2, 20 modules |
| **3.17.0** (in progress) | KMP search, Boyer-Moore, LCS, prefix trie, Jaro/Hamming metrics, soundex, replace-all, title/snake case, word count, arg parse v2, token stream, inherit chain, variable scope, session fuzzy-match, breadcrumb, wrap cursor, password policy v2, 21 modules |
| **3.16.0** (in progress) | Rect ops, easing, color mix, vec2, Bresenham line, clamp/remap, box drawing, gradient, circular buffer, int stack, subnet calc, TCP state, retry table, sequence runner, quick connect, tab strip, table format, base58, 22 modules |
| **3.15.0** (in progress) | LZ77, Huffman lengths, delta+zigzag, Adler-32, bit reader/writer, hex codec, chunked decode, Content-Type, query params, TLS version, scroll region, dot path, throttle v2, list view v2, roman numerals, EMA v2, 23 modules |
| **3.14.0** (in progress) | Damage map, selection span, reflow v2, layout grid, flexbox, anchor/tooltip, JSON escape, ascii85, time parse, percentile, fuzzy score, ETag, FTP codes, pipeline v2, watchdog v2, merge config, misleading-indentation cleanup (51->25), 26 modules |
| **3.13.0** (in progress) | SSH banner parse, HTTP status, cookie jar, WebSocket frame, proxy URL, JSON Pointer, TOML value, CSI v2, OSC parse, DEC graphics, SGR mouse, JWT decode, cert subject, auth lockout, expect parser, cron field, macro record, semver, hex view, 28 modules |
| **3.12.0** (in progress) | Stat accumulator, min-heap, LRU cache, fixed-point, moving avg, CRC-16, varint, EDF scheduler, backoff, bandwidth meter, DNS cache v2, IPv6 format, palette 256, blink timer, TOTP window, redact, unit parse, 30 modules |
| **3.11.0** (in progress) | Trim/slugify/wordwrap, Levenshtein ratio, base32, URL encode, RLE, Fletcher-16, CIDR v4, MAC parse, SGR parse, tab stops, char width, password entropy, glob v2, keymap chords, template engine, token-bucket limiter, 31 modules |
| **3.10.0** (in progress) | Ring queue, bit set, base64url, cell diff tracker, keepalive v4 (RTT-adaptive), DoH client, circuit breaker, workflow validation, env expansion, warning cleanup (101->52), 32 modules |
| **3.9.0** (in progress) | JSON patch, text diff (LCS), grid/panel layout, menu bar, DNS cache v2, TLS verification, event bus v2, task dependency graph, retry strategies, warning cleanup (474->101), 32 modules |
| **3.8.0** (in progress) | Base32/TOTP/CRC32, UTF-8 codec, URL parser, ANSI 256-color, text selection v2, list/dropdown/spinner widgets, key store, protocol negotiation, verify.sh warning tracking, 31 modules |
| **3.7.0** (in progress) | i18n + accessibility v2, BiDi, IPv6, function profiler, script debugger, syntax highlighting, Levenshtein, connection state machine, verify.sh quality gate, zero-warning build, 31 modules |
| **3.6.0** (in progress) | Gap buffer editor, regex/fuzzy v2, sparklines, WebSocket client, plugin host, bookmarks v2, dynamic palette, settings store, test hardening (38/38 pass), 31 modules |
| **3.5.0** (in progress) | VT220 conformance, DCS/APC parsers, FIDO2/PKCS#11 hardware auth, SSH tunnels, HTTPS proxy, SCP v2, glob matching, modal dialogs, text reflow, 32 modules |
| **3.4.0** (in progress) | SSH rekey/compression, port forwarding v2, keepalive v3, host key rotation, SSHFP, SOCKS5, mouse v2 (SGR pixel), rectangular selection, powerline status, color wheel, 33 modules |
| **3.3.0** (in progress) | Anomaly detection, command prediction (Markov), session scoring, log clustering, rsync delta sync, load balancer, connection graph, smart copy/safety, JSONPath, cron matcher, 32 modules |
| **3.2.0** (in progress) | Enterprise compliance (PCI/SOC2/HIPAA), RBAC, tamper-evident recording, secret scanner, Sixel v2, DNSSEC, GeoIP, hex editor, three-way merge, state machine, alert rules, 32 modules |
| **3.1.0** (in progress) | Post-quantum KEM (ML-KEM/hybrid), WASM plugin runtime, edge/IoT profiles, Unicode 15.1 emoji, NAT hole-punching, Kitty keyboard protocol, synchronized output, session folder tree, 34 modules |
| **3.0.0** New Era | AI copilot + autonomous agent, zero-trust policy engine, MFA flow, distributed sessions (quorum), GPU compositor + frame graph, SSH multiplexer, CRDT sync, observability (metrics/tracing), 30 flagship modules |
| **2.4.0** | AI assist (prompt context, command explainer, sequence planner), connection multiplexing (mux channels, QUIC flow), cloud profiles, advanced SFTP (resumable, mirror, throttle), CSI parser, OSC 52/9;4 |
| **2.3.0** | Touch gestures, on-screen keyboard, accessibility (screen-reader queue, high-contrast), Mosh/SSH3 protocols, Happy Eyeballs, session MRU, local analytics |
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
Modular.    — 1120+ modules organized by function. Extend what you need.
Private.    — Zero telemetry. No cloud. Your keys stay on your machine.
```

## Windows SmartScreen

First launch may show a SmartScreen warning — normal for unsigned open-source software.

Click **"More info"** → **"Run anyway"**.

## License

MIT — see [LICENSE](LICENSE).

---

<div align="center">

**1120+ modules | 1590+ commits | 120,000+ lines of C99**

Built with care. No bloat. No compromise.

**[Download PuttyAlt](https://github.com/chillymasterio/puttyalt/releases/download/v3.0.0/puttyalt.msi)**

</div>
