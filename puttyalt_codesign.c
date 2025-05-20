#include "puttyalt_codesign.h"
#include <string.h>
#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

int codesign_remove_zone_id(const char *exe_path)
{
    /* Remove the Zone.Identifier alternate data stream
       that causes SmartScreen warnings */
    char ads_path[1024];
    snprintf(ads_path, sizeof(ads_path), "%s:Zone.Identifier", exe_path);
    return DeleteFileA(ads_path) ? 0 : -1;
}

int codesign_verify_signature(const char *exe_path)
{
    /* Check if the executable has a valid Authenticode signature.
       Uses WinVerifyTrust API in production. */
    (void)exe_path;
    /* TODO: WinVerifyTrust integration */
    return 0;
}

int codesign_is_trusted(void)
{
    /* Check if current executable is running from a trusted location
       or has been previously allowed by the user */
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    /* Check for Zone.Identifier — if absent, we're trusted */
    char ads[MAX_PATH + 32];
    snprintf(ads, sizeof(ads), "%s:Zone.Identifier", path);
    HANDLE h = CreateFileA(ads, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE)
        return 1; /* No zone ID — trusted */
    CloseHandle(h);
    return 0;
}

#else
/* Unix: no SmartScreen concept */
#endif
