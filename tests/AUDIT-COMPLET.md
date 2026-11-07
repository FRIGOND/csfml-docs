# Audit complet des pages écrites à la main

Périmètre : les pages hors `reference/`, `cookbook/` et `3-0/migration/api-changes` (générées). Date : 2026-11-07. Régénérer ce fichier : `python3 tools/audit-report.py`.

## Méthode et résultats automatiques (relançables)

| Contrôle | Outil | Sortie | Résultat |
|---|---|---|---|
| Noms, types, constantes, includes, nombre d'arguments des appels, chiffres, 2.6/3.0 | `tools/audit-pages.py` | `logs/audit-complet/auto.md` | 0 erreur ; exceptions volontaires listées dans le script (liste ALLOW) |
| Blocs de code : importé / extrait d'un exemple / écrit à la main | `tools/audit-blocks.py` | `logs/audit-complet/blocs.md` | voir ci-dessous |
| Compilation des blocs C écrits à la main | `tools/test-snippets.py` | `logs/audit-complet/snippets.md` | tous compilent avec `-fsyntax-only` (pas d'exécution) dans la version visée |
| Affirmations sur l'environnement | `tools/audit-claims.py` | `logs/audit-complet/affirmations.md` | 0 paragraphe sans preuve ni marque ; aucune phrase « Epitech utilise la version X » |
| Relecture débutant | 2 sous-agents distincts de l'auteur | un rapport par page | tableau ci-dessous |

Blocs de code : 38 `<Example/>` importés (vrais `main.c` compilés et exécutés par `tools/run-all-examples.sh`), 73 extraits de ces exemples, 66 blocs C écrits à la main (compilés en syntaxe seulement), 48 blocs shell (testés seulement quand la page cite un test de `tests/RESULTATS.md`).

**Limites honnêtes.** Les relecteurs ont écrit leurs rapports vite (le second dit les avoir générés par script depuis ses notes) : beaucoup de lignes « mineur » sont des remarques répétées ou de simples confirmations. Je n'ai corrigé que les points bloquants et la plupart des points gênants que j'ai pu vérifier moi-même ; les autres lignes restent ouvertes. Un point signalé (« le sprite reste blanc » en 2.6, tutoriel 06) n'est pas vérifié. Un point (`sfKeyReturn` dans concepts/events) n'a pas été retrouvé dans la page.

Total des lignes des rapports : 355 (4 bloquant, 104 gênant, 247 mineur).

## Tableau par page

| Page | Problèmes trouvés | Corrigé ? | Détail |
|---|---|---|---|
| 2-6/getting-started/first-window | 5 (1 bloquant, 1 gênant, 3 mineur) | **partiel** : encadré sur test_mode.h / example_frames (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__getting-started__first-window.md) |
| 2-6/tutorials/01-window | 9 (0 bloquant, 4 gênant, 5 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__01-window.md) |
| 2-6/tutorials/02-shapes | 7 (0 bloquant, 2 gênant, 5 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__02-shapes.md) |
| 2-6/tutorials/03-movement | 7 (0 bloquant, 3 gênant, 4 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__03-movement.md) |
| 2-6/tutorials/04-input | 7 (0 bloquant, 2 gênant, 5 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__04-input.md) |
| 2-6/tutorials/05-collisions | 6 (0 bloquant, 2 gênant, 4 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__05-collisions.md) |
| 2-6/tutorials/06-sprites | 7 (0 bloquant, 4 gênant, 3 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__06-sprites.md) |
| 2-6/tutorials/07-text-score | 7 (0 bloquant, 2 gênant, 5 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__07-text-score.md) |
| 2-6/tutorials/08-sound-states | 8 (0 bloquant, 2 gênant, 6 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__08-sound-states.md) |
| 2-6/tutorials/09-final | 7 (0 bloquant, 2 gênant, 5 mineur) | **partiel** : option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/2-6__tutorials__09-final.md) |
| 3-0/getting-started/first-window | 5 (1 bloquant, 1 gênant, 3 mineur) | **partiel** : encadré sur test_mode.h / example_frames (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__getting-started__first-window.md) |
| 3-0/migration/checklist | 6 (0 bloquant, 1 gênant, 5 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/3-0__migration__checklist.md) |
| 3-0/migration/events | 6 (0 bloquant, 0 gênant, 6 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/3-0__migration__events.md) |
| 3-0/migration/objects-creation | 5 (0 bloquant, 2 gênant, 3 mineur) | **partiel** : « idem » remplacé par « non testé » ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__migration__objects-creation.md) |
| 3-0/migration/overview | 8 (0 bloquant, 4 gênant, 4 mineur) | **partiel** : ligne GlFunctionPointer réparée, NULL de sfText/sfSound reformulé (non testé) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__migration__overview.md) |
| 3-0/migration/support-both | 6 (0 bloquant, 1 gênant, 5 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/3-0__migration__support-both.md) |
| 3-0/migration/types | 5 (0 bloquant, 0 gênant, 5 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/3-0__migration__types.md) |
| 3-0/tutorials/01-window | 8 (0 bloquant, 3 gênant, 5 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__01-window.md) |
| 3-0/tutorials/02-shapes | 6 (0 bloquant, 1 gênant, 5 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__02-shapes.md) |
| 3-0/tutorials/03-movement | 6 (0 bloquant, 2 gênant, 4 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__03-movement.md) |
| 3-0/tutorials/04-input | 7 (0 bloquant, 1 gênant, 6 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__04-input.md) |
| 3-0/tutorials/05-collisions | 5 (0 bloquant, 1 gênant, 4 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__05-collisions.md) |
| 3-0/tutorials/06-sprites | 6 (0 bloquant, 2 gênant, 4 mineur) | **partiel** : test n°31 cité; encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__06-sprites.md) |
| 3-0/tutorials/07-text-score | 6 (0 bloquant, 2 gênant, 4 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__07-text-score.md) |
| 3-0/tutorials/08-sound-states | 8 (0 bloquant, 2 gênant, 6 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__08-sound-states.md) |
| 3-0/tutorials/09-final | 6 (0 bloquant, 1 gênant, 5 mineur) | **partiel** : encadré test_mode.h (composant Example) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/3-0__tutorials__09-final.md) |
| en/2-6/getting-started/first-window | 4 (0 bloquant, 1 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/en__2-6__getting-started__first-window.md) |
| en/3-0/getting-started/first-window | 4 (0 bloquant, 2 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/en__3-0__getting-started__first-window.md) |
| en/index | 2 (0 bloquant, 0 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/en__index.md) |
| guides/concepts/api-conventions | 5 (0 bloquant, 1 gênant, 4 mineur) | **partiel** : test n°31 cité ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__concepts__api-conventions.md) |
| guides/concepts/audio-without-device | 5 (0 bloquant, 2 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__audio-without-device.md) |
| guides/concepts/coordinates-views | 3 (0 bloquant, 0 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__coordinates-views.md) |
| guides/concepts/events | 3 (0 bloquant, 1 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__events.md) |
| guides/concepts/game-loop | 3 (0 bloquant, 1 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__game-loop.md) |
| guides/concepts/index | 2 (0 bloquant, 0 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__index.md) |
| guides/concepts/memory | 7 (0 bloquant, 4 gênant, 3 mineur) | **partiel** : source du chiffre 259 204 corrigée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__concepts__memory.md) |
| guides/concepts/ownership | 4 (0 bloquant, 1 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__ownership.md) |
| guides/concepts/resources | 6 (0 bloquant, 2 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__resources.md) |
| guides/concepts/time | 4 (0 bloquant, 0 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__concepts__time.md) |
| guides/epitech/before-you-submit | 7 (1 bloquant, 4 gênant, 2 mineur) | **partiel** : phrase « l'image actuelle a la 3.0.0 » reformulée, marquée non vérifié ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__epitech__before-you-submit.md) |
| guides/epitech/coding-style | 6 (0 bloquant, 1 gênant, 5 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__epitech__coding-style.md) |
| guides/epitech/environment | 5 (0 bloquant, 2 gênant, 3 mineur) | **partiel** : « le correcteur compile dans… » reformulé : version du correcteur non vérifiée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__epitech__environment.md) |
| guides/epitech/index | 2 (0 bloquant, 0 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__epitech__index.md) |
| guides/epitech/makefile | 5 (0 bloquant, 2 gênant, 3 mineur) | **partiel** : CC=clang marqué (Dockerfile public, non vérifié) ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__epitech__makefile.md) |
| guides/epitech/project-notions | 4 (0 bloquant, 0 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__epitech__project-notions.md) |
| guides/help/about | 6 (0 bloquant, 2 gênant, 4 mineur) | **partiel** : phrase sur la CI reformulée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__help__about.md) |
| guides/help/faq | 6 (0 bloquant, 1 gênant, 5 mineur) | **partiel** : phrase « image Epitech actuelle » reformulée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__help__faq.md) |
| guides/help/glossary | 5 (0 bloquant, 1 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__help__glossary.md) |
| guides/index | 4 (0 bloquant, 1 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__index.md) |
| guides/install/arch | 4 (0 bloquant, 1 gênant, 3 mineur) | **partiel** : source du pkg-config citée, Arch non vérifié ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__install__arch.md) |
| guides/install/check-your-setup | 6 (0 bloquant, 3 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__install__check-your-setup.md) |
| guides/install/compiling | 8 (0 bloquant, 3 gênant, 5 mineur) | **partiel** : example.mk décrit correctement (CSFML_PREFIX, system) ; $P défini ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__install__compiling.md) |
| guides/install/fedora | 5 (0 bloquant, 1 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__install__fedora.md) |
| guides/install/from-source | 8 (1 bloquant, 4 gênant, 3 mineur) | **partiel** : sudo nécessaire pour les -dev, prérequis g++, garde-fou sur rm -rf $PREFIX ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__install__from-source.md) |
| guides/install/index | 5 (0 bloquant, 1 gênant, 4 mineur) | **partiel** : remarque Epitech et lignes 2.6 marquées non vérifié ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__install__index.md) |
| guides/install/macos-windows | 5 (0 bloquant, 1 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__install__macos-windows.md) |
| guides/install/ubuntu-debian | 5 (0 bloquant, 2 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__install__ubuntu-debian.md) |
| guides/install/wsl | 5 (0 bloquant, 2 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__install__wsl.md) |
| guides/troubleshooting/black-window | 4 (0 bloquant, 1 gênant, 3 mineur) | **partiel** : différence 3.0/2.6 de sfTexture_updateFromRenderWindow signalée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__troubleshooting__black-window.md) |
| guides/troubleshooting/compile-errors | 5 (0 bloquant, 2 gênant, 3 mineur) | **partiel** : gcc 14 marqué non testé ; forme {{800,600},32} limitée à la 3.0 ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__troubleshooting__compile-errors.md) |
| guides/troubleshooting/headless-xvfb | 5 (0 bloquant, 1 gênant, 4 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__troubleshooting__headless-xvfb.md) |
| guides/troubleshooting/index | 3 (0 bloquant, 1 gênant, 2 mineur) | **partiel** : source tests/RESULTATS.md citée ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__troubleshooting__index.md) |
| guides/troubleshooting/link-errors | 4 (0 bloquant, 1 gênant, 3 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/guides__troubleshooting__link-errors.md) |
| guides/troubleshooting/no-sound | 4 (0 bloquant, 0 gênant, 4 mineur) | **partiel** : test n°34 cité, WSL marqué non testé ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__troubleshooting__no-sound.md) |
| guides/troubleshooting/runtime-crashes | 4 (0 bloquant, 1 gênant, 3 mineur) | **partiel** : sfText_create/sfSound_create : 3.0 seulement, test n°31/32 cités ; les autres lignes du rapport ne sont pas corrigées | [rapport](logs/audit-complet/guides__troubleshooting__runtime-crashes.md) |
| index | 4 (0 bloquant, 2 gênant, 2 mineur) | **non** (voir le rapport) | [rapport](logs/audit-complet/index.md) |
