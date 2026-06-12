/* puttyalt_optparse.c - Command-line option parser (getopt-like). */
#include <string.h>
#include <stdio.h>
#define OP_MAX 32
#define OP_NAME 32
#define OP_VAL 128
typedef struct {
    char name[OP_NAME];
    char value[OP_VAL];
    int present;
    int has_arg;
} op_option;
typedef struct {
    op_option options[OP_MAX];
    int n;
    char positional[OP_MAX][OP_VAL];
    int npos;
} OptParse;
void optparse_init(OptParse *o) {
    if (o) memset(o, 0, sizeof(*o));
}
int optparse_define(OptParse *o, const char *name, int has_arg) {
    if (!o || o->n >= OP_MAX || !name) return -1;
    snprintf(o->options[o->n].name, OP_NAME, "%s", name);
    o->options[o->n].has_arg = has_arg ? 1 : 0;
    return o->n++;
}
int optparse_run(OptParse *o, int argc, const char **argv) {
    if (!o || !argv) return -1;
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];
        if (arg[0] == '-') {
            const char *name = arg[1] == '-' ? arg + 2 : arg + 1;
            for (int j = 0; j < o->n; j++) {
                if (strcmp(o->options[j].name, name) == 0) {
                    o->options[j].present = 1;
                    if (o->options[j].has_arg && i + 1 < argc)
                        snprintf(o->options[j].value, OP_VAL, "%s", argv[++i]);
                    break;
                }
            }
        } else if (o->npos < OP_MAX) {
            snprintf(o->positional[o->npos++], OP_VAL, "%s", arg);
        }
    }
    return 0;
}
int optparse_is_set(const OptParse *o, const char *name) {
    if (!o || !name) return 0;
    for (int i = 0; i < o->n; i++)
        if (strcmp(o->options[i].name, name) == 0) return o->options[i].present;
    return 0;
}
const char *optparse_value(const OptParse *o, const char *name) {
    if (!o || !name) return 0;
    for (int i = 0; i < o->n; i++)
        if (strcmp(o->options[i].name, name) == 0) return o->options[i].value;
    return 0;
}
int optparse_positional_count(const OptParse *o) {
    return o ? o->npos : -1;
}
