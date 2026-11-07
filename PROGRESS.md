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

## Phase D (en cours) — mise à jour 2026-11-07
- Fait et commité : référence générée (3.0 et 2.6), cookbook (31 exemples/version), annexe de migration, vérificateur de liens,
  workflows GitHub Actions (`pages.yml`, `examples.yml`), README/LICENSE/CONTRIBUTING/CHANGELOG, `tools/update-csfml.sh`.
- Agents en arrière-plan (non commités, à relire) : guides (`site/src/content/docs/guides/**`),
  tutoriels casse-briques + migration (`examples/tutorials/`, `site/.../tutorials`, `3-0/migration/`).
- Les workflows n'ont PAS été exécutés (pas de GitHub ici) : à valider au premier push.
- Reste : relecture/intégration des agents, `run-all-examples.sh --valgrind`, `npm run build && npm run check`,
  badge « testé sur » une fois par page, EN optionnel, Phase E (audit indépendant, RESULTATS.md, limites connues).
- Reprise : lire ce fichier puis `git status` ; si une limite d'usage coupe, dire « reprends ».

## Audit complet des pages (démarré 2026-11-07)
Plan : (1) tools/audit-pages.py (noms/signatures/options CMake vs JSON) → tests/logs/audit-complet/auto.md ;
(2) blocs de code (importés via <Example> ou écrits à la main) ; (3) affirmations factuelles vs tests/RESULTATS.md ;
(4) relecture débutant par 2 sous-agents (rapports par page dans tests/logs/audit-complet/) ; (5) corrections ;
(6) tests/AUDIT-COMPLET.md ; (7) npm run build + check + run-all-examples + commit + push + vérif workflows.
Périmètre : 66 pages écrites à la main (liste dans l'en-tête de tests/AUDIT-COMPLET.md). Reprise : « reprends » → lire ce fichier.
Étape en cours : (1) script de vérification.
- Fait : tools/audit-pages.py (0 ERREUR, liste blanche ALLOW + chiffres recomptés), tools/audit-blocks.py, tools/audit-claims.py ; sorties dans tests/logs/audit-complet/{auto,blocs,affirmations}.md.
- En cours : 2 relecteurs en arrière-plan (guides+en / tutoriels+migration) → rapports tests/logs/audit-complet/<page>.md. Moi : correction des affirmations « Epitech utilise… » et paragraphes SANS MARQUE.
- Reste : corrections issues des rapports, tests/AUDIT-COMPLET.md, build+check, run-all-examples, commit, push, workflows.
- Fait (2026-11-07) : tools/test-snippets.py (66 extraits C non issus d'examples/ : compilation -fsyntax-only dans la bonne version, 0 échec), affirmations : 0 SANS MARQUE (formulations « Epitech » corrigées dans install/index, epitech/environment, makefile, faq), chiffres recomptés (140, 40, 42, 926, 850).
- En attente : rapports des 2 relecteurs (tests/logs/audit-complet/<page>.md). Ensuite : corrections, tests/AUDIT-COMPLET.md, build+check, run-all-examples, push, workflows.
- Relecteurs relancés après limite d'usage (42 pages sans rapport : voir ls tests/logs/audit-complet). Reprise : relancer SendMessage si besoin, puis corrections + AUDIT-COMPLET.md.
