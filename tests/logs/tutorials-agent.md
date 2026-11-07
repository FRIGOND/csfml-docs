# Tutoriels casse-briques + pages de migration : compte rendu

Date : 2026-11-07. Aucun commit. Écrit uniquement dans `examples/tutorials/**`, `site/src/content/docs/{3-0,2-6}/tutorials/**`,
`site/src/content/docs/3-0/migration/**` et ce fichier.

## 1. Tutoriels (9 étapes, 3.0 et 2.6)

Dossiers `examples/tutorials/<NN>-<nom>/` (Makefile = `include ../../common/example.mk`, `meta.json`, `3.0/main.c`, `2.6/main.c`).
Chaque étape est un programme complet. Le code 2.6 est dérivé du code 3.0 par un script de travail (hors dépôt) puis relu/compilé séparément.

| Étape | 3.0 | 2.6 | valgrind | Vérification en mode test (EXAMPLE_FRAMES=60) |
|---|---|---|---|---|
| 01-window | OK | OK | OK | taille de fenêtre 800x600 |
| 02-shapes | OK | OK | OK | dernière brique dans la fenêtre au-dessus de la raquette, balle centrée |
| 03-movement | OK | OK | OK | balle dans la fenêtre, au moins 1 rebond (dt fixe 1/60) |
| 04-input | OK | OK | OK | raquette dans la fenêtre et déplacée (touches simulées par le code) |
| 05-collisions | OK | OK | OK | >= 1 brique détruite, vivantes + détruites = 50 (raquette = IA) |
| 06-sprites | OK | OK | OK | + texture de la balle : centre opaque, coin transparent, 16x16 |
| 07-text-score | OK | OK | OK | + texte "Score : N" = 10 x briques, 1er caractère du titre = U+00C9 |
| 08-sound-states | OK | OK | OK | état final PERDU, 1 son par brique cassée, buffer > 0 ms |
| 09-final | OK | OK | OK | partie scriptée gagnée : 50 briques, score 500, vies > 0, 50 sons |

- `tools/run-all-examples.sh --only tutorials/` : 18 réussis, 0 échoué. `--valgrind --only tutorials/` : 18 réussis (supp `tests/valgrind.supp` pour la fuite FreeType).
- Contrôle par script : toutes les fonctions/types/constantes `sf*` de chaque `main.c` existent dans le JSON de la bonne version et `deprecated == false` ; lignes <= 80 colonnes ; fonctions <= 20 lignes (corps) ; 0 problème.
- `-std=c11 -Wall -Wextra -Werror` (2.6 : `-Wno-error=deprecated-declarations` ajouté par `example.mk`).
- NB : `run-all-examples.sh` réécrit `tests/logs/examples.log` à chaque lancement (il ne contient donc plus que les tutoriels après mon dernier passage).

### Surprises / décisions
- 09 : pour qu'une partie entière tienne dans 60 images, chaque image joue 240 pas de simulation à 1/60 s en mode test (`TEST_SPEEDUP`). Il faut au moins ~30 images (20 : non gagné, 30 : gagné). La raquette scriptée vise `balle + décalage` (le décalage change à chaque renvoi) : sans décalage la balle ne ferait que monter/descendre.
- 08 : le test vérifie la **défaite** (raquette qui fuit à gauche), 09 vérifie la **victoire**. Le statut d'un son n'est jamais testé (sans carte son, `sfMusic_getStatus` diffère entre 3.0.0 et 2.6.1 : RESULTATS lot 4 n°34).
- `doodle_pop.ogg` dure 24 002 ms (ce n'est pas un vrai "pop" court) ; `sfSound_play` sur un son qui joue déjà le relance.
- 2.6 : pas de `sfChar32` (type `sfUint32` pour `sfText_setUnicodeString`/`getUnicodeString`) ; `sfSprite_create()`+`setTexture(…, sfTrue)`, `sfText_create()`+`setFont`, `sfSound_create()`+`setBuffer`, `sfImage_*`/`getPixel` avec scalaires, `sfFloatRect.left/top/width/height`, création de fenêtre à 4 paramètres.
- Texte accentué : `U"Éclate-les toutes !"` (littéral UTF-8 source, type char32_t = uint32_t sous Linux/glibc) passé à `sfText_setUnicodeString` sans avertissement en 3.0 et 2.6.
- Les `build/` produits par le runner ont été supprimés ; aucun fichier temporaire dans `examples/tutorials/`.

### Points non vérifiés (marqués `:::caution[Non vérifié]` dans les pages)
- `sfKeyboard_isKeyPressed` avec de vraies touches (Xvfb : aucune touche) ; seules les touches simulées sont exercées.
- Le test "sfText_setString + UTF-8 cassé" a été fait en 3.0.0 seulement (RESULTATS n°18) ; non refait en 2.6.
- Pas de mesure en environnement avec vraie carte son/GPU (tout sous Xvfb + rendu logiciel).

## 2. Pages de tutoriel

`site/src/content/docs/{3-0,2-6}/tutorials/NN-nom.mdx` x 9 (18 pages), même plan : objectif, prérequis, pas à pas, programme complet
(`<Example example="tutorials/NN-nom" version="…" />`), compiler/lancer, ce que tu as appris, erreurs fréquentes (liens vers `/guides/...`),
exercices sans corrigé, étape précédente/suivante, page jumelle, `<TestedBadge>` une seule fois. Les pages 01-window existantes sont remplacées (mêmes slugs).
Générées par un script de travail (hors dépôt) : tous les extraits de code des pages sont **vérifiés ligne à ligne** contre le `main.c` correspondant (le script s'arrête en cas d'écart), tous les liens de fonctions pointent vers la page de référence de la bonne version.

## 3. Migration 2.6 -> 3.0 (`site/src/content/docs/3-0/migration/`)

- `overview.mdx` (réécrit) : 10 changements avec avant/après en `<Tabs syncKey="csfml-version">`, tableau ancien -> nouveau (signatures copiées du JSON), ce qui ne change pas, exemple `window/basic-window` dans les deux versions.
- `events.mdx`, `objects-creation.mdx`, `types.mdx`, `checklist.mdx`, `support-both.mdx` (nouveaux). `api-changes.mdx` (annexe) est générée par le script de l'utilisateur : non touchée, mais liée.
- Vérifications (script, échec si faux) : fonctions supprimées/renommées absentes de 3.0 et présentes en 2.6 ; champs de structures (`sfVideoMode`, `sfContextSettings`, événements, rectangles, `sfRenderStates`) lus dans les JSON ; énumérations (types d'événements, touches, boutons souris, `sfPrimitiveType`) ; types de `sfInputStream*`.
- Les 8 paires avant/après de `research/04` §4 sont reprises **verbatim** (déjà compilées : tests/RESULTATS-migration.md, 16/16). Les extraits ajoutés (`tests`: wait, mouse, texture, types, transform, settings, handle, view) ont été compilés en 2.6.1 (avant) et 3.0.0 (après), `-std=c11 -Wall -Wextra`, 0 avertissement/erreur hors l'avertissement `deprecated` du header 2.6 (scratchpad `mig/before.c`, `mig/after.c`).
- `support-both.mdx` : `compat.h` + `main.c` compilés et exécutés sous Xvfb avec 3.0.0 et 2.6.1 (`-DAPP_CSFML=3|2` puis détection `__has_include`, `-Werror`), sortie `CSFML 3 : bord droit du sprite = 16` / `CSFML 2 : … = 16`.
  - **Piège constaté** : avec `-I~/csfml-2.6/include` et le paquet système CSFML 3 installé, tester `<CSFML/Config.h>` d'abord choisit la 3.0 (compile sans erreur avec les mauvais headers). Le fichier teste `<SFML/Config.h>` en premier ; `-DAPP_CSFML` recommandé.
  - Non compilé (indiqué dans la page) : fonction `app_event_x` d'exemple pour les événements.
- Les commandes `sed` de la checklist ont été exécutées sur un fichier d'essai (include, `sfBool`, `sfUintN`) : résultat conforme.
- Les 3 écarts du rapport déjà corrigés par `research/CORRECTIONS.md` sont respectés : `{800, 600, 32}` = simple avertissement `-Wmissing-braces` (pas un mauvais résultat) ; NULL -> assert (paquet Ubuntu) / segfault (Release local) ; `destroy(NULL)` sûr en 3.0.0, `sfRenderTexture_destroy(NULL)` plante en 2.6.1.
- Marqué non vérifié : `sfEvtMouseMovedRaw` (jamais exécuté), `sfSoundBuffer_createFromSamples`/`sfSoundStream_create` avec carte des canaux (non compilé), effet de `sfRenderStates` à zéro, exemple de flux d'entrée.

## 4. Validation du site

`cd site && source ../tools/env.sh && npm run build && npm run check` : build OK (559 pages, tous les liens internes valides, validateur de liens Starlight), `check` : 0 problème.
