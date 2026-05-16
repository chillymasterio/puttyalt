# PuttyAlt Roadmap

## v1.1.0 (Current) — Terminal Engine & UI Foundation
- [x] Terminal emulation engine with ANSI parser
- [x] Text selection (char, word, line, rectangle)
- [x] Scrollback ring buffer with search
- [x] 256-color palette manager
- [x] Font fallback chain
- [x] URL detection and clickable links
- [x] Right-click context menus
- [x] Quick connect bar
- [x] Session favorites with tags
- [x] Connection health monitoring
- [x] Auto-reconnect with exponential backoff
- [x] Jump host chain manager
- [x] UI abstraction layer
- [x] Dockable panel layout
- [x] Tab manager with pin/move/close
- [x] Broadcast v2 with filters
- [x] Session recording and replay
- [x] Setup wizard
- [x] Configuration validation

## v2.0.0 (Planned) — Modern UI Rewrite
**The big one.** Complete UI rewrite with a modern rendering engine.

### Planned Features
- **GPU-accelerated rendering** — Direct2D/OpenGL backend replacing GDI
- **Custom widget toolkit** — Smooth animations, rounded corners, blur effects
- **Ligature support** — Full font shaping with HarfBuzz
- **True color** — 24-bit RGB in terminal
- **Image protocol** — Inline images via iTerm2/Sixel protocols
- **Split panes** — Fully functional split terminal panes
- **Floating panels** — Undockable panels with snap-to-edge
- **Smooth scrolling** — Pixel-level scroll with momentum
- **Search overlay** — Ctrl+F overlay with regex and highlight
- **Command palette** — VS Code-style Ctrl+Shift+P launcher
- **Minimap** — Scrollback minimap like modern code editors
- **Session thumbnails** — Visual preview of session content in tab bar

### Architecture Changes
- Migrate from Win32 GDI to Direct2D/SDL2 hybrid renderer
- Separate logic layer from rendering (via UILayer abstraction)
- Plugin API v2 with sandboxed execution
- WebSocket-based remote access (optional)

### Timeline
- Q3 2025: Renderer prototype
- Q4 2025: Widget toolkit + split panes
- Q1 2026: Beta release
- Q2 2026: Stable v2.0.0

## v2.1.0 (Future)
- AI-powered command suggestions
- Cloud session sync
- Team collaboration mode
- Mobile companion app
