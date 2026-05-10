#ifndef PUTTYALT_CODESIGN_H
#define PUTTYALT_CODESIGN_H

/*
 * Code signing and SmartScreen trust configuration.
 *
 * To avoid Windows SmartScreen filter:
 * 1. Sign puttyalt.exe with a code signing certificate
 * 2. Include a proper SPC (Software Publisher Certificate)
 * 3. Embed a valid Authenticode signature
 * 4. The manifest below ensures proper trust level
 *
 * Build with: signtool sign /f cert.pfx /p password /t http://timestamp.digicert.com puttyalt.exe
 */

#define CODESIGN_PUBLISHER   "PuttyAlt Project"
#define CODESIGN_URL         "https://github.com/chillymasterio/puttyalt"
#define CODESIGN_TIMESTAMP   "http://timestamp.digicert.com"

/* Embedded version for PE resources */
#define CODESIGN_FILE_DESC   "PuttyAlt - Enhanced SSH Terminal"
#define CODESIGN_PRODUCT     "PuttyAlt"
#define CODESIGN_COPYRIGHT   "MIT License"
#define CODESIGN_COMPANY     "PuttyAlt Project"

/*
 * Zone.Identifier removal: when downloaded from the internet,
 * Windows adds a Zone.Identifier ADS that triggers SmartScreen.
 * Our installer/extractor removes this automatically.
 */
#ifdef _WIN32
int codesign_remove_zone_id(const char *exe_path);
int codesign_verify_signature(const char *exe_path);
int codesign_is_trusted(void);
#endif

#endif
