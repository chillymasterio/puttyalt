/* puttyalt_csvparse.c - RFC 4180 CSV row parser (quotes, escapes, embedded commas). */
#include <string.h>
#define CSV_MAX_FIELDS 32
#define CSV_FIELD 128
typedef struct { char fields[CSV_MAX_FIELDS][CSV_FIELD]; int nfields; } CsvRow;
int csvparse_row(const char *line, CsvRow *out) {
    if(!line||!out) return -1;
    out->nfields=0;
    const char *p=line; int fi=0, ci=0; int in_quotes=0;
    while (*p && fi<CSV_MAX_FIELDS) {
        char c=*p;
        if (in_quotes) {
            if (c=='"') { if (p[1]=='"') { if(ci<CSV_FIELD-1) out->fields[fi][ci++]='"'; p++; } else in_quotes=0; }
            else if (ci<CSV_FIELD-1) out->fields[fi][ci++]=c;
        } else {
            if (c=='"') in_quotes=1;
            else if (c==',') { out->fields[fi][ci]=0; fi++; ci=0; }
            else if (c=='\n'||c=='\r') break;
            else if (ci<CSV_FIELD-1) out->fields[fi][ci++]=c;
        }
        p++;
    }
    out->fields[fi][ci]=0; out->nfields=fi+1;
    return out->nfields;
}
const char *csvparse_field(const CsvRow *r, int idx) {
    if(!r||idx<0||idx>=r->nfields) return 0;
    return r->fields[idx];
}
