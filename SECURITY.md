# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| 1.0.x   | ✅ Active |
| 0.4.x   | ⚠️ Critical fixes only |
| < 0.4   | ❌ Unsupported |

## Reporting a Vulnerability

If you discover a security vulnerability, please report it responsibly:

1. **Do not** create a public GitHub issue
2. Email: security@puttyalt.dev (or open a private advisory on GitHub)
3. Include: description, reproduction steps, affected versions
4. Expected response: within 48 hours

## Security Practices

- All credential storage uses encrypted buffers
- Sensitive data is zeroed after use
- Stack protector and FORTIFY_SOURCE enabled by default
- No telemetry or network calls except user-initiated connections
- Authenticode signing for Windows releases (SmartScreen trust)
- ASLR, DEP, and Control Flow Guard compatible

## Code Signing

Windows releases are signed with an Authenticode certificate.
The manifest includes OS compatibility declarations and
requests `asInvoker` privileges (no admin needed).
