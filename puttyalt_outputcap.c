#include "puttyalt_outputcap.h"
#include <string.h>
#include <stdio.h>
#include <time.h>

void outcap_init(OutputCapture *oc)
{
    memset(oc, 0, sizeof(*oc));
    oc->add_timestamps = 1;
    oc->strip_ansi = 1;
    oc->max_size_mb = 100;
    oc->flush_interval = 50;
}

int outcap_start(OutputCapture *oc, const char *path, int append)
{
    if (oc->state == CAP_ACTIVE) return -1;
    FILE *f = fopen(path, append ? "a" : "w");
    if (!f) return -1;
    snprintf(oc->path, sizeof(oc->path), "%s", path);
    oc->file_handle = f;
    oc->state = CAP_ACTIVE;
    oc->append_mode = append;
    oc->start_time = (unsigned long)time(NULL);
    oc->bytes_captured = 0;
    /* write header */
    if (oc->add_timestamps) {
        char ts[64];
        time_t now = time(NULL);
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(f, "--- Capture started: %s ---\n", ts);
    }
    return 0;
}

static int is_ansi_escape(const char *p, int remaining)
{
    if (remaining < 2 || p[0] != '\033') return 0;
    if (p[1] == '[') {
        for (int i = 2; i < remaining; i++) {
            char c = p[i];
            if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) return i + 1;
            if (c != ';' && (c < '0' || c > '9') && c != '?' && c != '=') return 0;
        }
    }
    return 0;
}

int outcap_write(OutputCapture *oc, const char *data, int len)
{
    if (oc->state != CAP_ACTIVE || !oc->file_handle) return -1;
    if (oc->max_size_mb > 0 && oc->bytes_captured > (unsigned long)oc->max_size_mb * 1048576UL)
        return -1;
    FILE *f = (FILE *)oc->file_handle;
    if (oc->strip_ansi) {
        for (int i = 0; i < len; ) {
            int esc = is_ansi_escape(data + i, len - i);
            if (esc > 0) { i += esc; continue; }
            fputc(data[i], f);
            oc->bytes_captured++;
            i++;
        }
    } else {
        fwrite(data, 1, len, f);
        oc->bytes_captured += len;
        oc->lines_since_flush++;
        if (oc->lines_since_flush >= oc->flush_interval) {
            fflush(f);
            oc->lines_since_flush = 0;
        }
    }
    return 0;
}

int outcap_pause(OutputCapture *oc) { if (oc->state == CAP_ACTIVE) { oc->state = CAP_PAUSED; return 0; } return -1; }
int outcap_resume(OutputCapture *oc) { if (oc->state == CAP_PAUSED) { oc->state = CAP_ACTIVE; return 0; } return -1; }

int outcap_stop(OutputCapture *oc)
{
    if (!oc->file_handle) return -1;
    FILE *f = (FILE *)oc->file_handle;
    if (oc->add_timestamps) {
        char ts[64];
        time_t now = time(NULL);
        strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", localtime(&now));
        fprintf(f, "\n--- Capture ended: %s (%lu bytes) ---\n", ts, oc->bytes_captured);
    }
    fclose(f);
    oc->file_handle = NULL;
    oc->state = CAP_IDLE;
    return 0;
}

int outcap_is_active(OutputCapture *oc) { return oc->state == CAP_ACTIVE; }
