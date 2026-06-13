#include "puttyalt_osdetect.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

void osdetect_init(OSDetectResult *r) { memset(r, 0, sizeof(*r)); }

int osdetect_from_uname(OSDetectResult *r, const char *output)
{
    if (!output) return -1;
    if (strstr(output, "Linux")) {
        r->detected_os = OS_LINUX;
        snprintf(r->os_name, sizeof(r->os_name), "Linux");
    } else if (strstr(output, "Darwin")) {
        r->detected_os = OS_MACOS;
        snprintf(r->os_name, sizeof(r->os_name), "macOS");
    } else if (strstr(output, "FreeBSD") || strstr(output, "OpenBSD")) {
        r->detected_os = OS_BSD;
        snprintf(r->os_name, sizeof(r->os_name), "BSD");
    } else if (strstr(output, "CYGWIN") || strstr(output, "MINGW")) {
        r->detected_os = OS_WINDOWS;
        snprintf(r->os_name, sizeof(r->os_name), "Windows");
    } else if (strstr(output, "SunOS")) {
        r->detected_os = OS_SOLARIS;
        snprintf(r->os_name, sizeof(r->os_name), "Solaris");
    }
    /* extract kernel version */
    const char *p = output;
    while (*p && *p != ' ') p++;
    if (*p) {
        p++;
        int i = 0;
        while (*p && *p != ' ' && i < 63) r->kernel[i++] = *p++;
        r->kernel[i] = '\0';
    }
    return r->detected_os;
}

int osdetect_from_prompt(OSDetectResult *r, const char *prompt)
{
    if (!prompt) return -1;
    if (strstr(prompt, "ubuntu") || strstr(prompt, "Ubuntu")) {
        r->detected_os = OS_LINUX;
        snprintf(r->distro, sizeof(r->distro), "Ubuntu");
    } else if (strstr(prompt, "centos") || strstr(prompt, "CentOS")) {
        r->detected_os = OS_LINUX;
        snprintf(r->distro, sizeof(r->distro), "CentOS");
    } else if (strstr(prompt, "debian") || strstr(prompt, "Debian")) {
        r->detected_os = OS_LINUX;
        snprintf(r->distro, sizeof(r->distro), "Debian");
    }
    if (strstr(prompt, "#")) r->has_sudo = 0;
    /* root prompt */
    else if (strstr(prompt, "$")) r->has_sudo = 1;
    return r->detected_os;
}

void osdetect_suggest_snippets(OSDetectResult *r)
{
    r->snippet_count = 0;
    switch (r->detected_os) {
        case OS_LINUX:
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "uname -a");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "cat /etc/os-release");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "df -h");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "free -m");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "systemctl list-units --failed");
            break;
        case OS_MACOS:
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "sw_vers");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "system_profiler SPHardwareDataType");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "df -h");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "top -l1 | head -10");
            break;
        case OS_BSD:
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "uname -a");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "sysctl hw.physmem");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "df -h");
            break;
        default:
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "uname -a");
            snprintf(r->suggested_snippets[r->snippet_count++], 128, "whoami");
            break;
    }
}

const char *osdetect_os_name(int os)
{
    switch (os) {
        case OS_LINUX: return "Linux";
        case OS_MACOS: return "macOS";
        case OS_BSD: return "BSD";
        case OS_WINDOWS: return "Windows";
        case OS_SOLARIS: return "Solaris";
        default: return "Unknown";
    }
}
