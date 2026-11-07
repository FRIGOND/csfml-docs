#!/usr/bin/env bash
# Tests P1 sans fenêtre (compilation, liens, symboles). Sortie : tests/logs/p1-compile.log
source "$(dirname "$0")/../tools/env.sh"
cd "$(dirname "$0")/p1"; W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
SYS=/usr; L="-lcsfml-graphics -lcsfml-window -lcsfml-system"
t(){ echo; echo "=== $*"; }
t "3  système 3.0.0 : clock.c compile et tourne"; gcc -Wall -Wextra -o $W/c clock.c -lcsfml-system && $W/c; echo "exit=$?"
t "4  <SFML/Graphics.h> avec 3.0.0"; printf '#include <SFML/Graphics.h>\nint main(void){return 0;}\n' > $W/old.c; gcc -c $W/old.c -o $W/old.o 2>&1 | head -3
t "5a lien graphics+window+system"; gcc -o $W/l link.c $L 2>&1|head -3; echo "exit=$?"
t "5b lien sans system"; gcc -o $W/l link.c -lcsfml-graphics -lcsfml-window 2>&1 | head -3
t "6  -l avant la source"; gcc -lcsfml-system clock.c -o $W/bad 2>&1 | head -3
t "9  RUNPATH des libs locales 3.0"; readelf -d $P3/lib/libcsfml-graphics.so.3.0.0 | grep -Ei 'runpath|rpath'; readelf -d $P3/lib/libsfml-graphics.so.3.0.2 | grep -Ei 'runpath|rpath'
t "10 rpath vers préfixe local"; gcc -Wall -Wextra -I$P3/include -o $W/cl clock.c -L$P3/lib -Wl,-rpath,$P3/lib -lcsfml-system && ldd $W/cl | grep sfml; $W/cl
t "15 headers parapluie -std=c11 -pedantic (3.0.0 système)"; gcc -std=c11 -Wall -Wextra -pedantic -c umbrella.c -o $W/u.o 2>&1|head; echo "exit=${PIPESTATUS[0]}"
t "16a sfBool/sfInt64 en 3.0.0"; gcc -c bool.c -o $W/b.o 2>&1 | grep -E 'error' | head -4
t "16b bool/int64_t en 3.0.0"; gcc -Wall -Wextra -c bool_ok.c -o $W/b.o 2>&1|head -3; echo "exit=${PIPESTATUS[0]}"
t "29 sfVideoMode m={800,600,32}"; gcc -Wall -Wextra -o $W/v videomode.c -lcsfml-window 2>&1|head -6; $W/v
t "30 ancien appel create à 4 args"; gcc -c create_old.c -o $W/o.o 2>&1 | grep -E 'error|note' | head -4
t "31a NULL en 3.0.0"; gcc -o $W/n3 null3.c $L && $W/n3; echo "exit=$?"
t "31b NULL en 2.6.1 (préfixe local)"; gcc -I$P2/include -o $W/n26 null26.c -L$P2/lib -Wl,-rpath,$P2/lib $L && $W/n26; echo "exit=$?"
