/* puttyalt_ruleeval.c - Evaluate simple field-based rules.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#include <stdlib.h>
/* Evaluate "field op value" against a provided field value. */
int rev_eval(const char *field_val,const char *op,const char *rule_val){
    if(!field_val||!op||!rule_val)return -1;
    if(!strcmp(op,"eq"))return strcmp(field_val,rule_val)==0;
    if(!strcmp(op,"ne"))return strcmp(field_val,rule_val)!=0;
    if(!strcmp(op,"contains"))return strstr(field_val,rule_val)!=0;
    if(!strcmp(op,"gt"))return atol(field_val)>atol(rule_val);
    if(!strcmp(op,"lt"))return atol(field_val)<atol(rule_val);
    if(!strcmp(op,"prefix"))return strncmp(field_val,rule_val,strlen(rule_val))==0;
    return -1;
}
