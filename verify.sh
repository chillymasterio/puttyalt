#!/bin/bash
#
# verify.sh - Local quality gate for PuttyAlt.
#
# Compiles every library source listed in CMakeLists.txt with the MinGW
# cross-compiler, links the real Windows GUI executable, checks for
# duplicate symbols across the full object set, and runs the unit tests.
#
# Usage:  ./verify.sh
# Exit:   0 on success, non-zero if any stage fails.
#
# Requirements: x86_64-w64-mingw32-gcc (+windres, nm, ar), python3, gcc (tests).

set -u
CC=x86_64-w64-mingw32-gcc
NM=x86_64-w64-mingw32-nm
WINDRES=x86_64-w64-mingw32-windres
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT" || exit 1

OBJ=$(mktemp -d)
trap 'rm -rf "$OBJ"' EXIT
FAIL=0

echo "=== PuttyAlt verify ==="

# Linux-only modules that are not part of the Windows GUI build.
SKIP="puttyalt_linux_main"

# [1] Extract the unique library source list from CMakeLists.txt.
python3 - "$OBJ/srcs.txt" <<'PY'
import re, sys
seen = []
for s in re.findall(r'(src/\S+\.c)', open('CMakeLists.txt').read()):
    if s not in seen:
        seen.append(s)
open(sys.argv[1], 'w').write('\n'.join(seen))
print("  [1] library sources:", len(seen))
PY
[ -s "$OBJ/srcs.txt" ] || { echo "  ERROR: no sources found"; exit 1; }

# [2] Compile every source.
CERR=0
while read -r s; do
    base=$(basename "$s" .c)
    case " $SKIP " in *" $base "*) continue ;; esac
    if ! "$CC" -c -std=c99 -Wall -O2 -DNDEBUG -Iinclude -I. "$s" \
         -o "$OBJ/$(echo "$s" | tr / _ | sed 's/\.c$/.o/')" 2>>"$OBJ/cc.log"; then
        echo "  CERR: $s"; CERR=$((CERR + 1))
    fi
done < "$OBJ/srcs.txt"
WARN=$(grep -c "warning:" "$OBJ/cc.log" 2>/dev/null || echo 0)
echo "  [2] compile errors: $CERR | warnings: $WARN"
[ "$CERR" -eq 0 ] || FAIL=1

# [3] Duplicate-symbol check across the full object set.
python3 - "$OBJ" <<'PY'
import subprocess, glob, os, collections, sys
objdir = sys.argv[1]
m = collections.defaultdict(list)
for o in glob.glob(os.path.join(objdir, "*.o")):
    nm = subprocess.run(["x86_64-w64-mingw32-nm", o], capture_output=True, text=True)
    for line in nm.stdout.splitlines():
        p = line.split()
        if len(p) == 3 and p[1] in ("T", "D", "B"):
            m[p[2]].append(os.path.basename(o))
dups = {s: v for s, v in m.items() if len(v) > 1}
print("  [3] duplicate symbols:", len(dups))
for s, v in list(dups.items())[:10]:
    print("       ", s, v)
sys.exit(1 if dups else 0)
PY
[ $? -eq 0 ] || FAIL=1

# [4] Link the real GUI executable.
"$CC" -c -std=c99 -O2 -DNDEBUG -Iinclude -I. main.c -o "$OBJ/main.o" 2>>"$OBJ/cc.log" \
    || { echo "  [4] main.c failed to compile"; FAIL=1; }
if [ -f puttyalt.rc ]; then
    "$WINDRES" puttyalt.rc "$OBJ/rc.o" 2>>"$OBJ/cc.log" || echo "  [4] windres warning"
fi
if "$CC" "$OBJ"/*.o -o "$OBJ/puttyalt.exe" \
     -mwindows -s -Wl,--gc-sections -Wl,--dynamicbase -Wl,--high-entropy-va -Wl,--nxcompat \
     -lws2_32 -lcomctl32 -lgdi32 -lcomdlg32 -luser32 -lkernel32 -ladvapi32 \
     -lshell32 -lole32 -loleaut32 -luuid -ldwmapi -lcrypt32 -static 2>>"$OBJ/link.log"; then
    echo "  [4] linked GUI exe: $(du -h "$OBJ/puttyalt.exe" | cut -f1)"
else
    echo "  [4] LINK FAILED:"
    grep -iE "undefined|multiple" "$OBJ/link.log" | sed 's/.*: //' | sort -u | head
    FAIL=1
fi

# [5] Unit tests.
if command -v gcc >/dev/null 2>&1; then
    if make -C tests test >"$OBJ/tests.log" 2>&1; then
        echo "  [5] tests: $(grep -c 'passed' "$OBJ/tests.log") suites passed"
    else
        echo "  [5] TESTS FAILED:"; tail -5 "$OBJ/tests.log"; FAIL=1
    fi
    make -C tests clean >/dev/null 2>&1
else
    echo "  [5] tests skipped (no native gcc)"
fi

echo ""
if [ "$FAIL" -eq 0 ]; then
    echo "=== VERIFY OK ==="
else
    echo "=== VERIFY FAILED ==="
fi
exit "$FAIL"
