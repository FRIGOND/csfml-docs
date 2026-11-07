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

## A.4 — Tests P1 (lot 4 : buffers en mémoire, audio sans périphérique, Xvfb 2.6)

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 22 | `sfFont_createFromMemory` : le buffer doit rester vivant | ✅ | valgrind : `Invalid read of size 8` (`memmove` ← `sf::MemoryInputStream::read` ← FreeType ← `sf::Font::loadGlyph`) quand on demande un glyphe après `free(buf)` |
| 23 | `sfMusic_createFromMemory` : idem | ✅ | valgrind : `Invalid read of size 1` dans le thread audio (`Music::onGetData` ← libvorbisfile). Sans valgrind le programme « marche » (offset 0.99 s) : comportement indéfini masqué. |
| 33 | Xvfb + `LIBGL_ALWAYS_SOFTWARE=1` : fenêtre + display | ✅ | 3.0.0 et 2.6.1 : `window=<ptr>`, exit 0. Bruit stderr Mesa : `amdgpu_device_initialize failed` (inoffensif). Messages SFML : `Failed to create input context ... TextEntered`, `Setting vertical sync not supported`. |
| 34 | Audio sans périphérique | ⚠️ messages différents du rapport | **3.0.0** (PulseAudio/ALSA rendus inaccessibles) : `miniaudio ERROR: [PulseAudio] Failed to connect PulseAudio context`, `Failed to get audio playback devices: Resource does not exist`, `Failed to initialize sound: No engine available` ; `sfMusic_createFromFile` et `sfSoundBuffer_createFromFile` retournent un pointeur valide, `sfMusic_getStatus` = 2 (Playing), pas de crash, exit 0. Le message « Using NULL audio backend » du rapport n'a **pas** été observé dans cette configuration. **2.6.1** (OpenAL, driver pulse indisponible) : `Failed to open the audio device` (×2 par objet), objets créés, `sfMusic_getStatus` = 0 (Stopped), pas de crash, exit 0. |

**Note (lot 4, 2.6.1)** : `-Wall -Wextra` + `#include <SFML/Graphics.h>` (CSFML 2.6.1 compilée localement) émet `warning: type is deprecated [-Wdeprecated-declarations]` depuis le header (`SFML/Window/Event.h:133`, type `sfMouseWheelEvent`), même sans l'utiliser. Un projet en `-Werror` ne compile donc pas en 2.6.1 sans `-Wno-deprecated-declarations`. À confirmer sur 2.6.0 (paquet Ubuntu 24.04) en phase 2 ; important pour la section Epitech.
Sans `DISPLAY`, 2.6.1 se comporte comme 3.0.0 : message `Failed to open X11 display...` puis abort (exit 134). SFML 3.0.2 ne contient aucune mention de Wayland (`strings libsfml-window.so | grep -ci wayland` = 0) : X11 seulement (via XWayland).

## A.4 — Tests P1 (lot 5 : build statique, SFML_ROOT)

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 7 | Build SFML 3.0.2 sans sudo, partagée | ✅ | fait par `tools/build-csfml.sh` : `libsfml-*.so.3.0.2`, `lib/cmake/SFML`, `lib/pkgconfig/sfml-*.pc` présents |
| 8 | CSFML 3.0.0 trouve SFML via `SFML_ROOT` | ✅ | testé dans le test 11 (`-DSFML_ROOT=<préfixe>`, sans `SFML_DIR`) ; le build principal utilisait `-DSFML_DIR` |
| 11 | SFML 3 statique par défaut ; CSFML ne la trouve pas | ✅ | SFML installée avec `libsfml-*-s.a` (statique par défaut). Configuration de CSFML 3.0.0 : `CMake Error at …/SFMLConfig.cmake:185: Requested SFML configuration (Shared) was not found. Set SFML_STATIC_LIBRARIES to ON for static libraries`, puis `find_package(SFML) … NOT FOUND` (`CMakeLists.txt:69`). Avec `-DCSFML_LINK_SFML_STATICALLY=ON` : configuration réussie (génération OK, build non lancé). |

## A.4 — Tests P2 (lot 1 : comportement de l'API, sans fenêtre)

Sources : `tests/p2/` (`gen_destroy.py`, `p2_misc.c`, `t63.c`, `t82.c`, `t86.c`, `t101.c`). Locale `C.UTF-8`.

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 62 | `sfX_destroy(NULL)` sans effet (3.0.0) | ✅ | les 40 fonctions `*_destroy` appelées avec NULL (une par processus fils) : 0 crash, sur le paquet Ubuntu **et** sur le build local |
| 91 | idem en 2.6.1 | ⚠️ | 42 fonctions `*_destroy` : 41 OK, **`sfRenderTexture_destroy(NULL)` plante (SIGSEGV)** |
| 63 | `assert` actifs dans le paquet Ubuntu | ✅ | `sfClock_getElapsedTime(NULL)` → `./src/CSFML/System/Clock.cpp:59: sfTime sfClock_getElapsedTime(const sfClock*): Assertion \`clock' failed.` exit 134 (le build Release local : pas d'assert) |
| 67 | `sfImage_saveToMemory` remplit un `sfBuffer` de l'appelant | ✅ | `sfBuffer_create()` puis `saveToMemory(img, buf, "png")` → `true`, taille 76 octets (image 2×2) |
| 72 | `sfText_getString` : pointeur interne | ✅ | deux appels successifs renvoient la **même adresse** ; le contenu a changé (`xyz`) ; l'ancienne valeur doit être copiée si on la garde |
| 78 | `sfIpAddress_toString` | ✅ | `sizeof(sfIpAddress.address)` = 16 en 3.0.0 ; `255.255.255.255` correctement écrit (débordement ASan non testé) |
| 79 | `sfIpAddress_fromString("localhost")` | ✅ | `127.0.0.1` ; une chaîne invalide donne une adresse dont `toString` est vide |
| 81 | `U"…"` accepté par `sfText_setUnicodeString` | ✅ | compile avec `-std=c11 -Wall -Wextra` sans avertissement ; `"été"` → 3 points de code |
| 82 | `sfFree` absent des headers parapluie | ✅ | avec seulement `<CSFML/System.h>` : `implicit declaration of function ‘sfFree’` ; avec `<CSFML/System/Alloc.h>` : OK |
| 83 | `sfMouse_getPositionRenderWindow` dans libcsfml-graphics | ✅ | présent dans `libcsfml-graphics.so.3.0.0` (1), absent de `libcsfml-window.so.3.0.0` (0) |
| 86 | Fonction supprimée = erreur avec gcc récent | ✅ | gcc 15 : `error: implicit declaration of function ‘sfSound_setLoop’; did you mean ‘sfSound_setCone’?` ; clang 21 : `error: call to undeclared function ‘sfSound_setLoop’` |
| 87 | `sfClock_restart` renvoie le temps écoulé | ✅ partiel | `restart` renvoie 0.10 s puis le temps repart de 0 (0.10 s mesuré après 100 ms). `sfClock_reset` (pause) : non testé. |
| 101 | `EXIT_FAILURE` sans `#include <stdlib.h>` | ✅ | compile avec seulement `<CSFML/Audio.h>` et `<CSFML/Graphics.h>` (inclusion transitive) : ne pas s'y fier |
| 102 | Pas de `CSFMLConfig.cmake` | ✅ | `dpkg -L libcsfml-dev` : aucun fichier cmake ; le préfixe local ne contient que `lib/cmake/SFML` |
| 110 | Epitech `latest` : CSFML 3.0.0 | ✅ | voir A.2 |
| 111 | `epitest-docker:ubuntu24` : CSFML 2.6.0 | ✅ | voir A.2 (tag existant, `libcsfml-dev 2.6.0-3build1`) |

## A.5 — Référence API générée depuis les headers (libclang)

Outil : `tools/gen_api.py <préfixe> <CSFML|SFML> <version>` → `api/csfml-3.0.0.json` et `api/csfml-2.6.1.json`.
Binding Python : roue `libclang` 18.1.1 (PyPI) extraite dans `~/local/py` (pas de venv/pip ni sudo disponibles) ;
`export PYTHONPATH=$HOME/local/py/libclang-18.1.1.data/platlib`. 0 erreur clang sur les deux versions.

| Version | Fonctions (libclang) | Fonctions (regex indépendante) | Types (entrées / noms uniques) | Constantes | Fonctions avec `\brief` | `CSFML_DEPRECATED` |
|---|---|---|---|---|---|---|
| CSFML 3.0.0 (headers `CSFML/`) | **926** | 926 (✅ identiques) | 258 / 141 | 23 | 880 (46 sans) | 0 |
| CSFML 2.6.1 (headers `SFML/`) | **850** | 850 (✅ identiques) | 249 / — | 21 | 806 | 11 |

Par module (3.0.0) : System 21, Window 103, Graphics 467, Audio 191, Network 144 = 926 → **identique à research/03 §5**.
Par module (2.6.1) : System 26, Window 99, Graphics 470, Audio 119, Network 136 = 850.

**Écarts avec research/03 (tranchés par les headers réels) :**
- Fonctions 3.0.0 : 926, module par module identique ✅.
- Types nommés 3.0.0 : le rapport dit 133/134 ; libclang donne **141 noms uniques** (88 struct, 31 enum, 1 union, 138 typedef ; certains noms existent en `struct` + `typedef`). Écart de 7 à 8 : le rapport ne comptait pas la même chose (à recompter avec la définition exacte « noms de typedef + enums nommés »).
- Constantes : 23 ✅.
- **`\brief` : 916 (agent 07) / 1083 (agent 10)** ne s'opposent pas : `git grep '\brief'` dans `include/` donne **950 au tag 3.0.0** et **1083 sur master** ; au tag 3.0.0, **880 fonctions sur 926 ont un `\brief`** (et 12 blocs `\code`). 916 ne correspond à aucun comptage reproductible ici.
- Les 46 fonctions sans `\brief` en 3.0.0 incluent `sfIntRect_contains/intersects` et les `sfRenderTexture_draw*` : leur résumé devra être écrit à la main (marqué « non vérifié »).
- 2.6.1 : 11 fonctions `CSFML_DEPRECATED`. 3.0.0 : 0 (alors que master en marque 40).

Le JSON contient pour chaque fonction : nom, module, header:ligne, signature exacte, paramètres (type, nom, doc), retour, `\brief` et doc brute, drapeau deprecated ; pour les types : struct/union/enum/typedef avec champs et valeurs d'enum ; les constantes globales et `CSFML_VERSION_*`.

## A.4 — Tests P1 25, 26, 27 (exemples avant/après de research/04 §4)

Détail par extrait : `tests/RESULTATS-migration.md` (programmes générés dans `tests/p1-migration/`, log `tests/logs/migration.log`). Fait par un sous-agent ; résumé relu par l'agent principal, non rejoué.

| N° | Test | Verdict | Constat |
|---|---|---|---|
| 25 | Les 8 AVANT compilent en 2.6.1, les 8 APRÈS en 3.0.0 | ✅ 16/16 | `-std=c11 -Wall -Wextra`. Seul avertissement : `-Wdeprecated-declarations` (`Event.h:133`) sur les AVANT 4.2 à 4.8 en 2.6.1. Contexte ajouté dans les wrappers (déclarations factices, includes) ; extraits non modifiés. |
| 26 | Les AVANT échouent en 3.0.0 (include changé) et inversement | ✅ | 8/8 AVANT en échec en 3.0.0 ; 8/8 APRÈS en échec en 2.6.1. Piège silencieux : `{800,600,32}` et `{0,0,110,110}` ne donnent qu'un `-Wmissing-braces` (visible seulement avec `-Wall`). |
| 27 | Exécution fenêtre+sprite+texte et son sous Xvfb | ✅ | exit 0 (timeout 20). Son : 0 octet « definitely lost ». Fenêtre : 1 646 octets perdus en 8 blocs, toutes les piles dans `libfreetype` (`FT_Glyph_To_Bitmap` via `sf::Font::loadGlyph`) : fuite interne à SFML/FreeType, pas du code utilisateur. |
