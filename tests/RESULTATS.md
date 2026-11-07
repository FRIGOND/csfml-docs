# RESULTATS des tests locaux

Légende : ✅ confirmé · ❌ contredit · ⚠️ partiel / à nuancer. Date : 2026-11-07.
Machine : Ubuntu 26.04.1, gcc 15.2.0, clang 21.1.8, cmake 4.2.3, git 2.53, node 22.22, python 3.14.
Absents : docker (accès refusé), xvfb, pkgconf, python3-clang. Présents : valgrind 3.26.

## A.1 Environnement

| Outil | État |
|---|---|
| gcc / clang / cmake / make / git / node / valgrind | présents |
| docker | ⚠️ installé mais l'utilisateur n'est pas dans le groupe `docker` (accès refusé) |
| xvfb-run / Xvfb | ❌ absent (sudo requis) |
| pkg-config / pkgconf | ❌ absent (sudo requis) |
| libclang (C) | ✅ `libclang-21.so.1` présent ; binding python `clang` absent (utiliser ctypes ou le CLI clang) |

## A.2 — Test P1 n°1 : version CSFML dans les environnements Epitech

Méthode : docker inaccessible, donc lecture directe des couches de l'image depuis l'API du registre
Docker Hub (flux tar, extraction de `/var/lib/dpkg/status` et des `Config.h`). Équivalent à
`dpkg -l` dans le conteneur, mais sur les fichiers de l'image, sans l'exécuter.

| Image | Base | libcsfml-dev | libsfml-dev | Headers CSFML | Verdict |
|---|---|---|---|---|---|
| `epitechcontent/epitest-docker:latest` | Ubuntu 26.04 (libc6 2.43, gcc 15.2) | 3.0.0-1 | 3.0.2+dfsg-2 | `/usr/include/CSFML` (CSFML_VERSION 3.0.0) | ✅ CSFML 3.0.0 confirmé |
| `epitechcontent/epitest-docker:ubuntu24` | Ubuntu 24.04 | **2.6.0-3build1** | 2.6.1+dfsg-2build2 | `/usr/include/SFML` (CSFML_VERSION 2.6.0) | ✅ CSFML **2.6.0** (pas 2.6.1) |

- `latest` contient aussi `pkgconf`, `valgrind`, `cmake`, `make`, gcc 15 et 16, clang 21.
- Fedora : non testé ici (aucune image Fedora d'Epitech identifiée). Fedora reste à 2.6.1 d'après 01-versions.md.

**Conséquences :**
1. Le poids de 3.0.0 est confirmé pour l'environnement actuel Epitech. 2.6 garde un poids égal (décision utilisateur) à cause de Fedora et des anciens projets.
2. **Correction au rapport** : l'ancien environnement Epitech et Ubuntu 24.04 ont la 2.6.**0**, pas la 2.6.1. La doc 2.6 doit signaler les différences 2.6.0 → 2.6.1 (voir research/01 et 10). Voir research/CORRECTIONS.md.

## A.3 — Builds locaux sans sudo

Script : `tools/build-csfml.sh <csfml-tag> <sfml-tag> <prefix>` (SFML puis CSFML, partagées, `CMAKE_INSTALL_LIBDIR=lib`, rpath `$ORIGIN`).

| Prefix | CSFML | SFML | Résultat |
|---|---|---|---|
| `~/csfml-3.0` | 3.0.0 (commit 24c8ddb) | 3.0.2 | ✅ compile et installe du premier coup ; `include/CSFML`, `lib/libcsfml-*.so.3.0.0`, `.pc`, `lib/cmake/SFML` |
| `~/csfml-2.6` | 2.6.1 (commit e153a8a) | 2.6.2 | ✅ compile et installe ; `include/SFML` (headers C en 2.6), `lib/libcsfml-*.so.2.6.1`, `.pc` |

- Recette de research/02 §3.5 validée telle quelle pour 3.0.0 (avec `-DSFML_DIR=` à la place de `SFML_ROOT`).
- Choix : SFML 2.6.2 avec CSFML 2.6.1 (dernière 2.6.x). Ubuntu et l'image `ubuntu24` d'Epitech ont SFML 2.6.1 et CSFML 2.6.0.
- Docs : les deux préfixes ont leurs `.pc` ; sans `pkgconf` installé ils ne sont pas encore testés.

## A.4 — Tests P1 (lot 1 : compilation, liens, symboles, sans fenêtre)

Script : `tests/run-p1-compile.sh` (sources dans `tests/p1/`, sortie complète dans `tests/logs/p1-compile.log`).
Outils locaux sans sudo : `tools/env.sh` (Xvfb et pkgconf extraits de .deb dans `~/local/root`).

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 3 | Paquet système 3.0.0 : programme System compile et tourne | ✅ | affiche `0.10`, exit 0 |
| 4 | `#include <SFML/Graphics.h>` échoue en 3.0.0 | ✅ | `fatal error: SFML/Graphics.h: Aucun fichier ou dossier de ce nom` |
| 5 | Il faut lier chaque lib (`-lcsfml-system` nécessaire) | ✅ | sans lui : `référence indéfinie vers « sfClock_create »` ; avec les 3 : OK |
| 6 | `-l` avant la source échoue | ✅ | `référence indéfinie vers « sfClock_create »` et `« sfMilliseconds »` |
| 9 | Libs locales avec `RUNPATH=$ORIGIN` | ✅ | `libcsfml-graphics.so.3.0.0` et `libsfml-graphics.so.3.0.2` |
| 10 | `-Wl,-rpath` vers préfixe local, sans `LD_LIBRARY_PATH` | ✅ | `ldd` : libs résolues dans `~/csfml-3.0/lib` ; sortie `0.10` |
| 15 | Headers parapluie en `-std=c11 -Wall -Wextra -pedantic` | ✅ | aucun avertissement (3.0.0 système) |
| 16 | `sfBool`/`sfTrue`/`sfInt64` n'existent plus | ✅ | `unknown type name ‘sfBool’`, `‘sfTrue’ undeclared`, `unknown type name ‘sfInt64’` ; `bool`/`int64_t` : OK |
| 29 | `sfVideoMode m = {800, 600, 32};` | ❌ **contredit** | compile avec `-Wmissing-braces` et donne les **bonnes** valeurs (`size=800,600 bpp=32`) grâce à l'élision d'accolades. Le rapport (04 §3) disait « mauvais résultat » : faux. À corriger : c'est un simple avertissement, la forme correcte est `{{800, 600}, 32}`. |
| 30 | Ancien appel à 4 arguments de `sfRenderWindow_create` | ✅ | `too few arguments ... expected 5, have 4` et `incompatible type for argument 4 ... expected ‘sfWindowState’` |
| 31 | NULL → assert en 3.0.0 | ⚠️ **nuancé** | `sfSprite_create(NULL)` : **paquet Ubuntu** → `Assertion \`texture' failed` (`./src/CSFML/Graphics/Sprite.cpp:42`), abort, exit 134 ; **build local Release** (`-DNDEBUG`) → **segfault** (exit 139). `sfSprite_setTexture(NULL, NULL, true)` : aucun effet, exit 0, sans message, en 3.0.0 comme en 2.6.1. En 2.6.1, aucun message « trying to use null » observé. |

Rappel dans les pages : sur un build maison en Release, les `assert` de CSFML sont retirés ; tester le retour de `create*` ne suffit pas pour `sfSprite_create(NULL)`, il faut ne jamais passer NULL.

Le paquet système a bien 1 appel à `__assert_fail` dans `libcsfml-graphics` ; le build local n'en a aucun.

## A.4 — Tests P1 (lot 2 : exemples officiels, symboles, Makefile, pkg-config)

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 12 | Makefile minimal de 02 §4.5 (paquet système puis préfixe local) | ✅ | `make` : libs résolues dans `/usr/lib/x86_64-linux-gnu` ; `make re CSFML_PREFIX=$P3` : résolues dans `~/csfml-3.0/lib` ; programme affiche `0.10` |
| 13, 36 | Exemple `doc/mainpage.hpp` @ 3.0.0 ne compile pas | ✅ | 4 erreurs : `sfRenderWindow_create` « expected 5, have 4 » + « incompatible type for argument 4 », `sfSprite_create` « expected 1, have 0 », `sfText_create` « expected 1, have 0 ». À documenter dans « Erreurs connues de la doc officielle ». |
| 37 | Même test sur `master` | ✅ | `sfText_create` corrigé ; restent `sfRenderWindow_create` et `sfSprite_create` |
| 38 | `examples/example.c` @ 3.0.0 | ✅ | compile sans avertissement (`-Wall -Wextra`) ; lancé sous Xvfb, tourne jusqu'au timeout (exit 124 attendu, boucle interactive). Fermeture propre non testée. Assets : `sfml_logo.png`, `tuffy.ttf`, `doodle_pop.ogg` (dans `tests/p1/`). |
| 14 | Inventaire (926 fonctions, 23 constantes) vs `.so` | ✅ | headers 3.0.0 : 926 fonctions déclarées, **926 exportées** (`nm -D`), 0 manquante ; 960 symboles exportés = 926 + 23 constantes + 11 symboles typeinfo C++ (`_ZTI…`, bruit). Les 109 noms du doc non exportés sont les ajouts de `master` (section « écarts sur master »). |
| pkg-config | `.pc` des préfixes et du paquet | ✅ | `pkgconf 2.5.1` extrait ; `pkg-config --cflags --libs csfml-graphics` OK pour `$P3`, `$P2`, système ; compile + lance `clock.c` via `$(pkg-config --cflags --libs csfml-system)`. Les `.pc` 3.0.0 ajoutent `-I/usr/include/freetype2 -I/usr/include/libpng16`. |

## A.4 — Tests P1 (lot 3 : fenêtre, polices, valgrind sous Xvfb)

Script : `tests/run-p1-window.sh` (log : `tests/logs/p1-window.log`). Bibliothèque : `~/csfml-3.0` (build Release, SFML 3.0.2).

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 17 | `sfRenderWindow_waitEvent(w, timeout, &e)` | ✅ | après avoir vidé la file : `waitEvent(200ms)` → `got=0`, `elapsed≈207 ms` (local) et 206 ms (système). `sfTime_Zero` : attend sans limite (bloqué > 3 s). À la création d'une fenêtre sous Xvfb : 2 événements, type 1 (`sfEvtResized`) et type 3 (`sfEvtFocusGained`). |
| 18 | UTF-8 dans `sfText_setString` | ✅ **confirmé : cassé** | `"été"` (5 octets) → 5 points de code dont `U+FFFFFFFF` ×4 ; `sfText_setUnicodeString` avec `{0xE9,0x74,0xE9,0}` → 3 points de code. Pour du texte accentué : `sfText_setUnicodeString` avec des `sfChar32`. Locale du test : `C.UTF-8`. |
| 19 | Sprite vs texture détruite | ✅ | valgrind : `Invalid read of size 1` après `sfTexture_destroy`, bloc libéré à la ligne du destroy |
| 20 | Text vs font détruite | ✅ | valgrind : `Invalid read of size 8` dans `sfText_getLocalBounds` |
| 21 | Sound vs buffer détruit | ⚠️ | `sfSound_getBuffer(s) == b` vrai (pointeur gardé, pas de copie) ; mais aucune erreur valgrind observée après `sfSoundBuffer_destroy` (lecture 200 ms). Comportement indéfini non démontré ; ne pas affirmer « crash ». |
| 24 | `sfKeyboard_getDescription` | ✅ avec nuance | le header déclare `const char*` (3.0.0) ; la source fait `strdup(...)`. Sans `free` : valgrind `2 bytes in 1 blocks are definitely lost` ; avec `free` : 0 erreur. Écrire `char *d = ...` donne `-Wdiscarded-qualifiers` : il faut `free((void *)d)` ou garder un `const char *` et caster au `free`. |
| 32 | Sans `DISPLAY`, `sfRenderWindow_create` | ✅ | message `Failed to open X11 display; make sure the DISPLAY environment variable is set correctly` puis **abort** (exit 134), pas de retour NULL : paquet système **et** build local |
