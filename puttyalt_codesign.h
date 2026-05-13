#ifndef PUTTYALT_CODESIGN_H
#define PUTTYALT_CODESIGN_H

/*
 * Code signing, SmartScreen trust, and Windows security integration.
 *
 * SmartScreen bypass strategy (defense-in-depth):
 *
 * 1. AUTHENTICODE SIGNING (primary):
 *    Sign with EV or standard code signing certificate:
 *    signtool sign /fd SHA256 /tr http://timestamp.digicert.com /td SHA256 \
 *      /f cert.pfx /p password puttyalt.exe
 *
 * 2. ZONE IDENTIFIER REMOVAL:
 *    When exe is downloaded, Windows marks it with Zone.Identifier ADS.
 *    We remove this ADS on first run to prevent future SmartScreen checks.
 *
 * 3. WINDOWS MANIFEST:
 *    Proper manifest with supportedOS GUIDs signals a well-behaved app.
 *    See res/puttyalt.manifest.
 *
 * 4. REPUTATION BUILDING:
 *    SmartScreen uses Microsoft reputation service. Signed EXEs from
 *    known publishers with enough downloads gain automatic trust.
 *
 * 5. WINTRUST VERIFICATION:
 *    We can verify our own signature at runtime and alert the user
 *    if the binary has been tampered with.
 */

#define CODESIGN_PUBLISHER      "PuttyAlt Project"
#define CODESIGN_URL            "https://github.com/chillymasterio/puttyalt"
#define CODESIGN_TIMESTAMP_URL  "http://timestamp.digicert.com"
#define CODESIGN_TIMESTAMP_ALT  "http://timestamp.sectigo.com"

#define CODESIGN_FILE_DESC      "PuttyAlt - Enhanced SSH Terminal"
#define CODESIGN_PRODUCT        "PuttyAlt"
#define CODESIGN_COPYRIGHT      "MIT License"
#define CODESIGN_COMPANY        "PuttyAlt Project"

/* SmartScreen trust result */
typedef enum {
    CS_TRUST_SIGNED     = 0,   /* Valid Authenticode signature */
    CS_TRUST_UNSIGNED   = 1,   /* No signature */
    CS_TRUST_TAMPERED   = 2,   /* Signature invalid / binary modified */
    CS_TRUST_EXPIRED    = 3,   /* Certificate expired */
    CS_TRUST_UNTRUSTED  = 4,   /* Self-signed or untrusted CA */
    CS_TRUST_UNKNOWN    = 5    /* Could not determine */
} CSTrustResult;

#ifdef _WIN32

/* Zone.Identifier management */
int  codesign_remove_zone_id(const char *exe_path);
int  codesign_remove_zone_id_self(void);
int  codesign_has_zone_id(const char *path);

/* Authenticode verification via WinVerifyTrust */
CSTrustResult codesign_verify_signature(const char *exe_path);
CSTrustResult codesign_verify_self(void);
int  codesign_is_trusted(void);

/* SmartScreen integration */
int  codesign_smartscreen_check(const char *exe_path);
int  codesign_apply_trust_fixes(void);

/* Certificate info extraction */
int  codesign_get_signer_name(const char *exe_path, char *out, int outsz);
int  codesign_get_timestamp(const char *exe_path, char *out, int outsz);

/* Signing helper: generates a self-signed certificate for testing */
int  codesign_generate_test_cert(const char *pfx_path, const char *password);

/* PowerShell-based signing (no signtool required) */
int  codesign_sign_with_powershell(const char *exe_path, const char *cert_thumbprint);

#else
/* Unix stubs */
#define codesign_remove_zone_id(p)       0
#define codesign_remove_zone_id_self()   0
#define codesign_has_zone_id(p)          0
#define codesign_is_trusted()            1
#define codesign_apply_trust_fixes()     0
#endif

#endif
