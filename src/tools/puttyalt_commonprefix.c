/* puttyalt_commonprefix.c - Longest common prefix/suffix.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
int lcp_prefix(const char *const *strs, int n){
    if(n<=0)return 0;
    int len=(int)strlen(strs[0]);
    for(int i=1;i<n;i++){ int j=0; while(j<len && strs[i][j] && strs[i][j]==strs[0][j])j++; len=j; }
    return len;
}
int lcp_pair(const char *a, const char *b){ int i=0; while(a[i]&&a[i]==b[i])i++; return i; }
