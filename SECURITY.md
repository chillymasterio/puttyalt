# Security Policy

## Supported Versions

| Version | Supported          |
| ------- | ------------------ |
| 0.4.x   | :white_check_mark: |
| 0.3.x   | :white_check_mark: |
| 0.2.x   | Security fixes only |
| < 0.2   | :x:                |

## Reporting a Vulnerability

If you discover a security vulnerability in PuttyAlt, please report it
responsibly:

1. **Do not** open a public issue
2. Email: security@puttyalt.dev (or open a private advisory on GitHub)
3. Include: steps to reproduce, impact assessment, affected versions
4. We aim to acknowledge within 48 hours and patch within 7 days

## Scope

PuttyAlt handles SSH connections, credentials, and key material.
We take all of the following seriously:

- Memory safety issues (buffer overflows, use-after-free)
- Credential exposure (plaintext passwords in logs, memory dumps)
- Authentication bypasses
- Key material handling
- Injection vulnerabilities (command injection via hostnames, etc.)
- Timing side-channels in authentication

## Hardening

PuttyAlt builds with:
- `-fstack-protector-strong`
- `-D_FORTIFY_SOURCE=2`
- Full RELRO (`-Wl,-z,relro,-z,now`)
- Position-independent executables (PIE)
