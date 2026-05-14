#ifndef PUTTYALT_CODESIGN_H
#define PUTTYALT_CODESIGN_H

/*
 * Build metadata and trust verification status.
 * All trust establishment is done externally by the user
 * (e.g. right-click > Properties > Unblock, or code signing).
 * This module only provides read-only version/publisher info.
 */

#define CODESIGN_PUBLISHER      "PuttyAlt Project"
#define CODESIGN_URL            "https://github.com/chillymasterio/puttyalt"
#define CODESIGN_FILE_DESC      "PuttyAlt - Enhanced SSH Terminal"
#define CODESIGN_PRODUCT        "PuttyAlt"
#define CODESIGN_COPYRIGHT      "MIT License"
#define CODESIGN_COMPANY        "PuttyAlt Project"

typedef enum {
    CS_TRUST_SIGNED     = 0,
    CS_TRUST_UNSIGNED   = 1,
    CS_TRUST_UNKNOWN    = 5
} CSTrustResult;

/* Read-only queries — portable, no platform APIs */
CSTrustResult codesign_get_trust_status(void);
const char   *codesign_get_publisher(void);
const char   *codesign_get_product(void);

#endif
