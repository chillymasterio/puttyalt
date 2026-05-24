#ifndef PUTTYALT_URLDETECT_H
#define PUTTYALT_URLDETECT_H
#include "puttyalt_termemu.h"

#define URL_MAX_LEN 2048

typedef struct {
    int row, col_start, col_end;
    char url[URL_MAX_LEN];
} DetectedURL;

typedef struct {
    DetectedURL *urls;
    int count;
    int capacity;
    int enabled;
    int underline_urls;
    int click_opens;
} URLDetector;

void urldetect_init(URLDetector *ud);
void urldetect_destroy(URLDetector *ud);
void urldetect_scan_line(URLDetector *ud, TermState *ts, int row);
void urldetect_scan_all(URLDetector *ud, TermState *ts);
DetectedURL *urldetect_at(URLDetector *ud, int row, int col);
void urldetect_clear(URLDetector *ud);

#endif
