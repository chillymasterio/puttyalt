/* puttyalt_conditionchk.c - Condition checker for automation guards. */
#include <string.h>
#include <stdlib.h>
enum cc_op { CC_EQ = 0, CC_NE = 1, CC_GT = 2, CC_LT = 3, CC_CONTAINS = 4, CC_STARTS = 5 };
int conditionchk_eval(const char *left, int op, const char *right) {
    if (!left || !right) return 0;
    switch (op) {
        case CC_EQ: return strcmp(left, right) == 0;
        case CC_NE: return strcmp(left, right) != 0;
        case CC_GT: return atol(left) > atol(right);
        case CC_LT: return atol(left) < atol(right);
        case CC_CONTAINS: return strstr(left, right) != NULL;
        case CC_STARTS: return strncmp(left, right, strlen(right)) == 0;
        default: return 0;
    }
}
int conditionchk_op_from_str(const char *s) {
    if (!s) return -1;
    if (!strcmp(s, "==")) return CC_EQ;
    if (!strcmp(s, "!=")) return CC_NE;
    if (!strcmp(s, ">")) return CC_GT;
    if (!strcmp(s, "<")) return CC_LT;
    if (!strcmp(s, "contains")) return CC_CONTAINS;
    if (!strcmp(s, "startswith")) return CC_STARTS;
    return -1;
}
int conditionchk_and(const int *results, int n) {
    for (int i = 0; i < n; i++)
        if (!results[i]) return 0;
    return 1;
}
int conditionchk_or(const int *results, int n) {
    for (int i = 0; i < n; i++)
        if (results[i]) return 1;
    return 0;
}
