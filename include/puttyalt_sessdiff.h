#ifndef PUTTYALT_SESSDIFF_H
#define PUTTYALT_SESSDIFF_H

#define DIFF_MAX_LINES 4096

typedef enum { DIFF_EQUAL, DIFF_ADD, DIFF_DEL } DiffType;

typedef struct {
    DiffType type;
    int  line_a;
    int  line_b;
    char text[512];
} DiffLine;

typedef struct {
    DiffLine *lines;
    int count;
    int capacity;
    int adds;
    int dels;
    int equals;
} DiffResult;

void sessdiff_init(DiffResult *dr);
void sessdiff_free(DiffResult *dr);
int  sessdiff_compare(DiffResult *dr, const char **a, int a_count, const char **b, int b_count);
DiffLine *sessdiff_get(DiffResult *dr, int index);
void sessdiff_stats(DiffResult *dr, int *adds, int *dels, int *equals);

#endif
