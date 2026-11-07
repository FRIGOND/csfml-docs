# 06 — CSFML dans le contexte Epitech

Date : 2026-11-07. Voir `01-versions.md` §6 (version en environnement Epitech), `05-pieges.md` (pièges).
Règle de ce document : **aucun sujet recopié, aucune solution de projet.** Seulement des notions et des fonctions CSFML génériques.

## En bref

- Image de correction Epitech (2026-08-26) : Ubuntu 26.04 → `libcsfml-dev` **3.0.0** [CONFIRMÉ via Dockerfile, voir 01 §6] ; l'ancien Fedora / Ubuntu 24.04 → 2.5/2.6. Le code d'anciens étudiants (2.x) ne compile plus en 3.0.
- Sur la machine locale (Ubuntu 26.04), `epiclang`, `banana-check-repo` (paquet `banana-coding-style-checker` 20261006101948), `epitech-cpool` 1.22 sont installés [CONFIRMÉ : `dpkg -l`, 2026-11-07].
- Flags usuels : `-Wall -Wextra -I./include`, libs `-lcsfml-graphics -lcsfml-window -lcsfml-system` (+ `-lcsfml-audio` si son, `-lm` si maths). [DÉDUIT, voir exemples publics ci-dessous]
- Makefile attendu : règles `all`, `clean`, `fclean`, `re`, `$(NAME)` ; pas de relink inutile. [DÉDUIT des sujets publics ; « le sujet fait foi »]
- Coding style : le plugin banana expose les règles `C-A3, C-C1…C-C3, C-F2…C-F8, C-G1…C-G10, C-H1…C-H3, C-L1…C-L6, C-O3, C-V1, C-V3` (extraits des chaînes du `.so` ; [CONFIRMÉ] pour les identifiants, [DÉDUIT] pour leur sens).
- Pour une doc éthique : montrer des briques (ouvrir une fenêtre, charger une texture, animer un rectangle), jamais l'architecture d'un projet noté.

## 1. Flags de compilation

| Élément | Valeur usuelle | Source / étiquette |
|---|---|---|
| Warnings | `-Wall -Wextra` (souvent aussi `-Werror` en projet) | sujets publics, Makefile publics [secondaire] [DÉDUIT] |
| Includes | `-I./include` | idem |
| Libs CSFML | `-lcsfml-graphics -lcsfml-window -lcsfml-system` ; `-lcsfml-audio` ; `-lcsfml-network` | Makefile publics : ex. https://github.com/JustRomeo/DuckHunt/blob/main/Makefile (graphics+audio+system) [secondaire] ; https://github.com/Bazouz660/RaycasterProject/blob/main/Makefile (ajoute window) [secondaire] |
| Maths | `-lm` | [DÉDUIT] |
| Compilateur | `CC=clang` dans l'image de test (`CC=clang` du Dockerfile epitest-docker) | 01 §6 [CONFIRMÉ] |
| Includes dans le code | 3.0.0 : `#include <CSFML/Graphics.h>` ; 2.6.1 : `#include <SFML/Graphics.h>` | 04 §2.1 [CONFIRMÉ] |

Un Makefile de projet doit **fonctionner dans les deux versions** ? Non : choisir 3.0.0 (image 2026). Option : `pkg-config --cflags --libs csfml-graphics` (fichiers `.pc` présents, binaire `pkg-config` non installé localement, 01/02). Mais les sujets demandent souvent des `-l` explicites. [INCERTAIN]

## 2. Makefile attendu (principes, pas de solution)

- Règles obligatoires : `all`, `clean` (supprime les `.o`), `fclean` (supprime aussi le binaire), `re` (= `fclean` puis `all`). `NAME` = nom du binaire du sujet. [DÉDUIT des sujets publics ; ex. https://www.github.com/cyrilcolinet/MUL_my_hunter_2017]
- « Pas de relink » : si rien n'a changé, `make` ne doit rien recompiler : les `.o` dépendent des `.c`, le binaire des `.o` ; ne pas déclarer `all` en `.PHONY` à tort sur les fichiers. [DÉDUIT]
- Pas de `*` joker dans la liste des sources (interdit en Epitech) [DÉDUIT, usage connu ; INCERTAIN selon le projet].
- Dépôt Git : pas de `.o`, binaire, `libmy.a` commités (`.gitignore`). Cohérent avec le contexte de l'étudiant (lib dans `lib/my`).

## 3. Norme de codage (coding style) — règles utiles pour des exemples

Identifiants des règles du plugin : [CONFIRMÉ] (`strings /usr/lib/epiclang-plugin-banana.so.20261006101948`). Sens ci-dessous : [DÉDUIT] des documents publics (ex. https://github.com/ronanboiteau/NormEZ, https://github.com/doom/normoulinette, copie Scribd https://www.scribd.com/document/615699169/Epitech-c-Coding-Style) et de l'usage ; la version 3.x exacte est à vérifier par l'outil (`epiclang` sur un exemple, phase 2).

| Règle | Sens (résumé) | Effet sur nos exemples |
|---|---|---|
| C-F3 | ligne ≤ 80 colonnes (une tabulation compte plusieurs colonnes) | couper les appels CSFML longs |
| C-F4 | fonction ≤ 20 lignes | un exemple = plusieurs petites fonctions |
| C-F5 | ≤ 4 paramètres par fonction | utiliser une `struct` d'état |
| C-F6 | `void` explicite dans les prototypes sans paramètre | `int main(void)` |
| C-F2 | noms de fonctions en `snake_case` | nos exemples : `init_window`, pas `initWindow` |
| C-G1 | en-tête (header Epitech) en tête de fichier | exemples dans la doc : non obligatoire, signaler |
| C-G2 | une ligne vide entre fonctions ; C-G3 : indentation / accolades | style K&R ; C-G4 : pas de globales non constantes |
| C-V1 | nommage (types en `snake_case`…) | |
| C-H1…H3 | headers : include guard, pas de code dans `.h` | |
| C-L1…L6 | espacements / layout (une instruction par ligne, une déclaration par ligne…) | |
| C-O3 | pas plus de 5 fonctions par fichier | |
| C-A3 | fichier se termine par un saut de ligne | |

Les sens de G2–G10, L1–L6, V1/V3 ne sont pas vérifiés ici. [INCERTAIN] → phase 2 : lancer `epiclang` sur chaque exemple de la doc (le brief interdit de compiler en phase 1).
- Fonctions interdites typiques : `printf` n'est pas toujours autorisé (utiliser `write`/`my_putstr`), pas de `goto`, pas de `for`/`do…while`/`switch` selon le projet (varie : C-C…). [INCERTAIN]
- Attention aux exemples de doc : les macros et `static` sont encore soumis à la norme.

## 4. Projets Epitech utilisant CSFML — notions utiles (sans solution)

Les projets sont connus publiquement (my_hunter, my_runner, my_radar, my_paint, my_world, my_rpg, my_defender). Je ne reproduis aucun sujet. Les listes ci-dessous sont des **notions** CSFML typiquement utiles, à relier au chapitre de la doc ; elles sont [DÉDUIT] de dépôts publics d'étudiants [secondaire] et du modèle d'API (03).

| Projet | Notions CSFML génériques | Fonctions typiques (3.0.0 ; vérifiées dans 03) |
|---|---|---|
| my_hunter | fenêtre, sprites, clics souris, animation par rectangle, son, temps | `sfRenderWindow_create`, `sfSprite_setTextureRect`, `sfMouse_getPositionRenderWindow`, `sfClock_restart` |
| my_runner | boucle, delta time, défilement (parallax), texture répétée, musique | `sfClock_*`, `sfSprite_move`, `sfMusic_*` |
| my_radar | formes (`sfRectangleShape`, `sfCircleShape`), texte, grand nombre d'entités (perf), vue | `sf*Shape_*`, `sfText_*`, `sfView_*` |
| my_paint | `sfImage`, `sfTexture_updateFromImage`, souris continue, `sfRenderTexture`, enregistrement | `sfImage_*`, `sfRenderTexture_*` |
| my_world | pseudo-3D / isométrique avec `sfVertexArray` / `sfVertexBuffer` | `sfVertexArray_*` (3.0 : `sfTriangles` à la place de `sfQuads`, 04 §4.8) |
| my_rpg / my_defender | machine d'états, plusieurs scènes, UI (boutons), sauvegarde, sons | struct d'état, `sfFloatRect_contains` (voir 03), `sfView` |

Lien avec les pièges (05) : initialisation/nettoyage propre, destroy en ordre inverse, `pollEvent` en `while`, delta time, VSync/limit, chemins relatifs.

Fonctions autorisées typiques (tous projets CSFML Epitech, **le sujet fait foi**) : `malloc`, `free`, `memset`, `rand`, `srand`, `time`, `open`/`read`/`close`/`write`, `getline`, + « toutes les fonctions CSFML » (formulation du sujet my_hunter 2017 : https://www.github.com/cyrilcolinet/MUL_my_hunter_2017 [secondaire]). Toute autre fonction est de facto interdite. [DÉDUIT]. Conséquences pour la doc : éviter `printf`, `strlen`, etc. dans les exemples « destinés à Epitech » ou mettre un avertissement ; préférer `write(1, …)`. Version 2026 inconnue. [INCERTAIN]

## 5. Version CSFML en environnement Epitech (rappel de 01 §6)

- Image de correction `epitechcontent/epitest-docker` depuis 2026-08-26 : Ubuntu 26.04, `epitech-cpool 1.22` dépend de `libcsfml-dev` (3.0.0-1) [CONFIRMÉ, 01]. Test final : `docker run --rm epitechcontent/epitest-docker:latest dpkg -l libcsfml-dev` (P1).
- Postes étudiants : non confirmé. Fedora conserve 2.6.1 [CONFIRMÉ, 01 §5].
- Un étudiant peut donc avoir **2.6.1 en local et 3.0.0 chez le correcteur** : la doc doit proposer un tableau de migration, et un `#if __has_include(<CSFML/Graphics.h>)` pour du code bi-version [DÉDUIT, à tester] (04 §2.1).
- Ordre de lib : avec `-Wl,--as-needed`, mettre les `-l` après les sources.

## 6. Recommandations éthiques pour la doc

1. Pas de sujet recopié (les PDF sont privés à Epitech).
2. Exemples **génériques et petits** (≤ 40 lignes) : une fenêtre qui se ferme, un sprite animé par rectangle, un son. Pas de jeu complet.
3. Pas de code d'architecture de projet noté (pas de « squelette de my_hunter »). Les « recettes » portent sur un mécanisme (ex. « lire la position de la souris »), pas sur un livrable.
4. Encadré visible : « Cette documentation est communautaire, non officielle, non affiliée à Epitech/SFML ; tes projets doivent être ton travail. »
5. Les exemples respectent la norme par défaut (fonctions ≤ 20 lignes, 4 paramètres) et signalent quand ils ne la respectent pas.
6. Lister les fonctions autorisées par sujet : non, renvoyer au sujet.
7. Ne pas mentionner de moulinette (contenu des tests de correction) au-delà des sources publiques.

## Décisions prises
1. Version cible : 3.0.0, avec colonne 2.6.1 (01).
2. Sens des règles banana laissés [DÉDUIT] (outil local utilisable en phase 2 pour valider).
3. Aucun projet détaillé : seules les notions.

## Points [INCERTAIN]
- Sens exact des règles banana (version 2026) et liste des fonctions autorisées 2026-2027.
- Version CSFML des postes étudiants.
- Nom exact des modules CSFML imposés par chaque sujet.
- Présence de `pkg-config` dans l'image de correction (`PKG_CONFIG_PATH` est défini ; 01 §6).
