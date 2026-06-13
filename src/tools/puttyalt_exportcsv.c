/* puttyalt_exportcsv.c - CSV exporter with proper quoting/escaping. */
#include <string.h>
#include <stdio.h>
int exportcsv_field(const char *value, char *out, int outlen) {
    if(!value||!out) return -1;
    int needs_quote = strchr(value,',')||strchr(value,'"')||strchr(value,'\n')||strchr(value,'\r');
    if (!needs_quote) return snprintf(out,outlen,"%s",value);
    int o=0; if(o<outlen-1) out[o++]='"';
    for (const char *p=value;*p && o<outlen-2;p++) { if(*p=='"'){ out[o++]='"'; } if(o<outlen-2) out[o++]=*p; }
    if (o<outlen-1) out[o++]='"';
    out[o]=0; return o;
}
int exportcsv_row(const char **fields, int nfields, char *out, int outlen) {
    if(!fields||!out) return -1;
    int pos=0;
    for (int i=0;i<nfields && pos<outlen-1;i++) {
        if (i>0 && pos<outlen-1) out[pos++]=',';
        pos+=exportcsv_field(fields[i],out+pos,outlen-pos);
    }
    if (pos<outlen-1) out[pos++]='\n';
    out[pos]=0; return pos;
}
