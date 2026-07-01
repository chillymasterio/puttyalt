/* puttyalt_csvschema.c - Validate CSV rows against a column schema.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <string.h>
#define CVS_COLS 16
enum { CVS_ANY, CVS_INT, CVS_NONEMPTY };
typedef struct { int types[CVS_COLS]; int ncols; } CsvSchema;
void cvs_init(CsvSchema *s, int ncols) { if (s) { s->ncols = ncols>CVS_COLS?CVS_COLS:ncols; for(int i=0;i<CVS_COLS;i++) s->types[i]=CVS_ANY; } }
void cvs_set_type(CsvSchema *s, int col, int type) { if (s && col>=0 && col<s->ncols) s->types[col]=type; }
static int cvs_is_int(const char *v) { if(!*v)return 0; if(*v=='-')v++; for(;*v;v++) if(*v<'0'||*v>'9')return 0; return 1; }
int cvs_validate_field(const CsvSchema *s, int col, const char *value) {
    if (!s || col<0 || col>=s->ncols || !value) return 0;
    switch (s->types[col]) {
        case CVS_INT: return cvs_is_int(value);
        case CVS_NONEMPTY: return *value != 0;
        default: return 1;
    }
}
