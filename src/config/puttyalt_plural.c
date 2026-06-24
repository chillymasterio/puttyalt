/* puttyalt_plural.c - Plural-form selection for i18n.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
/* English plural rule: 0=other, 1=one. */
int pl3_english(long n) { return n == 1 ? 0 : 1; }
/* Russian/Slavic plural rule: 0=one, 1=few, 2=many. */
int pl3_russian(long n) {
    long m10 = n % 10, m100 = n % 100;
    if (m10 == 1 && m100 != 11) return 0;
    if (m10 >= 2 && m10 <= 4 && (m100 < 12 || m100 > 14)) return 1;
    return 2;
}
/* Select a form string from a set given a count and rule index. */
const char *pl3_select(const char *const *forms, int nforms, int form_index) {
    if (!forms || form_index < 0 || form_index >= nforms) return forms && nforms ? forms[0] : "";
    return forms[form_index];
}
