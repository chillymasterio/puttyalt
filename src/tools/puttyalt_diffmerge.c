/* puttyalt_diffmerge.c - Three-way text merge with conflict markers. */
#include <string.h>
#include <stdio.h>
int diffmerge_line(const char *base, const char *ours, const char *theirs, char *out, int outlen) {
    if(!out) return -1;
    int ob = base&&ours? strcmp(base,ours)!=0 : (base!=ours);
    int tb = base&&theirs? strcmp(base,theirs)!=0 : (base!=theirs);
    if (!ob && !tb) return snprintf(out,outlen,"%s",base?base:"");
    if (ob && !tb) return snprintf(out,outlen,"%s",ours?ours:"");
    if (!ob && tb) return snprintf(out,outlen,"%s",theirs?theirs:"");
    if (ours&&theirs&&strcmp(ours,theirs)==0) return snprintf(out,outlen,"%s",ours);
    return snprintf(out,outlen,"<<<<<<< ours\n%s\n=======\n%s\n>>>>>>> theirs",ours?ours:"",theirs?theirs:"");
}
int diffmerge_has_conflict(const char *base, const char *ours, const char *theirs) {
    int ob = base&&ours? strcmp(base,ours)!=0 : (base!=ours);
    int tb = base&&theirs? strcmp(base,theirs)!=0 : (base!=theirs);
    if (ob && tb && !(ours&&theirs&&strcmp(ours,theirs)==0)) return 1;
    return 0;
}
