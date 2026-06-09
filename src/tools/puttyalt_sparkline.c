/* puttyalt_sparkline.c - Sparkline chart renderer (Unicode block chars). */
#include <string.h>
#include <stdio.h>
static const char *SPARK_CHARS[]={"\xe2\x96\x81","\xe2\x96\x82","\xe2\x96\x83","\xe2\x96\x84","\xe2\x96\x85","\xe2\x96\x86","\xe2\x96\x87","\xe2\x96\x88"};
int sparkline_render(const int *values, int n, char *out, int outlen) {
    if(!values||!out||n<=0) return -1;
    int min=values[0], max=values[0];
    for (int i=1;i<n;i++) { if(values[i]<min)min=values[i]; if(values[i]>max)max=values[i]; }
    int range=max-min; if(range==0)range=1;
    int pos=0;
    for (int i=0;i<n && pos<outlen-4;i++) {
        int level=(values[i]-min)*7/range; if(level<0)level=0; if(level>7)level=7;
        pos+=snprintf(out+pos,outlen-pos,"%s",SPARK_CHARS[level]);
    }
    out[pos]=0; return n;
}
int sparkline_render_normalized(const int *values, int n, int max_val, char *out, int outlen) {
    if(!values||!out||n<=0||max_val<=0) return -1;
    int pos=0;
    for (int i=0;i<n && pos<outlen-4;i++) {
        int level=values[i]*7/max_val; if(level<0)level=0; if(level>7)level=7;
        pos+=snprintf(out+pos,outlen-pos,"%s",SPARK_CHARS[level]);
    }
    out[pos]=0; return n;
}
