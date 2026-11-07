#!/usr/bin/env bash
# Compile et exécute tous les exemples, pour chaque version présente (3.0 / 2.6).
# Usage : tools/run-all-examples.sh [--valgrind] [--only <motif>] [--version 3.0|2.6]
# Variables : CSFML_PREFIX_3_0, CSFML_PREFIX_2_6 (sinon ~/csfml-3.0, ~/csfml-2.6 ; "system" = paquet).
# Codes : chaque exemple doit se terminer par 0 (EXAMPLE_FRAMES=60, timeout 20 s). Sortie : exit 1 si un échec.
set -u
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
VG=0; ONLY=""; VERS="3.0 2.6"
while [ $# -gt 0 ]; do case $1 in --valgrind) VG=1;; --only) ONLY=$2; shift;; --version) VERS=$2; shift;; esac; shift; done
if command -v xvfb-run >/dev/null; then RUN=(xvfb-run -a); else source tools/env.sh; start_xvfb; RUN=(); fi
export EXAMPLE_ASSETS="$ROOT/examples/assets" LIBGL_ALWAYS_SOFTWARE=1 EXAMPLE_FRAMES=60 LC_ALL=C.UTF-8
mkdir -p tests/logs; LOG=tests/logs/examples.log; : > "$LOG"
pass=0; fail=0; failed=()
for dir in examples/*/*/; do
  [ -f "$dir/Makefile" ] || continue; name=${dir#examples/}; name=${name%/}
  [[ -n $ONLY && $name != *$ONLY* ]] && continue
  for v in $VERS; do
    [ -f "$dir/$v/main.c" ] || continue
    pv=CSFML_PREFIX_${v/./_}; extra=(); [ -n "${!pv:-}" ] && extra=(CSFML_PREFIX="${!pv}")
    { echo "=== $name [$v]"; make -s -C "$dir" CSFML=$v "${extra[@]}" re 2>&1; } >> "$LOG"
    if [ ! -x "$dir/build/$v/example" ]; then fail=$((fail+1)); failed+=("$name[$v]: build"); echo "KO  $name [$v] (build)"; continue; fi
    out=$(mktemp -d); cmd=("${RUN[@]}" timeout 20 "$PWD/$dir/build/$v/example")
    [ $VG = 1 ] && cmd=("${RUN[@]}" timeout 120 valgrind -q --leak-check=full --errors-for-leak-kinds=definite --suppressions="$ROOT/tests/valgrind.supp" --error-exitcode=99 "$PWD/$dir/build/$v/example")
    (cd "$out" && "${cmd[@]}" >> "$ROOT/$LOG" 2>&1); rc=$?; rm -rf "$out"
    if [ $rc = 0 ]; then pass=$((pass+1)); echo "OK  $name [$v]"; else fail=$((fail+1)); failed+=("$name[$v]: exit $rc"); echo "KO  $name [$v] (exit $rc)"; fi
  done
done
echo "--- $pass réussis, $fail échoués"; printf '%s\n' "${failed[@]}"; [ $fail = 0 ]
