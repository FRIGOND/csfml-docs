# Résultats — migration 2.x → 3.x, §4 « Mini-exemples avant/après » (tests 25, 26, 27)

Source : `research/04-migration-2-vers-3.md` §4 (16 blocs ```c). Générateur : `tests/p1-migration/gen.py`
(lancé avec `python3 -I`, après `source tools/env.sh`). Programmes générés : `tests/p1-migration/gen/`.
Journal complet (messages gcc exacts + valgrind) : `tests/logs/migration.log`.
Compilation : `gcc -std=c11 -Wall -Wextra -I$P{2,3}/include -c` (compilation seule, sans édition de liens).

## Contexte ajouté par le wrapper (les extraits eux-mêmes sont copiés sans modification)
- Chaque extrait est placé dans `int block(void) { ... return 0; }` (les extraits ont des instructions au niveau « fichier »).
- Sections 4.2 à 4.8 : pas de `#include` dans l'extrait -> le wrapper ajoute `Graphics.h` + `Audio.h` de la bonne version.
- Déclarations factices (extern) : `win`, `event`, `spr`, `paused`, `tileset`, `shoot()`, `resize()`, `zoom()`, `start()`, `hit()`.
  (`paused` : `sfBool` en AVANT, `bool` en APRÈS ; `zoom(int)` en AVANT, `zoom(float)` en APRÈS.)
- `<stdbool.h>` ajouté par le wrapper ; vérifié inutile : les APRÈS compilent aussi sans (les headers 3.0.0 le fournissent).
- Les `return 84;` des APRÈS compilent car le wrapper renvoie `int`.

## Test 25 — chaque AVANT compile en 2.6.1, chaque APRÈS en 3.0.0

| § | Version | Compile | Message si ❌ | Avertissements | Exécution | Correction proposée |
|---|---------|---------|---------------|----------------|-----------|---------------------|
| 4.1 | AVANT 2.6.1 | ✅ | | `unused variable` (wrapper) | n/a | aucune |
| 4.1 | APRÈS 3.0.0 | ✅ | | `unused variable` (wrapper) | n/a (cf. 27) | aucune |
| 4.2 | AVANT 2.6.1 | ✅ | | `Event.h:133: type is deprecated [-Wdeprecated-declarations]` (le `sfMouseWheelEvent` / `mouseWheel` de l'extrait est déprécié en 2.6.1) | n/a | aucune (l'avertissement est cohérent avec le propos du doc) |
| 4.2 | APRÈS 3.0.0 | ✅ | | aucun | n/a | aucune. Le commentaire final `sfRenderWindow_waitEvent(win, sfTime_Zero, &event)` correspond bien à la signature 3.0.0 (`bool sfRenderWindow_waitEvent(sfRenderWindow*, sfTime timeout, sfEvent*)`) |
| 4.3 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated (via header) | n/a | aucune |
| 4.3 | APRÈS 3.0.0 | ✅ | | aucun | n/a (cf. 27) | aucune |
| 4.4 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated | n/a | aucune |
| 4.4 | APRÈS 3.0.0 | ✅ | | aucun | n/a | aucune (`sfRenderWindow_mapPixelToCoords(win, event.mouseButton.position, NULL)` et `sfFloatRect_contains(&b, p)` ont les bonnes signatures) |
| 4.5 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated | n/a | aucune |
| 4.5 | APRÈS 3.0.0 | ✅ | | aucun | n/a (cf. 27) | aucune |
| 4.6 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated | n/a | aucune |
| 4.6 | APRÈS 3.0.0 | ✅ | | aucun | n/a (cf. 27) | aucune |
| 4.7 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated | n/a | aucune |
| 4.7 | APRÈS 3.0.0 | ✅ | | aucun | n/a | aucune (les `(sfVector2u){64, 64}` sont des littéraux composés C99, OK en `-std=c11`) |
| 4.8 | AVANT 2.6.1 | ✅ | | `Event.h:133` deprecated | n/a | aucune |
| 4.8 | APRÈS 3.0.0 | ✅ | | aucun | n/a | aucune (`sfRenderStates_default` est bien une variable en 3.0.0) |

Bilan : **16/16 extraits compilent** dans leur version. **Aucune erreur réelle trouvée** dans les extraits de §4
(signatures, noms, ordre des arguments tous corrects). Seul point à noter : les AVANT 4.2 à 4.8 émettent un avertissement
`-Wdeprecated-declarations` en 2.6.1, ce qui vient du header `Event.h` inclus (pas de leur code, sauf 4.2 qui utilise réellement `mouseWheel`).
Piste d'amélioration facultative (pas une erreur) : l'en-tête de §4 dit que la ligne de compilation est identique ;
c'est vrai pour les libs, mais il faut `-I<préfixe>/include` pour un préfixe non système.

## Test 26 — les AVANT, avec seulement l'include changé (`<SFML/..>` -> `<CSFML/..>`), échouent-ils en 3.0.0 ?

(Sections 4.2+ : include `<CSFML/Graphics.h>` ajouté par le wrapper. Contre-épreuve : les APRÈS avec `<SFML/..>` en 2.6.1 échouent aussi, cf. dernière colonne.)

| § | AVANT sous 3.0.0 | Premières erreurs exactes (gcc) | APRÈS sous 2.6.1 |
|---|------------------|----------------------------------|------------------|
| 4.1 | ❌ erreur | `incompatible type for argument 4 of 'sfRenderWindow_create'` ; `too few arguments to function 'sfRenderWindow_create'; expected 5, have 4`. NB : `sfVideoMode mode = {800, 600, 32};` passe avec seulement `warning: missing braces around initializer [-Wmissing-braces]` (**comportement silencieux** : initialise `size.x=800, size.y=600, bitsPerPixel=32`, donc correct par chance ; sans `-Wall` aucun signal) | ❌ `'sfWindowed' undeclared`, `too many arguments to function 'sfRenderWindow_create'; expected 4, have 5` |
| 4.2 | ❌ | `'sfMouseButtonEvent' has no member named 'x'` (et 'y') ; `'sfSizeEvent' has no member named 'width'` ; `'sfEvtLostFocus' undeclared` ; `'sfTrue' undeclared` ; `'sfKeyReturn' undeclared` ; `'sfEvtMouseWheelMoved' undeclared` ; `'sfEvent' has no member named 'mouseWheel'` | ❌ `'sfMouseButtonEvent' has no member named 'position'`, `'sfSizeEvent' has no member named 'size'`, `'sfEvtFocusLost' undeclared` ... |
| 4.3 | ❌ | `too few arguments to function 'sfSprite_create'; expected 1, have 0` ; `'sfTrue' undeclared` ; `'sfIntRect' has no member named 'left'` (+ warning `missing braces` sur `{0, 0, 110, 110}`, silencieux sans -Wall) | ❌ `too many arguments to function 'sfSprite_create'; expected 0, have 1` ; `'sfIntRect' has no member named 'position'` |
| 4.4 | ❌ | `'sfMouseButtonEvent' has no member named 'x'` ; `too many arguments to function 'sfFloatRect_contains'; expected 2, have 3` | ❌ `no member named 'position'` ; `too few arguments to function 'sfFloatRect_contains'; expected 3, have 2` |
| 4.5 | ❌ | `too few arguments to function 'sfText_create'; expected 1, have 0` ; `implicit declaration of function 'sfText_setColor'; did you mean 'sfText_setFillColor'?` | ❌ `too many arguments to function 'sfText_create'; expected 0, have 1` |
| 4.6 | ❌ | `too few arguments to function 'sfSound_create'; expected 1, have 0` ; `implicit declaration of function 'sfMusic_setLoop'; did you mean 'sfMusic_setCone'?` ; `'sfTrue' undeclared` | ❌ `too many arguments to function 'sfSound_create'; expected 0, have 1` ; `implicit declaration of function 'sfMusic_setLooping'; did you mean 'sfMusic_setLoop'?` |
| 4.7 | ❌ | `incompatible type for argument 1 of 'sfImage_create'` ; `too many arguments to function 'sfImage_create'; expected 1, have 2` ; idem `sfImage_setPixel`, `sfTexture_create`, `sfTexture_updateFromImage`, `sfRenderWindow_setIcon` | ❌ `too few arguments ...` pour les mêmes 5 fonctions |
| 4.8 | ❌ | `'sfQuads' undeclared (first use in this function)` ; `called object 'sfRenderStates_default' is not a function or function pointer` | ❌ `invalid initializer` (`sfRenderStates st = sfRenderStates_default;` : fonction en 2.6.1) |

Remarque : les erreurs `unknown type name 'sfBool'` n'apparaissent PAS pour les extraits (c'est la déclaration de `paused` du wrapper
qui l'aurait provoquée ; remplacée par `int paused`). Dans l'extrait 4.2, `paused = sfTrue;` échoue bien par `'sfTrue' undeclared`.

Bilan test 26 : **8/8 AVANT échouent à la compilation en 3.0.0** (erreurs claires, aucun ne passe silencieusement). Seul
comportement dangereux : les initialiseurs plats `{800, 600, 32}` et `{0, 0, 110, 110}` ne produisent qu'un warning `-Wmissing-braces`
(et seulement avec `-Wall`) ; dans 4.3 AVANT ce warning est de toute façon suivi d'erreurs ailleurs.

## Test 27 — exécution sous Xvfb (:99), timeout 20, puis valgrind `--leak-check=full`

Programmes : `tests/p1-migration/t27_window.c` (fenêtre + sprite avec `sfSprite_create(tex)` + `setTextureRect` + texte
`sfText_create(font)` + `setFillColor`, boucle `pollEvent`, 30 frames) et `tests/p1-migration/t27_sound.c`
(`sfSound_create(buf)`, `sfMusic_setLooping`, ~30 x 16 ms). Assets : `tests/p1/{sfml_logo.png,tuffy.ttf,doodle_pop.ogg}`.
Compilation `-std=c11 -Wall -Wextra` : 0 avertissement, 0 erreur.

| Programme | Exécution (timeout 20) | Sortie | valgrind (code) | definitely lost |
|-----------|------------------------|--------|-----------------|-----------------|
| t27_window | code 0 | `frames ok` ; stderr SFML : `Failed to create input context for window -- TextEntered event won't be able to return unicode` et `Setting vertical sync not supported` (Xvfb, sans gravité) | 9 (`--error-exitcode=9`, dû aux fuites ci-dessous) | **1 646 octets en 8 blocs** : tous dans `libfreetype` appelée par `FT_Glyph_To_Bitmap` <- `sf::Font::loadGlyph` <- `sf::Text::ensureGeometryUpdate` / `sf::Font::getKerning` (libsfml-graphics 3.0.2). Aucune pile ne passe par le code utilisateur : fuite interne SFML/FreeType, pas de `destroy` oublié (tout est détruit) |
| t27_sound | code 0 | `status snd=2 mus=2` (2 = Playing) | 0 | 0 octet |

Bilan test 27 : les APRÈS fenêtre/sprite/texte/son **s'exécutent sans erreur** (code 0, objets bien détruits). Pas de fuite de notre fait ;
une petite fuite « definitely lost » (1,6 Ko) est interne à SFML 3.0.2 + FreeType lors du rendu de glyphes. Pas de « still reachable » rapporté ici.

## Conclusions

- **Test 25 : validé.** 16/16 extraits compilent dans leur version (8 AVANT en 2.6.1, 8 APRÈS en 3.0.0). Aucune correction à apporter à §4.
- **Test 26 : validé.** 8/8 AVANT échouent en 3.0.0 avec le seul changement d'include (et les 8 APRÈS échouent en 2.6.1). Piège silencieux :
  initialiseurs plats de `sfVideoMode`/`sfIntRect` (juste un `-Wmissing-braces`).
- **Test 27 : validé.** Exécution OK (code 0). Fuite valgrind de 1 646 octets, interne à SFML/FreeType, sans lien avec les extraits.
