# 01 — Versions de CSFML et de SFML (état au 2026-11-07)

Livrable de recherche, phase 1. Rien n'a été compilé ni installé.
Étiquettes : [CONFIRMÉ] = lu dans une source primaire ou constaté ; [DÉDUIT] = raisonnement à partir de sources ; [INCERTAIN] = source faible ou absente.
Abréviations : `$S` = dossier scratchpad de la session ; « clone CSFML » = `$S/repos/CSFML` ; « clone SFML » = `$S/repos/SFML`.

## En bref

- **Dernière CSFML stable : 3.0.0**, release GitHub publiée le **2026-01-14** (tag créé le 2026-01-13). Aucune CSFML 3.1 n'existe. [CONFIRMÉ] ([API releases CSFML](https://api.github.com/repos/SFML/CSFML/releases))
- **Dernière SFML stable : 3.1.0**, publiée le **2026-04-16**. [CONFIRMÉ] ([API releases SFML](https://api.github.com/repos/SFML/SFML/releases), [sfml-dev.org/download](https://www.sfml-dev.org/download/))
- La branche `master` de CSFML se déclare déjà **3.1.0**, demande **SFML 3.1** et **CMake 3.28**. Elle ajoute TLS, IPv6, DNS, SFTP et le text shaping. Elle n'est **pas publiée** (20 commits après 3.0.0, dernier le 2026-11-01). [CONFIRMÉ]
- **CSFML 3.0.0 se construit aussi avec SFML 3.1.0** : Debian sid l'a reconstruite (`3.0.0-1+b2`) et le paquet dépend de `libsfml-graphics3.1`. Il faut quand même **recompiler** : le soname SFML change à chaque version mineure (`.so.3.0` puis `.so.3.1`). [CONFIRMÉ]
- La règle officielle : **la version majeure de SFML doit être la même que celle de CSFML** (readme CSFML 3.0.0). [CONFIRMÉ]
- **Rupture 2.x → 3.0** : les headers passent de `<SFML/Graphics.h>` à `<CSFML/Graphics.h>`, `sfBool` disparaît au profit de `bool`, et beaucoup de signatures changent. Les détails sont dans `04-migration-2-vers-3.md`. [CONFIRMÉ]
- **Distributions** : Ubuntu 26.04 LTS et Debian testing ont 3.0.0, Arch et Homebrew aussi. Debian 13 stable a 2.6.1, Ubuntu 24.04 a 2.6.0, et **Fedora (même 45 et Rawhide) reste en 2.6.1**. [CONFIRMÉ]
- **Epitech** : l'image officielle `epitechcontent/epitest-docker:latest` est basée sur **Ubuntu 26.04 (« resolute ») depuis le 2026-08-26**. Elle installe `libcsfml-dev` depuis Ubuntu, c'est-à-dire **CSFML 3.0.0**. Avant, c'était Ubuntu 24.04 avec CSFML 2.6.0. [DÉDUIT de sources primaires, à confirmer par `docker run`]
- Conséquence pour la doc : **viser CSFML 3.0.0** en priorité, avec un encadré 2.6 pour Fedora et pour les anciens projets.

---

## 1. Dernières versions stables (tags et releases GitHub)

Méthode : dates du tag git lues dans les clones (`git for-each-ref`, `git log -1`), puis dates de publication GitHub lues dans l'API (`published_at`). L'API a été consultée le 2026-11-07. [CONFIRMÉ]

### CSFML

| Version | Date du commit tagué | Release GitHub publiée | Statut | SFML visée |
|---|---|---|---|---|
| **3.0.0** | 2026-01-13 | **2026-01-14 09:12 UTC** | Latest | SFML 3.0.x (CI : 3.0.2) |
| 3.0.0-rc.3 | 2025-09-03 | 2025-09-04 | pre-release | SFML 3 |
| 3.0.0-rc.2 | 2025-08-29 | 2025-08-29 | pre-release | SFML 3 |
| 3.0.0-rc.1 | 2025-01-22 | 2025-01-22 | pre-release | SFML 3 |
| 2.6.1 | 2024-05-19 | 2024-05-19 | stable | SFML 2.6 |
| 2.6.0 | 2024-02-04 | 2024-02-04 | stable | SFML 2.6 |
| 2.5.2 | 2023-06-09 | 2023-06-09 | stable | SFML 2.5 |
| 2.5.1 | 2021-07-11 | **2021-08-02** (≠ tag) | stable | SFML 2.x |
| 2.5 | 2018-11-19 | **2018-12-08** (≠ tag) | stable | SFML 2 |

Sources : https://github.com/SFML/CSFML/releases, https://api.github.com/repos/SFML/CSFML/releases, `git for-each-ref refs/tags` sur le clone CSFML. Les écarts entre tag et release ne concernent que 2.5 et 2.5.1. Les releases 2.0 à 2.4 ont toutes été (re)publiées le 2018-12-08. [CONFIRMÉ]

Fichiers joints à la release 3.0.0 : `CSFML-windows-32-bit.zip`, `CSFML-windows-64-bit.zip`, `CSFML-windows-arm64.zip`, `CSFML-macOS-clang-64-bit.tar.gz`, `CSFML-macOS-clang-arm64.tar.gz` et `CSFML.3.0.0.nupkg`. **Aucun binaire Linux** (la page de téléchargement n'en propose que pour 2.0, 2.1 et 2.2). [CONFIRMÉ] ([API](https://api.github.com/repos/SFML/CSFML/releases), [page download CSFML](https://www.sfml-dev.org/download/csfml/))

Branche `2.6.x` : 4 commits après 2.6.1 (dernier le 2025-01-08), et un jalon GitHub « 2.6.2 » ouvert. Mais **aucune CSFML 2.6.2 n'est publiée**. [CONFIRMÉ] (`git log 2.6.1..origin/2.6.x`, [milestones](https://api.github.com/repos/SFML/CSFML/milestones?state=all))

### SFML (C++, pour savoir ce qu'il y a « sous » CSFML)

| Version | Date du commit tagué | Release GitHub publiée |
|---|---|---|
| **3.1.0** | 2026-04-16 | **2026-04-16 14:22 UTC** |
| 3.0.2 | 2025-09-17 | 2025-09-18 05:52 UTC |
| 3.0.1 | 2025-04-22 | 2025-04-22 |
| 3.0.0 | 2024-12-20 (21 en UTC) | 2024-12-21 |
| 2.6.2 | 2024-11-09 | 2024-11-10 |
| 2.6.1 | 2023-10-29 (30 en UTC) | **2023-11-04** |
| 2.6.0 | 2023-06-20 | 2023-06-20 |
| 2.5.1 | 2018-10-15 | 2018-10-15 |

Source : https://api.github.com/repos/SFML/SFML/releases ; https://www.sfml-dev.org/download/ annonce 3.1.0 comme « Latest stable version ». [CONFIRMÉ]
Note : le « fait déjà constaté » du brief (tag SFML 2.6.1 au 2023-10-29) est juste en heure locale (+02:00). En UTC, c'est le 2023-10-30, et la release n'a été publiée que le 2023-11-04. Ce n'est pas une contradiction, juste une précision.

---

## 2. Y a-t-il une CSFML 3.1 ? État de `master`

- **Non, pas de CSFML 3.1 publiée.** La release la plus récente est 3.0.0. Il n'y a pas de jalon « 3.1 », seulement un jalon « 3.0.1 » ouvert (4 tickets). [CONFIRMÉ] ([milestones](https://api.github.com/repos/SFML/CSFML/milestones?state=all))
- `master` = 20 commits après `3.0.0` (constaté avec `git log 3.0.0..master`). Ces commits ont été écrits début octobre 2026 et intégrés le 2026-11-01. [CONFIRMÉ]
- Dans `master:CMakeLists.txt` : `cmake_minimum_required(VERSION 3.28)` (ligne 1), `project(CSFML VERSION 3.1.0)` (ligne 16), `find_package(SFML 3.1 COMPONENTS ...)` (ligne 69). Dans `master:include/CSFML/Config.h` (lignes 38 à 40) : `CSFML_VERSION_MINOR 1`. [CONFIRMÉ]
- **SFML n'est pas récupéré par FetchContent.** CSFML (3.0.0 comme `master`) cherche une SFML déjà installée avec `find_package`. FetchContent ne sert qu'à Catch2, pour les tests (`test/CMakeLists.txt:1-9`). La CI de `master` construit SFML `3.1.0` à part (`.github/workflows/ci.yml:12`, `SFML_VERSION: 3.1.0`). [CONFIRMÉ]
- Contenu de `master` depuis 3.0.0 (résumé des titres de commits, [CONFIRMÉ]) :
  - passage à SFML 3.1.0 ; les fonctions C qui enveloppent des API SFML dépréciées sont marquées `CSFML_DEPRECATED` (ex. `sfText_findCharacterPos`, `sfTouch_getPositionRenderWindow`, toutes les `sfFtp*`) ;
  - `sfGetVersion` (version de SFML lue à l'exécution) ;
  - miter limit sur les shapes ; ascent, descent et glyph par ID dans les polices ; API de text shaping, d'alignement et d'orientation ;
  - `sfPlaybackDevice` (choix du périphérique audio) ;
  - réseau : IPv6, `sfDns`, TLS sur les sockets TCP, vérification HTTPS dans `sfHttp`, `sfSftp` (remplace `sfFtp`), callbacks dans `sfSocketSelector`, `sfTimeoutWithPredicate` ;
  - doc : support QOI pour les images, exemple corrigé (`sfText_create` prend une police).
- Conclusion : une **CSFML 3.1.0 est très probablement en préparation**, mais sa date de sortie est inconnue. [INCERTAIN]

---

## 3. Compatibilité CSFML ↔ SFML

| CSFML | SFML demandée par CMake | Accepte… | Source |
|---|---|---|---|
| 2.5 / 2.5.1 | `find_package(SFML 2 ...)` | toute SFML 2.x | `src/SFML/CMakeLists.txt:24` (tags 2.5, 2.5.1) |
| 2.5.2 | `find_package(SFML 2.5 ...)` | SFML ≥ 2.5, < 3 | `src/SFML/CMakeLists.txt:24` (tag 2.5.2) |
| 2.6.0 / 2.6.1 | `find_package(SFML 2.6 ...)` | SFML 2.6.x | `src/SFML/CMakeLists.txt:24` (2.6.0), `:37` (2.6.1) |
| **3.0.0** | `find_package(SFML 3.0 ...)` | **SFML 3.0.x et 3.1.x** | `CMakeLists.txt:69` (tag 3.0.0) |
| master (3.1.0) | `find_package(SFML 3.1 ...)` | SFML ≥ 3.1, < 4 | `CMakeLists.txt:69` (master) |

Pourquoi CSFML 3.0.0 accepte SFML 3.1 :
- SFML écrit son fichier de version CMake avec `COMPATIBILITY SameMajorVersion` (`cmake/Macros.cmake:446` au tag 3.0.2, `:482` au tag 3.1.0). Une demande « 3.0 » accepte donc toute 3.x ≥ 3.0. [CONFIRMÉ]
- **Preuve que la compilation passe** : dans Debian sid et forky, `libcsfml-graphics3.0` en version `3.0.0-1+b2` (une recompilation binNMU) dépend de `libsfml-graphics3.1 (>= 3.1.0+dfsg)`. [CONFIRMÉ] ([packages.debian.org/sid/libcsfml-graphics3.0](https://packages.debian.org/sid/libcsfml-graphics3.0), [madison](https://qa.debian.org/madison.php?package=libcsfml-dev&text=on)). Ubuntu 26.10 a fait pareil (`3.0.0-1build1` avec `libsfml 3.1.0+dfsg-4`). [CONFIRMÉ] (API Launchpad)
- Il peut y avoir des avertissements de dépréciation, car SFML 3.1 déprécie des API que CSFML 3.0.0 appelle encore. [DÉDUIT] (commit `35713f1` de master)

**Compatibilité binaire (ABI)** : SFML fixe `SOVERSION ${MAJOR}.${MINOR}` (`cmake/Macros.cmake:150`, tags 3.0.2 et 3.1.0). CSFML fait pareil (`cmake/Macros.cmake:55`, tag 3.0.0). Sur la machine locale, `libcsfml-graphics.so.3.0.0` a besoin de `libsfml-graphics.so.3.0` (vu avec `objdump -p`). Une CSFML compilée contre SFML 3.0 **ne se charge pas** avec SFML 3.1 seule : il faut recompiler CSFML. [CONFIRMÉ]

Règle officielle (readme de CSFML 3.0.0, section *Building*) : la version majeure de SFML doit correspondre à celle de CSFML. [CONFIRMÉ]

---

## 4. Versions encore pertinentes

| CSFML | Sortie (release) | SFML | Pourquoi elle compte encore | Headers |
|---|---|---|---|---|
| 2.5 (paquet Debian/Ubuntu « 2.5-1.1 ») | 2018-12 | 2.5.1 | Ubuntu 22.04 et 23.10, Debian 12 ; l'image Epitech de 2024-07 à 2025-09 | `<SFML/...h>` |
| 2.5.2 | 2023-06-09 | 2.5.x | Alpine edge ; ancien Fedora (≤ 39) | `<SFML/...h>` |
| 2.6.0 | 2024-02-04 | 2.6.x | Ubuntu 24.04 LTS ; l'image Epitech de 2025-09 à 2026-08 | `<SFML/...h>` |
| 2.6.1 | 2024-05-19 | 2.6.x | Debian 13 (stable), **Fedora 43 à Rawhide**, Ubuntu 25.04 et 25.10, nixpkgs `csfml_2` | `<SFML/...h>` |
| **3.0.0** | 2026-01-14 | 3.0.x / 3.1.x | Ubuntu 26.04 LTS, Debian testing, Arch, Homebrew, MacPorts ; **Epitech depuis 2026-08** | `<CSFML/...h>` |
| master (3.1.0 non publiée) | — | 3.1.x | nouveautés réseau et texte ; pour contributeurs seulement | `<CSFML/...h>` |

### Différences principales, en résumé (le détail est dans `04-migration-2-vers-3.md`)

- **Dossier des headers** : 2.6.1 → `include/SFML/Graphics.h` ; 3.0.0 → `include/CSFML/Graphics.h`. [CONFIRMÉ] (`git ls-tree` des deux tags). Sur Ubuntu 26.04, `/usr/include/SFML/` ne contient que les `.hpp` de SFML C++ : un `#include <SFML/Graphics.h>` écrit pour la 2.x **ne trouve plus le fichier**. [CONFIRMÉ] (`ls /usr/include/SFML/*.h` sur la machine locale → aucun fichier)
- **Booléens** : 2.6.1 a `typedef int sfBool;` (`include/SFML/Config.h:153`) ; en 3.0.0, `sfBool` n'existe plus dans les headers et c'est `bool` qui est utilisé (C99 `stdbool.h`, inclus par `Config.h`). [CONFIRMÉ]
- **Exemples de signatures modifiées** (copiées des headers, [CONFIRMÉ]) :
  - 2.6.1 `include/SFML/Graphics/RenderWindow.h:55` : `sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, sfUint32 style, const sfContextSettings* settings);`
  - 3.0.0 `include/CSFML/Graphics/RenderWindow.h:59-64` : `sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);`
  - `sfSprite_create(void)` (2.6.1, `Sprite.h:46`) devient `sfSprite_create(const sfTexture* texture)` (3.0.0, `Sprite.h:48`).
  - `sfText_create(void)` (2.6.1, `Text.h:59`) devient `sfText_create(const sfFont* font)` (3.0.0, `Text.h:62`).
- **Types entiers** : les `sfUint32`/`sfInt32` de 2.x sont remplacés par `uint32_t`/`int32_t` en 3.0.0 (ex. `sfMilliseconds(int32_t)` dans `System/Time.h:96`, contre `sfMilliseconds(sfInt32)` en 2.6.1). [CONFIRMÉ]
- **Audio** : SFML 3 utilise miniaudio et non plus OpenAL (`src/SFML/Audio/CMakeLists.txt` au tag 3.0.2 inclut `extlibs/headers/miniaudio`). La release CSFML 3.0.0 cite « Remove references to OpenAL ». [CONFIRMÉ]
- **Build** : CSFML 3 demande CMake ≥ 3.22 et un compilateur C11 + C++17 (`cmake/Macros.cmake:23` : `c_std_11 cxx_std_17`). CSFML 2.6.1 demandait CMake ≥ 3.7.2. [CONFIRMÉ]
- **pkg-config** : les fichiers `.pc` n'existent que **depuis CSFML 2.6.0** (release 2.6.0 : « Add pkg-config support (#188) »). 2.5.x n'en a pas (aucun `tools/pkg-config` aux tags 2.5, 2.5.1, 2.5.2). [CONFIRMÉ]

---

## 5. Version packagée dans les distributions

Sources principales : [API Repology csfml](https://repology.org/api/v1/project/csfml) et [sfml](https://repology.org/api/v1/project/sfml), [Debian madison](https://qa.debian.org/madison.php?package=libcsfml-dev&text=on), API Launchpad (paquet source `libcsfml`), [packages.fedoraproject.org](https://packages.fedoraproject.org/pkgs/CSFML/CSFML-devel/), [API Arch](https://archlinux.org/packages/search/json/?name=csfml), [API Homebrew](https://formulae.brew.sh/api/formula/csfml.json), [MSYS2](https://packages.msys2.org/base/mingw-w64-csfml). Toutes consultées le 2026-11-07. [CONFIRMÉ]

| Distribution | Paquet (dev) | CSFML | SFML en dessous | Source / date |
|---|---|---|---|---|
| Debian 12 bookworm (oldstable) | `libcsfml-dev` | 2.5-1.1 | 2.5.1 | madison |
| **Debian 13 trixie (stable)** | `libcsfml-dev` | **2.6.1-1** | 2.6.2 | madison ; stable depuis le 2025-08-09 ([debian.org/releases](https://www.debian.org/releases/)) |
| Debian 14 forky (testing) / sid | `libcsfml-dev` | 3.0.0-1+b2 | **3.1.0** | madison, packages.debian.org |
| Ubuntu 22.04 jammy | `libcsfml-dev` | 2.5-1.1 | 2.5.1 | Launchpad |
| **Ubuntu 24.04 noble LTS** | `libcsfml-dev` | **2.6.0-3build1** | 2.6.1 | Launchpad (publié le 2024-04-04) |
| Ubuntu 25.04 / 25.10 | `libcsfml-dev` | 2.6.1-1 | 2.6.2 | Launchpad |
| **Ubuntu 26.04 resolute LTS** | `libcsfml-dev` | **3.0.0-1** | 3.0.2 | Launchpad (publié le 2026-01-18) ; installé sur la machine locale (dpkg) |
| Ubuntu 26.10 stonking | `libcsfml-dev` | 3.0.0-1build1 | 3.1.0 | Launchpad (2026-06-14) |
| Ubuntu 27.04 tenacious (dév.) | `libcsfml-dev` | 3.0.0-1build1 | 3.1.0 | Launchpad (2026-10-15) |
| **Fedora 43 / 44 / 45 / Rawhide** | `CSFML-devel` | **2.6.1** (-5 à -8) | 2.6.2 | packages.fedoraproject.org ; spec `CSFML.spec` (headers dans `%{_includedir}/SFML/`, `.pc` fournis) |
| Arch Linux (extra) | `csfml` | 3.0.0-1 (compilé le 2026-04-18) | 3.1.0 | API archlinux.org |
| Homebrew (macOS/Linux) | `csfml` | 3.0.0 | 3.0.2 (`sfml`) | formulae.brew.sh |
| MacPorts | `csfml` | 3.0.0 | 3.1.0 | Repology |
| MSYS2 (ucrt64, clang64, clangarm64) | `mingw-w64-ucrt-x86_64-csfml`… | **3.0.0.rc3-2** (en retard) | 3.1.0 | packages.msys2.org (compilé le 2026-11-02) |
| vcpkg | — | **pas de port CSFML** (404) | sfml 3.1.0 | `ports/csfml/vcpkg.json` absent sur GitHub microsoft/vcpkg |
| nixpkgs 26.05 / unstable | `csfml` / `csfml_2` | 3.0.0 / 2.6.1 | 3.0.2 / 2.6.2 | Repology |
| FreeBSD ports | `devel/csfml` | 3.0.0 | 3.0.0 (`sfml3`) | Repology |
| Alpine edge (testing) | `csfml-dev` | 2.5.2 | 2.6.1 | Repology |
| Void Linux | `CSFML-devel` | 2.5 | 2.6.2 | Repology |
| openSUSE | — | pas de CSFML (seulement `sfml2`/`sfml3`) | — | Repology |

À retenir :
- Avec **Fedora**, une doc écrite pour CSFML 3 ne marche pas telle quelle avec le paquet officiel : l'utilisateur a 2.6.1, avec d'autres headers et d'autres signatures. Il faut l'avertir et lui proposer le build depuis les sources (voir `02-install.md`). [DÉDUIT]
- Sur **Arch et Debian testing**, CSFML 3.0.0 tourne déjà au-dessus de SFML 3.1.0. [CONFIRMÉ]

---

## 6. Version probablement utilisée à Epitech

### Ce qui est confirmé (sources primaires)

1. **Dockerfile actuel** de l'image de test officielle ([github.com/Epitech/epitest-docker](https://github.com/Epitech/epitest-docker), fichier `Dockerfile` sur `master`) : `FROM ubuntu:resolute` (Ubuntu 26.04). Il ajoute `ppa:epitech/ppa` et installe les méta-paquets `epitech-cpool`, `epitech-premsc`, `epitech-tek1`, `epitech-tek2` et `epitech-web`. Il fixe aussi `CC=clang` et `PKG_CONFIG_PATH=/usr/local/lib/pkgconfig`. [CONFIRMÉ]
2. Commit `46e2f78c` du **2026-08-26**, « chore: Update to Ubuntu Resolute » : `FROM ubuntu:noble` devient `FROM ubuntu:resolute`. Le dernier commit date du 2026-10-07. [CONFIRMÉ] ([API commits](https://api.github.com/repos/Epitech/epitest-docker/commits))
3. **PPA Epitech** ([index Packages resolute](https://ppa.launchpadcontent.net/epitech/ppa/ubuntu/dists/resolute/main/binary-amd64/Packages.gz)) : `epitech-cpool 1.22` dépend de `libcsfml-dev` et de `libcsfml-doc`. Le PPA **ne fournit pas sa propre CSFML** : il ne contient que banana, epiclang et les méta-paquets `epitech-*` (vérifié avec l'API Launchpad `getPublishedSources`, 22 paquets). [CONFIRMÉ]
4. Dans Ubuntu resolute, `libcsfml-dev` = **3.0.0-1** (API Launchpad, et le même paquet est installé sur la machine locale). [CONFIRMÉ]
5. Docker Hub `epitechcontent/epitest-docker` : tags `latest` et `devel` poussés le **2026-10-07** ; le tag `ubuntu24` a été poussé le 2026-08-26 (sans doute l'ancienne image noble). [CONFIRMÉ] ([API Docker Hub](https://hub.docker.com/v2/repositories/epitechcontent/epitest-docker/tags?page_size=10))

### Ce qu'on en déduit

- **L'image de correction Epitech actuelle contient CSFML 3.0.0 (sur SFML 3.0.2).** [DÉDUIT] Il reste à le vérifier avec `docker run --rm epitechcontent/epitest-docker:latest dpkg -l libcsfml-dev` (Docker est présent sur la machine locale ; voir la liste « à tester »).
- Les postes étudiants (« dump ») installés avec `epitech-full` sur Ubuntu 26.04 auraient la même version. [DÉDUIT, plus faible] Je n'ai trouvé aucune source publique sur le dump 2026.
- **Historique de l'image** (lu dans les Dockerfiles des commits, [CONFIRMÉ] pour la base, [DÉDUIT] pour la version CSFML, déduite du paquet de la distribution) :

| Période | Base de l'image | CSFML |
|---|---|---|
| 2021-12 → 2024-07 | Fedora 34 → 38 (`CSFML-devel` via dnf, commit « feat: CSFML with DNF ») | 2.5.x |
| 2024-07-23 → 2025-09-01 | Ubuntu 23.10 mantic (`libcsfml-dev`) | 2.5 (2.5-1.1) |
| 2025-09-01 → 2026-08-26 | Ubuntu 24.04 noble | 2.6.0 |
| **depuis 2026-08-26** | **Ubuntu 26.04 resolute** | **3.0.0** |

- **Point d'attention** : les sujets Epitech, les exemples d'anciens étudiants et la plupart des tutoriels en ligne sont écrits pour CSFML 2.x (`#include <SFML/Graphics.h>`, `sfBool`, `sfSprite_create()` sans argument). Avec l'image 2026, ce code **ne compile plus**. La doc doit donc couvrir 3.0.0 en priorité et proposer une table de migration claire. [DÉDUIT]
- Je ne sais pas si les sujets Epitech 2026-2027 demandent explicitement une version. Les PDF des sujets ne sont pas publics. [INCERTAIN]

---

## Décisions prises

1. **Version de référence de la doc : CSFML 3.0.0** (dernière stable, celle d'Ubuntu 26.04 et de l'image Epitech). La branche `master` (future 3.1) sera seulement signalée dans un encadré « à venir », car elle n'est pas publiée.
2. **CSFML 2.6.x reste couverte comme « legacy »** : c'est la version de Fedora, de Debian stable et d'Ubuntu 24.04, et celle de tout le code Epitech d'avant 2026.
3. Pour les dates, j'ai retenu la **date de publication de la release GitHub** (`published_at`) comme date de sortie, et indiqué la date du tag quand elle diffère.
4. Pour Epitech, je me suis appuyé sur le Dockerfile public et sur le PPA, et non sur des témoignages, faute de source secondaire fiable sur 2026.

## Points [INCERTAIN]

- Date de sortie et contenu final d'une future CSFML 3.1.0 (seul `master` existe).
- Version CSFML réellement présente dans l'image Docker Epitech `latest` : c'est très probable mais pas vérifié par exécution (test P1 dans la liste « à tester »).
- Version installée sur les postes étudiants Epitech (dump 2026) : aucune source publique.
- Version exacte de CSFML dans les images Fedora d'Epitech de 2021 à 2024 (2.5 ou 2.5.2 selon la version de Fedora) : non vérifié, et sans importance pour la doc.
- Quantité d'avertissements de dépréciation quand on compile CSFML 3.0.0 contre SFML 3.1.0 : non mesurée.
