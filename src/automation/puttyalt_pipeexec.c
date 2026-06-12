/* puttyalt_pipeexec.c - Pipeline executor (stage chaining + data flow). */
#include <string.h>
#include <stdio.h>
#define PE_MAX 16
#define PE_NAME 48
enum pe_state { PE_PENDING = 0, PE_RUNNING = 1, PE_DONE = 2, PE_FAILED = 3 };
typedef struct {
    char name[PE_NAME];
    int state;
    int exit_code;
} pe_stage;
typedef struct {
    pe_stage stages[PE_MAX];
    int n;
    int cursor;
    int stop_on_error;
} PipeExec;
void pipeexec_init(PipeExec *p, int stop_on_error) {
    if (!p) return;
    memset(p, 0, sizeof(*p));
    p->stop_on_error = stop_on_error ? 1 : 0;
}
int pipeexec_add(PipeExec *p, const char *name) {
    if (!p || p->n >= PE_MAX || !name) return -1;
    snprintf(p->stages[p->n].name, PE_NAME, "%s", name);
    return p->n++;
}
int pipeexec_next(PipeExec *p) {
    if (!p || p->cursor >= p->n) return -1;
    if (p->cursor > 0 && p->stop_on_error && p->stages[p->cursor - 1].state == PE_FAILED)
        return -1;
    p->stages[p->cursor].state = PE_RUNNING;
    return p->cursor;
}
int pipeexec_complete(PipeExec *p, int exit_code) {
    if (!p || p->cursor >= p->n) return -1;
    p->stages[p->cursor].state = exit_code == 0 ? PE_DONE : PE_FAILED;
    p->stages[p->cursor].exit_code = exit_code;
    p->cursor++;
    return p->cursor >= p->n ? 1 : 0;
}
int pipeexec_failures(const PipeExec *p) {
    if (!p) return -1;
    int n = 0;
    for (int i = 0; i < p->n; i++)
        if (p->stages[i].state == PE_FAILED) n++;
    return n;
}
int pipeexec_count(const PipeExec *p) {
    return p ? p->n : -1;
}
