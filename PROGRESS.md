# PROGRESS

## Fait
- Phase 1 (recherche) : research/00 à 10, sources.md, A-TESTER.md.
- A.1 environnement, A.2 test P1 n°1 (verdict dans tests/RESULTATS.md : Epitech `latest` = CSFML 3.0.0 ; `ubuntu24` = 2.6.0).
- A.3 builds : ~/csfml-3.0 (CSFML 3.0.0 + SFML 3.0.2) et ~/csfml-2.6 (CSFML 2.6.1 + SFML 2.6.2) OK. Script tools/build-csfml.sh.

## En cours
- A.4 : tests P1 de research/A-TESTER.md (46), résultats dans tests/RESULTATS.md.

## Reste
- A.4 P2 (puis P3 si budget), A.5 référence libclang, A.6 xvfb/valgrind
- Phases B, C (STOP validation), D, E

## Bloquants utilisateur
- docker : accès refusé (utilisateur pas dans le groupe docker) — contourné pour A.2 via l'API du registre
- xvfb, pkgconf, python3-clang absents (sudo) : tests avec fenêtre et pkg-config en attente
