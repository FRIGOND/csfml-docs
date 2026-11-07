# PROGRESS

## Fait
- Phase 1 (recherche) : research/00 à 10 ; corrections : research/CORRECTIONS.md (10 entrées).
- Phase A : environnement, builds ~/csfml-3.0 et ~/csfml-2.6, tous les P1 (sauf 39 couvert par examples/ + run-all-examples), ~20 P2, référence libclang (`api/csfml-{3.0.0,2.6.1}.json`). Résultats : tests/RESULTATS.md.
- Outils sans sudo : `source tools/env.sh` (Xvfb, pkgconf, Node 24, `start_xvfb`). libclang python : `PYTHONPATH=~/local/py/libclang-18.1.1.data/platlib`.
- Phase B (infra) : examples/ (Makefile commun, test_mode.h, meta.json), tools/run-all-examples.sh (--valgrind, --only, --version). Exemples faits : window/basic-window, graphics/screenshot.
- Phase C : prototype Starlight dans site/ (build OK, captures vues) — EN ATTENTE DE VALIDATION UTILISATEUR (« go »).

## Fait (phase B exemples)
- 31 exemples x 2 versions = 62 programmes (window 7, graphics 11, system 3, audio 5, network 5) : `tools/run-all-examples.sh` et `--valgrind` : 62 réussis, 0 échoué. Suppression FreeType dans tests/valgrind.supp.

## Reste
- Phase B : tutoriels casse-briques (8 étapes, examples/tutorials/) ; P2 restants et P3 selon budget.
- Phase C : présenter le prototype (arborescence, 3 pages, thème, recherche, sélecteur) puis STOP → « go ».
- Phase D (après go) : site complet, pages de référence générées, CI GitHub Actions, README/CONTRIBUTING/LICENSE/CHANGELOG, script de mise à jour.
- Phase E : contrôle final + relecture indépendante.

## Bloquants / à signaler
- docker inaccessible (groupe docker) : contourné.
- Test pluriel/singulier Pagefind non fait (navigateur nécessaire).
