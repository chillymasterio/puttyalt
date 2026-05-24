#ifndef PUTTYALT_OSDETECT_H
#define PUTTYALT_OSDETECT_H

#define OS_UNKNOWN  0
#define OS_LINUX    1
#define OS_MACOS    2
#define OS_BSD      3
#define OS_WINDOWS  4
#define OS_SOLARIS  5

typedef struct {
    int detected_os;
    char os_name[64];
    char kernel[64];
    char shell[32];
    char distro[64];
    int  has_sudo;
    int  has_docker;
    int  has_systemctl;
    char suggested_snippets[8][128];
    int  snippet_count;
} OSDetectResult;

void osdetect_init(OSDetectResult *r);
int  osdetect_from_uname(OSDetectResult *r, const char *uname_output);
int  osdetect_from_prompt(OSDetectResult *r, const char *prompt_text);
void osdetect_suggest_snippets(OSDetectResult *r);
const char *osdetect_os_name(int os);

#endif
