# Security Policy

## Supported Versions

| Version | Supported |
|---------|-----------|
| 1.3.x   | Yes       |
| 1.2.x   | Yes       |
| 1.1.x   | Yes       |
| < 1.1   | No        |

## Reporting a Vulnerability

If you find a security vulnerability in PuttyAlt, please report it responsibly:

1. **Do not** open a public GitHub issue
2. Email: chillymasterio@gmail.com
3. Include: version, steps to reproduce, impact assessment
4. We aim to respond within 48 hours

## Security Design Principles

- **No suspicious Win32 APIs**: PuttyAlt avoids LoadLibrary, ShellExecute, registry access, and other APIs that trigger antivirus false positives
- **No network calls home**: PuttyAlt never contacts external servers unless explicitly configured (auto-update is opt-in)
- **Credential isolation**: Stored credentials use OS keychain (Windows DPAPI / macOS Keychain / Linux secret-service)
- **No eval/exec**: No dynamic code execution from user input
- **Memory safety**: Buffer bounds checking on all string operations
- **ASLR + DEP**: Enabled in all Windows builds

## SmartScreen & Antivirus

PuttyAlt may trigger Windows SmartScreen because the executable is not EV code-signed.
This is a reputation issue, not a security issue. See [README.md](../README.md#windows-smartscreen-notice) for bypass instructions.
