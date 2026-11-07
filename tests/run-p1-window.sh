#!/usr/bin/env bash
# Tests P1 avec fenêtre/polices/audio sous Xvfb. Sortie : tests/logs/p1-window.log
source "$(dirname "$0")/../tools/env.sh"; start_xvfb
cd "$(dirname "$0")/p1"; W=$(mktemp -d); trap 'rm -rf "$W"' EXIT
L="-lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio"
b(){ gcc -g -Wall -Wextra -I$P3/include -o $W/$1 $1.c -L$P3/lib -Wl,-rpath,$P3/lib $L; }
t(){ echo; echo "=== $*"; }
for n in t17_waitevent t18_utf8 t24_kbd t32_nodisplay t19_sprite_uaf t20_text_uaf t21_sound; do b $n || echo "BUILD FAIL $n"; done
t "17 waitEvent / pollEvent";      timeout 20 $W/t17_waitevent 2>&1 | grep -v amdgpu
t "18 UTF-8 dans sfText_setString"; timeout 20 $W/t18_utf8
t "24 sfKeyboard_getDescription sous valgrind"; valgrind --leak-check=full --error-exitcode=9 $W/t24_kbd 2>&1 | grep -E 'description|definitely|ERROR SUMMARY|in use at exit'
t "24b sans free() : fuite ?";  sed 's/free(d);//' t24_kbd.c > $W/t24b.c; gcc -I$P3/include -o $W/t24b $W/t24b.c -L$P3/lib -Wl,-rpath,$P3/lib -lcsfml-window -lcsfml-system; valgrind --leak-check=full $W/t24b 2>&1 | grep -E 'definitely|ERROR SUMMARY'
t "32 DISPLAY vide";            DISPLAY= $W/t32_nodisplay 2>&1 | grep -v amdgpu; echo "exit=${PIPESTATUS[0]}"
t "19 sprite vs texture détruite (valgrind)";  valgrind -q $W/t19_sprite_uaf 2>&1 | grep -E 'sprite|Invalid|Address' | head -5
t "20 text vs font détruite (valgrind)";       valgrind -q $W/t20_text_uaf 2>&1 | grep -E 'sfText|Invalid|Address|bounds' | head -5
t "21 sound vs buffer détruit (valgrind)";     valgrind -q $W/t21_sound 2>&1 | grep -vE 'amdgpu|MESA' | head -8
