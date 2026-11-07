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
