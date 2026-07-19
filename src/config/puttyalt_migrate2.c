/* puttyalt_migrate2.c - Config version migration steps.
 * Self-contained PuttyAlt module (MinGW/Windows target).
 * Compile: x86_64-w64-mingw32-gcc -c -Wall -std=c99
 */
#include <stddef.h>
typedef struct { int from_version; int to_version; } MigrationStep;
/* Compute number of migration steps needed. */
int mig_steps_needed(int current,int target){ return target>current?target-current:0; }
int mig_next_version(int current,int target){ return current<target?current+1:current; }
int mig_is_current(int version,int target){ return version>=target; }
