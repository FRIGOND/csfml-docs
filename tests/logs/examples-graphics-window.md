# Exemples graphics + window : compte rendu

16 exemples ecrits, chacun en 3.0 et 2.6 (dossiers `examples/window/<nom>/` et `examples/graphics/<nom>/`,
avec Makefile, meta.json, 3.0/main.c, 2.6/main.c). Chaque fonction CSFML appelee a ete verifiee dans
`api/csfml-3.0.0.json` / `api/csfml-2.6.1.json` (script : existence + `deprecated == false`, 0 probleme).
Regles verifiees par script : lignes <= 80 colonnes, fonctions <= 20 lignes. Compilation `-Wall -Wextra -Werror`.

## Validation
`tools/run-all-examples.sh --only window/` et `--only graphics/` : tout OK (3.0 et 2.6).
`tools/run-all-examples.sh --valgrind` : tout OK sauf `graphics/text` (voir plus bas, bug freetype connu).

| Exemple | 3.0 | 2.6 | valgrind | Verification en mode test (EXAMPLE_FRAMES>=0) |
|---|---|---|---|---|
| window/events | OK | OK | OK | affichage seul (Resized 800x600 recu au demarrage) |
| window/keyboard-realtime | OK | OK | OK | affichage seul (pas de touche sous Xvfb) |
| window/mouse-position | OK | OK | OK | affichage seul |
| window/video-mode | OK | OK | OK | bureau non nul, 1er mode plein ecran valide |
| window/window-settings | OK | OK | OK | getSize == 500x400 apres setSize |
| window/cursor | OK | OK | OK | les 3 curseurs sont crees |
| graphics/shapes | OK | OK | OK | 4 pixels (rect, contour, cercle, convexe) |
| graphics/sprite-texture | OK | OK | OK | pixel vert du logo a la position donnee par sfSprite_getTransform |
| graphics/sprite-sheet-animation | OK | OK | OK | couleur du pixel == couleur de l'image courante |
| graphics/text | OK | OK | KO (freetype) | centre de getGlobalBounds == centre voulu (ecart 0.00) ; 1er caractere unicode == 0xE9 |
| graphics/view-camera | OK | OK | OK | mapPixelToCoords(centre ecran) == centre de la vue |
| graphics/render-texture | OK | OK | OK | pixels centre (rouge) et coin (bleu) de la texture |
| graphics/vertex-array | OK | OK | OK | 9 sommets, pixel rouge dans le triangle |
| graphics/transform | OK | OK | OK | (10,0) -> (100,70) ; pixel du rectangle transforme |
| graphics/blend-mode | OK | OK | OK | alpha -> 0,255,0 ; add -> 255,255,0 |
| graphics/clock-delta-time | OK | OK | OK | somme des dt ~ temps total (< 0.1 s d'ecart) |

## Surprises / notes
- valgrind `graphics/text` : plusieurs blocs « definitely lost » (891 octets, 2752 octets, ...) venant de libfreetype :
  `malloc <- libfreetype.so.6.20.5 <- FT_Glyph_To_Bitmap <- sf::Font::loadGlyph <- sf::Font::getGlyph
  <- sf::Text::ensureGeometryUpdate <- sf::Text::getLocalBounds <- sfText_getLocalBounds`.
  Bug interne connu de SFML/freetype (rien a voir avec l'exemple : tout est detruit). Sortie valgrind = exit 99.
  A supprimer dans tests/valgrind.supp (non modifie, comme demande). Valable en 3.0 et 2.6.
- Les libs installees sont SFML 3.0.2 (`libsfml-graphics.so.3.0.2`) pour CSFML 3.0.0.
- 3.0 n'a pas `sfRenderStates_default()` : c'est une variable `const sfRenderStates sfRenderStates_default`
  (2.6 : fonction). `sfTransform_Identity` est une variable dans les deux versions.
- 3.0 : `sfRenderTexture_create(size, settings)` accepte `settings == NULL` (verifie dans src/CSFML/Graphics/RenderTexture.cpp) ;
  2.6 : `sfRenderTexture_create(w, h, sfBool depthBuffer)`.
- 3.0 : `sfCursor_destroy(const sfCursor*)`, `sfRenderWindow_destroy(const ...)` : les destroy prennent des pointeurs const.
- 3.0 `sfRenderWindow_mapPixelToCoords(window, point, view)` : on passe `sfRenderWindow_getView(window)` (la doc dit
  « the view to use » ; le comportement avec NULL n'a pas ete teste).
- 3.0 : plus de `sfQuads` (vertex-array utilise 2 triangles par quad dans les 2 versions).
- 2.6 : `sfText_setColor` existe mais est deprecie, on utilise `sfText_setFillColor`. `sfSprite_create()` + `sfSprite_setTexture(s, tex, sfTrue)`
  en 2.6 contre `sfSprite_create(texture)` en 3.0 ; idem `sfText_create()` + `sfText_setFont`.
- mouse-position 3.0 : sous Xvfb la position relative au demarrage etait negative (ex. (-112, -84)) car le pointeur est
  hors de la fenetre ; en 2.6 elle valait (400, 300). Normal : relative a la fenetre, depend de son placement.
- events : un evenement Resized (800x600) arrive au demarrage sous Xvfb.
- window-settings : sous Xvfb sans gestionnaire de fenetres, setSize et setPosition sont bien appliques (500x400, (100,100)).
- Les messages « Failed to create input context » / « vertical sync » sont presents (normaux).
- video-mode sous Xvfb : bureau 1024x768 24 bits, 6 modes plein ecran (1024x768 en 32/24/16/8/4 bits...).
- Pas de `#include "test_mode.h"` dans video-mode (pas de fenetre ni de boucle).
- Les `make re` de run-all-examples font `rm -rf build` : j'ai fait `make clean` dans tous les dossiers a la fin.
