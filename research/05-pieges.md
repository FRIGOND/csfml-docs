# 05 — Pièges courants avec CSFML (2.6.1 et 3.0.0)

Date de rédaction : 2026-11-07. Voir aussi `01-versions.md`, `03-api-inventaire.md` §2.5 (ownership), `04-migration-2-vers-3.md` §3.
Sources locales : clones `$S/repos/CSFML` et `$S/repos/SFML` (tags 2.6.1, 3.0.0, 3.0.2).

## En bref

- Règle d'or : **tout `sfX_create*` a un `sfX_destroy`**, et on détruit dans l'ordre inverse de la création (sprite avant texture, text avant font, sound avant buffer). [CONFIRMÉ, 03 §2.5]
- En 3.0, `sfSprite_create(texture)`, `sfText_create(font)`, `sfSound_create(buffer)` **exigent** un objet non NULL (`assert`) et ne le copient pas. [CONFIRMÉ, 04 §3.1]
- Ne créez jamais texture/font/son **dans** la boucle de jeu : fuite et lenteur.
- Vider la file d'événements à chaque tour (`while (sfRenderWindow_pollEvent(...))`), sinon la fenêtre « ne répond pas ». [DÉDUIT]
- Sans écran : erreur « Failed to open X11 display » ; sans carte audio : 2.6 = messages OpenAL, 3.0 = repli sur un backend « null » (silencieux). [CONFIRMÉ source]
- SFML 3.0.x n'a **pas de backend Wayland natif** : X11 / XWayland uniquement. [CONFIRMÉ, 0 occurrence de "wayland" dans `WindowImplX11.cpp`@3.0.2 ; INCERTAIN pour master]
- Valgrind : beaucoup de bruit (Mesa, X11, drivers) venant de la lib, pas de votre code : lire « definitely lost » et filtrer par suppressions.

Format de chaque piège : **Symptôme / Cause / Solution / Version / Source / Étiquette**.

## 1. Mémoire

### 1.1 `destroy` oublié (fuite)
- Symptôme : valgrind « definitely lost » avec `sfTexture_createFromFile`, `sfSprite_create`… dans la pile.
- Cause : CSFML alloue sur le tas (C++ `new` derrière) ; rien n'est libéré automatiquement. Toutes les fonctions `sfX_create*` (et `sfX_copy`) renvoient un objet à libérer. [CONFIRMÉ, 03 §6.1]
- Solution : un `destroy` par `create`, y compris sur les chemins d'erreur (create qui échoue au milieu d'une initialisation). Une fonction `cleanup(struct game *g)` unique aide.
- Version : les deux. Source : `include/CSFML/Graphics/Sprite.h` (`sfSprite_destroy`), tags 2.6.1 et 3.0.0. [CONFIRMÉ]

### 1.2 Ressources recréées dans la boucle
- Symptôme : la mémoire grimpe, FPS qui chute, valgrind « still reachable/lost » énorme.
- Cause : `sfTexture_createFromFile` à chaque frame (lecture disque + upload GPU) sans destroy.
- Solution : charger **avant** la boucle, détruire **après**. Modifier un sprite (`setPosition`, `setTextureRect`) ne recrée rien.
- Version : les deux. [DÉDUIT]

### 1.3 Double destroy
- Symptôme : `free(): double free detected` / SIGSEGV à la fin.
- Cause : deux `destroy` sur le même pointeur (ex. struct copiée par valeur contenant des pointeurs, ou `destroy` dans deux fonctions de nettoyage).
- Solution : mettre le pointeur à `NULL` après destroy et ne pas appeler destroy sur NULL **sans test** : en 3.0, `destroy(NULL)` fait un `delete` sur NULL (sans effet) mais en 2.6 certains `destroy` passent par `CSFML_CALL` ; **à tester** (P2).
- Version : les deux. Étiquette : [DÉDUIT]/[À TESTER].
- Attention : les getters qui **empruntent** (ex. `sfSprite_getTexture`, `sfText_getFont`) renvoient un pointeur à **ne pas** détruire (03 §6.2). [CONFIRMÉ]

### 1.4 Use-after-free sprite/texture, text/font, sound/buffer
- Symptôme : carré blanc (2.6 et 3.0 pour sprite dont la texture est détruite), texte invisible ou crash, son muet ou crash.
- Cause : `Sprite`, `Text`, `Sound` gardent un **pointeur** vers la ressource, pas une copie. Si on détruit la texture avant (ou si elle est une variable locale d'une fonction terminée), le pointeur pend. [CONFIRMÉ : `Graphics/Sprite.h`, `Graphics/Text.h`, `Audio/Sound.h`@3.0.0 ; en 2.6.1 `sfSprite_setTexture` a la même sémantique]
- Solution : durée de vie de la ressource > durée de vie de l'objet ; ranger les deux dans la même struct ; détruire le sprite d'abord.
- Cas particuliers : `sfFont_createFromFile` et `sfMusic_createFromFile` lisent le fichier **en continu** : le fichier (ou le buffer en mémoire) doit rester accessible tant que l'objet vit (03 §2.5 B). Un `sfSound` dont le `sfSoundBuffer` est détruit en cours de lecture est un comportement indéfini. [CONFIRMÉ doc C++]
- En 3.0 : le constructeur impose l'objet : `sfSprite_create(NULL)` → `assert` → SIGABRT (04 §3.1). En 2.6.1 : `sfSprite_create()` sans argument + `setTexture` ; NULL donnait un avertissement « trying to use a null object » seulement hors `NDEBUG`.
- Version : les deux (symptômes différents). [CONFIRMÉ]

### 1.5 Texture « blanche »
- Symptôme : rectangle blanc au lieu de l'image.
- Causes classiques : (a) `sfTexture_createFromFile` a renvoyé NULL (chemin faux) et on n'a pas testé ; (b) texture détruite ; (c) `sfSprite_setTextureRect` hors image.
- Solution : toujours tester le retour de `create*` (`if (!tex) return 84;`) ; lire stderr (voir §6).
- Version : les deux. [DÉDUIT]

### 1.6 Valgrind et bruit des drivers
- Symptôme : des centaines de lignes (`libGL`, `libX11`, `libnvidia`, `libLLVM`, `libdrm`), « still reachable » de plusieurs Mo, même pour un programme qui crée juste une fenêtre.
- Cause : Mesa/X11/drivers GPU allouent à la création du contexte OpenGL et ne libèrent pas tout à la sortie. Ce n'est **pas** votre fuite. [DÉDUIT, source secondaire courante ; INCERTAIN en chiffres]
- Solution : regarder d'abord le résumé `definitely lost` / `indirectly lost` et chercher vos fonctions dans la pile ; utiliser `--leak-check=full --show-leak-kinds=definitely` ; écrire un fichier de suppressions (`valgrind --gen-suppressions=all` sur un programme minimal qui ouvre et ferme la fenêtre, puis `--suppressions=mesa.supp`). Pour un test sans bruit : `LIBGL_ALWAYS_SOFTWARE=1` (Mesa llvmpipe) peut changer le bruit. À tester (P1/P2).
- Version : les deux. Étiquette : [DÉDUIT] / [À TESTER].

## 2. Boucle principale

### 2.1 File d'événements non vidée
- Symptôme : fenêtre gelée / « ne répond pas » ; la touche reste « pressée ».
- Cause : un seul `pollEvent` par frame (avec `if`) laisse des événements en attente.
- Solution : `while (sfRenderWindow_pollEvent(win, &event)) {...}`. La signature : 2.6.1 renvoie `sfBool`, 3.0.0 renvoie `bool` (`Graphics/RenderWindow.h`@3.0.0 l.141 : `bool sfRenderWindow_pollEvent(sfRenderWindow*, sfEvent*)`). [CONFIRMÉ]
- Version : les deux.
- Autre : en 3.0 les champs de `sfEvent` ont changé (`event.type` énum `sfEvtClosed`, union par type : voir 04 §2.3). Code 2.x → erreur de compilation. [CONFIRMÉ, 04]

### 2.2 Oublier `sfRenderWindow_close`
- Symptôme : la fenêtre ne se ferme pas après un clic sur la croix.
- Cause : `sfEvtClosed` ne ferme pas seul ; il faut appeler `sfRenderWindow_close`. [CONFIRMÉ : doc SFML/tutoriel fenêtre]
- Version : les deux.

### 2.3 Temps, delta time
- Symptôme : le jeu va deux fois plus vite sur une machine rapide.
- Cause : déplacement par frame au lieu de par seconde.
- Solution : `sfClock` : `sfClock_restart` renvoie le temps écoulé **et** remet à zéro (3.0.0 `sfTime sfClock_restart(sfClock*)`, `System/Clock.h` l.117) ; puis `sfTime_asSeconds(dt)`. Ne pas utiliser `sfClock_reset` (3.0 : met en pause ; 04 §3.7). Détruire l'horloge (`sfClock_destroy`). [CONFIRMÉ]
- Piège : `sfTime_asMilliseconds` renvoie `int32_t` (3.0.0) ; `sfTime_asSeconds` un `float`. Un `dt` énorme au premier tour (après un chargement long) fait « téléporter » les objets : plafonner `dt`. [DÉDUIT]
- Version : les deux (les noms existent dans les deux).

### 2.4 VSync vs `setFramerateLimit`
- Symptôme : CPU à 100 % (aucun des deux), ou saccades / FPS divisés (les deux ensemble).
- Cause : VSync cale sur le taux de rafraîchissement du moniteur ; la limite dort après chaque `display`. Doc : « avoid using both at the same time » (SFML) ; le header dit que VSync limite aux Hz du moniteur. [CONFIRMÉ : `Window/Window.h`@3.0.0 l.349 ; l'avertissement « ne pas combiner » : tutoriel SFML window, à citer]
- Solution : en C : `sfRenderWindow_setFramerateLimit(win, 60)` **ou** `sfRenderWindow_setVerticalSyncEnabled(win, true)` (3.0 : `bool`, 2.6 : `sfBool`) — pas les deux. Sous `xvfb` ou Mesa logiciel, le VSync peut être ignoré : utiliser la limite. [DÉDUIT]
- Version : les deux.

### 2.5 `sfRenderWindow_clear` / `display` oubliés
- Symptôme : traînées ou fenêtre noire/figée.
- Solution : ordre `clear → draw… → display` à chaque frame. [DÉDUIT]

## 3. Textures, sprites, polices

### 3.1 Texture trop grande
- Symptôme : message « Failed to create texture, its internal size is too high » sur stderr, texture NULL. [CONFIRMÉ : `Graphics/Texture.cpp`@3.0.2 l.259 (message)]
- Solution : taille max via `sfTexture_getMaximumSize()` ; découper l'image.

### 3.2 Chemins relatifs au répertoire courant
- Symptôme : `Failed to load font (failed to open file): No such file or directory` suivi du chemin ; ça marche depuis le dossier du projet, pas depuis ailleurs (ou depuis un IDE).
- Cause : les chemins de fichiers sont relatifs au **répertoire courant du processus** (celui d'où on lance), pas à l'exécutable. [CONFIRMÉ : `Graphics/Font.cpp`@3.0.2 l.168 affiche l'erreur `strerror` et un `formatDebugPathInfo(filename)` ; le comportement relatif est celui de `fopen`]
- Solution : lancer depuis la racine du projet ; ou chemin absolu ; ou un `#define` de chemin. Le correcteur automatique lance souvent depuis un autre dossier : ne pas dépendre d'assets non livrés [INCERTAIN, dépend du sujet].
- Version : les deux (le message de 3.0.2 est plus précis ; en 2.6.1 : « Failed to load font "x" (failed to open file) »).

### 3.3 Police introuvable sur le système
- Symptôme : `sfFont_createFromFile("arial.ttf")` NULL sur Linux.
- Cause : SFML ne cherche pas dans les polices système. Fournir un `.ttf` ou un chemin comme `/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf` (présence selon la distribution). [DÉDUIT]

### 3.4 Sprite copié / struct copiée
- `sfSprite_copy` existe (3.0.0) mais la copie garde le **même** pointeur de texture. [DÉDUIT du header ; à tester]

## 4. Audio

### 4.1 Pas de périphérique audio
- **3.0.x (miniaudio)** : `AudioDevice` essaie d'abord la liste de backends par défaut ; s'il n'y a aucun périphérique, il affiche `No audio playback devices available on the system`, puis tente le backend **null** et écrit `Using NULL audio backend for playback`. Le programme continue, sans son. Si même le null échoue : `Failed to initialize audio device or engine`. [CONFIRMÉ : `src/SFML/Audio/AudioDevice.cpp`@3.0.2 l.100–139]
- **2.6.1 (OpenAL Soft)** : `Failed to open the audio device` / `Failed to create the audio context` sur stderr (l.86 et 91 de `AudioDevice.cpp`@2.6.1), puis les appels audio échouent ou se taisent ; crash possible si l'objet n'a pas de buffer valide. [CONFIRMÉ messages ; comportement exact INCERTAIN]
- Conséquence pour les tests/CI : ne pas supposer qu'un `sfMusic_createFromFile` NULL = chemin faux ; lire stderr. [DÉDUIT]
- Tests à faire : machine sans audio (`pulseaudio` arrêté, conteneur Docker) en 2.6.1 et 3.0.0 (P1).

### 4.2 `sfSound` détruit alors qu'il joue / buffer détruit
- Symptôme : crash ou claquement. Solution : `sfSound_stop` puis `sfSound_destroy`, puis `sfSoundBuffer_destroy`. [CONFIRMÉ header 3.0.0 : le buffer n'est pas copié]

### 4.3 `sfMusic` vs `sfSound`
- Gros fichiers → `sfMusic` (streaming). Effets courts → `sfSoundBuffer` + `sfSound`. Un `sfSound` par son simultané : un seul `sfSound` rejoue depuis le début. [DÉDUIT ; doc SFML]

## 5. Environnement d'exécution

### 5.1 Pas d'écran (SSH, conteneur, CI)
- Symptôme : `Failed to open X11 display; make sure the DISPLAY environment variable is set correctly`. [CONFIRMÉ : `src/SFML/Window/Unix/Display.cpp`@3.0.2 l.71]
- Cause : `XOpenDisplay(nullptr)` échoue ; en 3.0 les `create` renvoient ensuite NULL ou, selon la version, un crash. 
- Solution : `xvfb-run -a ./prog` (Xvfb), avec Mesa llvmpipe pour OpenGL : `LIBGL_ALWAYS_SOFTWARE=1`. `xvfb-run` n'est pas installé sur la machine locale (brief). [CONFIRMÉ absence ; test à faire]
- Version : les deux. À tester (P1) : `xvfb-run -a ./prog` pour une fenêtre + un `sfRenderWindow_display`, avec 2.6.1 et 3.0.0.
- Note : les captures d'écran / `sfRenderWindow_capture` / `sfTexture_copyToImage` fonctionnent en rendu logiciel. [DÉDUIT]

### 5.2 Wayland
- SFML 3.0.2 n'a que le backend X11 sous Linux (aucune mention de Wayland dans `WindowImplX11.cpp`) : sous Wayland, la fenêtre passe par XWayland si `DISPLAY` est défini. Problèmes possibles : mise à l'échelle floue (HiDPI), décorations, `sfRenderWindow_setPosition` ignoré, fenêtre non centrée. [CONFIRMÉ absence de code ; symptômes DÉDUITS, INCERTAIN]
- Version : les deux. À vérifier sur master (support Wayland éventuel). Source : issue SFML « Wayland support » [À VÉRIFIER].

### 5.3 WSL / WSLg
- WSLg fournit un serveur X11 + Wayland : `DISPLAY=:0` est positionné automatiquement ; OpenGL via Mesa `d3d12` ou llvmpipe ; le GPU peut ne pas être exposé. Audio via PulseAudio de WSLg. Pièges : WSL1 n'a pas WSLg ; fenêtre noire si GL indirect. [DÉDUIT, source secondaire à citer ; INCERTAIN] → à tester (P2).

### 5.4 macOS
- Les chemins d'assets sont relatifs au CWD ; en bundle `.app` : le CWD est `/`. CSFML sur macOS : Homebrew `brew install csfml` ; le build nécessite `-framework` non requis pour les libs dynamiques mais OpenGL est déprécié. Les `.dylib` portent le numéro de version. [DÉDUIT, INCERTAIN ; test impossible ici]

### 5.5 Windows (DLL)
- `csfml-graphics-3.dll` (2.6 : `csfml-graphics-2.dll`) doit être à côté de l'exe ou dans `PATH`, avec les DLL SFML C++ associées (`sfml-*-3.dll`) ; en 3.0 plus d'`openal32.dll`. Erreur typique : « The code execution cannot proceed because csfml-... was not found » (0xc0000135). [DÉDUIT de 04 §2.1 ; noms exacts non vérifiés]

## 6. Erreurs de compilation / lien

| Symptôme | Cause | Solution | Version |
|---|---|---|---|
| `fatal error: SFML/Graphics.h: No such file` | 3.0 : headers dans `CSFML/` ; `/usr/include/SFML/` contient du C++ `.hpp` | `#include <CSFML/Graphics.h>` | 3.0 [CONFIRMÉ, 04 §2.1] |
| `fatal error: CSFML/Graphics.h: No such file` | lib 2.6 installée | `#include <SFML/Graphics.h>` | 2.6 |
| `unknown type name 'sfBool'` / `sfTrue undeclared` | 3.0 : `bool`/`true`/`false` | utiliser `<stdbool.h>` (inclus par `CSFML/Config.h`) | 3.0 [CONFIRMÉ] |
| `implicit declaration of function 'sfSound_setLoop'` / too many arguments to `sfSprite_create` | API 2.x contre headers 3.x (GCC ≥ 14 : erreur) | voir 04 | 3.0 [CONFIRMÉ GCC 14] |
| `undefined reference to 'sfRenderWindow_create'` | `-l` manquant ou après les `.c` | mettre `-lcsfml-graphics -lcsfml-window -lcsfml-system` **après** les `.o`/`.c` | les deux |
| `undefined reference to 'sin'` / `sqrt` | `-lm` manquant | ajouter `-lm` en fin de ligne | les deux |
| `cannot open shared object file: libcsfml-graphics.so.3.0` | exécutable lié à 3.0 mais `LD_LIBRARY_PATH` / ldconfig absent ou lib installée ailleurs | `ldconfig`, `LD_LIBRARY_PATH=...`, `ldd ./prog` pour voir | les deux (so.2.6 / so.3.0) [CONFIRMÉ SONAME, 04 §2.1] |
| Compile mais plante au lancement | headers 2.x compilés, libs 3.x chargées (ou l'inverse) : structs de tailles différentes | une seule version installée ; `-I` explicite ; `ldd` + `dpkg -l libcsfml-dev` | mélange [DÉDUIT] |
| Dépendance : il faut `csfml-window` avec `csfml-graphics` ? | `graphics` dépend de `window` et `system` ; l'ordre `-lcsfml-graphics -lcsfml-window -lcsfml-system` | garder cet ordre pour les libs statiques ; en dynamique l'ordre pèse peu (`--as-needed`) | les deux [DÉDUIT] |

## 7. Erreurs typiques d'étudiants Epitech [DÉDUIT]
- Variables globales de `sfRenderWindow*` non initialisées et destroy sur pointeur indéfini.
- `sfRenderWindow_create` sans test NULL puis boucle sur `isOpen(NULL)`.
- Sprite créé dans la boucle de `main`.
- Mettre le code dans `main` sans découpage (fonctions > 20 lignes → C-F4).
- Oublier `sfClock_destroy` / `sfMusic_destroy` : fuite signalée par valgrind.
- Écrire `sfVector2f pos = {10, 20};` : valide (champs `x`,`y`), mais attention à `sfVideoMode` / `sfIntRect` : l'ordre des champs a changé entre 2.x et 3.0 (voir 04 §3.4).
- Compiler avec `-lcsfml-*` seulement pour les modules utilisés : `-lcsfml-audio` oublié si `sfMusic` utilisé.

## Décisions prises
1. Pièges classés par thème, version indiquée pour chacun ; priorité 3.0.0 mais 2.6.1 couvert à égalité.
2. Aucune compilation en phase 1 : tout comportement non lu dans le code est marqué [DÉDUIT] ou [À TESTER].
3. Les chiffres de bruit valgrind et les détails WSL/macOS sont laissés [INCERTAIN] faute de sources primaires.

## Points [INCERTAIN]
- Comportement exact de `destroy(NULL)` en 2.6.1 et 3.0.0.
- Bruit valgrind (taille, suppressions) : à mesurer.
- Wayland : état sur master/SFML 3.1.
- WSLg, macOS, Windows : non testables ici.
- Message exact quand `DISPLAY` absent côté CSFML 3.0.0 (create renvoie NULL ? assert ?).
