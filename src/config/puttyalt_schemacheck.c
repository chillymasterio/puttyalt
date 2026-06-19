/* puttyalt_schemacheck.c - Validate config values against a schema.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
enum { SCM_STR, SCM_INT, SCM_BOOL, SCM_ENUM };
typedef struct { int type; long min; long max; } ScmRule;
/* Validate a string value against a rule. Returns 0 if valid. */
int scm_validate(const ScmRule *rule, const char *value) {
    if (!rule || !value) return -1;
    switch (rule->type) {
        case SCM_INT: {
            char *end; long v = strtol(value, &end, 10);
            if (*end != 0) return -1;
            if (v < rule->min || v > rule->max) return -1;
            return 0;
        }
        case SCM_BOOL:
            return (!strcmp(value,"true")||!strcmp(value,"false")||
                    !strcmp(value,"0")||!strcmp(value,"1")) ? 0 : -1;
        case SCM_STR: {
            int len = (int)strlen(value);
            if (len < rule->min || len > rule->max) return -1;
            return 0;
        }
        default: return 0;
    }
}
