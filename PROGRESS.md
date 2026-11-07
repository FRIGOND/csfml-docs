# PROGRESS

## Fait
- Phase 1 (recherche) : research/00 à 10, sources.md, A-TESTER.md.
- A.1 environnement ; A.2 test P1 n°1 (Epitech `latest` = CSFML 3.0.0 ; `ubuntu24` = 2.6.0) ; A.3 builds ~/csfml-3.0 et ~/csfml-2.6 (tools/build-csfml.sh).
- Outils sans sudo : Xvfb et pkgconf extraits dans ~/local (tools/env.sh : `source tools/env.sh; start_xvfb`).
- A.4 P1 faits (voir tests/RESULTATS.md) : n°1-6, 9, 10, 12-21, 24, 29-32, 35-38 (+ 7, 8 par le build).
- Corrections au rapport : research/CORRECTIONS.md (5 entrées).

## En cours
- A.4 P1 restants : 11 (SFML statique), 22-23 (createFromMemory), 25-27 (exemples avant/après de 04), 33-34, 39 (xvfb/audio sans carte son).
- Les P1 40-46 concernent le site : à faire en phase C (prototype Starlight), Node 24 requis ? (Node 22.22 local).

## Reste
- A.4 P2 (puis P3 si budget), A.5 référence libclang (tools/), A.6 captures/valgrind
- Phases B (exemples), C (prototype site, STOP validation), D, E

## Bloquants utilisateur
- docker : accès refusé (groupe docker) — contourné via l'API du registre. Pas bloquant.
- Node : 22.22.1 présent ; le rapport dit Astro 7 exige Node 24 (test n°41). Si bloquant en phase C : `nvm`/Node 24 sans sudo.
