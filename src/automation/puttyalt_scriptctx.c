/* puttyalt_scriptctx.c - Script execution context (variables + return values). */
#include <string.h>
#include <stdio.h>
#define SC_VARS 32
#define SC_NAME 32
#define SC_VAL 96
typedef struct {
    char name[SC_NAME];
    char value[SC_VAL];
} sc_var;
typedef struct {
    sc_var vars[SC_VARS];
    int n;
    int return_code;
    char return_value[SC_VAL];
} ScriptCtx;
void scriptctx_init(ScriptCtx *s) {
    if (s) memset(s, 0, sizeof(*s));
}
int scriptctx_set(ScriptCtx *s, const char *name, const char *value) {
    if (!s || !name) return -1;
    for (int i = 0; i < s->n; i++) {
        if (strcmp(s->vars[i].name, name) == 0) {
            snprintf(s->vars[i].value, SC_VAL, "%s", value ? value : "");
            return 0;
        }
    }
    if (s->n >= SC_VARS) return -1;
    snprintf(s->vars[s->n].name, SC_NAME, "%s", name);
    snprintf(s->vars[s->n].value, SC_VAL, "%s", value ? value : "");
    s->n++;
    return 0;
}
const char *scriptctx_get(const ScriptCtx *s, const char *name) {
    if (!s || !name) return NULL;
    for (int i = 0; i < s->n; i++) {
        if (strcmp(s->vars[i].name, name) == 0) return s->vars[i].value;
    }
    return NULL;
}
void scriptctx_set_return(ScriptCtx *s, int code, const char *value) {
    if (!s) return;
    s->return_code = code;
    snprintf(s->return_value, SC_VAL, "%s", value ? value : "");
}
int scriptctx_return_code(const ScriptCtx *s) {
    return s ? s->return_code : -1;
}
int scriptctx_count(const ScriptCtx *s) {
    return s ? s->n : -1;
}
