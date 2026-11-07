# PROGRESS

## Fait
- Phase 1 (recherche) : research/00 à 10, sources.md, A-TESTER.md.
- A.1 environnement ; A.2 test P1 n°1 (Epitech `latest` = CSFML 3.0.0 ; `ubuntu24` = 2.6.0) ; A.3 builds ~/csfml-3.0 et ~/csfml-2.6 (tools/build-csfml.sh).
- Outils sans sudo : Xvfb et pkgconf extraits dans ~/local (tools/env.sh : `source tools/env.sh; start_xvfb`).
- A.4 P1 faits (voir tests/RESULTATS.md) : n°1-6, 9, 10, 12-21, 24, 29-32, 35-38 (+ 7, 8 par le build).
- Corrections au rapport : research/CORRECTIONS.md (5 entrées).

## Fait (suite)
- A.4 P1 : 11, 22-23, 33-34 faits. P2 lot 1 (62-67, 72, 78-83, 86-87, 91, 101-102, 110-111) fait.
- A.5 : tools/gen_api.py + api/csfml-{3.0.0,2.6.1}.json (libclang 18 via ~/local/py ; PYTHONPATH=~/local/py/libclang-18.1.1.data/platlib).

## En cours
- Agent en cours : tests P1 25-27 (exemples avant/après de 04) → tests/RESULTATS-migration.md (à fusionner dans RESULTATS.md).
- P1 restants : 39 (CI xvfb), 40-46 (site, phase C).
- Les P1 40-46 concernent le site : à faire en phase C (prototype Starlight), Node 24 requis ? (Node 22.22 local).

## Reste
- A.4 P2 (puis P3 si budget), A.5 référence libclang (tools/), A.6 captures/valgrind
- Phases B (exemples), C (prototype site, STOP validation), D, E

## Bloquants utilisateur
- docker : accès refusé (groupe docker) — contourné via l'API du registre. Pas bloquant.
- Node : 22.22.1 présent ; le rapport dit Astro 7 exige Node 24 (test n°41). Si bloquant en phase C : `nvm`/Node 24 sans sudo.
