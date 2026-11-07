# 02 — Installer CSFML et compiler un programme C (état au 2026-11-07)

Livrable de recherche, phase 1. **Aucune commande de ce fichier n'a été exécutée** : toutes sont marquées [À TESTER] et reprises dans `$S/parts/a-tester-01.md`.
Étiquettes : [CONFIRMÉ] / [DÉDUIT] / [INCERTAIN] / [À TESTER]. Les références de fichiers ont la forme `chemin:ligne` suivie du tag git (dépôts SFML/CSFML clonés dans le scratchpad).
Voir aussi `01-versions.md` pour savoir quelle version on obtient selon la méthode.

## En bref

- **Le plus simple** : le paquet de la distribution. `sudo apt install libcsfml-dev` (Debian/Ubuntu), `sudo dnf install CSFML-devel` (Fedora), `sudo pacman -S csfml` (Arch), `brew install csfml` (macOS). **Attention : Fedora et Debian 13 donnent CSFML 2.6.1, Ubuntu 24.04 donne 2.6.0.** [CONFIRMÉ]
- **Sans sudo** : on compile SFML puis CSFML avec CMake dans un préfixe personnel. **CSFML ne télécharge pas SFML** : il faut installer SFML d'abord, puis indiquer à CMake où il se trouve (`SFML_ROOT` ou `CMAKE_PREFIX_PATH`). [CONFIRMÉ]
- **Piège n°1** : SFML 3 se compile **en statique par défaut** (`BUILD_SHARED_LIBS` vaut OFF), alors que CSFML cherche par défaut une SFML **partagée** sous Linux. Il faut donc passer `-DBUILD_SHARED_LIBS=ON` pour SFML. [CONFIRMÉ]
- **Piège n°2** : avec un préfixe local, les `.so` de CSFML ne savent pas où trouver ceux de SFML. Il faut `-DCMAKE_INSTALL_RPATH='$ORIGIN'` à l'installation, ce que fait la CI officielle, ou bien `LD_LIBRARY_PATH`. [CONFIRMÉ pour la CI, DÉDUIT pour le reste]
- **Piège n°3** : CSFML installe toujours ses bibliothèques dans `<préfixe>/lib`, mais met ses `.pc` dans `CMAKE_INSTALL_LIBDIR`, qui peut valoir `lib64` sur Fedora. Il faut forcer `-DCMAKE_INSTALL_LIBDIR=lib`. [CONFIRMÉ (fichiers) + DÉDUIT (conséquence)]
- Dépendances Linux de SFML 3.0 : X11, Xrandr, Xcursor, Xi, udev, OpenGL, freetype, FLAC, vorbis/ogg. **SFML 3.1** ajoute HarfBuzz, mbedTLS et libssh2, et télécharge SheenBidi avec git **dans tous les cas** (il faut Internet). [CONFIRMÉ]
- Pour compiler : `gcc ... main.c -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio`. Les bibliothèques se placent **après** les `.c`/`.o`. Chaque lib CSFML ne dépend que de SFML et pas des autres libs CSFML : il faut lier **chaque** module dont on appelle une fonction. [CONFIRMÉ (nm/objdump) + À TESTER]
- Des fichiers `pkg-config` (`csfml-graphics.pc`, etc., dont `csfml-all.pc`) existent depuis CSFML 2.6.0. Mais le programme `pkg-config` n'est pas installé sur la machine locale. [CONFIRMÉ]

---

## 1. Quelle méthode choisir ?

| Situation | Méthode conseillée | Version obtenue |
|---|---|---|
| Ubuntu 26.04 / image Epitech 2026 | `apt install libcsfml-dev` | 3.0.0 (sur SFML 3.0.2) |
| Debian testing, Arch, macOS (brew) | paquet système | 3.0.0 |
| Fedora, Debian 13, Ubuntu 24.04 et 25.x | paquet système **si on vise la 2.6** ; sinon build depuis les sources | 2.6.x / 3.0.0 |
| Pas de droits sudo (poste partagé, machine de l'école) | build SFML + CSFML dans un préfixe local (§3) | ce qu'on choisit |
| Windows | archives officielles ou MSYS2 (§5) | 3.0.0 (zip), 3.0.0-rc.3 (MSYS2) |

Décision pour la doc : présenter **d'abord le paquet système**, puis le build local comme plan B. Le build est long et demande des en-têtes `-dev` qui, eux, s'installent normalement avec sudo.

---

## 2. Paquets système (noms exacts)

| Système | Commande | Paquet / version au 2026-11-07 | Source |
|---|---|---|---|
| Debian / Ubuntu | `sudo apt install libcsfml-dev` | tire `libcsfml-{system,window,graphics,audio,network}3.0` et `libsfml-dev` (>= 3.0) sur Ubuntu 26.04 | `apt-cache show libcsfml-dev` (machine locale) [CONFIRMÉ] |
| Doc hors ligne (Debian/Ubuntu) | `sudo apt install libcsfml-doc` | installée par `epitech-cpool` | index du PPA Epitech [CONFIRMÉ] |
| Fedora | `sudo dnf install CSFML-devel` | 2.6.1 (F43 à Rawhide), headers dans `/usr/include/SFML/` et `.pc` dans `%{_libdir}/pkgconfig` | [packages.fedoraproject.org](https://packages.fedoraproject.org/pkgs/CSFML/CSFML-devel/), [CSFML.spec](https://src.fedoraproject.org/rpms/CSFML/raw/rawhide/f/CSFML.spec) [CONFIRMÉ] |
| Arch / Manjaro | `sudo pacman -S csfml` | 3.0.0-1 (dépend de `sfml` 3.1.0) | [API Arch](https://archlinux.org/packages/search/json/?name=csfml) [CONFIRMÉ] |
| macOS / Linuxbrew | `brew install csfml` | 3.0.0 (dépend de `sfml` 3.0.2) | [formulae.brew.sh](https://formulae.brew.sh/api/formula/csfml.json) [CONFIRMÉ] |
| MSYS2 (Windows) | `pacman -S mingw-w64-ucrt-x86_64-csfml` | 3.0.0.rc3-2 (en retard sur 3.0.0) | [packages.msys2.org](https://packages.msys2.org/base/mingw-w64-csfml) [CONFIRMÉ] |
| vcpkg | — | aucun port `csfml` | `ports/csfml/vcpkg.json` → HTTP 404 [CONFIRMÉ] |

Sur Ubuntu 26.04, `libcsfml-dev` 3.0.0-1 installe (constaté sur la machine locale) [CONFIRMÉ] :
- les headers dans `/usr/include/CSFML/` (`Graphics.h`, `Window.h`, `System.h`, `Audio.h`, `Network.h`, `OpenGL.h`, `Config.h`, `GPUPreference.h` et les sous-dossiers) ;
- les bibliothèques `/usr/lib/x86_64-linux-gnu/libcsfml-<module>.so` → `.so.3.0` → `.so.3.0.0` ;
- les fichiers `/usr/lib/x86_64-linux-gnu/pkgconfig/csfml-{all,system,window,graphics,audio,network}.pc`.

Pour gcc, aucun `-I` ni `-L` n'est nécessaire avec le paquet système, car ce sont des chemins standard. [DÉDUIT, À TESTER]

---

## 3. Compiler depuis les sources, sans sudo

### 3.1 Prérequis

| Outil | CSFML 3.0.0 | CSFML master | SFML 3.0.x | SFML 3.1.0 | CSFML 2.6.1 / SFML 2.6.2 |
|---|---|---|---|---|---|
| CMake minimum | 3.22 (`CMakeLists.txt:1`) | 3.28 | 3.22 sous Linux, 3.24 sous Windows (`CMakeLists.txt:1-9`, tag 3.0.2) | 3.28 (`CMakeLists.txt:1`) | 3.7.2 |
| Compilateur | C11 + C++17 (`cmake/Macros.cmake:23`) | idem | C++17 | C++17 | C++ (C++03/11) |
| git + Internet | non | non | seulement si `SFML_USE_SYSTEM_DEPS=OFF` | **oui, toujours** (SheenBidi est téléchargé par git) | non |

Sources : les fichiers cités, lus avec `git show <tag>:<fichier>`. [CONFIRMÉ]
Machine locale : CMake 4.2.3, gcc/g++ 15.2.0, GNU ld 2.46, git présents [CONFIRMÉ]. **CMake 4.x refuse les projets qui demandent une version < 3.5** ([notes de version CMake 4.0](https://cmake.org/cmake/help/latest/release/4.0.html)). CSFML 2.5 et 2.5.1 (`cmake_minimum_required(VERSION 2.8.3)`) ne se configurent donc plus sans `-DCMAKE_POLICY_VERSION_MINIMUM=3.5`. Ce n'est pas le cas de 2.5.2, 2.6.x ni 3.0.0. [CONFIRMÉ (fichiers) + DÉDUIT]

### 3.2 Dépendances Linux pour construire SFML 3

Sous Linux, SFML 3 utilise **par défaut les bibliothèques du système** (`SFML_USE_SYSTEM_DEPS` vaut ON : `CMakeLists.txt:110-116`, tag 3.0.2 ; `:98-104`, tag 3.1.0). Avec `-DSFML_USE_SYSTEM_DEPS=OFF`, SFML télécharge et compile lui-même (FetchContent) : **freetype, ogg, flac, vorbis** (3.0.2, `src/SFML/Audio/CMakeLists.txt:101-118`, `src/SFML/Graphics/CMakeLists.txt:130`), plus **HarfBuzz, mbedTLS, libssh2** en 3.1.0 (`src/SFML/Graphics/CMakeLists.txt:142,159`, `src/SFML/Network/CMakeLists.txt:75,99`). [CONFIRMÉ]
**Ce qui n'est jamais embarqué** : X11 (+ Xrandr, Xcursor, Xi), OpenGL, udev et pthread (`src/SFML/Window/CMakeLists.txt:279` X11, `:311` OpenGL, `:324` UDev, tag 3.0.2). Ces en-têtes doivent déjà être sur la machine. EGL n'est demandé que pour Android (`:307`). [CONFIRMÉ]
Le tutoriel officiel donne la même liste ([build-from-source 3.0](https://www.sfml-dev.org/tutorials/3.0/getting-started/build-from-source/), [3.1](https://www.sfml-dev.org/tutorials/3.1/getting-started/build-from-source/)). [CONFIRMÉ]

| Besoin | Debian/Ubuntu (apt) | Fedora (dnf) | Arch (pacman) |
|---|---|---|---|
| X11, Xrandr, Xcursor, Xi | `libx11-dev libxrandr-dev libxcursor-dev libxi-dev` | `'pkgconfig(x11)' 'pkgconfig(xrandr)' 'pkgconfig(xcursor)' 'pkgconfig(xi)'` | `libx11 libxrandr libxcursor libxi` |
| udev | `libudev-dev` | `'pkgconfig(libudev)'` (systemd-devel) | `systemd-libs` |
| OpenGL | `libgl1-mesa-dev` (CI) ou `libgl-dev` | `'pkgconfig(gl)'` | `mesa` |
| freetype | `libfreetype-dev` | `'pkgconfig(freetype2)'` | `freetype2` |
| FLAC, ogg/vorbis | `libflac-dev libvorbis-dev` | `'pkgconfig(flac)' 'pkgconfig(vorbis)'` | `flac libvorbis libogg` |
| SFML 3.1 en plus | `libharfbuzz-dev libmbedtls-dev libssh2-1-dev` | `'pkgconfig(harfbuzz)' mbedtls-devel libssh2-devel` | `harfbuzz mbedtls libssh2` |

Fiabilité du tableau :
- La colonne apt est **[CONFIRMÉ]** : c'est la liste de la CI. CSFML 3.0.0 `.github/workflows/ci.yml:48` ; CSFML master `ci.yml:50` (ajoute harfbuzz, mbedtls, ssh2) ; SFML 3.0.2 `ci.yml:129` ; SFML 3.1.0 `.github/workflows/build.yml:127`.
- La colonne Fedora est **[DÉDUIT]** : la forme `pkgconfig(...)` est celle du `SFML.spec` de Fedora (2.6.2), mais les noms exacts n'ont pas été vérifiés pour 3.1 (mbedtls, ssh2).
- La colonne Arch est **[DÉDUIT]** des dépendances du paquet `sfml` 3.1.0-2 (API Arch).

Machine locale (Ubuntu 26.04) : `libx11-dev`, `libxrandr-dev`, `libxcursor-dev`, `libxi-dev`, `libudev-dev`, `libflac-dev`, `libvorbis-dev`, `libfreetype-dev`, `libgl-dev` et `libopenal-dev` sont **déjà installés**. `libharfbuzz-dev`, `libmbedtls-dev` et `libssh2-1-dev` **ne le sont pas** (dpkg-query) [CONFIRMÉ]. SFML 3.0.2 devrait donc se compiler ici sans sudo ; SFML 3.1.0 demanderait `-DSFML_USE_SYSTEM_DEPS=OFF`. [DÉDUIT, À TESTER]

### 3.3 Options CMake de SFML (lues dans le code, aucune inventée)

| Option | Défaut | Où (tag 3.0.2 sauf mention) | Commentaire |
|---|---|---|---|
| `BUILD_SHARED_LIBS` | **OFF (statique)** | pas de `sfml_set_option`, seulement forcée à ON pour Android (`CMakeLists.txt:44-47`) ; le tutoriel 3.0 dit « Defaults to OFF » | **différent de SFML 2.6.2**, où le défaut était TRUE (`CMakeLists.txt:48`, tag 2.6.2) |
| `CMAKE_BUILD_TYPE` | Release | `CMakeLists.txt:28` | |
| `SFML_BUILD_WINDOW` / `GRAPHICS` / `AUDIO` / `NETWORK` | ON | `CMakeLists.txt:50-53` | GRAPHICS force WINDOW |
| `SFML_USE_SYSTEM_DEPS` | ON sous Linux, OFF sous Windows/macOS | `CMakeLists.txt:110-116` | voir §3.2 |
| `SFML_INSTALL_PKGCONFIG_FILES` | ON sous Linux/BSD | `CMakeLists.txt:205-211` | installe `sfml-*.pc` |
| `SFML_PKGCONFIG_INSTALL_DIR` | `<libdir>/pkgconfig` | `CMakeLists.txt:225` | |
| `SFML_BUILD_EXAMPLES` / `SFML_BUILD_TEST_SUITE` / `SFML_BUILD_DOC` | OFF | `CMakeLists.txt:419`, `:425`, `:413` | |
| `SFML_ENABLE_PCH` | OFF | `CMakeLists.txt:238` | statique seulement |
| `SFML_USE_STATIC_STD_LIBS` | OFF (Windows) | `CMakeLists.txt:139` | |
| `SFML_OPENGL_ES`, `SFML_USE_DRM` | OFF | `CMakeLists.txt:57`, `:61` | cas spéciaux |
| `SFML_FATAL_OPENGL_ERRORS` | OFF | `CMakeLists.txt:407` (**tag 3.1.0 seulement**) | |

Le fichier de config CMake de SFML s'installe dans `${CMAKE_INSTALL_LIBDIR}/cmake/SFML` (`cmake/Macros.cmake:451`). Par défaut, il ne cherche **que la variante partagée** : `SFML_STATIC_LIBRARIES` OFF donne `config_name "Shared"` (`cmake/SFMLConfig.cmake.in:120-129`, tag 3.0.2). [CONFIRMÉ]

### 3.4 Options CMake de CSFML

| Option | Défaut | 3.0.0 (`CMakeLists.txt`) | 2.6.1 (`CMakeLists.txt`) | Rôle |
|---|---|---|---|---|
| `CMAKE_BUILD_TYPE` | Release | :13 | :13 | |
| `CSFML_BUILD_WINDOW` / `GRAPHICS` / `AUDIO` / `NETWORK` | ON | :27-30 | :27-30 (TRUE) | modules à compiler |
| `BUILD_SHARED_LIBS` | ON | :33 | :33 (TRUE) | CSFML partagée ou statique |
| `CSFML_LINK_SFML_STATICALLY` | OFF sous Linux/macOS, ON sous Windows | :40-45 | :40-45 | **doit correspondre** à la SFML installée (readme 3.0.0) |
| `CSFML_USE_STATIC_STD_LIBS` | OFF (Windows) | :48-50 | `STATIC_STD_LIBS` :52 | |
| `CSFML_BUILD_DOC` | OFF | :36 | :36 | Doxygen requis |
| `CSFML_INSTALL_PKGCONFIG_FILES` | ON sous Linux/BSD | :78-84 | :73-79 | installe les `csfml-*.pc` |
| `CSFML_PKGCONFIG_INSTALL_DIR` | `${CMAKE_INSTALL_LIBDIR}/pkgconfig` | :87 (+ `cmake/Config.cmake:17`) | :82 | |
| `CSFML_BUILD_EXAMPLES` | OFF | :112 | :102 | |
| `CSFML_BUILD_TEST_SUITE` | OFF | :118 | — | télécharge Catch2 (FetchContent) |
| `CSFML_WARNINGS_AS_ERRORS` | OFF | `cmake/CompilerWarnings.cmake:7` | `WARNINGS_AS_ERRORS` (même fichier) | |
| `CLANG_FORMAT_EXECUTABLE`, `CLANG_TIDY_EXECUTABLE` | — | :124, :129 | — | pour les contributeurs |

Comment CSFML trouve SFML [CONFIRMÉ] :
- 3.0.0 : `find_package(SFML 3.0 COMPONENTS ${SFML_MODULES} REQUIRED)` (`CMakeLists.txt:69`) ; 2.6.1 : `find_package(SFML 2.6 ...)` (`src/SFML/CMakeLists.txt:37`).
- **Aucun FetchContent de SFML**, ni en 3.0.0, ni en 2.6.1, ni sur master.
- Le readme 3.0.0 (*Building*) conseille `SFML_ROOT`, et la CI l'utilise (`ci.yml:78`, `-DSFML_ROOT=SFML/install`). `SFML_ROOT` marche parce que CMake applique la politique CMP0074 aux projets qui demandent CMake ≥ 3.12, ce qui est le cas de 3.0.0 (3.22). [DÉDUIT]
- **CSFML 2.6.1 déclare 3.7.2** : CMP0074 n'y est pas active et `SFML_ROOT` risque d'être ignoré. Il vaut mieux utiliser `-DSFML_DIR=<préfixe>/lib/cmake/SFML` ou `-DCMAKE_PREFIX_PATH=<préfixe>`. Cette dernière méthode est celle recommandée dans `SFMLConfig.cmake.in` (tag 3.0.2, commentaire des lignes 40-41). [DÉDUIT, À TESTER]
- CSFML **n'installe pas de fichier de config CMake** (`CSFMLConfig.cmake`) : aucun `install(EXPORT ...)` au tag 3.0.0. C'est une demande ouverte, l'issue [#395](https://github.com/SFML/CSFML/issues/395). [CONFIRMÉ]

Bizarrerie à connaître (**piège n°3**) [CONFIRMÉ (fichiers), DÉDUIT (effet)] :
- CSFML installe ses bibliothèques dans `lib${LIB_SUFFIX}` (`cmake/Macros.cmake:83-86`, tags 2.6.1, 3.0.0 et master), donc `<préfixe>/lib`.
- Mais il génère ses `.pc` avec `libdir=${exec_prefix}/@CMAKE_INSTALL_LIBDIR@` et les installe dans `${CMAKE_INSTALL_LIBDIR}/pkgconfig`.
- Or [GNUInstallDirs](https://cmake.org/cmake/help/latest/module/GNUInstallDirs.html) choisit `lib` ou `lib64` selon la distribution. Sur une distribution « lib64 » (Fedora, openSUSE), les `.pc` pointeraient donc vers un mauvais dossier.
- La PR qui corrige ça, [#398 « Remove LIB_SUFFIX reference »](https://github.com/SFML/CSFML/pull/398), est **toujours ouverte**. Fedora applique un patch tiré de cette PR (`CSFML-2.6.1-Use_GNUInstallDirs.patch` dans le spec).
- **Parade** : passer `-DCMAKE_INSTALL_LIBDIR=lib` à SFML et à CSFML.

Le preset `dev` (`CMakePresets.json`, tag 3.0.0) active les exemples, les tests et `CSFML_WARNINGS_AS_ERRORS`. Il sert aux contributeurs, pas à une simple installation. [CONFIRMÉ]

### 3.5 Recette : SFML 3.0.2 + CSFML 3.0.0 dans `$HOME/.local/csfml` [À TESTER]

Choix : SFML **3.0.2**, la version exacte de la CI de CSFML 3.0.0 (`ci.yml:10`) et d'Ubuntu 26.04. Préfixe dédié : on désinstalle avec `rm -rf`.

```sh
# Dossier d'installation (aucun sudo)
PREFIX="$HOME/.local/csfml"
mkdir -p "$PREFIX" ~/src && cd ~/src

# 1) SFML (C++) — en bibliothèques PARTAGÉES
git clone --depth 1 --branch 3.0.2 https://github.com/SFML/SFML.git
cmake -S SFML -B SFML/build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL_PREFIX="$PREFIX" \
  -DCMAKE_INSTALL_LIBDIR=lib \
  -DCMAKE_INSTALL_RPATH='$ORIGIN'
cmake --build SFML/build -j"$(nproc)"
cmake --install SFML/build

# 2) CSFML (C) — trouve SFML via SFML_ROOT
git clone --depth 1 --branch 3.0.0 https://github.com/SFML/CSFML.git
cmake -S CSFML -B CSFML/build \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL_PREFIX="$PREFIX" \
  -DCMAKE_INSTALL_LIBDIR=lib \
  -DCMAKE_INSTALL_RPATH='$ORIGIN' \
  -DSFML_ROOT="$PREFIX"
cmake --build CSFML/build -j"$(nproc)"
cmake --install CSFML/build
```

Résultat attendu : `$PREFIX/include/CSFML/*.h`, `$PREFIX/include/SFML/*.hpp`, `$PREFIX/lib/libcsfml-*.so*`, `$PREFIX/lib/libsfml-*.so*`, `$PREFIX/lib/pkgconfig/{csfml,sfml}-*.pc` et `$PREFIX/lib/cmake/SFML/`. [DÉDUIT]

D'où viennent ces options :
- `'$ORIGIN'` entre apostrophes simples : le shell ne doit pas remplacer `$ORIGIN`. Le script officiel `tools/nuget/build.linux.sh` (tag 3.0.0, lignes 105-142) fait pareil (`'-DCMAKE_INSTALL_RPATH=$ORIGIN'`) et ajoute `LDFLAGS="-z origin"` et `-DCMAKE_BUILD_WITH_INSTALL_RPATH=ON`. La CI de master l'applique aussi au build de SFML (`ci.yml:67`, avec le commentaire « Let the installed SFML libraries find each other »). [CONFIRMÉ]
- Variante sans paquets `-dev` pour freetype, flac et vorbis : ajouter `-DSFML_USE_SYSTEM_DEPS=OFF` à l'étape 1. Il faut alors Internet. [CONFIRMÉ (option), À TESTER (résultat)]
- Si on oublie `-DBUILD_SHARED_LIBS=ON` à l'étape 1, SFML s'installe en statique et l'étape 2 échoue au `find_package`, car CSFML cherche les cibles « Shared ». Autre solution : passer `-DCSFML_LINK_SFML_STATICALLY=ON`, comme le fait la matrice « Static » de la CI (`ci.yml:33-34`, tag 3.0.0). [DÉDUIT, À TESTER]

### 3.6 Variantes

- **SFML 3.1.0 + CSFML 3.0.0** : la même recette avec `--branch 3.1.0`. Il faut CMake ≥ 3.28, Internet (SheenBidi) et soit les paquets harfbuzz, mbedtls et ssh2, soit `-DSFML_USE_SYSTEM_DEPS=OFF`. Debian l'a fait avec succès (voir `01-versions.md` §3). [CONFIRMÉ (faisable) + À TESTER (ici)]
- **SFML 3.1.0 + CSFML master** (future 3.1) : `git clone https://github.com/SFML/CSFML.git` sans `--branch`. **Réservé aux contributeurs** : rien n'est publié et l'API peut encore changer. [DÉDUIT]
- **SFML 2.6.2 + CSFML 2.6.1** (pour coller à Fedora, Debian 13 ou un vieux projet) : SFML 2.6 demande en plus **OpenAL** (`find_package(OpenAL REQUIRED)`, `src/SFML/Audio/CMakeLists.txt:71`, tag 2.6.2). Son `BUILD_SHARED_LIBS` vaut TRUE par défaut. Pour CSFML 2.6.1, utiliser `-DSFML_DIR="$PREFIX/lib/cmake/SFML"` plutôt que `SFML_ROOT`. Prendre un **autre préfixe** que la 3.0 : les deux versions installent `include/SFML/` et entreraient en conflit. [CONFIRMÉ (fichiers) + DÉDUIT + À TESTER]

### 3.7 Autres pièges du build

- `cmake --install` sans droits dans `/usr/local` échoue (c'est le préfixe par défaut). Il faut toujours donner `CMAKE_INSTALL_PREFIX`. [DÉDUIT]
- CSFML 2.6.1 met `CMAKE_SKIP_BUILD_RPATH TRUE` (`CMakeLists.txt:48`) : les exemples compilés dans le dossier de build ne trouvent pas leurs `.so`. Cela ne gêne pas l'installation. [CONFIRMÉ (ligne), DÉDUIT (effet)]
- Build de SFML 3.1 hors ligne : impossible sans pré-télécharger SheenBidi (`src/SFML/Graphics/CMakeLists.txt:238-251`, tag 3.1.0, commentaire : pas de paquet système pour SheenBidi). [CONFIRMÉ]

---

## 4. Compiler et lier un programme C avec gcc

### 4.1 Quelle bibliothèque contient quelle fonction

Constaté avec `nm -D --defined-only` sur les `.so` 3.0.0 de la machine locale [CONFIRMÉ] :

| Fonction | Bibliothèque |
|---|---|
| `sfRenderWindow_create`, `sfRenderWindow_pollEvent`, `sfTexture_createFromFile`, `sfSprite_create`, `sfMouse_getPositionRenderWindow` | `-lcsfml-graphics` |
| `sfWindow_create`, `sfKeyboard_isKeyPressed` | `-lcsfml-window` |
| `sfClock_create`, `sfSleep`, `sfSeconds` | `-lcsfml-system` |
| `sfMusic_createFromFile` | `-lcsfml-audio` |
| `sfTcpSocket_create` | `-lcsfml-network` |

Dépendances (`objdump -p`, champ `NEEDED`) [CONFIRMÉ] :
- `libcsfml-graphics.so.3.0.0` → `libsfml-graphics/window/system.so.3.0` + libstdc++ ;
- `libcsfml-window` → `libsfml-window/system` ;
- `libcsfml-audio` → `libsfml-audio` ;
- `libcsfml-network` → `libsfml-network/system` ;
- `libcsfml-system` → `libsfml-system`.

**Aucune lib CSFML ne dépend d'une autre lib CSFML.** Un programme qui appelle `sfClock_create` doit donc lier `-lcsfml-system` lui-même, même s'il lie déjà `-lcsfml-graphics`. [DÉDUIT, À TESTER : l'oubli doit donner « undefined reference to `sfClock_create' »]

### 4.2 Ordre des options

- gcc d'Ubuntu passe `--as-needed` à l'éditeur de liens (constaté dans `gcc -dumpspecs` sur la machine locale [CONFIRMÉ]). Les `-l...` doivent donc venir **après** les fichiers `.c`/`.o`, sinon on obtient « undefined reference ». [DÉDUIT, À TESTER]
- Entre les libs CSFML partagées, l'ordre n'a pas d'importance, puisqu'elles ne dépendent pas les unes des autres. Par habitude, on met la plus haute d'abord : `-lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lcsfml-network`. Cet ordre est celui qui compte pour des libs **statiques**. [DÉDUIT]
- En C, pas besoin de `-lstdc++` : les `.so` CSFML apportent libstdc++ dans leurs `NEEDED`. [DÉDUIT, À TESTER]

### 4.3 Avec un préfixe local

```sh
# [À TESTER]
gcc -Wall -Wextra -I"$PREFIX/include" -o my_app main.c \
    -L"$PREFIX/lib" -Wl,-rpath,"$PREFIX/lib" \
    -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio
```

- `-I` sert à la compilation (trouver `CSFML/Graphics.h`), `-L` à l'édition de liens, `-Wl,-rpath` à l'exécution (le programme sait où chercher `libcsfml-*.so`).
- **Rpath et dépendances indirectes** : ld d'Ubuntu écrit par défaut un `DT_RUNPATH` (« new dtags »), qui ne s'applique qu'aux dépendances **directes** du programme. Pour que `libcsfml-graphics.so` trouve `libsfml-graphics.so.3.0` au lancement, il faut donc soit le `RUNPATH=$ORIGIN` posé sur les libs au build (§3.5), soit `LD_LIBRARY_PATH`. [DÉDUIT, À TESTER avec `readelf -d` et `ldd`]
- Solution de secours, sans rpath : `LD_LIBRARY_PATH="$PREFIX/lib" ./my_app`.

### 4.4 pkg-config

Contenu des `.pc` du paquet Ubuntu 26.04 (lus dans `/usr/lib/x86_64-linux-gnu/pkgconfig/`, [CONFIRMÉ]) :
- `csfml-graphics.pc` : `Requires: csfml-window, csfml-system`, `Requires.private: sfml-graphics`, `Libs: -L${libdir} -lcsfml-graphics`, `Cflags: -I${includedir}`, `Version: 3.0.0`.
- `csfml-window.pc` et `csfml-audio.pc` / `csfml-network.pc` : `Requires: csfml-system`.
- `csfml-all.pc` : seulement `Requires: csfml-system, csfml-window, csfml-graphics, csfml-audio, csfml-network`, sans `Libs` propre.
- Ces fichiers sont identiques aux modèles `tools/pkg-config/csfml-*.pc.in` du tag 3.0.0, une fois les variables remplacées. Les modèles 2.6.1 ont le même contenu. [CONFIRMÉ]

Usage [À TESTER] :
```sh
gcc -Wall -Wextra -o my_app main.c $(pkg-config --cflags --libs csfml-graphics csfml-audio)
# préfixe local :
PKG_CONFIG_PATH="$PREFIX/lib/pkgconfig" pkg-config --cflags --libs csfml-all
```

Pièges :
- Le **binaire `pkg-config` n'est pas installé** sur la machine locale. Sur Debian/Ubuntu, le paquet est `pkgconf` (ou `pkg-config`). Je ne sais pas s'il est dans l'image Epitech. [CONFIRMÉ (local), INCERTAIN (Epitech)]
- `Requires.private: sfml-graphics` : pkg-config doit aussi trouver `sfml-graphics.pc`. Avec une SFML compilée soi-même, ce fichier contient à son tour `Requires.private: sfml-system, freetype2` (modèle `tools/pkg-config/sfml-graphics.pc.in`, tag 3.0.2). Si `freetype2.pc` manque (cas de `SFML_USE_SYSTEM_DEPS=OFF`), pkg-config peut refuser. [CONFIRMÉ (fichiers), DÉDUIT (effet), À TESTER]
- Le `.pc` ne fournit **pas de rpath** : avec un préfixe local, il faut ajouter `-Wl,-rpath,...` à la main ou utiliser `LD_LIBRARY_PATH`. [CONFIRMÉ (contenu), DÉDUIT]

### 4.5 Makefile minimal [À TESTER]

```make
NAME     = my_app
SRC      = main.c
OBJ      = $(SRC:.c=.o)
CC       = gcc
CFLAGS   = -Wall -Wextra
# Laisser vide si CSFML vient du paquet système (apt, dnf, pacman)
CSFML_PREFIX =
ifneq ($(CSFML_PREFIX),)
CFLAGS  += -I$(CSFML_PREFIX)/include
LDFLAGS += -L$(CSFML_PREFIX)/lib -Wl,-rpath,$(CSFML_PREFIX)/lib
endif
LDLIBS   = -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $(NAME) $(OBJ) $(LDFLAGS) $(LDLIBS)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

Usage : `make` (paquet système) ou `make CSFML_PREFIX=$HOME/.local/csfml` (préfixe local).
Notes :
- L'image Epitech définit `CC=clang` dans l'environnement (Dockerfile, ligne `ENV`). L'affectation `CC = gcc` du Makefile l'emporte sur l'environnement (sauf avec `make -e`). Un Makefile qui ne fixe pas `CC`, ou qui écrit `CC ?= gcc`, compilera avec clang dans l'image. [CONFIRMÉ (ENV), DÉDUIT (règles de make), À TESTER]
- La règle implicite `%.o: %.c` de make utilise `$(CC) $(CFLAGS) -c`, donc les `-I` ajoutés à `CFLAGS` s'appliquent bien. [DÉDUIT]

### 4.6 Programme de test sans écran (pour la phase 2)

La machine locale n'a pas `xvfb-run`. Pour vérifier la **liaison** et le **chargement** des `.so` sans ouvrir de fenêtre, on peut utiliser seulement le module System (signatures copiées de `include/CSFML/System/Clock.h:42,60,74`, `Sleep.h:44`, `Time.h:56,96`, tag 3.0.0, [CONFIRMÉ]) :
- `sfClock* sfClock_create(void);`
- `void sfClock_destroy(const sfClock* clock);`
- `sfTime sfClock_getElapsedTime(const sfClock* clock);`
- `void sfSleep(sfTime duration);`
- `float sfTime_asSeconds(sfTime time);`
- `sfTime sfMilliseconds(int32_t amount);`

Le test : créer une horloge, dormir 100 ms, afficher le temps écoulé (environ 0.1), puis détruire l'horloge. Pour le module graphics, un test de **liaison seule** suffit (référencer `sfRenderWindow_create` sans l'appeler, ou compiler sans lancer).

---

## 5. Windows et macOS (survol)

- **Windows, archives officielles** : la release 3.0.0 fournit `CSFML-windows-32-bit.zip`, `-64-bit.zip` et `-arm64.zip`, plus un paquet NuGet ([releases](https://github.com/SFML/CSFML/releases/tag/3.0.0), [download](https://www.sfml-dev.org/download/csfml/)). Sous Windows, CSFML lie SFML **en statique par défaut** (`CSFML_LINK_SFML_STATICALLY` ON, `CMakeLists.txt:40-45`) : les DLL CSFML contiennent SFML. [CONFIRMÉ (option), DÉDUIT (contenu des DLL)]
- **Windows, MSYS2** : `pacman -S mingw-w64-ucrt-x86_64-csfml` donne 3.0.0.rc3-2 au 2026-11-07, et non la 3.0.0 finale. [CONFIRMÉ]
- **Windows, WSL2** : on peut suivre la procédure Ubuntu (apt). L'affichage graphique passe par WSLg. [INCERTAIN, non vérifié]
- **macOS** : `brew install csfml` (3.0.0, avec sfml 3.0.2), MacPorts `csfml` 3.0.0, ou les archives `CSFML-macOS-clang-{64-bit,arm64}.tar.gz`. Avec Homebrew sur Apple Silicon, les headers sont sous `/opt/homebrew/include` : prévoir `-I$(brew --prefix)/include -L$(brew --prefix)/lib`. [CONFIRMÉ (versions), DÉDUIT (chemins)]

---

## Décisions prises

1. **Recette de référence : SFML 3.0.2 + CSFML 3.0.0**, comme la CI de CSFML 3.0.0 et Ubuntu 26.04 (donc l'image Epitech), plutôt que SFML 3.1.0, qui demande CMake 3.28, Internet et trois dépendances de plus.
2. **Préfixe local `$HOME/.local/csfml`** (dossier dédié) plutôt que `$HOME/.local` : on désinstalle avec un simple `rm -rf` et on évite les conflits `include/SFML/` entre 2.6 et 3.0.
3. On impose toujours `-DCMAKE_INSTALL_LIBDIR=lib` et `-DCMAKE_INSTALL_RPATH='$ORIGIN'`, pour neutraliser les pièges lib/lib64 et rpath transitif.
4. **Bibliothèques partagées partout** (SFML et CSFML), car c'est le défaut de CSFML sous Linux et ce qu'utilisent les paquets des distributions.
5. Le Makefile d'exemple fixe `CC = gcc`, comme le demande la mission. La note sur `CC=clang` dans l'image Epitech est gardée.
6. Fedora et openSUSE ne sont donnés qu'à titre indicatif (noms `pkgconfig(...)`), sans garantie.

## Points [INCERTAIN]

- Noms exacts des paquets Fedora pour les dépendances de SFML 3.1 (mbedtls, libssh2, harfbuzz), et comportement de `dnf install 'pkgconfig(...)'`.
- Effet réel de `SFML_ROOT` avec CSFML 2.6.1 (CMP0074 inactive) : ignoré avec un avertissement, ou pris en compte ?
- Besoin réel de `-DCMAKE_INSTALL_RPATH='$ORIGIN'` sur Ubuntu 26.04 (DT_RUNPATH non transitif) : déduit de la théorie et de la CI, pas encore observé.
- Présence de `pkg-config`/`pkgconf` dans l'image Epitech (le Dockerfile définit `PKG_CONFIG_PATH` mais n'installe pas l'outil explicitement).
- Comportement de pkg-config quand `freetype2.pc` manque (dépend de l'implémentation : pkgconf ou pkg-config).
- Fonctionnement de CSFML sous WSL2/WSLg.
