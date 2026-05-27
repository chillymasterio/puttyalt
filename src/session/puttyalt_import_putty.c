#include <string.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif
#define MAX_IMPORT 128
typedef struct { char name[128]; char host[256]; int port; char user[64]; int protocol; } ImportedSession;
static ImportedSession g_imported[MAX_IMPORT]; static int g_import_count = 0;
#ifdef _WIN32
int import_putty_sessions(void) {
    HKEY hKey; g_import_count = 0;
    if (RegOpenKeyExA(HKEY_CURRENT_USER, "Software\\SimonTatham\\PuTTY\\Sessions", 0, KEY_READ, &hKey) != ERROR_SUCCESS) return 0;
    char name[256]; DWORD idx=0, namelen=256;
    while (RegEnumKeyExA(hKey, idx++, name, &namelen, NULL, NULL, NULL, NULL) == ERROR_SUCCESS && g_import_count < MAX_IMPORT) {
        HKEY hSess; ImportedSession *s = &g_imported[g_import_count];
        memset(s, 0, sizeof(*s)); snprintf(s->name, 128, "%s", name); s->port = 22;
        if (RegOpenKeyExA(hKey, name, 0, KEY_READ, &hSess) == ERROR_SUCCESS) {
            DWORD sz=256, type; char val[256];
            if (RegQueryValueExA(hSess,"HostName",NULL,&type,(LPBYTE)val,&sz)==ERROR_SUCCESS) snprintf(s->host,256,"%s",val);
            sz=sizeof(DWORD);
            if (RegQueryValueExA(hSess,"PortNumber",NULL,&type,(LPBYTE)&s->port,&sz)!=ERROR_SUCCESS) s->port=22;
            RegCloseKey(hSess);
            if (s->host[0]) g_import_count++;
        }
        namelen = 256;
    }
    RegCloseKey(hKey); return g_import_count;
}
#else
int import_putty_sessions(void) { return 0; }
#endif
int import_get_count(void) { return g_import_count; }
ImportedSession *import_get(int idx) { return (idx>=0&&idx<g_import_count)?&g_imported[idx]:NULL; }
