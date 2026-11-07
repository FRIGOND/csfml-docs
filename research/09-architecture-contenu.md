# 09 — Architecture du contenu

> Mission 9. Rédigé par l'agent principal le 2026-11-07, à partir de la liste réelle des headers
> CSFML 3.0.0 (`git ls-tree 3.0.0 include/` sur https://github.com/SFML/CSFML) et des livrables 01 à 08.
> Ce fichier ne dépend pas de l'outil de site. Le placement des fichiers dans l'outil retenu est décrit dans
> [08-architecture-site.md](08-architecture-site.md).

## En bref

- **Neuf sections**, de la plus guidée à la plus technique : Démarrer → Concepts → Tutoriels → Recettes →
  Référence → Migration 2→3 → Dépannage → Spécial Epitech → Glossaire. Plus une page « À propos / méthode ».
- **Versions : CSFML 3.0.0 et 2.6.1 à poids égal** (décision utilisateur), deux jeux d'exemples compilés en CI.
- **Référence : une page par header ou par type**, pas une page par fonction. Environ 70 pages pour environ
  950 déclarations d'API (comptage brut : System 22, Window 103, Graphics 485, Audio 191, Network 148 ;
  chiffre exact dans [03-api-inventaire.md](03-api-inventaire.md)). [CONFIRMÉ, comptage `git grep` sur le tag 3.0.0]
- **Chaque exemple est un vrai fichier `.c`** compilé et exécuté en CI, inclus dans la page. Il n'est
  jamais recopié à la main.
- **Gabarits fixes** pour les cinq types de pages (référence, tutoriel, recette, dépannage, concept), avec
  un frontmatter commun qui permet les vérifications automatiques (couverture, version testée).
- **Fil rouge des tutoriels : un casse-briques.** Il ne correspond à aucun projet Epitech connu (my_hunter,
  my_runner, my_radar, my_defender, my_paint, my_world, my_rpg), pour ne pas fournir de solution notée.
- **Bilingue :** le français est rédigé en premier, l'anglais est traduit. Les slugs (chemins d'URL) sont
  en anglais et identiques dans les deux langues.

## 1. Arborescence

Les chemins sont donnés sans le préfixe de langue (`/fr/`, `/en/`) et sans le préfixe de version.

```text
/                                   Accueil : promesse, version testée, 3 boutons (Installer, Tutoriel 1, Référence)
├── getting-started/                DÉMARRER
│   ├── what-is-csfml               CSFML vs SFML, les 5 modules, quelle version choisir
│   ├── install/
│   │   ├── ubuntu-debian           apt (libcsfml-dev)
│   │   ├── fedora                  dnf
│   │   ├── arch                    pacman
│   │   ├── macos                   brew
│   │   ├── windows                 binaires officiels / MSYS2
│   │   ├── wsl                     WSLg, limites
│   │   └── from-source             SFML puis CSFML, CMake, préfixe local, sans sudo
│   ├── first-program               fenêtre qui s'ouvre, boucle, fermeture propre
│   ├── compiling                   gcc, ordre des -l, Makefile minimal, pkg-config, rpath
│   └── check-your-setup            programme de diagnostic (version, fenêtre, police, son)
├── concepts/                       CONCEPTS
│   ├── api-conventions             sf*, create/copy/destroy, types opaques, passage par valeur, bool, NULL
│   ├── ownership-lifetime          qui possède quoi : texture/sprite, font/text, buffer/sound, music (stream)
│   ├── game-loop                   événements → mise à jour → dessin → display
│   ├── events                      sfEvent, polling vs attente, vider la file à chaque frame
│   ├── time                        sfTime, sfClock, delta time, pas de temps fixe
│   ├── coordinates-transforms      pixels vs coordonnées monde, origine, rotation, échelle, sfTransform
│   ├── views                       sfView, caméra, redimensionnement, viewport
│   ├── drawing                     draw, sfRenderStates, blend modes, vertex arrays, ordre de dessin
│   ├── resources                   textures, images, polices, sons : chargement unique, chemins relatifs
│   ├── audio-model                 sfSound vs sfMusic, buffers, spatialisation, absence de périphérique
│   ├── networking-model            sockets bloquants/non bloquants, sfPacket, sélecteur
│   └── errors                      valeurs de retour, NULL, messages SFML sur stderr
├── tutorials/                      TUTORIELS PROGRESSIFS (fil rouge : casse-briques)
│   ├── 01-window                   fenêtre, boucle, fermeture
│   ├── 02-shapes-colors            sfRectangleShape, sfCircleShape, sfColor
│   ├── 03-movement-delta-time      sfClock, vitesse en pixels/seconde
│   ├── 04-input                    clavier (état temps réel vs événement), souris
│   ├── 05-collisions               sfFloatRect, intersections, rebonds
│   ├── 06-textures-sprites         sfTexture, sfSprite, rectangle de texture
│   ├── 07-animation                spritesheet, minuterie d'animation
│   ├── 08-text-fonts               sfFont, sfText, score, centrer un texte
│   ├── 09-sound-music              sfSoundBuffer, sfSound, sfMusic
│   ├── 10-game-states              menu / jeu / pause / fin, machine à états en C
│   ├── 11-camera-views             sfView, suivi, interface fixe
│   ├── 12-project-structure        structs, découpage en fichiers, Makefile, libération propre
│   ├── 13-render-texture-shaders   (avancé) sfRenderTexture, sfShader
│   └── 14-networking               (avancé) petit échange UDP/TCP
├── recipes/                        RECETTES (une tâche = une page courte)
│   ├── center-text                 ├── fps-counter              ├── frame-limit-vsync
│   ├── fullscreen-resize           ├── mouse-to-world           ├── clickable-button
│   ├── spritesheet-animation       ├── parallax-background      ├── tilemap-vertex-array
│   ├── particles-vertex-array      ├── screenshot               ├── window-icon
│   ├── custom-cursor               ├── loop-music               ├── many-sounds-at-once
│   ├── volume-control              ├── timer-cooldown           ├── random-numbers
│   ├── circle-rect-collision       ├── load-assets-safely       ├── save-high-score
│   ├── gamepad-input               ├── text-input               └── headless-run-xvfb
├── reference/                      RÉFÉRENCE (squelette généré depuis les headers)
│   ├── index                       index alphabétique de toutes les fonctions et types (généré)
│   ├── system/                     clock, time, vector2, vector3, sleep, input-stream, buffer, alloc, types
│   ├── window/                     window, window-base, video-mode, event, keyboard, mouse, joystick,
│   │                               touch, sensor, clipboard, cursor, context, vulkan, window-handle
│   ├── graphics/                   render-window, render-texture, texture, image, sprite, text, font,
│   │                               glyph-fontinfo, shape, circle-shape, rectangle-shape, convex-shape,
│   │                               transformable, transform, view, color, rect, vertex, vertex-array,
│   │                               vertex-buffer, primitive-type, render-states, blend-mode,
│   │                               stencil-mode, shader, glsl, coordinate-type
│   ├── audio/                      music, sound, sound-buffer, sound-stream, sound-recorder,
│   │                               sound-buffer-recorder, listener, sound-source-cone, sound-channel,
│   │                               sound-status, effect-processor
│   ├── network/                    ip-address, packet, tcp-socket, tcp-listener, udp-socket,
│   │                               socket-selector, socket-status, http, ftp
│   └── config                      Config.h, macros de version, GPUPreference.h, OpenGL.h
├── migration/                      MIGRATION 2 → 3
│   ├── overview                    pourquoi ça casse, stratégie, ordre conseillé
│   ├── cheat-sheet                 tableau « ancien → nouveau » complet (repris de 04)
│   ├── events                      la nouvelle boucle d'événements, avant/après
│   ├── objects-creation            sprite, texte, fenêtre, texture : nouvelles signatures
│   ├── types                       sfBool → bool, entiers, angles, vecteurs
│   ├── audio / network             changements spécifiques
│   ├── checklist                   checklist de migration pas à pas
│   └── support-both                compiler pour 2.6 et 3.0 (macros CSFML_VERSION_MAJOR)
├── troubleshooting/                DÉPANNAGE (une page par symptôme, titres = message d'erreur)
│   ├── compile-errors              en-tête introuvable, type inconnu (mélange 2.x/3.x)
│   ├── link-errors                 undefined reference, ordre des -l, cannot open shared object
│   ├── runtime-crashes             segfault à la création (NULL non testé), use-after-free
│   ├── black-or-frozen-window      clear/display oubliés, événements non vidés
│   ├── no-sound                    pas de périphérique audio, formats
│   ├── memory-leaks-valgrind       vraies fuites vs bruit des drivers, fichier de suppressions
│   ├── wsl-wayland-macos           particularités par plateforme
│   └── headless-ci                 xvfb, Mesa llvmpipe
├── epitech/                        SPÉCIAL EPITECH
│   ├── environment                 version de CSFML probable, image Docker, vérifier chez soi
│   ├── makefile-flags              règles attendues, flags, relink
│   ├── coding-style-with-csfml     découper la boucle, structs, 20 lignes par fonction
│   ├── allowed-functions           principe, « le sujet fait foi »
│   ├── before-you-submit           valgrind, assets, clone vierge, environnement de correction
│   └── project-concepts            my_hunter, my_runner… : notions utiles et liens, SANS solution
├── glossary                        GLOSSAIRE FR/EN (sprite, texture, delta time, blit, viewport…)
└── about/
    ├── method                      comment la doc est écrite (IA) et vérifiée (CI), limites
    ├── tested-versions             matrice des versions testées (générée par la CI)
    ├── contributing                signaler une erreur, proposer un exemple
    └── license                     licence du contenu et des exemples, attribution SFML
```

### Regroupements dans la référence (décision)

Les 85 headers ne donnent pas 85 pages. Règles retenues :

| Cas | Traitement |
|---|---|
| `Export.h`, `Types.h` de chaque module | Pas de page. Leurs types opaques sont listés sur la page du type concerné. |
| Petits headers d'énumération (`SoundStatus.h`, `SocketStatus.h`, `PrimitiveType.h`, `CoordinateType.h`, `SoundChannel.h`) | Page courte, ou section dans la page du type principal qui les utilise (choix à la rédaction, noté dans le frontmatter). |
| `Glyph.h` + `FontInfo.h` | Une seule page `glyph-fontinfo`, liée depuis `font`. |
| `JoystickIdentification.h` | Section de la page `joystick`. |
| `Config.h`, `GPUPreference.h`, `OpenGL.h` | Page `reference/config`. |
| `Alloc.h` (`sfFree` ou équivalent) | Page courte `alloc`, très liée à `concepts/ownership-lifetime`. Contenu à confirmer par [03](03-api-inventaire.md). |

## 2. Gabarits de pages

Tous les gabarits commencent par le même frontmatter.

```yaml
---
title: sfSprite
description: Afficher une texture (ou une partie) avec position, rotation et échelle.
csfml: "3.0.0"                 # version dont les signatures sont tirées
tested_on: ["csfml-3.0.0/ubuntu-26.04"]   # rempli par la CI, pas à la main
functions:                     # fonctions couvertes par la page (vérifié contre les headers)
  - sfSprite_create
  - sfSprite_destroy
examples:                      # fichiers .c réellement compilés
  - examples/reference/graphics/sprite_basic.c
since: "2.0"                   # première version de CSFML où le type existe (si connu)
status: draft | reviewed | verified
last_verified: 2026-11-07      # mis à jour par la CI
---
```

Le contrôle automatique (voir [10-workflow.md](10-workflow.md)) vérifie que chaque nom dans `functions`
existe dans les headers de la version `csfml`, et que chaque fonction des headers apparaît dans au moins
une page de référence.

### 2.1 Page de référence (un type ou un header)

1. **Résumé** : 2 ou 3 phrases. À quoi ça sert, quand l'utiliser.
2. **En-tête** : la ligne `#include <CSFML/Graphics/Sprite.h>` et les bibliothèques à lier.
3. **Cycle de vie** : tableau création / copie / destruction, et ce qui doit vivre plus longtemps que
   l'objet (ex. la texture d'un sprite).
4. **Exemple minimal compilable** : un programme complet, court, inclus depuis `examples/`.
5. **Fonctions**, groupées par thème (création, position, apparence, requêtes…). Pour chacune :
   - signature exacte, **générée** depuis le header (jamais tapée à la main) ;
   - description en une phrase ;
   - tableau des paramètres (nom, type, rôle, unité, valeurs valides) ;
   - valeur de retour, et qui la libère si c'est un pointeur ;
   - remarques : ownership, coût, pièges ;
   - « Depuis » et « Modifié en 3.0 » si la fonction a changé (lien vers la migration).
6. **Erreurs fréquentes** : 2 à 5 puces, avec un lien vers la page de dépannage.
7. **Voir aussi** : concepts, recettes et tutoriels liés.
8. **Pied de page** : badge « Testé sur CSFML x.y.z », date, lien « Signaler une erreur ».

### 2.2 Tutoriel

Objectif de l'étape → prérequis (tutoriel précédent) → code de départ (fichier de l'étape précédente) →
étapes numérotées, chacune avec un extrait du fichier final → programme complet de l'étape (inclus,
compilé en CI) → « Ce que tu as appris » → exercices facultatifs sans corrigé → étape suivante.

### 2.3 Recette

Problème (une phrase) → solution courte (extrait) → programme complet (inclus) → explication →
variantes → pièges → fonctions utilisées (liens vers la référence).

### 2.4 Page de dépannage

Titre = le symptôme ou le message d'erreur exact (pour être trouvé par recherche) → contexte (version,
plateforme) → cause → solution pas à pas → comment vérifier que c'est réglé → liens.

### 2.5 Page de concept

Idée en une phrase → schéma si utile → explication → mini-exemple → erreurs classiques → liens vers la
référence.

## 3. Exemples de code : règles

- Un exemple = un fichier `.c` autonome dans `examples/`, rangé comme la page qui l'utilise.
- Il compile avec `-std=c11 -Wall -Wextra -Werror` (à confirmer en phase 2, voir [A-TESTER.md](A-TESTER.md)).
- Il accepte un mode test (variable d'environnement ou argument) qui ferme la fenêtre après N frames et
  peut sauvegarder une capture. Le mécanisme exact est décrit dans [10-workflow.md](10-workflow.md).
- Il teste chaque retour `NULL` des fonctions `create*` et libère tout ce qu'il crée.
- Les assets (image, police, son) sont libres de droits, stockés une seule fois dans `examples/assets/`,
  avec leur licence.
- Les exemples respectent autant que possible la norme Epitech (fonctions courtes, 80 colonnes), sans
  en faire une obligation pour les exemples avancés. Décision : la doc reste lisible par un public non
  Epitech. Voir [06-epitech.md](06-epitech.md).

## 4. Versions dans le contenu

Décision de l'utilisateur (reprise du 2026-11-07) : **CSFML 2.6.1 et 3.0.0 ont un poids égal.** Raisons :
Fedora reste en CSFML 2.6.1 [CONFIRMÉ, voir 01 §5] et aucune doc API CSFML n'existe en ligne, ni pour 2.6 ni
pour 3.0 [CONFIRMÉ, voir 07].

| Version | Place dans le site | Contenu |
|---|---|---|
| CSFML 3.0.0 | Version de première classe | Toutes les sections |
| CSFML 2.6.1 | Version de première classe (même arborescence, sélecteur de version) | Démarrer, Concepts, Tutoriels, Recettes, Référence (générée depuis les headers 2.6.1), Dépannage, Epitech |
| CSFML master / future 3.1 | Aucune page tant qu'il n'y a pas de release | Encadré « À venir » sur l'accueil. Ajouts de master : voir [04](04-migration-2-vers-3.md) §6. |

Conséquences pour la rédaction :

- Les tutoriels et recettes existent en **deux jeux de fichiers `.c`** (`examples/3.0/…`, `examples/2.6/…`),
  tous deux compilés en CI. La page affiche le bon selon la version choisie. Les concepts qui ne changent pas
  (boucle de jeu, temps, coordonnées) sont partagés, avec des encadrés « 2.6 » et « 3.0 » aux endroits qui
  diffèrent (événements, `sfBool`, création des sprites et textes).
- Chaque page de référence porte `csfml:` et `tested_on:` **par version**.
- La section Migration devient le pont entre les deux versions, pas une section « legacy ».
- Le travail de rédaction est donc à peu près doublé pour les exemples. Priorité : rédiger d'abord en 3.0,
  puis dériver la variante 2.6 à l'aide de la table de migration de [04](04-migration-2-vers-3.md).

## 5. Ordre de rédaction proposé (phase 3)

1. Démarrer (installation Ubuntu + sources, premier programme, compiler) : c'est ce qui bloque le plus.
2. Concepts : conventions, ownership, boucle, événements, temps.
3. Migration 2 → 3 (fort besoin, peu d'existant).
4. Dépannage (lien, exécution, son, WSL).
5. Tutoriels 1 à 9.
6. Référence Graphics et Window (les plus utilisés), puis System, Audio, Network.
7. Recettes, Spécial Epitech, Glossaire.
8. Tutoriels avancés 10 à 14.

## Décisions prises

- **3.0.0 et 2.6.1 à poids égal** (décision de l'utilisateur, reprise du 2026-11-07), car Fedora est encore en
  2.6.1 et aucune doc API n'existe en ligne pour 2.6. 3.0.0 est la dernière release stable (voir
  [01-versions.md](01-versions.md)) et sert de version rédigée en premier.
- **Une page par header / type** dans la référence : une page par fonction donnerait environ 950 pages
  très pauvres, difficiles à relire.
- **Slugs en anglais, identiques dans les deux langues** : liens stables, génération plus simple, et la
  traduction ne casse pas les URLs.
- **Le français d'abord** : public principal = étudiants francophones (Epitech). Une traduction anglaise
  vient ensuite pour la portée communautaire.
- **Casse-briques comme fil rouge**, pour ne pas reproduire un projet Epitech noté.
- **La section Epitech donne des notions et des liens, jamais de solution de projet.**

## Points [INCERTAIN]

- La mise en œuvre du sélecteur de version dépend de l'outil choisi dans [08-architecture-site.md](08-architecture-site.md).
- Le nombre exact de déclarations d'API (environ 950) vient d'un `git grep` brut. Le chiffre de référence
  est celui de [03-api-inventaire.md](03-api-inventaire.md).
- Le flag `-std=c11` pour les exemples est une hypothèse. CSFML 3 exige peut-être un standard C précis
  (voir [04](04-migration-2-vers-3.md) et [A-TESTER.md](A-TESTER.md)).
