# Security Policy

## Supported Versions

| Version | Supported          |
|---------|--------------------|
| 0.1.x   | :white_check_mark: |

## Reporting a Vulnerability

**Do not open a public issue for security vulnerabilities.**

Please report security issues via [GitHub Security Advisories](https://github.com/chillymasterio/puttyalt/security/advisories/new).

You can expect:
- Acknowledgement within 48 hours
- A fix timeline within 7 days for critical issues
- Credit in the changelog (unless you prefer anonymity)

## Security Practices

- PuttyAlt is built on PuTTY 0.83, which includes ML-KEM post-quantum key exchange
- All upstream security patches are merged within 48 hours of release
- Builds use `-fstack-protector-strong`, `FORTIFY_SOURCE=2`, full RELRO
- Stored credentials use AES-256-GCM encryption with a master password
- No telemetry, no analytics, no outbound network calls beyond your SSH connection
- Sensitive memory is zeroed with `smemclr()` before freeing
