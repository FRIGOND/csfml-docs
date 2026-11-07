# Migration CSFML 2.x → 3.x (2.6.1 → 3.0.0)

Recherche PHASE 1 (aucune compilation). Rédigé le 2026-11-07.
Référence : tags git `2.6.1` (2024-05-19) et `3.0.0` (tag créé le 2026-01-13, release GitHub publiée le 2026-01-14T09:12:39Z selon https://api.github.com/repos/SFML/CSFML/releases/tags/3.0.0) du dépôt https://github.com/SFML/CSFML.
Les headers installés sur la machine (`/usr/include/CSFML`, paquet Ubuntu `libcsfml-dev 3.0.0-1`) sont **identiques octet pour octet** à `include/CSFML/` du tag 3.0.0 (`diff -r` vide, constaté le 2026-11-07) [CONFIRMÉ].

Notation : `@2.6.1` = fichier lu au tag 2.6.1 (chemin `include/SFML/...`), `@3.0.0` = fichier lu au tag 3.0.0 (chemin `include/CSFML/...`). URL équivalentes : `https://github.com/SFML/CSFML/blob/<tag>/<chemin>`. Les signatures citées sont copiées des headers (espaces normalisés par script, rien d'autre).

---

## En bref

Les 10 changements qui cassent le plus de code étudiant (projets type « my_hunter », « my_runner », « my_radar ») :

1. **Chemin des headers** : `#include <SFML/Graphics.h>` → `#include <CSFML/Graphics.h>` (tous les headers ont migré de `include/SFML/` à `include/CSFML/`). Premier message d'erreur garanti sur tout ancien projet. [CONFIRMÉ, commit c5158c7 / PR #269]
2. **`sfBool`, `sfTrue`, `sfFalse` et `sfInt8…sfUint64` n'existent plus** : utiliser `bool`/`true`/`false` et `int8_t…uint64_t` (`CSFML/Config.h` inclut `<stdbool.h>` et `<stdint.h>`). 140 fonctions ne changent que par ces types. [CONFIRMÉ, PR #225]
3. **`sfRenderWindow_create` prend un paramètre de plus** : `sfWindowState state` (`sfWindowed` ou `sfFullscreen`) entre `style` et `settings`. `sfFullscreen` n'est plus un style : `sfFullscreen` passé en `style` compile sans erreur mais vaut `sfTitlebar` (piège silencieux). [CONFIRMÉ headers ; effet silencieux DÉDUIT]
4. **`sfVideoMode`** : `{width, height, bitsPerPixel}` → `{sfVector2u size; bitsPerPixel}` ; `mode.width` → `mode.size.x`. [CONFIRMÉ]
5. **Événements** : même boucle `sfRenderWindow_pollEvent` + `switch (event.type)`, mais `sfEvtLostFocus`/`sfEvtGainedFocus` → `sfEvtFocusLost`/`sfEvtFocusGained`, `sfEvtMouseWheelMoved` supprimé, et les champs `x`/`y` deviennent `position` (`event.mouseButton.position.x`), `event.size.width` → `event.size.size.x`. `waitEvent` prend un `sfTime timeout`. [CONFIRMÉ]
6. **Ressource obligatoire à la création** : `sfSprite_create(texture)`, `sfText_create(font)`, `sfSound_create(buffer)`. Passer `NULL` déclenche un `assert` → arrêt du programme (`abort`), car le paquet Ubuntu est compilé avec les asserts actifs. En 2.6.1, un pointeur NULL donnait un simple avertissement en debug. [CONFIRMÉ src + binaire]
7. **Rectangles** : `sfFloatRect`/`sfIntRect` passent de `left, top, width, height` à `position` + `size` (vecteurs) ; `sfFloatRect_contains(&r, x, y)` → `sfFloatRect_contains(&r, point)`. [CONFIRMÉ]
8. **Paires de scalaires → vecteurs** : `sfTexture_create(w, h)` → `sfTexture_create(size)`, idem `sfImage_create`, `sfImage_get/setPixel`, `sfRenderTexture_create`, `sfTransform_translate/scale`, `sfTexture_update*`, `*_setIcon`. [CONFIRMÉ, PR #279]
9. **Renommages** : `sfMusic/sfSound/sfSoundStream_setLoop/getLoop` → `_setLooping/_isLooping` ; `*_getSystemHandle` → `*_getNativeHandle` ; `sfText_setColor` → `sfText_setFillColor` ; `antialiasingLevel` → `antiAliasingLevel` ; alias de touches (`sfKeyReturn` → `sfKeyEnter`, etc.) ; `sfMouseXButton1/2` → `sfMouseButtonExtra1/2`. [CONFIRMÉ]
10. **Suppressions** : `sfQuads` (→ `sfTriangles`, 6 sommets par quad), `sfRenderStates_default()` devient une **variable** `sfRenderStates_default`, `sfView_reset`, `sfMutex_*`, `sfThread_*`, `sfShader_set*Parameter`, `sfRenderWindow_capture`. `sfRenderStates` gagne 2 champs (`stencilMode`, `coordinateType`). [CONFIRMÉ]

Ce qui **ne change pas** (rassurant) : les angles restent des `float` en **degrés** (pas de type angle comme en SFML C++) ; `sfVector2i/u/f`, `sfVector3f`, `sfColor` (sauf `uint8_t`), `sfTime` ; la plupart des fonctions courantes (`sfRenderWindow_clear/display/drawSprite/drawText/close/isOpen`, `sf*_setPosition/setScale/setOrigin/move/rotate`, `sfTexture_createFromFile`, `sfFont_createFromFile`, `sfMusic_createFromFile`, `sfClock_restart`…) ; les options de link `-lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio`. 578 des 850 fonctions de 2.6.1 ont une signature identique en 3.0.0. [CONFIRMÉ, script]

---

## 1. Méthode et chiffres

- Diff des headers : `git diff 2.6.1 3.0.0 -- include/` sur le clone, header par header. Fichiers : 86 headers en 2.6.1, 89 en 3.0.0. Tous déplacés de `include/SFML/` vers `include/CSFML/`. Supprimés : `System/Mutex.h`, `System/Thread.h`. Ajoutés : `Audio/EffectProcessor.h`, `Audio/SoundChannel.h`, `Audio/SoundSourceCone.h`, `Graphics/CoordinateType.h`, `Graphics/StencilMode.h`. (Git affiche de faux renommages, ex. `Mutex.h => Rect.h` : c'est un artefact de détection de renommage, vérifié par comparaison des listes de fichiers.) [CONFIRMÉ]
- Extraction exhaustive : scripts Python (`$S/work-migration/scripts/extract_api.py`, `diff_api.py`, `annex_md.py`, lancés avec `python3 -I`) qui lisent toutes les déclarations `CSFML_*_API` (fonctions et variables), les `typedef enum`, `typedef struct`, et les `typedef` simples, sur les arbres extraits par `git archive`.
- Vérification croisée : 926 fonctions + 23 variables déclarées en 3.0.0 = **949** ; `nm -D` sur les 5 `.so` installés donne exactement 949 symboles `sf*`, sans écart dans un sens ni dans l'autre. [CONFIRMÉ]

| Mesure (2.6.1 → 3.0.0) | Nombre |
|---|---|
| Fonctions 2.6.1 / 3.0.0 | 850 / 926 |
| Supprimées / ajoutées | 40 / 116 |
| Communes à signature identique | 578 |
| Communes modifiées seulement par `sfBool`/`sfIntN`/`sfUintN` → `bool`/`intN_t`/`uintN_t` | 140 |
| Communes dont `_destroy(X*)` devient `_destroy(const X*)` | 40 |
| Communes modifiées par un nom de paramètre seulement | 1 |
| Communes modifiées autrement (paramètres, types, retour) | 51 |

Le détail exhaustif est en **Annexe A** (générée).

---

## 2. Tableau principal (groupé par module)

Colonnes : ancien (2.6.1) → nouveau (3.0.0) → remarque. « Casse » = erreur de compilation ; « Silencieux » = compile mais se comporte autrement ; « Compatible » = l'ancien code compile encore.

### 2.1 Inclusion, compilation, bibliothèques

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| `#include <SFML/Graphics.h>` (et `<SFML/Audio.h>`, `<SFML/Window/Event.h>`…) | `#include <CSFML/Graphics.h>` (`<CSFML/Audio.h>`, `<CSFML/Window/Event.h>`…) | **Casse**. Raison officielle : distinguer les headers SFML (C++) et CSFML (commit https://github.com/SFML/CSFML/commit/c5158c7, 2024-08-23). Sur Ubuntu, `/usr/include/SFML/` contient les headers C++ `.hpp` de SFML 3 et aucun `.h` : l'ancien include échoue. [CONFIRMÉ] |
| Gardes `#ifndef SFML_SPRITE_H` … | `#pragma once` | Les macros `SFML_*_H` n'existent plus (rarement utilisées par les étudiants). PR #266. [CONFIRMÉ] |
| `CSFML_VERSION_MAJOR 2`, `MINOR 6`, `PATCH 1` | `3`, `0`, `0` (`CSFML/Config.h`) | Utilisable pour du code compatible deux versions, mais le chemin d'include diffère : il faudrait `__has_include` [DÉDUIT, à tester]. |
| `typedef int sfBool; #define sfFalse 0; #define sfTrue 1` (`SFML/Config.h`) | supprimés ; `CSFML/Config.h` inclut `<stdbool.h>` et `<stdint.h>` et définit `typedef uint32_t sfChar32;` | **Casse**. Remplacer par `bool`, `true`, `false`. Commit a51c22a (PR #225). [CONFIRMÉ] |
| `sfInt8`, `sfUint8`, `sfInt16`, `sfUint16`, `sfInt32`, `sfUint32`, `sfInt64`, `sfUint64` | `int8_t`, `uint8_t`, … `uint64_t` | **Casse** si le code déclare ces types. [CONFIRMÉ] |
| Aucun standard C imposé (`cmake/Macros.cmake`@2.6.1) | `target_compile_features(${target} PUBLIC c_std_11 cxx_std_17)` (`cmake/Macros.cmake`@3.0.0, commit c7ac60f « Require C11 ») | Impose C11 pour construire CSFML et pour qui consomme la cible CMake. Les headers publics n'utilisent aucune construction propre à C11 (recherche de `_Static_assert`, `_Generic`, `_Alignas`… vide) : un programme en C99 devrait compiler [DÉDUIT, à tester]. GCC 15 (Ubuntu 26.04) compile par défaut en `gnu23`, où `bool` est un mot-clé : compatible. |
| SFML 2.6 requis (`find_package(SFML 2.6 …)` dans `src/SFML/CMakeLists.txt`@2.6.1) | SFML 3.0 requis (`find_package(SFML 3.0 …)` dans `CMakeLists.txt`@3.0.0) ; le readme dit que la version majeure de SFML doit égaler celle de CSFML | [CONFIRMÉ] Le CI de 3.0.0 utilise SFML 3.0.2 (PR #415). |
| CMake ≥ 3.7.2 pour construire | CMake ≥ 3.22 | [CONFIRMÉ] |
| `libcsfml-graphics.so.2.6` (SOVERSION = MAJEUR.MINEUR) | `libcsfml-graphics.so.3.0` | Le nom de link ne change pas : `-lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lcsfml-network`. Sur Ubuntu : SONAME `libcsfml-graphics.so.3.0`, dépend de `libsfml-*.so.3.0` (objdump). [CONFIRMÉ] |
| Windows : `csfml-graphics-2.dll` ; suffixe debug `-d` | `csfml-graphics-3.dll` ; suffixe debug `-d` (donc `csfml-graphics-d-3.dll`) ; MinGW : pas de préfixe `lib`, bibliothèque d'import en `.a` | Règles lues dans `cmake/Macros.cmake`@3.0.0. Noms exacts non vérifiés dans l'archive Windows. [DÉDUIT] |
| Build statique : option `BUILD_SHARED_LIBS` présente mais sans `CSFML_STATIC` ni suffixe | Statique supporté : suffixes `-s` (release) et `-s-d` (debug), `CSFML_STATIC` définie automatiquement | PR #311. [CONFIRMÉ CMake] |
| Cibles CMake internes `csfml-system`, `csfml-window`, `csfml-graphics`, `csfml-audio`, `csfml-network` (+ `csfml-main` sous Windows) | mêmes noms ; aucune config CMake installée (`install(EXPORT…)` absent), pas de cibles `CSFML::…` (à la différence de SFML 3 : `SFML::Graphics`) | Pour CMake : pkg-config ou `add_subdirectory`/FetchContent (3.0.0 gère `PROJECT_IS_TOP_LEVEL`). [CONFIRMÉ] |
| Fichiers `csfml-*.pc` | modèles `tools/pkg-config/*.pc.in` identiques (diff vide) | [CONFIRMÉ] |
| Audio via OpenAL (SFML 2) | SFML 3 n'utilise plus OpenAL : `libsfml-audio.so.3.0` installé ne dépend que de vorbis/FLAC/ogg | Plus de `openal32.dll` à fournir sous Windows [DÉDUIT]. Release note PR #263 « Remove references to OpenAL ». [CONFIRMÉ objdump] |

### 2.2 System

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| `sfTime { sfInt64 microseconds; }` ; `CSFML_SYSTEM_API sfTime sfTime_Zero;` | `sfTime { int64_t microseconds; }` ; `CSFML_SYSTEM_API const sfTime sfTime_Zero;` | Compatible pour l'usage normal. `System/Time.h`. [CONFIRMÉ] |
| `sfInt32 sfTime_asMilliseconds(sfTime time)` / `sfTime sfMilliseconds(sfInt32 amount)` / `sfInt64 sfTime_asMicroseconds(sfTime time)` / `sfTime sfMicroseconds(sfInt64 amount)` | `int32_t …` / `int64_t …` | Compatible. `sfSeconds`, `sfTime_asSeconds`, `sfSleep` inchangés. [CONFIRMÉ] |
| `sfClock_create`, `sfClock_copy`, `sfClock_destroy(sfClock* clock)`, `sfClock_getElapsedTime`, `sfClock_restart` | identiques sauf `void sfClock_destroy(const sfClock* clock)` ; **ajouts** `bool sfClock_isRunning(const sfClock* clock)`, `void sfClock_start(sfClock* clock)`, `void sfClock_stop(sfClock* clock)`, `sfTime sfClock_reset(sfClock* clock)` | Attention : `sfClock_reset` remet à zéro **et met l'horloge en pause** (doc du header) ; `sfClock_restart` remet à zéro et continue. [CONFIRMÉ] |
| `System/Mutex.h` : `sfMutex_create/destroy/lock/unlock` ; `System/Thread.h` : `sfThread_create/destroy/launch/terminate/wait` | supprimés (headers supprimés, `System.h` ne les inclut plus) | **Casse**. Remplacer par `<threads.h>` (C11) ou pthreads. SFML 3 a supprimé ces primitives (migration.md SFML, section « Threading Primitives »). [CONFIRMÉ] |
| `typedef sfInt64 (*sfInputStreamReadFunc)(void* data, sfInt64 size, void* userData)` ; `typedef sfInt64 (*sfInputStreamSeekFunc)(sfInt64 position, void* userData)` | `typedef int64_t (*sfInputStreamReadFunc)(void* data, size_t size, void* userData)` ; `typedef int64_t (*sfInputStreamSeekFunc)(size_t position, void* userData)` | **Casse** pour qui a écrit un flux perso : la fonction callback doit changer de type de paramètre (GCC ≥ 14 refuse les pointeurs de fonction incompatibles). `System/InputStream.h`. [CONFIRMÉ headers ; erreur GCC DÉDUIT] |
| `sfVector2i`, `sfVector2u`, `sfVector2f`, `sfVector3f` | inchangés | Aucune fonction utilitaire ajoutée (pas d'équivalent de `length()`/`normalized()` de SFML C++). [CONFIRMÉ] |
| `void sfBuffer_destroy(sfBuffer* buffer)` ; `const sfUint8* sfBuffer_getData(const sfBuffer* buffer)` | `const` ajouté / `const uint8_t*` | Compatible. [CONFIRMÉ] |

### 2.3 Window (fenêtre, événements, clavier, souris)

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| `sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, sfUint32 style, const sfContextSettings* settings)` | `sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state, const sfContextSettings* settings)` | **Casse** (« too few arguments »). Ajouter `sfWindowed`. Idem `sfRenderWindow_createUnicode`, `sfWindow_create`, `sfWindow_createUnicode`, `sfWindowBase_create(…, uint32_t style, sfWindowState state)`. PR #316. `Graphics/RenderWindow.h`, `Window/Window.h`, `Window/WindowBase.h`. [CONFIRMÉ] |
| `sfWindowStyle { sfNone = 0, sfTitlebar = 1 << 0, sfResize = 1 << 1, sfClose = 1 << 2, sfFullscreen = 1 << 3, sfDefaultStyle = … }` | `sfWindowStyle` sans `sfFullscreen` ; nouvelle enum `sfWindowState { sfWindowed, sfFullscreen }` (`Window/WindowBase.h`) | **Silencieux** : `sfFullscreen` existe toujours mais vaut **1** (= `sfTitlebar`). `sfRenderWindow_create(mode, "x", sfFullscreen, sfWindowed, NULL)` compile et ouvre une fenêtre normale. Écrire `sfRenderWindow_create(mode, "x", sfDefaultStyle, sfFullscreen, NULL)`. Le commentaire du header dit encore « If style contains sfFullscreen » (doc périmée). [CONFIRMÉ headers ; absence d'avertissement DÉDUIT, à tester] |
| `sfVideoMode { unsigned int width; unsigned int height; unsigned int bitsPerPixel; }` | `sfVideoMode { sfVector2u size; unsigned int bitsPerPixel; }` | **Casse** sur `mode.width`. `sfVideoMode mode = {800, 600, 32};` compile encore grâce à l'élision d'accolades du C, peut-être avec un avertissement `-Wmissing-braces` [DÉDUIT, à tester] ; forme propre : `{{800, 600}, 32}` (exemple officiel `examples/example.c`@3.0.0). `Window/VideoMode.h`. |
| `sfContextSettings { …; unsigned int antialiasingLevel; …; sfUint32 attributeFlags; sfBool sRgbCapable; }` | `…; unsigned int antiAliasingLevel; …; uint32_t attributeFlags; bool sRgbCapable;` | **Casse** sur `settings.antialiasingLevel`. Ordre des champs inchangé. PR #319. `Window/Window.h`. [CONFIRMÉ] |
| `sfBool sfRenderWindow_pollEvent(sfRenderWindow* renderWindow, sfEvent* event)` | `bool sfRenderWindow_pollEvent(sfRenderWindow* renderWindow, sfEvent* event)` | Compatible. Idem `sfWindow_pollEvent`, `sfWindowBase_pollEvent`. [CONFIRMÉ] |
| `sfBool sfRenderWindow_waitEvent(sfRenderWindow* renderWindow, sfEvent* event)` | `bool sfRenderWindow_waitEvent(sfRenderWindow* renderWindow, sfTime timeout, sfEvent* event)` | **Casse**. `sfTime_Zero` = attente infinie (doc du header). Renvoie `false` aussi en cas de timeout. Idem `sfWindow_waitEvent`, `sfWindowBase_waitEvent`. [CONFIRMÉ] |
| `sfWindowHandle sfRenderWindow_getSystemHandle(const sfRenderWindow* renderWindow)` (+ `sfWindow_`, `sfWindowBase_`) | `sfWindowHandle sfRenderWindow_getNativeHandle(const sfRenderWindow* renderWindow)` (+ `sfWindow_`, `sfWindowBase_`) | **Casse** (renommage). [CONFIRMÉ] |
| `void sfRenderWindow_setIcon(sfRenderWindow* renderWindow, unsigned int width, unsigned int height, const sfUint8* pixels)` | `void sfRenderWindow_setIcon(sfRenderWindow* renderWindow, sfVector2u size, const uint8_t* pixels)` | **Casse**. Idem `sfWindow_setIcon`, `sfWindowBase_setIcon`. [CONFIRMÉ] |
| `…_setUnicodeTitle(…, const sfUint32* title)` | `…_setUnicodeTitle(…, const sfChar32* title)` | Casse seulement si le code utilise le type `sfUint32`. [CONFIRMÉ] |
| — | `void sfRenderWindow_setMinimumSize(sfRenderWindow* renderWindow, const sfVector2u* minimumSize)` ; `…_setMaximumSize(…)` (+ `sfWindow_`, `sfWindowBase_`) | Ajout ; `NULL` = pas de limite. PR #399. [CONFIRMÉ] |
| `typedef void (*GlFunctionPointer)(void)` ; `GlFunctionPointer sfContext_getFunction(const char* name)` | `typedef void (*sfGlFunctionPointer)(void)` ; `sfGlFunctionPointer sfContext_getFunction(const char* name)` | Casse si le type est nommé. [CONFIRMÉ] |
| **Types d'événements** `sfEvtLostFocus`, `sfEvtGainedFocus` | `sfEvtFocusLost`, `sfEvtFocusGained` | **Casse**. `Window/Event.h`. [CONFIRMÉ] |
| `sfEvtMouseWheelMoved` + `event.mouseWheel` (`sfMouseWheelEvent`, `int delta`, déjà déprécié en 2.6.1) | supprimés → `sfEvtMouseWheelScrolled` + `event.mouseWheelScroll` (`sfMouseWheel wheel; float delta; sfVector2i position;`) | **Casse**. Tester `event.mouseWheelScroll.wheel == sfMouseVerticalWheel`. [CONFIRMÉ] |
| — | `sfEvtMouseMovedRaw` + `event.mouseMoveRaw` (`sfVector2i delta`) | Ajout. Le commentaire de l'enum dit « data in event.mouseMove » mais le champ réel est `mouseMoveRaw` (src/CSFML/Window/ConvertEvent.hpp@3.0.0). [CONFIRMÉ] |
| `event.size.width`, `event.size.height` | `event.size.size.x`, `event.size.size.y` (`sfSizeEvent { sfEventType type; sfVector2u size; }`) | **Casse**. [CONFIRMÉ] |
| `event.mouseButton.x/.y`, `event.mouseMove.x/.y`, `event.mouseWheelScroll.x/.y`, `event.touch.x/.y` | `….position.x/.y` (`sfVector2i position`) | **Casse**. [CONFIRMÉ] |
| `event.sensor.x/.y/.z` | `event.sensor.value.x/.y/.z` (`sfVector3f value`) | **Casse**. [CONFIRMÉ] |
| `event.key.alt/control/shift/system` (`sfBool`), `event.text.unicode` (`sfUint32`) | `bool`, `uint32_t` | Compatible. `event.key.code` et `event.key.scancode` inchangés. [CONFIRMÉ] |
| Valeurs numériques de `sfEventType` | décalées (suppression de `sfEvtMouseWheelMoved`, insertion de `sfEvtMouseMovedRaw`) | Ne gêne que du code qui stocke les valeurs en dur. [CONFIRMÉ] |
| Alias dépréciés de `sfKeyCode` : `sfKeyTilde`, `sfKeyDash`, `sfKeyBack`, `sfKeyBackSlash`, `sfKeySemiColon`, `sfKeyReturn`, `sfKeyQuote` | supprimés → `sfKeyGrave`, `sfKeyHyphen`, `sfKeyBackspace`, `sfKeyBackslash`, `sfKeySemicolon`, `sfKeyEnter`, `sfKeyApostrophe` | **Casse**. Correspondances lues dans `Window/Keyboard.h`@2.6.1 (`sfKeyQuote = sfKeyApostrophe`, `sfKeyDash = sfKeyHyphen`…). Le tableau du guide SFML C++ contient deux erreurs sur ces lignes (Quote→Hyphen, Dash→Dash) : se fier au header CSFML. [CONFIRMÉ] |
| `sfKeyCount`, `sfScancodeCount`, `sfSensorCount`, `sfMouseButtonCount` dans les `typedef enum` | toujours définis, mais dans un `enum { … }` anonyme séparé (`sfKeyCount = sfKeyPause + 1`) | Compatible (ex. `bool keys[sfKeyCount];`). PR #366. [CONFIRMÉ] |
| `sfMouseXButton1`, `sfMouseXButton2` | `sfMouseButtonExtra1`, `sfMouseButtonExtra2` | **Casse**. PR #354. `Window/Mouse.h`. [CONFIRMÉ] |
| `sfBool sfKeyboard_isKeyPressed(sfKeyCode key)`, `sfBool sfMouse_isButtonPressed(sfMouseButton button)`, `sfJoystick_*`, `sfTouch_*`, `sfSensor_*` | même chose avec `bool` | Compatible. `sfMouse_getPositionRenderWindow` inchangé. [CONFIRMÉ] |
| `void sfWindow_destroy(sfWindow* window)` (et 39 autres `_destroy`) | `void sfWindow_destroy(const sfWindow* window)` | Compatible (permet `const sfTexture* t = …; sfTexture_destroy(t);`). PR #327. [CONFIRMÉ] |

### 2.4 Graphics

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| `sfSprite* sfSprite_create(void)` | `sfSprite* sfSprite_create(const sfTexture* texture)` | **Casse**. `assert(texture)` dans `src/CSFML/Graphics/Sprite.cpp`@3.0.0. `sfSprite_setTexture(sprite, NULL, …)` est, lui, ignoré sans erreur. [CONFIRMÉ] |
| `void sfSprite_setTexture(sfSprite* sprite, const sfTexture* texture, sfBool resetRect)` | `void sfSprite_setTexture(sfSprite* sprite, const sfTexture* texture, bool resetRect)` | Compatible si on passe `true`/`1`. [CONFIRMÉ] |
| `sfText* sfText_create(void)` | `sfText* sfText_create(const sfFont* font)` | **Casse**. `assert(font)` ; `sfText_setFont(text, NULL)` déclenche aussi un assert. [CONFIRMÉ] |
| `void sfText_setColor(sfText* text, sfColor color)` ; `sfColor sfText_getColor(const sfText* text)` | supprimés → `sfText_setFillColor` / `sfText_getFillColor` (déjà présents en 2.6.1) | **Casse**. [CONFIRMÉ] |
| `void sfText_setStyle(sfText* text, sfUint32 style)` ; `…sfText_setUnicodeString(sfText* text, const sfUint32* string)` | `uint32_t style` ; `const sfChar32* string` | Compatible sauf usage des anciens types. [CONFIRMÉ] |
| `sfTexture* sfTexture_create(unsigned int width, unsigned int height)` | `sfTexture* sfTexture_create(sfVector2u size)` | **Casse**. Ajouts : `sfTexture_createSrgb(sfVector2u size)`, `bool sfTexture_resize(sfTexture* texture, sfVector2u size)`, `bool sfTexture_resizeSrgb(…)`. `sfTexture_createFromFile(const char* filename, const sfIntRect* area)` **inchangé** (renvoie NULL en cas d'échec). [CONFIRMÉ] |
| `sfTexture_updateFromPixels(sfTexture* texture, const sfUint8* pixels, unsigned int width, unsigned int height, unsigned int x, unsigned int y)` ; `sfTexture_updateFromImage/FromTexture/FromWindow/FromRenderWindow(…, unsigned int x, unsigned int y)` | `(…, const uint8_t* pixels, sfVector2u size, sfVector2u offset)` ; `(…, sfVector2u offset)` | **Casse**. [CONFIRMÉ] |
| `typedef enum { sfTextureNormalized, sfTexturePixels } sfTextureCoordinateType;` ; `void sfTexture_bind(const sfTexture* texture, sfTextureCoordinateType type)` | `typedef enum { sfCoordinateTypeNormalized, sfCoordinateTypePixels } sfCoordinateType;` (`Graphics/CoordinateType.h`) ; `void sfTexture_bind(const sfTexture* texture, sfCoordinateType type)` | **Casse** (noms). [CONFIRMÉ] |
| `sfImage* sfImage_create(unsigned int width, unsigned int height)` ; `sfImage_createFromColor(unsigned int width, unsigned int height, sfColor color)` ; `sfImage_createFromPixels(unsigned int width, unsigned int height, const sfUint8* pixels)` | `sfImage_create(sfVector2u size)` ; `sfImage_createFromColor(sfVector2u size, sfColor color)` ; `sfImage_createFromPixels(sfVector2u size, const uint8_t* pixels)` | **Casse**. [CONFIRMÉ] |
| `sfColor sfImage_getPixel(const sfImage* image, unsigned int x, unsigned int y)` ; `void sfImage_setPixel(sfImage* image, unsigned int x, unsigned int y, sfColor color)` | `sfColor sfImage_getPixel(const sfImage* image, sfVector2u coords)` ; `void sfImage_setPixel(sfImage* image, sfVector2u coords, sfColor color)` | **Casse** (fréquent dans les projets « framebuffer »). [CONFIRMÉ] |
| `void sfImage_copyImage(sfImage* image, const sfImage* source, unsigned int destX, unsigned int destY, sfIntRect sourceRect, sfBool applyAlpha)` | `bool sfImage_copyImage(sfImage* image, const sfImage* source, sfVector2u dest, sfIntRect sourceRect, bool applyAlpha)` | **Casse** ; renvoie maintenant un booléen. [CONFIRMÉ] |
| `sfFloatRect { float left; float top; float width; float height; }` ; `sfIntRect { int left; int top; int width; int height; }` | `sfFloatRect { sfVector2f position; sfVector2f size; }` ; `sfIntRect { sfVector2i position; sfVector2i size; }` | **Casse** sur `.left/.top/.width/.height` → `.position.x/.position.y/.size.x/.size.y`. `sfIntRect r = {0, 0, 110, 110};` compile encore (élision d'accolades), forme propre `{{0, 0}, {110, 110}}` [DÉDUIT, à tester]. `Graphics/Rect.h`. [CONFIRMÉ] |
| `sfBool sfFloatRect_contains(const sfFloatRect* rect, float x, float y)` ; `sfBool sfIntRect_contains(const sfIntRect* rect, int x, int y)` | `bool sfFloatRect_contains(const sfFloatRect* rect, sfVector2f point)` ; `bool sfIntRect_contains(const sfIntRect* rect, sfVector2i point)` | **Casse** (« too many arguments »). [CONFIRMÉ] |
| `sfVector2f sfFloatRect_getPosition(const sfFloatRect* rect)`, `…_getSize`, `sfIntRect_getPosition/getSize` | supprimées | Lire directement `rect.position` / `rect.size`. [CONFIRMÉ] |
| `sfFloatRect_intersects(const sfFloatRect* rect1, const sfFloatRect* rect2, sfFloatRect* intersection)` | idem avec retour `bool` | Compatible (en C++, SFML 3 a remplacé par `findIntersection` ; pas en CSFML). [CONFIRMÉ] |
| `sfRenderStates { sfBlendMode blendMode; sfTransform transform; const sfTexture* texture; const sfShader* shader; }` | `sfRenderStates { sfBlendMode blendMode; sfStencilMode stencilMode; sfTransform transform; sfCoordinateType coordinateType; const sfTexture* texture; const sfShader* shader; }` | **Casse ou silencieux** : une initialisation positionnelle à 4 valeurs ne correspond plus. Une initialisation désignée qui omet `stencilMode`/`coordinateType` met ces champs à 0 = `sfStencilComparisonNever` et `sfCoordinateTypeNormalized` (la valeur par défaut est `Pixels`). Recommandation : partir de `sfRenderStates_default` et modifier les champs voulus. PR #368. [CONFIRMÉ struct ; effet visuel INCERTAIN, à tester] |
| `sfRenderStates sfRenderStates_default(void)` (fonction) | `const sfRenderStates sfRenderStates_default` (variable globale) | **Casse** : `sfRenderStates_default()` → erreur « called object is not a function ». Écrire `sfRenderStates s = sfRenderStates_default;`. PR #348. [CONFIRMÉ] |
| — | `Graphics/StencilMode.h` : `sfStencilMode`, `sfStencilComparison`, `sfStencilUpdateOperation`, `sfStencilValue`, `const sfStencilMode sfStencilMode_default` ; `sfRenderWindow_clearStencil`, `sfRenderWindow_clearColorAndStencil`, `sfRenderWindow_getScissor` (+ `sfRenderTexture_`) | Ajouts. [CONFIRMÉ] |
| `sfRenderTexture* sfRenderTexture_create(unsigned int width, unsigned int height, sfBool depthBuffer)` ; `sfRenderTexture* sfRenderTexture_createWithSettings(unsigned int width, unsigned int height, const sfContextSettings* settings)` | `sfRenderTexture* sfRenderTexture_create(sfVector2u size, const sfContextSettings* settings)` ; `createWithSettings` supprimée | **Casse**. `settings` peut être `NULL` (réglages par défaut, src). [CONFIRMÉ] |
| `unsigned int sfRenderTexture_getMaximumAntialiasingLevel(void)` | `unsigned int sfRenderTexture_getMaximumAntiAliasingLevel(void)` | **Casse** (majuscule A). [CONFIRMÉ] |
| `CSFML_DEPRECATED sfImage* sfRenderWindow_capture(const sfRenderWindow* renderWindow)` | supprimée | Remplacer par `sfTexture_create` + `sfTexture_updateFromRenderWindow(texture, window, (sfVector2u){0, 0})` + `sfTexture_copyToImage(texture)` (suggestion du guide SFML). [CONFIRMÉ suppression ; recette DÉDUIT] |
| `void sfView_reset(sfView* view, sfFloatRect rectangle)` | supprimée | Utiliser `sfView_setCenter` + `sfView_setSize` + `sfView_setRotation(view, 0)`, ou détruire et `sfView_createFromRect(rect)`. Ajouts : `sfView_setScissor`, `sfView_getScissor`. [CONFIRMÉ suppression ; remplacement DÉDUIT] |
| `sfShader_setFloatParameter`, `…Float2…`, `…Float3…`, `…Float4…`, `…Vector2…`, `…Vector3…`, `…Color…`, `…Transform…`, `…Texture…`, `…CurrentTextureParameter` (tous `CSFML_DEPRECATED`) | supprimées → `sfShader_setFloatUniform`, `sfShader_setVec2Uniform`, `sfShader_setVec3Uniform`, `sfShader_setVec4Uniform`, `sfShader_setColorUniform`, `sfShader_setMat3Uniform`, `sfShader_setTextureUniform`, `sfShader_setCurrentTextureUniform` (déjà présentes en 2.6.1) | **Casse**. [CONFIRMÉ] |
| `sfPrimitiveType` : `sfQuads`, `sfLinesStrip`, `sfTrianglesStrip`, `sfTrianglesFan` | supprimés ; restent `sfPoints`, `sfLines`, `sfLineStrip`, `sfTriangles`, `sfTriangleStrip`, `sfTriangleFan` | **Casse**. `sfQuads` n'a pas d'équivalent direct : 2 triangles (6 sommets) par quad. [CONFIRMÉ] |
| `void sfTransform_translate(sfTransform* transform, float x, float y)` ; `sfTransform_scale(…, float scaleX, float scaleY)` ; `sfTransform_rotateWithCenter(…, float angle, float centerX, float centerY)` ; `sfTransform_scaleWithCenter(…, float scaleX, float scaleY, float centerX, float centerY)` | `(…, sfVector2f offset)` ; `(…, sfVector2f scale)` ; `(…, float angle, sfVector2f center)` ; `(…, sfVector2f scale, sfVector2f center)` | **Casse**. `sfTransform_rotate(sfTransform* transform, float angle)` inchangé (degrés). [CONFIRMÉ] |
| Angles : `sf*_setRotation(…, float angle)`, `sf*_rotate`, `sf*_getRotation`, `sfView_setRotation`, `sfTransform_rotate` en degrés | **inchangé** : `float`, en degrés | Doc des headers « in degrees » ; implémentation `sprite->setRotation(sf::degrees(angle))` (`src/CSFML/Graphics/Sprite.cpp`@3.0.0). À la différence de SFML C++ 3 (`sf::Angle`). [CONFIRMÉ] |
| `CSFML_GRAPHICS_API sfColor sfBlack;` (et `sfWhite`, `sfRed`, `sfGreen`, `sfBlue`, `sfYellow`, `sfMagenta`, `sfCyan`, `sfTransparent`) | `CSFML_GRAPHICS_API const sfColor sfBlack;` … | Compatible (sauf code qui modifiait ces globales). `sfColor` : champs `uint8_t`. [CONFIRMÉ] |
| `sfShape`, `sfCircleShape`, `sfRectangleShape`, `sfConvexShape` | inchangés sauf `bool` et `const` dans `_destroy` ; ajout `sfVector2f sf*Shape_getGeometricCenter(const sf*Shape* shape)` | Compatible. PR #345. [CONFIRMÉ] |
| `sfVertexBuffer* sfVertexBuffer_create(unsigned int vertexCount, …)` ; `unsigned int sfVertexBuffer_getVertexCount(…)` | `size_t vertexCount` ; `size_t …getVertexCount(…)` | Compatible en C (conversion implicite). [CONFIRMÉ] |
| `sfFont_getGlyph(const sfFont* font, sfUint32 codePoint, unsigned int characterSize, sfBool bold, float outlineThickness)` etc. | `uint32_t`, `bool` | Compatible. `sfFont_createFromFile` inchangé. [CONFIRMÉ] |

### 2.5 Audio

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| `sfSound* sfSound_create(void)` | `sfSound* sfSound_create(const sfSoundBuffer* buffer)` | **Casse**. `assert(buffer)` ; `sfSound_setBuffer(sound, NULL)` est ignoré. [CONFIRMÉ] |
| `void sfSound_setLoop(sfSound* sound, sfBool loop)` ; `sfBool sfSound_getLoop(const sfSound* sound)` | `void sfSound_setLooping(sfSound* sound, bool loop)` ; `bool sfSound_isLooping(const sfSound* sound)` | **Casse**. Idem `sfMusic_setLoop/getLoop` → `sfMusic_setLooping/isLooping`, `sfSoundStream_…`. [CONFIRMÉ] |
| `sfSoundBuffer* sfSoundBuffer_createFromSamples(const sfInt16* samples, sfUint64 sampleCount, unsigned int channelCount, unsigned int sampleRate)` | `sfSoundBuffer* sfSoundBuffer_createFromSamples(const int16_t* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate, sfSoundChannel* channelMapData, size_t channelMapSize)` | **Casse**. « Channel map » obligatoire (nouvelle enum `sfSoundChannel`, `Audio/SoundChannel.h`). Même chose pour `sfSoundStream_create(…, unsigned int channelCount, unsigned int sampleRate, sfSoundChannel* channelMapData, size_t channelMapSize, void* userData)`. [CONFIRMÉ] |
| `void sfSoundRecorder_setProcessingInterval(…)` | supprimée | SFML 3 (miniaudio) cale l'intervalle sur le backend (migration.md SFML). [CONFIRMÉ] |
| `const char** sfSoundRecorder_getAvailableDevices(size_t* count)` | `const char* const* sfSoundRecorder_getAvailableDevices(size_t* count)` | Avertissement (qualificatif `const` perdu) si on garde `const char**` [DÉDUIT]. [CONFIRMÉ signature] |
| `sfMusic_createFromFile`, `sfMusic_play/pause/stop`, `sfMusic_setVolume`, `sfMusic_getDuration`, `sfMusic_get/setLoopPoints`, `sfSound_play`, `sfSound_getStatus`, `sfSoundStatus { sfStopped, sfPaused, sfPlaying }` | inchangés | [CONFIRMÉ] |
| — | 79 fonctions ajoutées : spatialisation et effets (`sf{Sound,Music,SoundStream}_setPan`, `_setSpatializationEnabled`, `_setDirection`, `_setCone`, `_setVelocity`, `_setDopplerFactor`, `_setMinGain/_setMaxGain`, `_setMaxDistance`, `_setEffectProcessor`…), `sfListener_setVelocity/setCone`, `_getChannelMap`, `sfSoundBufferRecorder_setChannelCount` | Ajouts (PR #349, #294/#300). [CONFIRMÉ] |

### 2.6 Network

| Ancien (2.6.1) | Nouveau (3.0.0) | Remarque / exemple |
|---|---|---|
| Fonctions sockets, `sfPacket_*`, `sfFtp*_isOk`, `sf*_setBlocking/isBlocking` avec `sfBool`/`sfIntN` | `bool`/`intN_t` | Compatible (33 fonctions). [CONFIRMÉ] |
| `sfIpAddress { char address[16]; }`, `sfIpAddress_fromString`, `sfIpAddress_None/Any/LocalHost/Broadcast` | inchangés en 3.0.0 | `sfIpAddress_fromString` résout encore les noms (`sf::IpAddress::resolve`, src 3.0.0). Change sur master (§6). [CONFIRMÉ] |
| `const char* sfFtpDirectoryResponse_getDirectory(const sfFtpDirectoryResponse* ftpDirectoryResponse)` : pointeur interne, à ne pas libérer (src 2.6.1 : `.c_str()`) | même signature, mais la chaîne est allouée (`strdup`) et « must be freed » (doc du header) ; ajout `const sfChar32* sfFtpDirectoryResponse_getDirectoryUnicode(…)` | **Silencieux** : fuite mémoire si non libérée. Libérer avec `free` (allocation `strdup`) [DÉDUIT]. [CONFIRMÉ src + doc] |
| `sfFtp_*` | **toujours présent** en 3.0.0 (non supprimé, non déprécié) | Déprécié seulement sur master (§6). [CONFIRMÉ] |
| — | `unsigned short sfTcpListener_anyPort(void)`, `unsigned short sfUdpSocket_anyPort(void)`, `void sfTcpListener_close(sfTcpListener* listener)`, `sfPacket_readInt64/readUint64/writeInt64/writeUint64` | Ajouts. [CONFIRMÉ] |

---

## 3. Changements de comportement et pièges (sans changement visible de signature)

1. **NULL → `assert` au lieu d'un avertissement.** En 2.6.1, les macros `CSFML_CHECK`/`CSFML_CHECK_RETURN` (`src/SFML/Internal.h`@2.6.1) affichaient « SFML warning: trying to use a null … object » et retournaient, mais seulement si `NDEBUG` n'était pas défini (sinon : rien, donc crash possible). En 3.0.0, 854 `assert(...)` les remplacent (commit 00f78c1, PR #287 ; #330, #333, #335, #339). Les 5 bibliothèques du paquet Ubuntu importent `__assert_fail` (`nm -D`) : les asserts sont **actifs**, donc `sfSprite_create(NULL)`, `sfText_create(NULL)`, `sfRenderWindow_drawSprite(window, NULL, NULL)` ou `sfTexture_createFromFile(NULL, NULL)` arrêtent le programme (SIGABRT). [CONFIRMÉ src + binaire ; message exact à tester]
2. **`sfFullscreen` a changé de nature** (voir §2.3). Code « migré à moitié » = fenêtre non plein écran sans aucun message. [DÉDUIT]
3. **`sfRenderStates` construit à la main** : champs `stencilMode` et `coordinateType` à zéro si on ne part pas de `sfRenderStates_default`. Côté SFML 3.0.0, un `StencilMode` différent du défaut active `GL_STENCIL_TEST` (`src/SFML/Graphics/RenderTarget.cpp`@3.0.0, `applyStencilMode`) : avec `sfStencilComparisonNever` (= 0), rien ne serait dessiné si la fenêtre a un tampon stencil. [DÉDUIT, INCERTAIN, à tester]
4. **Élision d'accolades** : `sfVideoMode m = {800, 600, 32};` et `sfIntRect r = {0, 0, 32, 32};` restent valides en C. Le code compile, mais les accès `.width`/`.left` cassent. [DÉDUIT, à tester avec `-Wall -Wextra`]
5. **Fonctions supprimées + GCC 14/15** : depuis GCC 14, une fonction non déclarée (ex. `sfSound_setLoop`) et les pointeurs incompatibles sont des **erreurs** par défaut (https://gcc.gnu.org/gcc-14/porting_to.html). GCC 15 (Ubuntu 26.04 : gcc 15.2.0) compile par défaut en `-std=gnu23` (https://gcc.gnu.org/gcc-15/porting_to.html). Conséquence : pas de « warning implicit declaration » toléré comme avant. [CONFIRMÉ GCC ; application à CSFML DÉDUIT]
6. **Mémoire** : `sfFtpDirectoryResponse_getDirectory` renvoie désormais une chaîne à libérer (§2.6). [CONFIRMÉ]
7. **`sfClock_reset` ≠ `sfClock_restart`** : `reset` met l'horloge en pause. [CONFIRMÉ doc header]
8. **Documentation officielle fausse** : l'exemple de la page d'accueil Doxygen `doc/mainpage.hpp`@3.0.0 utilise encore `sfRenderWindow_create(mode, "SFML window", sfResize | sfClose, NULL)` (4 arguments), `sfSprite_create()` et `sfText_create()` : il ne compile pas avec 3.0.0. Master n'a corrigé que `sfText_create(font)` (commit 9a2a909, 2026-10-03). L'exemple fiable est `examples/example.c`@3.0.0. [CONFIRMÉ lecture ; non-compilation DÉDUIT, à tester]
9. **Commentaires périmés dans les headers 3.0.0** : « If style contains sfFullscreen » (`Window/WindowBase.h`, `Window/Window.h`, `Graphics/RenderWindow.h`) ; `sfEvtMouseMovedRaw` « data in event.mouseMove » (en réalité `mouseMoveRaw`) ; `sfFont_hasGlyph` « sfFalse otherwise ». [CONFIRMÉ]
10. **`CSFML/OpenGL.h` sous Windows** : l'ordre des includes a été trié par clang-format (`<GL/gl.h>` avant `<windows.h>`). Or `GL/gl.h` de Windows a besoin de `windows.h` avant. Possible erreur de compilation sous Windows. [INCERTAIN, à tester sous Windows]

---

## 4. Mini-exemples avant/après [À TESTER]

Aucun de ces extraits n'a été compilé (PHASE 1). Signatures vérifiées dans les headers 2.6.1 et 3.0.0. Gestion d'erreurs réduite pour rester court. Ligne de compilation (identique dans les deux versions) : `gcc main.c -o prog -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio`.

### 4.1 Création de fenêtre (+ plein écran)

```c
/* AVANT (2.6.1) */
#include <SFML/Graphics.h>
sfVideoMode mode = {800, 600, 32};
sfRenderWindow *win = sfRenderWindow_create(mode, "my_hunter", sfResize | sfClose, NULL);
sfRenderWindow *full = sfRenderWindow_create(sfVideoMode_getDesktopMode(), "full", sfFullscreen, NULL);
/* mode.width, mode.height */
```

```c
/* APRÈS (3.0.0) */
#include <CSFML/Graphics.h>
sfVideoMode mode = {{800, 600}, 32};
sfRenderWindow *win = sfRenderWindow_create(mode, "my_hunter", sfResize | sfClose, sfWindowed, NULL);
sfRenderWindow *full = sfRenderWindow_create(sfVideoMode_getDesktopMode(), "full", sfDefaultStyle, sfFullscreen, NULL);
/* mode.size.x, mode.size.y */
```

### 4.2 Boucle d'événements

```c
/* AVANT (2.6.1) */
sfEvent event;
while (sfRenderWindow_pollEvent(win, &event)) {
    if (event.type == sfEvtClosed)
        sfRenderWindow_close(win);
    if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
        shoot(event.mouseButton.x, event.mouseButton.y);
    if (event.type == sfEvtResized)
        resize(event.size.width, event.size.height);
    if (event.type == sfEvtLostFocus)
        paused = sfTrue;
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyReturn)
        start();
    if (event.type == sfEvtMouseWheelMoved)
        zoom(event.mouseWheel.delta);
}
```

```c
/* APRÈS (3.0.0) */
sfEvent event;
while (sfRenderWindow_pollEvent(win, &event)) {
    if (event.type == sfEvtClosed)
        sfRenderWindow_close(win);
    if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
        shoot(event.mouseButton.position.x, event.mouseButton.position.y);
    if (event.type == sfEvtResized)
        resize(event.size.size.x, event.size.size.y);
    if (event.type == sfEvtFocusLost)
        paused = true;
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
        start();
    if (event.type == sfEvtMouseWheelScrolled
        && event.mouseWheelScroll.wheel == sfMouseVerticalWheel)
        zoom(event.mouseWheelScroll.delta);   /* float */
}
/* attente bloquante : sfRenderWindow_waitEvent(win, sfTime_Zero, &event) */
```

### 4.3 Sprite + animation par rectangle

```c
/* AVANT (2.6.1) */
sfTexture *tex = sfTexture_createFromFile("duck.png", NULL);
sfSprite *spr = sfSprite_create();
sfSprite_setTexture(spr, tex, sfTrue);
sfIntRect rect = {0, 0, 110, 110};
rect.left += 110;
sfSprite_setTextureRect(spr, rect);
```

```c
/* APRÈS (3.0.0) */
sfTexture *tex = sfTexture_createFromFile("duck.png", NULL);
if (tex == NULL)
    return 84;
sfSprite *spr = sfSprite_create(tex);          /* NULL interdit : assert */
sfIntRect rect = {{0, 0}, {110, 110}};
rect.position.x += 110;
sfSprite_setTextureRect(spr, rect);
```

### 4.4 Clic dans un sprite

```c
/* AVANT (2.6.1) */
sfFloatRect b = sfSprite_getGlobalBounds(spr);
if (sfFloatRect_contains(&b, event.mouseButton.x, event.mouseButton.y))
    hit();
```

```c
/* APRÈS (3.0.0) */
sfFloatRect b = sfSprite_getGlobalBounds(spr);
sfVector2f p = sfRenderWindow_mapPixelToCoords(win, event.mouseButton.position, NULL);
if (sfFloatRect_contains(&b, p))
    hit();
```

(`NULL` pour la vue : l'implémentation 3.0.0 utilise alors la vue courante, `src/CSFML/Graphics/RenderWindow.cpp`.)

### 4.5 Texte

```c
/* AVANT (2.6.1) */
sfFont *font = sfFont_createFromFile("arial.ttf");
sfText *txt = sfText_create();
sfText_setFont(txt, font);
sfText_setString(txt, "Score: 0");
sfText_setColor(txt, sfWhite);
```

```c
/* APRÈS (3.0.0) */
sfFont *font = sfFont_createFromFile("arial.ttf");
if (font == NULL)
    return 84;
sfText *txt = sfText_create(font);              /* NULL interdit : assert */
sfText_setString(txt, "Score: 0");
sfText_setFillColor(txt, sfWhite);
```

### 4.6 Son et musique

```c
/* AVANT (2.6.1) */
sfSoundBuffer *buf = sfSoundBuffer_createFromFile("shot.ogg");
sfSound *snd = sfSound_create();
sfSound_setBuffer(snd, buf);
sfSound_play(snd);
sfMusic *mus = sfMusic_createFromFile("theme.ogg");
sfMusic_setLoop(mus, sfTrue);
sfMusic_play(mus);
```

```c
/* APRÈS (3.0.0) */
sfSoundBuffer *buf = sfSoundBuffer_createFromFile("shot.ogg");
if (buf == NULL)
    return 84;
sfSound *snd = sfSound_create(buf);             /* NULL interdit : assert */
sfSound_play(snd);
sfMusic *mus = sfMusic_createFromFile("theme.ogg");
if (mus == NULL)
    return 84;
sfMusic_setLooping(mus, true);
sfMusic_play(mus);
```

### 4.7 Image, texture et icône (paires → vecteurs)

```c
/* AVANT (2.6.1) */
sfImage *img = sfImage_create(64, 64);
sfImage_setPixel(img, 10, 20, sfRed);
sfTexture *t = sfTexture_create(64, 64);
sfTexture_updateFromImage(t, img, 0, 0);
sfRenderWindow_setIcon(win, 64, 64, sfImage_getPixelsPtr(img));
```

```c
/* APRÈS (3.0.0) */
sfImage *img = sfImage_create((sfVector2u){64, 64});
sfImage_setPixel(img, (sfVector2u){10, 20}, sfRed);
sfTexture *t = sfTexture_create((sfVector2u){64, 64});
sfTexture_updateFromImage(t, img, (sfVector2u){0, 0});
sfRenderWindow_setIcon(win, (sfVector2u){64, 64}, sfImage_getPixelsPtr(img));
```

### 4.8 `sfQuads` → `sfTriangles` et `sfRenderStates`

```c
/* AVANT (2.6.1) : 4 sommets, primitive sfQuads */
sfVertexArray *va = sfVertexArray_create();
sfVertexArray_setPrimitiveType(va, sfQuads);
/* append a, b, c, d */
sfRenderStates st = sfRenderStates_default();
st.texture = tileset;
sfRenderWindow_drawVertexArray(win, va, &st);
```

```c
/* APRÈS (3.0.0) : 6 sommets (2 triangles) par quad */
sfVertexArray *va = sfVertexArray_create();
sfVertexArray_setPrimitiveType(va, sfTriangles);
/* append a, b, c puis a, c, d */
sfRenderStates st = sfRenderStates_default;    /* variable, plus de () */
st.texture = tileset;
sfRenderWindow_drawVertexArray(win, va, &st);
```

---

## 5. Différences 2.5.x → 2.6.x (bref)

Diff généré des headers, mêmes scripts. 2.5 : 787 fonctions ; 2.6.1 : 850 (1 supprimée, 64 ajoutées, 12 modifiées). Chemin `include/SFML/` dans les deux. Ce qui peut casser :

| Version | Changement | Statut |
|---|---|---|
| 2.5 → 2.5.1 | `sfRenderTexture_createWithSettings(unsigned int width, unsigned int height, sfContextSettings settings)` → `(…, const sfContextSettings* settings)` (par valeur → pointeur) | **Casse** [CONFIRMÉ] |
| 2.5.1 → 2.5.2 | `void sfTexture_bind(const sfTexture* texture)` → `void sfTexture_bind(const sfTexture* texture, sfTextureCoordinateType type)` ; `sfTexture_setSrgb` supprimée (→ `sfTexture_createSrgbFrom*`) | **Casse** dans une version « patch » [CONFIRMÉ] |
| 2.6.0 → 2.6.1 | `sfVulkan_getGraphicsRequiredInstanceExtensions(void)` → `(size_t* count)` | **Casse** (rare) [CONFIRMÉ] |
| 2.5 → 2.6.0 | `sfKeyEvent` gagne `sfScancode scancode` entre `code` et `alt` (initialisation positionnelle seulement) ; `sfKeyQuote`/`sfKeyTilde` deviennent des alias de `sfKeyApostrophe`/`sfKeyGrave` (mêmes valeurs) ; déclarations `()` → `(void)` | Compatible en pratique [CONFIRMÉ] |
| 2.5 → 2.6.x | Ajouts : `sfWindowBase_*`, `sfScancode` + `sfKeyboard_isScancodePressed/localize/delocalize/getDescription`, `sfBuffer_*`, `sfFree`, Vulkan, `sfRenderStates_default()` (2.5.2), `sfFloatRect_getPosition/getSize` (tous deux supprimés en 3.0.0) | — |
| SFML requis | CSFML 2.5 : `find_package(SFML 2 …)` ; 2.5.2 : SFML 2.5 ; 2.6.x : SFML 2.6 (`src/SFML/CMakeLists.txt`) | [CONFIRMÉ] |

Conclusion : du code 2.5 compile presque toujours en 2.6 ; la vraie rupture est 2.x → 3.0. [DÉDUIT]

---

## 6. Ce qui change sur master après 3.0.0

Master = 20 commits après 3.0.0 (du 2026-10-03 au 2026-11-01), version déclarée **3.1.0** (`project(CSFML VERSION 3.1.0)`, `CSFML_VERSION_MINOR 1`), exige **SFML 3.1** (`find_package(SFML 3.1 …)`, commit 48e07cb). Non publié en release au 2026-11-07. Diff des headers : 108 fonctions ajoutées, **0 supprimée**, 46 modifiées (dont 40 qui reçoivent seulement `CSFML_DEPRECATED`). [CONFIRMÉ]

- **Dépréciations** (commit 35713f1, 2026-10-03 ; les fonctions restent disponibles) : tout `sfFtp*` (remplacé par le nouveau `sfSftp`, commit 739b120), `sfIpAddress_resolve` (→ `sfDns_resolve`), `sfText_findCharacterPos`, `sfTouch_isDown/getPosition/getPositionWindowBase/getPositionRenderWindow` (→ événements tactiles). Avec GCC/Clang, l'appel produit un avertissement `-Wdeprecated-declarations`. [CONFIRMÉ headers master]
- **Changements de comportement** : `sfIpAddress_fromString` ne résout plus les noms d'hôte (doc master : « Network names are not resolved, use sfDns_resolve or sfIpAddress_resolve ») ; `sfIpAddress.address` passe de `char[16]` à `char[46]` (IPv6) ; `sfHttp_setHost` et `sfSocketSelector_add*/remove*` renvoient maintenant `bool` au lieu de `void` (compatible en C) ; `sfFontInfo` gagne `id`, `hasKerning`, `hasVerticalMetrics` (`id` en premier). [CONFIRMÉ headers master]
- **Ajouts** : `Audio/PlaybackDevice.h` (`sfPlaybackDevice_*`), `Network/Dns.h` (`sfDns_*`), `Network/Sftp.h`, TLS (`sfTcpSocket_setupTlsClient/Server…`), IPv6 (`sfIpAddress_isV6`, `sfIpAddress_AnyV6`…), `System/Version.h` (`sfVersion sfGetVersion(void)`), `System/TimeoutWithPredicate.h`, API de mise en forme du texte (`sfText_setLineAlignment`, `sfText_setTextOrientation`, `sfText_getShapedGlyphs`…), `sf*Shape_setMiterLimit`, `sfFont_getAscent/getDescent/getGlyphById`. [CONFIRMÉ]
- La SOVERSION étant `MAJEUR.MINEUR`, une future 3.1 produira `libcsfml-*.so.3.1` (incompatible binaire avec `.so.3.0`, cohérent avec le changement de taille de `sfIpAddress`). [DÉDUIT de `cmake/Macros.cmake`]
- Doc : l'exemple `doc/mainpage.hpp` de master reste faux pour `sfRenderWindow_create` et `sfSprite_create` (§3, point 8). [CONFIRMÉ]

---

## 7. Contexte : guides officiels

- Guide de migration SFML 2 → 3 (C++) : https://www.sfml-dev.org/tutorials/3.0/getting-started/migrate/ (aussi `migration.md` du dépôt SFML aux tags 3.0.0, 3.0.2, 3.1.0 ; https://www.sfml-dev.org/tutorials/3.1/getting-started/migrate/ renvoie 404 le 2026-11-07). Il ne parle pas de CSFML. Les sections utiles pour CSFML : vecteurs, entiers à taille fixe, styles et états de fenêtre, `sf::Rect`, renommages, suppression des constructeurs par défaut (Sprite/Text/Sound), primitives de thread, channel map, API dépréciées, anti-aliasing, `CoordinateType`. [CONFIRMÉ]
- **Aucun guide de migration propre à CSFML** : rien dans le dépôt (`git grep -i migrat` vide aux tags 3.0.0 et master), rien dans les notes de release 3.0.0 (https://github.com/SFML/CSFML/releases/tag/3.0.0), rien trouvé par recherche web. Le readme 3.0.0 renvoie aux tutoriels C++. [CONFIRMÉ]
- Différence de philosophie : SFML C++ 3 a refait les événements (`std::optional`, `getIf<T>`), les angles (`sf::Angle`), les enums scopées. **CSFML 3 garde l'union `sfEvent` + `event.type`, les `float` en degrés et des enums C préfixées.** La migration CSFML est donc beaucoup plus légère que la migration C++. [CONFIRMÉ par lecture de `ConvertEvent.hpp` et des headers]

---

## Décisions prises

- Base de comparaison : **2.6.1 → 3.0.0** (dernières versions publiées de chaque branche). 2.5.x traité à part (§5) car presque identique à 2.6.x.
- Source de vérité = headers des tags git, lus via le clone ; les notes de release et le guide SFML ne servent qu'à expliquer le « pourquoi ».
- Signatures dans les tableaux : normalisées (espaces) par script, jamais retapées à la main. Les rares recettes de remplacement (capture, `sfView_reset`, `sfQuads`) sont marquées [DÉDUIT].
- Les correspondances PR ↔ commit viennent des titres listés dans les notes de release (vérifiées par l'API GitHub pour #225 et #316).
- « Casse / Silencieux / Compatible » = jugement sur du C compilé avec GCC 15 `-Wall -Wextra` ; à confirmer en phase 2.
- Les 140 changements `sfBool`→`bool` sont regroupés (annexe A.4) au lieu d'être détaillés un par un : le remplacement est mécanique.
- Je ne recommande pas de header de compatibilité (`#define sfTrue true`, etc.) dans la doc principale : il masque les vrais changements (`sfFullscreen`, champs d'événements). À évoquer au plus comme astuce temporaire.

## Points [INCERTAIN]

- Effet réel d'un `sfRenderStates` dont `stencilMode` vaut zéro (dessin invisible ou non selon la présence d'un tampon stencil) — §3 point 3.
- Avertissements exacts de GCC 15 / Clang 21 pour l'élision d'accolades (`{800, 600, 32}`) et pour `sfFullscreen` passé comme style (probablement aucun) — §3 points 2 et 4.
- Noms exacts des DLL Windows (`csfml-graphics-3.dll`, `csfml-graphics-d-3.dll`) : déduits de CMake, pas vérifiés dans `CSFML-windows-64-bit.zip`.
- Compilation de `CSFML/OpenGL.h` sous Windows (ordre des includes) — §3 point 10.
- En plein écran (`sfFullscreen` comme état), le `style` est-il ignoré par SFML 3 ? Non vérifié ; l'exemple passe `sfDefaultStyle`.
- Compatibilité des headers 3.0.0 en `-std=c99` (déduite de l'absence de constructions C11).
- Message exact de l'assert (`Assertion 'texture' failed.`) : dépend de la glibc ; à capturer en phase 2.
- Année de la release GitHub 3.0.0 : l'API donne 2026-01-14 (la page web n'affiche pas l'année) ; le tag git est du 2026-01-13. Pas une contradiction : la release a été publiée le lendemain de la création du tag.

---

## Annexe A — Listes exhaustives générées (2.6.1 → 3.0.0)

Générées par `$S/work-migration/scripts/diff_api.py` puis `annex_md.py` (`$S` = dossier scratchpad de la session) à partir de `git archive 2.6.1` (`include/SFML`) et `git archive 3.0.0` (`include/CSFML`). Les chemins de fichier sont relatifs à `include/SFML/` (2.6.1) ou `include/CSFML/` (3.0.0). Signatures copiées des headers (espaces normalisés). [CONFIRMÉ]

Remarque A.7 : `sfRenderStates_default` existait en 2.6.1 comme **fonction** (voir A.1) ; en 3.0.0 c'est une **variable**.

