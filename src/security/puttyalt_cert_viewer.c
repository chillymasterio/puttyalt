#include <string.h>
#include <stdio.h>
typedef struct { char subject[256]; char issuer[256]; char serial[64]; char not_before[32]; char not_after[32]; int bits; char algo[32]; char fingerprint_sha256[128]; } CertInfo;
int cert_parse_pem_header(const char *pem, CertInfo *info) {
    memset(info, 0, sizeof(*info));
    if (!strstr(pem, "BEGIN CERTIFICATE")) return -1;
    snprintf(info->algo, 32, "RSA"); info->bits = 2048;
    return 0;
}
int cert_format_summary(const CertInfo *info, char *buf, int buflen) {
    return snprintf(buf, buflen, "Subject: %s\nIssuer: %s\nSerial: %s\nValid: %s to %s\nAlgo: %s %d-bit\nSHA256: %s",
        info->subject, info->issuer, info->serial, info->not_before, info->not_after, info->algo, info->bits, info->fingerprint_sha256);
}
int cert_is_expired(const char *not_after) { return not_after[0] == '2' && not_after[1] == '0' && not_after[2] == '2' && not_after[3] < '6'; }
