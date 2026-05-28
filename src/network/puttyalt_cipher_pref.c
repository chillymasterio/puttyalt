#include <string.h>
#include <stdio.h>
#define MAX_CIPHERS 16
typedef struct { char name[64]; int priority; int enabled; } CipherPref;
static CipherPref g_ciphers[MAX_CIPHERS]; static int g_cipher_count = 0;
void cipher_pref_init(void) {
    g_cipher_count = 0;
    const char *defaults[] = {"aes256-gcm@openssh.com","chacha20-poly1305@openssh.com","aes256-ctr","aes128-gcm@openssh.com","aes128-ctr",NULL};
    for (int i=0;defaults[i];i++) { snprintf(g_ciphers[g_cipher_count].name,64,"%s",defaults[i]); g_ciphers[g_cipher_count].priority=i; g_ciphers[g_cipher_count].enabled=1; g_cipher_count++; }
}
int cipher_pref_move_up(int idx) {
    if (idx<=0||idx>=g_cipher_count) return -1;
    CipherPref tmp = g_ciphers[idx]; g_ciphers[idx] = g_ciphers[idx-1]; g_ciphers[idx-1] = tmp; return 0;
}
int cipher_pref_toggle(int idx) { if (idx<0||idx>=g_cipher_count) return -1; g_ciphers[idx].enabled=!g_ciphers[idx].enabled; return 0; }
int cipher_pref_build_list(char *buf, int buflen) {
    int pos = 0;
    for (int i=0;i<g_cipher_count;i++) if (g_ciphers[i].enabled) pos+=snprintf(buf+pos,buflen-pos,"%s%s",pos>0?",":"",g_ciphers[i].name);
    return pos;
}
int cipher_pref_count(void) { return g_cipher_count; }
