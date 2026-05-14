#ifndef PUTTYALT_CODESIGN_H
#define PUTTYALT_CODESIGN_H

/*
 * Code signing and trust verification.
 *
 * SmartScreen bypass strategy:
 *
 * 1. AUTHENTICODE SIGNING (recommended, done externally):
 *    Use signtool or PowerShell to sign the exe AFTER building:
 *    signtool sign /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 \
 *      /f cert.pfx /p password puttyalt.exe
 *
 *    Or use scripts/sign-exe.ps1 for PowerShell-based signing.
 *
 * 2. MANUAL UNBLOCK:
 *    Right-click exe -> Properties -> Unblock -> Apply
 *    Or use scripts/unblock-exe.bat
 *
 * 3. WINDOWS MANIFEST:
 *    Proper manifest with supportedOS GUIDs (see res/puttyalt.manifest).
 *
 * Note: This module provides read-only trust status queries.
 * It does NOT modify system state, delete files, or run commands.
 * All trust establishment is done externally by the user.
 */

#define CODESIGN_PUBLISHER      "PuttyAlt Project"
#define CODESIGN_URL            "https://github.com/chillymasterio/puttyalt"
#define CODESIGN_TIMESTAMP_URL  "http://timestamp.digicert.com"

#define CODESIGN_FILE_DESC      "PuttyAlt - Enhanced SSH Terminal"
#define CODESIGN_PRODUCT        "PuttyAlt"
#define CODESIGN_COPYRIGHT      "MIT License"
#define CODESIGN_COMPANY        "PuttyAlt Project"

/* Trust verification result (read-only query) */
typedef enum {
    CS_TRUST_SIGNED     = 0,
    CS_TRUST_UNSIGNED   = 1,
    CS_TRUST_TAMPERED   = 2,
    CS_TRUST_EXPIRED    = 3,
    CS_TRUST_UNTRUSTED  = 4,
    CS_TRUST_UNKNOWN    = 5
} CSTrustResult;

#ifdef _WIN32

/* Read-only queries — these never modify system state */
CSTrustResult codesign_verify_signature(const char *exe_path);
CSTrustResult codesign_verify_self(void);
int  codesign_is_trusted(void);
int  codesign_get_signer_name(const char *exe_path, char *out, int outsz);
int  codesign_get_timestamp(const char *exe_path, char *out, int outsz);

#else
/* Unix stubs */
#define codesign_is_trusted()  1
#endif

#endif
