# Audit automatique des noms et signatures

Généré par `python3 tools/audit-pages.py` (relançable). ERREUR = à corriger ; A VERIFIER = vérifier à la main ; INFO = non contrôlable ici.

## 2-6/getting-started/first-window.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/01-window.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/02-shapes.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/03-movement.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/04-input.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/05-collisions.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/06-sprites.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/07-text-score.mdx  (contexte : 2.6)
- **OK** `sfChar32` n'existe qu'en 3.0 — voulu : dit que le type n'existe pas en 2.6

## 2-6/tutorials/08-sound-states.mdx  (contexte : 2.6)
- aucun problème détecté

## 2-6/tutorials/09-final.mdx  (contexte : 2.6)
- aucun problème détecté

## 3-0/getting-started/first-window.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/migration/checklist.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## 3-0/migration/events.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## 3-0/migration/objects-creation.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## 3-0/migration/overview.mdx  (contexte : 3.0, 2.6)
- **OK** `sfShader_set` absent des deux versions — voulu : famille sfShader_set*Parameter (motif avec *)
- **OK** `sfTexture_update` absent des deux versions — voulu : famille sfTexture_update* (motif avec *)

## 3-0/migration/support-both.mdx  (contexte : 3.0, 2.6)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DAPP_CSFML` (non contrôlable avec les JSON : voir tests/RESULTATS.md)

## 3-0/migration/types.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## 3-0/tutorials/01-window.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/02-shapes.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/03-movement.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/04-input.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/05-collisions.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/06-sprites.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/07-text-score.mdx  (contexte : 3.0)
- aucun problème détecté

## 3-0/tutorials/08-sound-states.mdx  (contexte : 3.0)
- **OK** `sfKeyReturn` n'existe qu'en 2.6 — voulu : dit que l'ancien nom a été supprimé en 3.0

## 3-0/tutorials/09-final.mdx  (contexte : 3.0)
- aucun problème détecté

## en/2-6/getting-started/first-window.mdx  (contexte : 2.6)
- aucun problème détecté

## en/3-0/getting-started/first-window.mdx  (contexte : 3.0)
- aucun problème détecté

## en/index.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/concepts/api-conventions.mdx  (contexte : 3.0, 2.6)
- **OK** `sfBool` n'existe qu'en 2.6 [onglet 3.0] — voulu : onglet 3.0 : dit que sfBool n'existe plus
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)
- **OK** `sfFalse` n'existe qu'en 2.6 [onglet 3.0] — voulu : onglet 3.0 : dit que sfFalse n'existe plus
- **A VERIFIER** `sfInt64` n'existe qu'en 2.6 (page commune : doit le dire)
- **OK** `sfTrue` n'existe qu'en 2.6 [onglet 3.0] — voulu : onglet 3.0 : dit que sfTrue n'existe plus
- **OK** `sfType_action` absent des deux versions — voulu : nom générique (motif)
- **A VERIFIER** `sfUint32` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfWindowState` n'existe qu'en 3.0 (page commune : doit le dire)
- **OK** `sfX_create` absent des deux versions — voulu : nom générique (motif)
- **OK** `sfX_createFromFile` absent des deux versions — voulu : nom générique (motif)
- **OK** `sfX_destroy` absent des deux versions — voulu : nom générique (motif)

## guides/concepts/audio-without-device.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/concepts/coordinates-views.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/concepts/events.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfEvtFocusGained` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfEvtFocusLost` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfEvtGainedFocus` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfEvtLostFocus` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfEvtMouseMovedRaw` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfEvtMouseWheelMoved` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfMouseWheelEvent` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/concepts/game-loop.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfFalse` n'existe qu'en 2.6 (page commune : doit le dire)
- **OK** `sfRenderWindow_draw` absent des deux versions — voulu : famille sfRenderWindow_draw… (motif)

## guides/concepts/index.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/concepts/memory.mdx  (contexte : 3.0, 2.6)
- **INFO** option CMake `-DNDEBUG` (non contrôlable avec les JSON : voir tests/RESULTATS.md)

## guides/concepts/ownership.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/concepts/resources.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfChar32` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfUint32` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/concepts/time.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfClock_reset` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfInt32` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/epitech/before-you-submit.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/epitech/coding-style.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfWindowed` n'existe qu'en 3.0 (page commune : doit le dire)

## guides/epitech/environment.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/epitech/index.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/epitech/makefile.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/epitech/project-notions.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfQuads` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/help/about.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/help/faq.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfMouseWheelEvent` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/help/glossary.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/index.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/install/arch.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/install/check-your-setup.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfWindowed` n'existe qu'en 3.0 (page commune : doit le dire)
- **OK** appel `sfRenderWindow_create` avec 5 argument(s), 4 attendu(s) en 2.6 — voulu : branches #if/#else : 3.0 puis 2.6
- **OK** appel `sfRenderWindow_create` avec 4 argument(s), 5 attendu(s) en 3.0 — voulu : branches #if/#else : 3.0 puis 2.6

## guides/install/compiling.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/install/fedora.mdx  (contexte : 3.0, 2.6)
- **INFO** option CMake `-DCMAKE_INSTALL_LIBDIR` (non contrôlable avec les JSON : voir tests/RESULTATS.md)

## guides/install/from-source.mdx  (contexte : 3.0, 2.6)
- **INFO** option CMake `-DCMAKE_BUILD_TYPE` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DBUILD_SHARED_LIBS` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_INSTALL_PREFIX` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_INSTALL_LIBDIR` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_INSTALL_RPATH` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_INSTALL_RPATH` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DSFML_DIR` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_POLICY_VERSION_MINIMUM` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DBUILD_SHARED_LIBS` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCSFML_LINK_SFML_STATICALLY` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DCMAKE_INSTALL_RPATH` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DSFML_DIR` (non contrôlable avec les JSON : voir tests/RESULTATS.md)
- **INFO** option CMake `-DSFML_ROOT` (non contrôlable avec les JSON : voir tests/RESULTATS.md)

## guides/install/index.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/install/macos-windows.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/install/ubuntu-debian.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/install/wsl.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/troubleshooting/black-window.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/troubleshooting/compile-errors.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfFalse` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfInt64` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfMouseWheelEvent` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfSound_setCone` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfSound_setLoop` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfSound_setLooping` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfTrue` n'existe qu'en 2.6 (page commune : doit le dire)
- **A VERIFIER** `sfWindowState` n'existe qu'en 3.0 (page commune : doit le dire)
- **A VERIFIER** `sfWindowed` n'existe qu'en 3.0 (page commune : doit le dire)

## guides/troubleshooting/headless-xvfb.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfEvtFocusGained` n'existe qu'en 3.0 (page commune : doit le dire)

## guides/troubleshooting/index.mdx  (contexte : 3.0, 2.6)
- **A VERIFIER** `sfBool` n'existe qu'en 2.6 (page commune : doit le dire)

## guides/troubleshooting/link-errors.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/troubleshooting/no-sound.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté

## guides/troubleshooting/runtime-crashes.mdx  (contexte : 3.0, 2.6)
- **INFO** option CMake `-DNDEBUG` (non contrôlable avec les JSON : voir tests/RESULTATS.md)

## index.mdx  (contexte : 3.0, 2.6)
- aucun problème détecté


## Chiffres recomptés depuis les JSON

| Page | Dans la page | Recompté | Sens | Verdict |
|---|---|---|---|---|
| 3-0/migration/types.mdx | 140 | 140 | fonctions dont seul le type change (sfBool→bool, sfUint8→uint8_t…) | OK |
| 3-0/migration/overview.mdx | 140 | 140 | fonctions dont seul le type change | OK |
| 3-0/migration/objects-creation.mdx | 40 | 40 | fonctions *_destroy 3.0 | OK |
| guides/help/about.mdx | 926 | 926 | fonctions 3.0 | OK |
| guides/help/about.mdx | 850 | 850 | fonctions 2.6 | OK |
| guides/concepts/memory.mdx | 40 | 40 | fonctions *_destroy 3.0 | OK |
| guides/concepts/memory.mdx | 42 | 42 | fonctions *_destroy 2.6 | OK |
| guides/troubleshooting/runtime-crashes.mdx | 40 | 40 | fonctions *_destroy 3.0 | OK |

Total : 66 pages, 0 ERREUR.
