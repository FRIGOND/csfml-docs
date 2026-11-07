# 03 — Inventaire complet de l'API CSFML 3.0.0 (et écarts sur `master`)

> Recherche PHASE 1, rédigée le 2026-11-07. Source de vérité : les headers du tag git `3.0.0` du dépôt
> https://github.com/SFML/CSFML (tag créé le 2026-01-13 ; la page de release GitHub affiche « 14 January »
> sans l'année, voir https://github.com/SFML/CSFML/releases/tag/3.0.0, consulté le 2026-11-07).
> Rien n'a été compilé. Les signatures sont copiées des headers par un script, puis vérifiées token par token.

## En bref

- **926 fonctions publiques** en CSFML 3.0.0 (System 21, Window 103, Graphics 467, Audio 191, Network 144), plus
  **23 constantes globales** (`sfBlack`, `sfTime_Zero`, `sfRenderStates_default`…) et 134 types nommés.
  Compte vérifié par trois moyens indépendants (parseur Python, `grep` sur les fichiers aplatis, vérification
  token par token) : 926 / 926 / 926. [CONFIRMÉ]
- Les headers installés par le paquet Ubuntu `libcsfml-dev 3.0.0-1` (`/usr/include/CSFML`) sont **identiques**
  au tag `3.0.0` (`diff -r` vide, 2026-11-07). [CONFIRMÉ]
- **Plus de `sfBool`** : CSFML 3.0.0 utilise le `bool` de C99 (`<stdbool.h>` inclus par `CSFML/Config.h`) et les
  entiers de `<stdint.h>` (`int64_t`, `uint32_t`…). `sfInt64`, `sfUint8`, `sfTrue`… n'existent plus. [CONFIRMÉ]
- Le chemin d'inclusion change : `#include <CSFML/Graphics.h>` (3.0.0) au lieu de `#include <SFML/Graphics.h>`
  (2.6.x). [CONFIRMÉ]
- Événements : `sfEvent` reste une **union** dont le premier champ est `type`. Fonctions exactes :
  `bool sfRenderWindow_pollEvent(sfRenderWindow*, sfEvent*)` et
  `bool sfRenderWindow_waitEvent(sfRenderWindow*, sfTime timeout, sfEvent*)` (le `timeout` est nouveau ; idem
  `sfWindow_*` et `sfWindowBase_*`). Les champs `x`/`y` deviennent `position` (`sfVector2i`). [CONFIRMÉ]
- Angles en **`float`, en degrés** partout (pas de type `sfAngle` en C). Les rectangles sont
  `{position, size}` (plus de `left/top/width/height`). Le temps est `sfTime { int64_t microseconds; }`. [CONFIRMÉ]
- `sfSprite_create(const sfTexture*)`, `sfText_create(const sfFont*)` et `sfSound_create(const sfSoundBuffer*)`
  exigent maintenant leur ressource. L'exemple de la page d'accueil de la doc officielle
  (`doc/mainpage.hpp`) appelle encore `sfSprite_create()` et `sfRenderWindow_create` avec 4 arguments au lieu de 5 :
  il **ne compile pas** avec 3.0.0 (et toujours pas sur `master`). [CONFIRMÉ par lecture, compilation à tester]
- Ownership : les objets gardent un **pointeur** vers la texture / police / buffer (pas de copie) ; seuls quelques
  retours sont à libérer : `sfKeyboard_getDescription`, `sfFtpDirectoryResponse_getDirectory[Unicode]` (avec
  `free`/`sfFree`), les `sfFtp*Response*`, `sfHttpResponse*`, `sfImage*` de `sfTexture_copyToImage` et la socket de
  `sfTcpListener_accept` (avec leur `*_destroy`). [CONFIRMÉ, sources `src/` du tag 3.0.0]
- **Aucune documentation CSFML 3.0 en ligne** sur sfml-dev.org : la doc Doxygen est fournie dans les paquets
  (`doc/` du dépôt, et paquet Ubuntu `libcsfml-doc` installé ici : `/usr/share/doc/libcsfml-dev/html`). [CONFIRMÉ]
- `master` (20 commits après 3.0.0, version `3.1.0` dans `Config.h`) : **+108 fonctions** (Sftp, Dns,
  PlaybackDevice, mise en forme de texte…), 7 signatures changées (`void` → `bool`), **40 fonctions marquées
  `CSFML_DEPRECATED`** (tout `sfFtp*`, `sfTouch_*`…), et `sfIpAddress.address` passe de 16 à 46 octets
  (rupture d'ABI). [CONFIRMÉ]

## 1. Méthode et sources

### 1.1 Extraction

- Headers extraits avec `git -C $S/repos/CSFML archive 3.0.0 include | tar -x` (et idem pour `master`) dans un
  dossier de travail de la session. Sources C++ de l'implémentation extraites de la même façon (`src/`), pour
  vérifier l'ownership quand le header ne dit rien.
- Script Python (`python3 -I`) : retire les commentaires, découpe en déclarations de niveau 0 (`;` hors accolades),
  garde celles qui commencent par `CSFML_<MODULE>_API`, sépare fonctions et variables, extrait les `typedef`
  (struct, enum, union, struct opaques, pointeurs de fonction), les enums anonymes et les `#define`.
- Format des signatures dans ce document : la macro d'export (`CSFML_GRAPHICS_API`…) est **omise**, les espaces
  sont normalisés sur une ligne. Tout le reste est identique au header. Contrôle : pour les 926 fonctions, la
  signature sans espaces est égale à la déclaration brute du header sans espaces (0 écart ; 0 écart aussi pour
  les 1034 de `master`). [CONFIRMÉ]
- Contrôles de comptage :

| Méthode | 3.0.0 | master |
|---|---|---|
| Parseur Python (fonctions) | 926 | 1034 |
| `tr '\n' ' '` puis `grep -oE 'CSFML_[A-Z]+_API [^;]*\('` | 926 | 1034 |
| Vérification token par token (script séparé) | 926 | 1034 |
| Lignes contenant `CSFML_X_API` (hors `#define`) | 949 = 926 fonctions + 23 variables | — |
| Constantes globales `CSFML_X_API const …` | 23 | 28 |
| Déclarations `CSFML_DEPRECATED` | 0 | 40 |

- `diff -r` entre `include/CSFML` du tag et `/usr/include/CSFML` (paquet Ubuntu `libcsfml-dev 3.0.0-1`) :
  aucune différence. [CONFIRMÉ, 2026-11-07]

### 1.2 Documentation officielle

- https://www.sfml-dev.org/documentation/ ne liste que la doc C++ de SFML (1.6 à 3.1.0), aucun lien CSFML
  (consulté le 2026-11-07). [CONFIRMÉ]
- https://www.sfml-dev.org/download/csfml/ propose CSFML 3.0.0 (dernière version) et indique que la doc de l'API
  est incluse dans les paquets téléchargeables ; aucune URL de doc en ligne (consulté le 2026-11-07). [CONFIRMÉ]
- URLs testées le 2026-11-07, toutes en **404** : `https://www.sfml-dev.org/documentation/csfml/`,
  `https://www.sfml-dev.org/documentation/3.0.0/csfml/`, `https://sfml.github.io/CSFML/`. [CONFIRMÉ]
- La doc Doxygen se génère depuis `doc/` du dépôt (`doc/doxyfile.in`, `doc/mainpage.hpp`, tag 3.0.0). Sur cette
  machine, le paquet `libcsfml-doc` est installé : `/usr/share/doc/libcsfml-dev/html` (307 fichiers, généré par
  Doxygen 1.15.0, titre « CSFML - C binding of the Simple and Fast Multimedia Library »). [CONFIRMÉ]
- Conséquence pour le projet : une doc CSFML 3 en ligne n'existe pas officiellement ; le site communautaire
  comble un vrai manque. [DÉDUIT]

## 2. Conventions transversales

Liens vers les headers : `https://github.com/SFML/CSFML/blob/3.0.0/<chemin>`. Chaque convention cite le fichier
qui la prouve.

### 2.1 Inclusion, modules, bibliothèques

- Un header « parapluie » par module : `CSFML/System.h`, `CSFML/Window.h`, `CSFML/Graphics.h`, `CSFML/Audio.h`,
  `CSFML/Network.h`. `Graphics.h` inclut `Window.h`, qui inclut `System.h` ; `Audio.h` et `Network.h` incluent
  `System.h` ([include/CSFML/Graphics.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Graphics.h), tag 3.0.0). [CONFIRMÉ]
- Pièges d'inclusion [CONFIRMÉ, tag 3.0.0] :
  - `CSFML/System/Alloc.h` (déclare `sfFree`) n'est inclus par **aucun** autre header : il faut l'inclure à la main.
  - `CSFML/OpenGL.h` et `CSFML/GPUPreference.h` ne sont pas inclus par les parapluies.
  - `Window/Cursor.h` et `Window/Context.h` sont inclus directement par `CSFML/Window.h` ; `Window/WindowBase.h`
    arrive via `Window/Window.h`, et `Window/Vulkan.h` via `Window/WindowBase.h`.
- Bibliothèques (paquet Ubuntu, voir brief commun) : `libcsfml-system`, `-window`, `-graphics`, `-audio`, `-network`
  (`.so.3.0.0`). Une fonction déclarée dans `Graphics/RenderWindow.h` est dans `csfml-graphics`, même si son
  préfixe est `sfMouse_` ou `sfTouch_` (voir §6.4). [DÉDUIT : la macro `CSFML_GRAPHICS_API` du header désigne
  l'export de la lib graphics]
- `CSFML/Config.h` définit `CSFML_VERSION_MAJOR 3`, `CSFML_VERSION_MINOR 0`, `CSFML_VERSION_PATCH 0`, la macro
  `CSFML_DEPRECATED` (désactivable par `CSFML_NO_DEPRECATED_WARNINGS`), `CSFML_STATIC` pour l'édition de liens
  statique, et `typedef uint32_t sfChar32;`
  ([include/CSFML/Config.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Config.h)). [CONFIRMÉ]
- Plateformes reconnues par `Config.h` : Windows, Linux, macOS, FreeBSD ; sinon `#error`. [CONFIRMÉ]

### 2.2 Nommage

- Tout est préfixé `sf`. Méthodes : `sf<Type>_<verbe>` (`sfSprite_setPosition`). Valeurs d'enum préfixées sans
  le nom du type complet (`sfEvtClosed`, `sfKeyA`, `sfScanA`, `sfMouseLeft`, `sfTitlebar`, `sfPlaying`,
  `sfSocketDone`…). Fonctions libres : `sfSeconds`, `sfMilliseconds`, `sfMicroseconds`, `sfSleep`, `sfFree`.
  [CONFIRMÉ, inventaire §4]
- Les 926 noms commencent tous par `sf`, aucun doublon. [CONFIRMÉ, script]
- Variantes Unicode : suffixe `Unicode` (`sfRenderWindow_createUnicode`, `sfText_setUnicodeString`,
  `sfClipboard_getUnicodeString`, `sfFtpDirectoryResponse_getDirectoryUnicode`, `sf*_setUnicodeTitle`). [CONFIRMÉ]
- Variantes sRGB de texture : `sfTexture_createSrgb*`, `sfTexture_resizeSrgb` (en C++ c'est un paramètre `bool`).
  [CONFIRMÉ header Texture.h ; équivalence C++ DÉDUITE]

### 2.3 Types opaques et struct publiques

- Chaque module déclare ses types opaques dans `Types.h` : `typedef struct sfSprite sfSprite;` etc.
  ([include/CSFML/Graphics/Types.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Graphics/Types.h)).
  On ne manipule que des **pointeurs** ; on ne peut pas déclarer `sfTexture t;` ni faire `sizeof`. [CONFIRMÉ]
- 41 struct opaques en 3.0.0 : System 2 (`sfBuffer`, `sfClock`), Window 5 (`sfContext`, `sfCursor`,
  `sfWindow`, `sfWindowBase`, plus `VkAllocationCallbacks` côté Vulkan), Graphics 16, Audio 6, Network 12.
  [CONFIRMÉ, script]
- Les petits types de valeur sont des struct publiques : `sfVector2i/u/f`, `sfVector3f`, `sfTime`, `sfColor`,
  `sfFloatRect`, `sfIntRect`, `sfTransform` (`float matrix[9]`), `sfVertex`, `sfVideoMode`, `sfContextSettings`,
  `sfBlendMode`, `sfStencilMode`, `sfRenderStates`, `sfGlyph`, `sfFontInfo`, `sfIpAddress` (`char address[16]`),
  `sfJoystickIdentification`, `sfInputStream`… [CONFIRMÉ]

### 2.4 Cycle de vie : create / createFrom… / copy / destroy

- Constructeurs : `sfX_create(...)` ou `sfX_createFromFile/Memory/Stream/Image/Pixels/Color/Samples/Rect/System/Handle`.
  Ils renvoient `sfX*` ; les `createFrom*` documentent « NULL if it failed »
  ([Font.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Graphics/Font.h),
  [Music.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Audio/Music.h)). [CONFIRMÉ]
- Destructeur : `void sfX_destroy(const sfX*)` — **toujours un pointeur `const`**. Les 40 implémentations font
  un simple `delete` (`src/CSFML/**/*.cpp`, tag 3.0.0), donc `sfX_destroy(NULL)` ne fait rien. [CONFIRMÉ source ;
  non documenté dans les headers → à tester]
- Copie : `sfX_copy(const sfX*)` renvoie un nouvel objet à détruire. **Pas de `_copy`** pour : `sfBuffer`,
  `sfContext`, `sfCursor`, `sfWindowBase`, `sfWindow`, `sfRenderWindow`, `sfRenderTexture`, `sfShader`,
  `sfShape`, `sfMusic`, `sfSoundStream`, `sfSoundRecorder`, `sfSoundBufferRecorder`, `sfFtp`, `sfHttp`,
  `sfHttpRequest`, les sockets et les réponses FTP/HTTP. [CONFIRMÉ, inventaire]
- Pièges de nommage : `sfFtp_createDirectory` est une commande FTP (renvoie un `sfFtpResponse*` à détruire),
  `sfImage_createMaskFromColor` modifie l'image (renvoie `void`), `sf*_createVulkanSurface` renvoie `bool`. [CONFIRMÉ]
- `sfWindow_create` et `sfRenderWindow_create` ne renvoient **jamais NULL** en 3.0.0 : l'implémentation fait
  `new` puis `create()` sans test (`src/CSFML/Graphics/RenderWindow.cpp`, `src/CSFML/Window/Window.cpp`, tag 3.0.0).
  Le test `if (!window)` de la doc est donc inutile ; vérifier plutôt `sfRenderWindow_isOpen`. [CONFIRMÉ source,
  comportement en cas d'échec à tester]
- Paramètres « réglages » optionnels : `const sfContextSettings* settings` accepte `NULL` (« pass NULL to use
  default values », [RenderWindow.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Graphics/RenderWindow.h)) ;
  `sfRenderTexture_create` l'accepte aussi (test `settings ?` dans `src/CSFML/Graphics/RenderTexture.cpp`), mais son
  header ne le dit pas. [CONFIRMÉ source]

### 2.5 Ownership : qui garde quoi, qui libère quoi

**A. Objets qui gardent un pointeur (pas de copie)** — la ressource doit vivre plus longtemps que l'objet qui
l'utilise :

| Fonction(s) | Ce qui est gardé | Preuve (tag 3.0.0) | Statut |
|---|---|---|---|
| `sfSprite_create`, `sfSprite_setTexture` | la `sfTexture` | `Graphics/Sprite.h` : « doesn't store its own copy of the texture » | [CONFIRMÉ] |
| `sf{Circle,Convex,Rectangle}Shape_setTexture`, `sfShape_setTexture` | la `sfTexture` | `Graphics/CircleShape.h` etc., même phrase | [CONFIRMÉ] |
| `sfText_create`, `sfText_setFont` | la `sfFont` | `Graphics/Text.h` : « doesn't store its own copy of the font » | [CONFIRMÉ] |
| `sfSound_create`, `sfSound_setBuffer` | le `sfSoundBuffer` | `Audio/Sound.h` : « the sound buffer is not copied » | [CONFIRMÉ] |
| `sfShader_setTextureUniform` | la `sfTexture` | `Graphics/Shader.h` : « texture must remain alive … no copy is made » | [CONFIRMÉ] |
| `sfSocketSelector_add*` | la socket | `Network/SocketSelector.h` : « keeps a weak pointer to the socket » | [CONFIRMÉ] |
| `sfRenderStates.texture`, `.shader` | pointeurs dans la struct | `Graphics/RenderStates.h` (champs `const sfTexture*`, `const sfShader*`) | [CONFIRMÉ] |
| `sf*_setMouseCursor` | le `sfCursor` | header CSFML muet ; SFML 3.0.2 `WindowBase.hpp` l.468 : « The cursor must not be destroyed while in use by the window » | [CONFIRMÉ C++ ; à tester en C] |
| `sfSoundStream_create`, `sfSoundRecorder_create`, `sfShape_create` | `userData` (pointeur brut) | `Audio/SoundStream.h`, `Graphics/Shape.h` | [DÉDUIT] |

**B. Données lues en continu (streaming)** — le header CSFML ne le dit pas, mais la doc C++ de SFML 3.0.2 l'exige :

- `sfFont_createFromFile/Memory/Stream` : le fichier, le buffer `data` ou le stream doit rester accessible tant
  que la police existe (« SFML cannot preload all the font data »,
  https://github.com/SFML/SFML/blob/3.0.2/include/SFML/Graphics/Font.hpp). CSFML appelle bien
  `openFromFile`/`openFromMemory`/`openFromStream` (`src/CSFML/Graphics/Font.cpp`, tag 3.0.0). [CONFIRMÉ C++ +
  source ; absent du header CSFML]
- `sfMusic_createFromFile/Memory/Stream` : idem, la musique est lue en continu (« the data buffer must remain
  accessible », https://github.com/SFML/SFML/blob/3.0.2/include/SFML/Audio/Music.hpp). [CONFIRMÉ C++ + source]
- `sfInputStream` : la **struct** est copiée par valeur dans un adaptateur (`CallbackStream(sfInputStream* stream)
  : myStream(*stream)`, `src/CSFML/CallbackStream.hpp`, tag 3.0.0) ; on peut donc libérer la struct elle-même,
  mais **pas** ce que pointe `userData` tant que la police / musique vit. [CONFIRMÉ source ; conséquence DÉDUITE]
- `sfSoundBuffer_createFrom*`, `sfImage_createFrom*`, `sfTexture_createFrom*` chargent tout d'un coup (pas de
  streaming) : le buffer peut être libéré après l'appel. [DÉDUIT de la doc C++ SFML ; à tester]

**C. Copies (on peut détruire l'original juste après)** :

- `sfRenderWindow_setView` / `sfRenderTexture_setView` copient la vue (`renderWindow->CurrentView = *view`,
  `src/CSFML/Graphics/RenderWindow.cpp`, tag 3.0.0). [CONFIRMÉ source]
- `sfText_setString` / `sfText_setUnicodeString`, `sf*_setTitle`, `sfClipboard_set*` copient la chaîne
  (conversion en `sf::String`). [DÉDUIT de la source]

**D. Pointeurs renvoyés** : voir le tableau complet au §6.1 (à libérer) et §6.2 (empruntés, ne pas libérer).

### 2.6 `const`

- Les getters prennent `const sfX*`, les setters `sfX*`. Les destructeurs prennent `const sfX*`. [CONFIRMÉ]
- Exceptions (getter avec pointeur non const) [CONFIRMÉ, script] :
  `sfFont_getTexture(sfFont*, unsigned int)`, `sfVertexArray_getVertex(sfVertexArray*, size_t)` (renvoie un
  `sfVertex*` modifiable), `sfSoundRecorder_getDevice(sfSoundRecorder*)`,
  `sfSoundBufferRecorder_getDevice(sfSoundBufferRecorder*)`, `sfTransform_equal(sfTransform*, sfTransform*)`.
- `sfSoundStream_create` et `sfSoundBuffer_createFromSamples` prennent `sfSoundChannel* channelMapData` non const
  (il est seulement lu, d'après `src/CSFML/Audio/SoundStreamStruct.hpp` ; non vérifié pour `SoundBuffer`).
  [CONFIRMÉ header ; lecture seule CONFIRMÉE pour SoundStream]

### 2.7 Passage et retour par valeur

- Les struct de valeur (`sfVector2f/i/u`, `sfVector3f`, `sfColor`, `sfFloatRect`, `sfIntRect`, `sfTime`,
  `sfTransform`, `sfVideoMode`, `sfIpAddress`, `sfSoundSourceCone`, `sfListenerCone`, `sfTimeSpan`, `sfGlsl*`
  vecteurs) sont **passées et renvoyées par valeur** : `void sfSprite_setPosition(sfSprite* sprite, sfVector2f
  position);`, `sfFloatRect sfSprite_getGlobalBounds(const sfSprite* sprite);`
  ([Sprite.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Graphics/Sprite.h)). [CONFIRMÉ]
- Exceptions passées **par pointeur** [CONFIRMÉ, inventaire] :
  - pour accepter `NULL` : `const sfContextSettings*`, `const sfRenderStates*` (NULL = états par défaut),
    `const sfIntRect* area` des `sfTexture_createFrom*` (NULL = image entière), `const sfVector2u*` de
    `sf*_setMinimumSize/setMaximumSize` (NULL = pas de limite, `Window/Window.h`), `sfFloatRect* intersection`
    de `sf*Rect_intersects` (« can be NULL ») ;
  - sans raison de nullabilité : `sfFloatRect_contains(const sfFloatRect* rect, sfVector2f point)`,
    `sfIntRect_contains`, les fonctions `sfTransform_*` (`sfTransform*` ou `const sfTransform*`), `sfShader_setMat3Uniform(…,
    const sfGlslMat3* matrix)` ;
  - paramètres de sortie : `size_t* count`, `size_t* received`, `size_t* sent`, `sfTcpSocket** connected`,
    `sfIpAddress* remoteAddress`, `unsigned short* remotePort`, `sfBuffer* output`, `char* string`.
- `sfRenderStates` est une struct à 6 champs. Ne pas l'initialiser à zéro (`= {0}` donnerait un `sfBlendMode` à
  facteurs `Zero`) : partir de `sfRenderStates_default` puis modifier. [DÉDUIT de `Graphics/RenderStates.h` et
  `Graphics/BlendMode.h` ; à tester]

### 2.8 `bool` contre `sfBool`, entiers à taille fixe

- 3.0.0 : `CSFML/Config.h` fait `#include <stdbool.h>` et `#include <stdint.h>` ; toutes les signatures utilisent
  `bool`, `true`, `false`, `int64_t`, `uint8_t`, `uint32_t`… `grep -r 'sfBool\|sfInt64\|sfUint\|sfInt32'` sur
  `include/` du tag 3.0.0 : **0 résultat**. [CONFIRMÉ]
- 2.6.1 : `Config.h` définissait `typedef int sfBool; #define sfFalse 0; #define sfTrue 1;` et `sfInt8`…`sfUint64`
  ([include/SFML/Config.h @ 2.6.1](https://github.com/SFML/CSFML/blob/2.6.1/include/SFML/Config.h)). [CONFIRMÉ]
- Conséquence : du code CSFML 2.x qui utilise `sfBool`/`sfTrue`/`sfInt64` ne compile plus. [DÉDUIT]
- Le dossier d'en-têtes a aussi changé : `include/SFML/…` (2.6.1) → `include/CSFML/…` (3.0.0). [CONFIRMÉ, `git ls-tree`]

### 2.9 Temps : `sfTime`

- `typedef struct { int64_t microseconds; } sfTime;` et la constante `sfTime_Zero`
  ([System/Time.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/System/Time.h)). [CONFIRMÉ]
- Construction : `sfSeconds(float)`, `sfMilliseconds(int32_t)`, `sfMicroseconds(int64_t)`. Lecture :
  `sfTime_asSeconds` (`float`), `sfTime_asMilliseconds` (`int32_t`), `sfTime_asMicroseconds` (`int64_t`). [CONFIRMÉ]
- Aucune fonction d'addition / comparaison : on manipule `.microseconds` directement. [CONFIRMÉ, inventaire]
- `sfClock` gagne `sfClock_isRunning`, `sfClock_start`, `sfClock_stop`, `sfClock_reset` (en plus de `restart`)
  ([System/Clock.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/System/Clock.h)). [CONFIRMÉ]
- Les fonctions bloquantes prennent un `sfTime timeout` (`sf*_waitEvent`, `sfHttp_sendRequest`,
  `sfTcpSocket_connect`, `sfSocketSelector_wait`, `sfIpAddress_getPublicAddress`, `sfFtp_connect`). Pour
  `waitEvent`, `sfTime_Zero` = attente infinie (doc de `RenderWindow.h`). [CONFIRMÉ]

### 2.10 Callbacks et `userData`

| Callback (typedef, tag 3.0.0) | Utilisé par | `userData` | Remarques |
|---|---|---|---|
| `sfInputStreamReadFunc/SeekFunc/TellFunc/GetSizeFunc` (`System/InputStream.h`) | `sf*_createFromStream` (Font, Image, Texture, Shader, SoundBuffer, Music) | champ `userData` de `sfInputStream`, passé en dernier argument | retour `int64_t` ; **-1 = erreur** (converti en `std::nullopt` par `src/CSFML/CallbackStream.hpp`) ; un callback NULL = erreur. [CONFIRMÉ source, non documenté dans le header] |
| `sfShapeGetPointCountCallback`, `sfShapeGetPointCallback` (`Graphics/Shape.h`) | `sfShape_create` | 3e argument | appeler `sfShape_update` quand les points changent. [CONFIRMÉ] |
| `sfSoundStreamGetDataCallback` (renvoie `bool`), `sfSoundStreamSeekCallback` (`Audio/SoundStream.h`) | `sfSoundStream_create` | dernier argument | les deux « can't be NULL » (doc) ; la source ne teste que `onSeek`. Appelés depuis le thread audio. [CONFIRMÉ header+source ; thread DÉDUIT de SFML] |
| `sfSoundRecorderStartCallback`, `ProcessCallback`, `StopCallback` (`Audio/SoundRecorder.h`) | `sfSoundRecorder_create` | dernier argument (peut être NULL) | `onStart`/`onStop` peuvent être NULL. [CONFIRMÉ] |
| `sfEffectProcessor` (`Audio/EffectProcessor.h`) | `sfSound/sfMusic/sfSoundStream_setEffectProcessor` | **aucun** | pas de `userData` : l'état doit être global ; appelé sur le thread audio. [CONFIRMÉ] |
| `sfGlFunctionPointer` (`Window/Context.h`), `sfVulkanFunctionPointer` (`Window/Vulkan.h`) | `sfContext_getFunction`, `sfVulkan_getFunction` | — | pointeurs de fonctions OpenGL / Vulkan. [CONFIRMÉ] |

### 2.11 Unités

- Angles : `float` en **degrés** (« in degrees » dans `Transformable.h`, `Sprite.h`, `Shape.h`, `Text.h`,
  `View.h`, `Transform.h`, cônes audio `SoundSourceCone.h`/`Listener.h`). CSFML convertit avec `sf::degrees()`
  (30 occurrences dans `src/`, 0 `sf::radians`). Il n'y a **pas** de type `sfAngle` en C, alors que SFML 3 C++ a
  `sf::Angle`. [CONFIRMÉ]
- Les getters de rotation devraient renvoyer un angle ramené dans [0, 360) : SFML 3.0.2 stocke
  `angle.wrapUnsigned()` (`src/SFML/Graphics/Transformable.cpp` l.47). [DÉDUIT ; à tester]
- Couleurs : composantes `uint8_t` 0–255 (`Graphics/Color.h`). [CONFIRMÉ]
- Temps : `sfTime` (µs) ; capteurs : gyroscope en degrés/s, orientation en degrés (`Window/Sensor.h`). [CONFIRMÉ]
- Positions et tailles : `float` (monde) ou `int`/`unsigned` (pixels, fenêtre). [CONFIRMÉ, types des signatures]
- Styles : `uint32_t style` combinant `sfWindowStyle` (`sfTitlebar | sfResize | sfClose`) ; le plein écran n'est
  plus un style mais un `sfWindowState` (`sfWindowed`, `sfFullscreen`) passé à part
  ([Window/WindowBase.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Window/WindowBase.h)). [CONFIRMÉ]
- Styles de texte : `uint32_t` combinant `sfTextStyle` (`Graphics/Text.h`). [CONFIRMÉ]

### 2.12 Événements : `sfEvent`

- `sfEvent` est une `union` de 13 membres : `type`, `size`, `key`, `text`, `mouseMove`, `mouseMoveRaw`,
  `mouseButton`, `mouseWheelScroll`, `joystickMove`, `joystickButton`, `joystickConnect`, `touch`, `sensor`.
  Chaque struct membre commence par `sfEventType type`
  ([Window/Event.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/Window/Event.h)). [CONFIRMÉ]
- `sfEventType` : 23 types + `sfEvtCount`. Nouveau en 3.0 : `sfEvtMouseMovedRaw` (champ `mouseMoveRaw.delta`). [CONFIRMÉ]
- Champs en 3.0.0 : `mouseMove.position`, `mouseButton.position`, `mouseWheelScroll.position`, `touch.position`
  (`sfVector2i`), `size.size` (`sfVector2u`), `key.code` + `key.scancode` + `alt/control/shift/system` (`bool`),
  `text.unicode` (`uint32_t`), `sensor.value` (`sfVector3f`). En 2.6.1 c'étaient des champs `x`, `y`, `width`,
  `height` (`include/SFML/Window/Event.h` @ 2.6.1, lignes 104-189). [CONFIRMÉ]
- Lecture : `bool sfRenderWindow_pollEvent(sfRenderWindow* renderWindow, sfEvent* event);` (non bloquante, à
  appeler en boucle) et `bool sfRenderWindow_waitEvent(sfRenderWindow* renderWindow, sfTime timeout, sfEvent*
  event);` (bloquante, `false` en cas de timeout). Mêmes fonctions pour `sfWindow_*` et `sfWindowBase_*`.
  En 2.6.1 : `sfBool sfWindow_waitEvent(sfWindow* window, sfEvent* event);` sans timeout. [CONFIRMÉ]
- Défaut de doc : le commentaire de `sfEvtMouseMovedRaw` dit « data in event.mouseMove » alors que le champ est
  `mouseMoveRaw`. [CONFIRMÉ, `Window/Event.h` tag 3.0.0]

### 2.13 Chaînes de caractères

- `const char*` = chaîne « ANSI » (dépend de la locale), `const sfChar32*` = UTF-32 terminé par 0
  (`Window/Clipboard.h` : « locale-dependent ANSI string » / « UTF-32 »). [CONFIRMÉ]
- Conséquence probable : passer de l'UTF-8 (« é ») à `sfText_setString` ou `sfRenderWindow_create` affiche des
  caractères faux ; il faut `sfText_setUnicodeString` / `*_createUnicode` avec un tableau UTF-32. [DÉDUIT :
  `sfText_setString` appelle `text->setString(string)` qui construit un `sf::String` depuis un `const char*`
  ANSI (`src/CSFML/Graphics/Text.cpp`) ; à tester, important pour une doc en français]
- `sfChar32` est `uint32_t` (`Config.h`) ; un littéral C11 `U"été"` (`char32_t`, `uint_least32_t`) devrait être
  compatible sous Linux/glibc. [DÉDUIT ; à tester]

### 2.14 Vérification des arguments

- L'implémentation vérifie les pointeurs avec `assert(...)` (ex. `sfSprite_create` : `assert(texture)`,
  `src/CSFML/Graphics/Sprite.cpp`, tag 3.0.0). La note de release 3.0.0 mentionne « assertions against null
  pointers » (https://github.com/SFML/CSFML/releases/tag/3.0.0). Dans une lib compilée en Release (`NDEBUG`),
  ces `assert` disparaissent : un `NULL` donne un comportement indéfini (crash probable), pas un message.
  [CONFIRMÉ source ; comportement du paquet Ubuntu à tester]
- Paramètres où `NULL` est documenté comme valide : `settings`, `states`, `area`, `intersection`, chemins de
  shader (« or NULL to skip this shader »), `sfTexture_bind(NULL, …)`, `sfShader_bind(NULL)`,
  `sfVertexBuffer_bind(NULL)`, `onStart`/`onStop`/`userData` de `sfSoundRecorder_create`, `relativeTo` de
  `sfMouse_getPosition` (NULL = bureau), `minimumSize`/`maximumSize`. [CONFIRMÉ, `grep '\\param.*NULL'`]

## 3. Headers « racine » (hors modules)

| Header (tag 3.0.0) | Contenu |
|---|---|
| `include/CSFML/Config.h` | `sfChar32`, `CSFML_VERSION_*`, macros d'export (`CSFML_API_EXPORT/IMPORT`, `CSFML_EXTERN_C`), `CSFML_DEPRECATED`, détection d'OS (`CSFML_SYSTEM_WINDOWS/LINUX/MACOS/FREEBSD`) |
| `include/CSFML/GPUPreference.h` | macro `SFML_DEFINE_DISCRETE_GPU_PREFERENCE` (exporte `NvOptimusEnablement` et `AmdPowerXpressRequestHighPerformance` sous Windows, vide ailleurs) |
| `include/CSFML/OpenGL.h` | inclut `<GL/gl.h>` et `<GL/glu.h>` (ou `<OpenGL/gl.h>` sous macOS, plus `<windows.h>` sous Windows) |
| `include/CSFML/{System,Window,Graphics,Audio,Network}.h` | headers parapluies (voir 2.1) |

```c
typedef uint32_t sfChar32;                 /* CSFML/Config.h */
#define CSFML_VERSION_MAJOR 3              /* CSFML/Config.h */
#define CSFML_VERSION_MINOR 0
#define CSFML_VERSION_PATCH 0
#define SFML_DEFINE_DISCRETE_GPU_PREFERENCE /* CSFML/GPUPreference.h, contenu selon l'OS */
```

Remarque : la macro garde le préfixe `SFML_` (pas `CSFML_`) et teste `SFML_SYSTEM_WINDOWS`, alors que
`Config.h` définit `CSFML_SYSTEM_WINDOWS`. Elle risque donc d'être toujours vide, même sous Windows.
[CONFIRMÉ par lecture de `GPUPreference.h` l.59 ; effet DÉDUIT, à tester sous Windows (hors périmètre Linux)]

`sfWindowHandle` dépend de la plateforme (`Window/WindowHandle.h`) : `struct HWND__*` (Windows), `unsigned long`
(Linux/FreeBSD, X11), `void*` (macOS). `VkSurfaceKHR` est un pointeur sur 64 bits, `uint64_t` sinon
(`Window/Vulkan.h`). [CONFIRMÉ]

## 4. Inventaire par module (tag 3.0.0)

Un sous-chapitre par module : types, définition des types non opaques, constantes, puis toutes les
fonctions groupées par header (et par préfixe de type à l'intérieur d'un header).


### Module System (`csfml-system`)

Toutes les déclarations ci-dessous sont copiées des headers du tag git `3.0.0` (extraction automatique, voir « Méthode »). [CONFIRMÉ]

#### System — types (12)

| Nom | Nature | Header (tag 3.0.0) |
|---|---|---|
| `sfInputStreamReadFunc` | pointeur de fonction (callback) | `include/CSFML/System/InputStream.h` |
| `sfInputStreamSeekFunc` | pointeur de fonction (callback) | `include/CSFML/System/InputStream.h` |
| `sfInputStreamTellFunc` | pointeur de fonction (callback) | `include/CSFML/System/InputStream.h` |
| `sfInputStreamGetSizeFunc` | pointeur de fonction (callback) | `include/CSFML/System/InputStream.h` |
| `sfInputStream` | struct (champs publics) | `include/CSFML/System/InputStream.h` |
| `sfTime` | struct (champs publics) | `include/CSFML/System/Time.h` |
| `sfBuffer` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/System/Types.h` |
| `sfClock` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/System/Types.h` |
| `sfVector2i` | struct (champs publics) | `include/CSFML/System/Vector2.h` |
| `sfVector2u` | struct (champs publics) | `include/CSFML/System/Vector2.h` |
| `sfVector2f` | struct (champs publics) | `include/CSFML/System/Vector2.h` |
| `sfVector3f` | struct (champs publics) | `include/CSFML/System/Vector3.h` |

Définition des types non opaques (format compact, une ligne par struct) :

```c
typedef int64_t (*sfInputStreamReadFunc)(void* data, size_t size, void* userData);   /* CSFML/System/InputStream.h */
typedef int64_t (*sfInputStreamSeekFunc)(size_t position, void* userData);   /* CSFML/System/InputStream.h */
typedef int64_t (*sfInputStreamTellFunc)(void* userData);   /* CSFML/System/InputStream.h */
typedef int64_t (*sfInputStreamGetSizeFunc)(void* userData);   /* CSFML/System/InputStream.h */
typedef struct sfInputStream { sfInputStreamReadFunc read; sfInputStreamSeekFunc seek; sfInputStreamTellFunc tell; sfInputStreamGetSizeFunc getSize; void* userData; } sfInputStream;   /* CSFML/System/InputStream.h */
typedef struct { int64_t microseconds; } sfTime;   /* CSFML/System/Time.h */
typedef struct { int x; int y; } sfVector2i;   /* CSFML/System/Vector2.h */
typedef struct { unsigned int x; unsigned int y; } sfVector2u;   /* CSFML/System/Vector2.h */
typedef struct { float x; float y; } sfVector2f;   /* CSFML/System/Vector2.h */
typedef struct { float x; float y; float z; } sfVector3f;   /* CSFML/System/Vector3.h */
```

#### System — constantes

```c
CSFML_SYSTEM_API const sfTime sfTime_Zero;   /* CSFML/System/Time.h */
```

#### System — fonctions (21)

##### `include/CSFML/System/Alloc.h` — 1 fonction(s)

```c
void sfFree(void* ptr);
```

##### `include/CSFML/System/Buffer.h` — 4 fonction(s)

```c
sfBuffer* sfBuffer_create(void);
void sfBuffer_destroy(const sfBuffer* buffer);
size_t sfBuffer_getSize(const sfBuffer* buffer);
const uint8_t* sfBuffer_getData(const sfBuffer* buffer);
```

##### `include/CSFML/System/Clock.h` — 9 fonction(s)

```c
sfClock* sfClock_create(void);
sfClock* sfClock_copy(const sfClock* clock);
void sfClock_destroy(const sfClock* clock);
sfTime sfClock_getElapsedTime(const sfClock* clock);
bool sfClock_isRunning(const sfClock* clock);
void sfClock_start(sfClock* clock);
void sfClock_stop(sfClock* clock);
sfTime sfClock_restart(sfClock* clock);
sfTime sfClock_reset(sfClock* clock);
```

##### `include/CSFML/System/Sleep.h` — 1 fonction(s)

```c
void sfSleep(sfTime duration);
```

##### `include/CSFML/System/Time.h` — 6 fonction(s)

```c
/* --- sfTime --- */
float sfTime_asSeconds(sfTime time);
int32_t sfTime_asMilliseconds(sfTime time);
int64_t sfTime_asMicroseconds(sfTime time);

/* --- (libre) --- */
sfTime sfSeconds(float amount);
sfTime sfMilliseconds(int32_t amount);
sfTime sfMicroseconds(int64_t amount);
```

### Module Window (`csfml-window`)

Toutes les déclarations ci-dessous sont copiées des headers du tag git `3.0.0` (extraction automatique, voir « Méthode »). [CONFIRMÉ]

#### Window — types (37)

| Nom | Nature | Header (tag 3.0.0) |
|---|---|---|
| `sfGlFunctionPointer` | pointeur de fonction (callback) | `include/CSFML/Window/Context.h` |
| `sfCursorType` | enum | `include/CSFML/Window/Cursor.h` |
| `sfEventType` | enum | `include/CSFML/Window/Event.h` |
| `sfKeyEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfTextEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfMouseMoveEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfMouseMoveRawEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfMouseButtonEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfMouseWheelScrollEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfJoystickMoveEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfJoystickButtonEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfJoystickConnectEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfSizeEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfTouchEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfSensorEvent` | struct (champs publics) | `include/CSFML/Window/Event.h` |
| `sfEvent` | union | `include/CSFML/Window/Event.h` |
| `sfJoystickAxis` | enum | `include/CSFML/Window/Joystick.h` |
| `sfJoystickIdentification` | struct (champs publics) | `include/CSFML/Window/JoystickIdentification.h` |
| `sfKeyCode` | enum | `include/CSFML/Window/Keyboard.h` |
| `sfScancode` | enum | `include/CSFML/Window/Keyboard.h` |
| `sfMouseButton` | enum | `include/CSFML/Window/Mouse.h` |
| `sfMouseWheel` | enum | `include/CSFML/Window/Mouse.h` |
| `sfSensorType` | enum | `include/CSFML/Window/Sensor.h` |
| `sfContext` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Window/Types.h` |
| `sfCursor` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Window/Types.h` |
| `sfWindow` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Window/Types.h` |
| `sfWindowBase` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Window/Types.h` |
| `sfVideoMode` | struct (champs publics) | `include/CSFML/Window/VideoMode.h` |
| `VkInstance` | alias (typedef) | `include/CSFML/Window/Vulkan.h` |
| `VkSurfaceKHR` | alias (typedef dépendant de la plateforme) | `include/CSFML/Window/Vulkan.h` |
| `VkAllocationCallbacks` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Window/Vulkan.h` |
| `sfVulkanFunctionPointer` | pointeur de fonction (callback) | `include/CSFML/Window/Vulkan.h` |
| `sfContextAttribute` | enum | `include/CSFML/Window/Window.h` |
| `sfContextSettings` | struct (champs publics) | `include/CSFML/Window/Window.h` |
| `sfWindowStyle` | enum | `include/CSFML/Window/WindowBase.h` |
| `sfWindowState` | enum | `include/CSFML/Window/WindowBase.h` |
| `sfWindowHandle` | alias (typedef dépendant de la plateforme) | `include/CSFML/Window/WindowHandle.h` |

Définition des types non opaques (format compact, une ligne par struct) :

```c
typedef void (*sfGlFunctionPointer)(void);   /* CSFML/Window/Context.h */
typedef enum {   /* CSFML/Window/Cursor.h, 21 valeurs */
    sfCursorArrow, sfCursorArrowWait, sfCursorWait, sfCursorText, sfCursorHand,
    sfCursorSizeHorizontal, sfCursorSizeVertical, sfCursorSizeTopLeftBottomRight,
    sfCursorSizeBottomLeftTopRight, sfCursorSizeLeft, sfCursorSizeRight, sfCursorSizeTop,
    sfCursorSizeBottom, sfCursorSizeTopLeft, sfCursorSizeBottomRight, sfCursorSizeBottomLeft,
    sfCursorSizeTopRight, sfCursorSizeAll, sfCursorCross, sfCursorHelp, sfCursorNotAllowed
} sfCursorType;
typedef enum {   /* CSFML/Window/Event.h, 24 valeurs */
    sfEvtClosed, sfEvtResized, sfEvtFocusLost, sfEvtFocusGained, sfEvtTextEntered, sfEvtKeyPressed,
    sfEvtKeyReleased, sfEvtMouseWheelScrolled, sfEvtMouseButtonPressed, sfEvtMouseButtonReleased,
    sfEvtMouseMoved, sfEvtMouseMovedRaw, sfEvtMouseEntered, sfEvtMouseLeft,
    sfEvtJoystickButtonPressed, sfEvtJoystickButtonReleased, sfEvtJoystickMoved,
    sfEvtJoystickConnected, sfEvtJoystickDisconnected, sfEvtTouchBegan, sfEvtTouchMoved,
    sfEvtTouchEnded, sfEvtSensorChanged, sfEvtCount
} sfEventType;
typedef struct { sfEventType type; sfKeyCode code; sfScancode scancode; bool alt; bool control; bool shift; bool system; } sfKeyEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; uint32_t unicode; } sfTextEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfVector2i position; } sfMouseMoveEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfVector2i delta; } sfMouseMoveRawEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfMouseButton button; sfVector2i position; } sfMouseButtonEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfMouseWheel wheel; float delta; sfVector2i position; } sfMouseWheelScrollEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; unsigned int joystickId; sfJoystickAxis axis; float position; } sfJoystickMoveEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; unsigned int joystickId; unsigned int button; } sfJoystickButtonEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; unsigned int joystickId; } sfJoystickConnectEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfVector2u size; } sfSizeEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; unsigned int finger; sfVector2i position; } sfTouchEvent;   /* CSFML/Window/Event.h */
typedef struct { sfEventType type; sfSensorType sensorType; sfVector3f value; } sfSensorEvent;   /* CSFML/Window/Event.h */
typedef union { sfEventType type; sfSizeEvent size; sfKeyEvent key; sfTextEvent text; sfMouseMoveEvent mouseMove; sfMouseMoveRawEvent mouseMoveRaw; sfMouseButtonEvent mouseButton; sfMouseWheelScrollEvent mouseWheelScroll; sfJoystickMoveEvent joystickMove; sfJoystickButtonEvent joystickButton; sfJoystickConnectEvent joystickConnect; sfTouchEvent touch; sfSensorEvent sensor; } sfEvent;   /* CSFML/Window/Event.h */
typedef enum {   /* CSFML/Window/Joystick.h, 8 valeurs */
    sfJoystickX, sfJoystickY, sfJoystickZ, sfJoystickR, sfJoystickU, sfJoystickV, sfJoystickPovX,
    sfJoystickPovY
} sfJoystickAxis;
typedef struct { const char* name; unsigned int vendorId; unsigned int productId; } sfJoystickIdentification;   /* CSFML/Window/JoystickIdentification.h */
typedef enum {   /* CSFML/Window/Keyboard.h, 102 valeurs */
    sfKeyUnknown = -1, sfKeyA, sfKeyB, sfKeyC, sfKeyD, sfKeyE, sfKeyF, sfKeyG, sfKeyH, sfKeyI,
    sfKeyJ, sfKeyK, sfKeyL, sfKeyM, sfKeyN, sfKeyO, sfKeyP, sfKeyQ, sfKeyR, sfKeyS, sfKeyT, sfKeyU,
    sfKeyV, sfKeyW, sfKeyX, sfKeyY, sfKeyZ, sfKeyNum0, sfKeyNum1, sfKeyNum2, sfKeyNum3, sfKeyNum4,
    sfKeyNum5, sfKeyNum6, sfKeyNum7, sfKeyNum8, sfKeyNum9, sfKeyEscape, sfKeyLControl, sfKeyLShift,
    sfKeyLAlt, sfKeyLSystem, sfKeyRControl, sfKeyRShift, sfKeyRAlt, sfKeyRSystem, sfKeyMenu,
    sfKeyLBracket, sfKeyRBracket, sfKeySemicolon, sfKeyComma, sfKeyPeriod, sfKeyApostrophe,
    sfKeySlash, sfKeyBackslash, sfKeyGrave, sfKeyEqual, sfKeyHyphen, sfKeySpace, sfKeyEnter,
    sfKeyBackspace, sfKeyTab, sfKeyPageUp, sfKeyPageDown, sfKeyEnd, sfKeyHome, sfKeyInsert,
    sfKeyDelete, sfKeyAdd, sfKeySubtract, sfKeyMultiply, sfKeyDivide, sfKeyLeft, sfKeyRight,
    sfKeyUp, sfKeyDown, sfKeyNumpad0, sfKeyNumpad1, sfKeyNumpad2, sfKeyNumpad3, sfKeyNumpad4,
    sfKeyNumpad5, sfKeyNumpad6, sfKeyNumpad7, sfKeyNumpad8, sfKeyNumpad9, sfKeyF1, sfKeyF2, sfKeyF3,
    sfKeyF4, sfKeyF5, sfKeyF6, sfKeyF7, sfKeyF8, sfKeyF9, sfKeyF10, sfKeyF11, sfKeyF12, sfKeyF13,
    sfKeyF14, sfKeyF15, sfKeyPause
} sfKeyCode;
typedef enum {   /* CSFML/Window/Keyboard.h, 147 valeurs */
    sfScanUnknown = -1, sfScanA = 0, sfScanB, sfScanC, sfScanD, sfScanE, sfScanF, sfScanG, sfScanH,
    sfScanI, sfScanJ, sfScanK, sfScanL, sfScanM, sfScanN, sfScanO, sfScanP, sfScanQ, sfScanR,
    sfScanS, sfScanT, sfScanU, sfScanV, sfScanW, sfScanX, sfScanY, sfScanZ, sfScanNum1, sfScanNum2,
    sfScanNum3, sfScanNum4, sfScanNum5, sfScanNum6, sfScanNum7, sfScanNum8, sfScanNum9, sfScanNum0,
    sfScanEnter, sfScanEscape, sfScanBackspace, sfScanTab, sfScanSpace, sfScanHyphen, sfScanEqual,
    sfScanLBracket, sfScanRBracket, sfScanBackslash, sfScanSemicolon, sfScanApostrophe, sfScanGrave,
    sfScanComma, sfScanPeriod, sfScanSlash, sfScanF1, sfScanF2, sfScanF3, sfScanF4, sfScanF5,
    sfScanF6, sfScanF7, sfScanF8, sfScanF9, sfScanF10, sfScanF11, sfScanF12, sfScanF13, sfScanF14,
    sfScanF15, sfScanF16, sfScanF17, sfScanF18, sfScanF19, sfScanF20, sfScanF21, sfScanF22,
    sfScanF23, sfScanF24, sfScanCapsLock, sfScanPrintScreen, sfScanScrollLock, sfScanPause,
    sfScanInsert, sfScanHome, sfScanPageUp, sfScanDelete, sfScanEnd, sfScanPageDown, sfScanRight,
    sfScanLeft, sfScanDown, sfScanUp, sfScanNumLock, sfScanNumpadDivide, sfScanNumpadMultiply,
    sfScanNumpadMinus, sfScanNumpadPlus, sfScanNumpadEqual, sfScanNumpadEnter, sfScanNumpadDecimal,
    sfScanNumpad1, sfScanNumpad2, sfScanNumpad3, sfScanNumpad4, sfScanNumpad5, sfScanNumpad6,
    sfScanNumpad7, sfScanNumpad8, sfScanNumpad9, sfScanNumpad0, sfScanNonUsBackslash,
    sfScanApplication, sfScanExecute, sfScanModeChange, sfScanHelp, sfScanMenu, sfScanSelect,
    sfScanRedo, sfScanUndo, sfScanCut, sfScanCopy, sfScanPaste, sfScanVolumeMute, sfScanVolumeUp,
    sfScanVolumeDown, sfScanMediaPlayPause, sfScanMediaStop, sfScanMediaNextTrack,
    sfScanMediaPreviousTrack, sfScanLControl, sfScanLShift, sfScanLAlt, sfScanLSystem,
    sfScanRControl, sfScanRShift, sfScanRAlt, sfScanRSystem, sfScanBack, sfScanForward,
    sfScanRefresh, sfScanStop, sfScanSearch, sfScanFavorites, sfScanHomePage,
    sfScanLaunchApplication1, sfScanLaunchApplication2, sfScanLaunchMail, sfScanLaunchMediaSelect
} sfScancode;
typedef enum {   /* CSFML/Window/Mouse.h, 5 valeurs */
    sfMouseLeft, sfMouseRight, sfMouseMiddle, sfMouseButtonExtra1, sfMouseButtonExtra2
} sfMouseButton;
typedef enum {   /* CSFML/Window/Mouse.h, 2 valeurs */
    sfMouseVerticalWheel, sfMouseHorizontalWheel
} sfMouseWheel;
typedef enum {   /* CSFML/Window/Sensor.h, 6 valeurs */
    sfSensorAccelerometer, sfSensorGyroscope, sfSensorMagnetometer, sfSensorGravity,
    sfSensorUserAcceleration, sfSensorOrientation
} sfSensorType;
typedef struct { sfVector2u size; unsigned int bitsPerPixel; } sfVideoMode;   /* CSFML/Window/VideoMode.h */
typedef struct VkInstance_T* VkInstance;   /* CSFML/Window/Vulkan.h */
typedef void (*sfVulkanFunctionPointer)(void);   /* CSFML/Window/Vulkan.h */
typedef enum {   /* CSFML/Window/Window.h, 3 valeurs */
    sfContextDefault = 0, sfContextCore = 1 << 0, sfContextDebug = 1 << 2
} sfContextAttribute;
typedef struct { unsigned int depthBits; unsigned int stencilBits; unsigned int antiAliasingLevel; unsigned int majorVersion; unsigned int minorVersion; uint32_t attributeFlags; bool sRgbCapable; } sfContextSettings;   /* CSFML/Window/Window.h */
typedef enum {   /* CSFML/Window/WindowBase.h, 5 valeurs */
    sfNone = 0, sfTitlebar = 1 << 0, sfResize = 1 << 1, sfClose = 1 << 2, sfDefaultStyle =
    sfTitlebar | sfResize | sfClose
} sfWindowStyle;
typedef enum {   /* CSFML/Window/WindowBase.h, 2 valeurs */
    sfWindowed, sfFullscreen
} sfWindowState;
```

#### Window — constantes

```c
enum { sfJoystickCount = 8, sfJoystickButtonCount = 32, sfJoystickAxisCount = 8 };   /* CSFML/Window/Joystick.h */
enum { sfKeyCount = sfKeyPause + 1 };   /* CSFML/Window/Keyboard.h */
enum { sfScancodeCount = sfScanLaunchMediaSelect + 1 };   /* CSFML/Window/Keyboard.h */
enum { sfMouseButtonCount = sfMouseButtonExtra2 + 1 };   /* CSFML/Window/Mouse.h */
enum { sfSensorCount = sfSensorOrientation + 1 };   /* CSFML/Window/Sensor.h */
```

#### Window — fonctions (103)

##### `include/CSFML/Window/Clipboard.h` — 4 fonction(s)

```c
const char* sfClipboard_getString(void);
const sfChar32* sfClipboard_getUnicodeString(void);
void sfClipboard_setString(const char* text);
void sfClipboard_setUnicodeString(const sfChar32* text);
```

##### `include/CSFML/Window/Context.h` — 7 fonction(s)

```c
sfContext* sfContext_create(void);
void sfContext_destroy(const sfContext* context);
bool sfContext_isExtensionAvailable(const char* name);
bool sfContext_setActive(sfContext* context, bool active);
sfGlFunctionPointer sfContext_getFunction(const char* name);
sfContextSettings sfContext_getSettings(const sfContext* context);
uint64_t sfContext_getActiveContextId(void);
```

##### `include/CSFML/Window/Cursor.h` — 3 fonction(s)

```c
sfCursor* sfCursor_createFromPixels(const uint8_t* pixels, sfVector2u size, sfVector2u hotspot);
sfCursor* sfCursor_createFromSystem(sfCursorType type);
void sfCursor_destroy(const sfCursor* cursor);
```

##### `include/CSFML/Window/Joystick.h` — 7 fonction(s)

```c
bool sfJoystick_isConnected(unsigned int joystick);
unsigned int sfJoystick_getButtonCount(unsigned int joystick);
bool sfJoystick_hasAxis(unsigned int joystick, sfJoystickAxis axis);
bool sfJoystick_isButtonPressed(unsigned int joystick, unsigned int button);
float sfJoystick_getAxisPosition(unsigned int joystick, sfJoystickAxis axis);
sfJoystickIdentification sfJoystick_getIdentification(unsigned int joystick);
void sfJoystick_update(void);
```

##### `include/CSFML/Window/Keyboard.h` — 6 fonction(s)

```c
bool sfKeyboard_isKeyPressed(sfKeyCode key);
bool sfKeyboard_isScancodePressed(sfScancode code);
sfKeyCode sfKeyboard_localize(sfScancode code);
sfScancode sfKeyboard_delocalize(sfKeyCode key);
const char* sfKeyboard_getDescription(sfScancode code);
void sfKeyboard_setVirtualKeyboardVisible(bool visible);
```

##### `include/CSFML/Window/Mouse.h` — 5 fonction(s)

```c
bool sfMouse_isButtonPressed(sfMouseButton button);
sfVector2i sfMouse_getPosition(const sfWindow* relativeTo);
void sfMouse_setPosition(sfVector2i position, const sfWindow* relativeTo);
sfVector2i sfMouse_getPositionWindowBase(const sfWindowBase* relativeTo);
void sfMouse_setPositionWindowBase(sfVector2i position, const sfWindowBase* relativeTo);
```

##### `include/CSFML/Window/Sensor.h` — 3 fonction(s)

```c
bool sfSensor_isAvailable(sfSensorType sensor);
void sfSensor_setEnabled(sfSensorType sensor, bool enabled);
sfVector3f sfSensor_getValue(sfSensorType sensor);
```

##### `include/CSFML/Window/Touch.h` — 3 fonction(s)

```c
bool sfTouch_isDown(unsigned int finger);
sfVector2i sfTouch_getPosition(unsigned int finger, const sfWindow* relativeTo);
sfVector2i sfTouch_getPositionWindowBase(unsigned int finger, const sfWindowBase* relativeTo);
```

##### `include/CSFML/Window/VideoMode.h` — 3 fonction(s)

```c
sfVideoMode sfVideoMode_getDesktopMode(void);
const sfVideoMode* sfVideoMode_getFullscreenModes(size_t* count);
bool sfVideoMode_isValid(sfVideoMode mode);
```

##### `include/CSFML/Window/Vulkan.h` — 3 fonction(s)

```c
bool sfVulkan_isAvailable(bool requireGraphics);
sfVulkanFunctionPointer sfVulkan_getFunction(const char* name);
const char* const* sfVulkan_getGraphicsRequiredInstanceExtensions(size_t* count);
```

##### `include/CSFML/Window/Window.h` — 32 fonction(s)

```c
sfWindow* sfWindow_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);
sfWindow* sfWindow_createUnicode(sfVideoMode mode, const sfChar32* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);
sfWindow* sfWindow_createFromHandle(sfWindowHandle handle, const sfContextSettings* settings);
void sfWindow_destroy(const sfWindow* window);
void sfWindow_close(sfWindow* window);
bool sfWindow_isOpen(const sfWindow* window);
sfContextSettings sfWindow_getSettings(const sfWindow* window);
bool sfWindow_pollEvent(sfWindow* window, sfEvent* event);
bool sfWindow_waitEvent(sfWindow* window, sfTime timeout, sfEvent* event);
sfVector2i sfWindow_getPosition(const sfWindow* window);
void sfWindow_setPosition(sfWindow* window, sfVector2i position);
sfVector2u sfWindow_getSize(const sfWindow* window);
void sfWindow_setSize(sfWindow* window, sfVector2u size);
void sfWindow_setMinimumSize(sfWindow* window, const sfVector2u* minimumSize);
void sfWindow_setMaximumSize(sfWindow* window, const sfVector2u* maximumSize);
void sfWindow_setTitle(sfWindow* window, const char* title);
void sfWindow_setUnicodeTitle(sfWindow* window, const sfChar32* title);
void sfWindow_setIcon(sfWindow* window, sfVector2u size, const uint8_t* pixels);
void sfWindow_setVisible(sfWindow* window, bool visible);
void sfWindow_setVerticalSyncEnabled(sfWindow* window, bool enabled);
void sfWindow_setMouseCursorVisible(sfWindow* window, bool visible);
void sfWindow_setMouseCursorGrabbed(sfWindow* window, bool grabbed);
void sfWindow_setMouseCursor(sfWindow* window, const sfCursor* cursor);
void sfWindow_setKeyRepeatEnabled(sfWindow* window, bool enabled);
void sfWindow_setFramerateLimit(sfWindow* window, unsigned int limit);
void sfWindow_setJoystickThreshold(sfWindow* window, float threshold);
bool sfWindow_setActive(sfWindow* window, bool active);
void sfWindow_requestFocus(sfWindow* window);
bool sfWindow_hasFocus(const sfWindow* window);
void sfWindow_display(sfWindow* window);
sfWindowHandle sfWindow_getNativeHandle(const sfWindow* window);
bool sfWindow_createVulkanSurface(sfWindow* window, const VkInstance* instance, VkSurfaceKHR* surface, const VkAllocationCallbacks* allocator);
```

##### `include/CSFML/Window/WindowBase.h` — 27 fonction(s)

```c
sfWindowBase* sfWindowBase_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state);
sfWindowBase* sfWindowBase_createUnicode(sfVideoMode mode, const sfChar32* title, uint32_t style, sfWindowState state);
sfWindowBase* sfWindowBase_createFromHandle(sfWindowHandle handle);
void sfWindowBase_destroy(const sfWindowBase* windowBase);
void sfWindowBase_close(sfWindowBase* windowBase);
bool sfWindowBase_isOpen(const sfWindowBase* windowBase);
bool sfWindowBase_pollEvent(sfWindowBase* windowBase, sfEvent* event);
bool sfWindowBase_waitEvent(sfWindowBase* windowBase, sfTime timeout, sfEvent* event);
sfVector2i sfWindowBase_getPosition(const sfWindowBase* windowBase);
void sfWindowBase_setPosition(sfWindowBase* windowBase, sfVector2i position);
sfVector2u sfWindowBase_getSize(const sfWindowBase* windowBase);
void sfWindowBase_setSize(sfWindowBase* windowBase, sfVector2u size);
void sfWindowBase_setMinimumSize(sfWindowBase* windowBase, const sfVector2u* minimumSize);
void sfWindowBase_setMaximumSize(sfWindowBase* windowBase, const sfVector2u* maximumSize);
void sfWindowBase_setTitle(sfWindowBase* windowBase, const char* title);
void sfWindowBase_setUnicodeTitle(sfWindowBase* windowBase, const sfChar32* title);
void sfWindowBase_setIcon(sfWindowBase* windowBase, sfVector2u size, const uint8_t* pixels);
void sfWindowBase_setVisible(sfWindowBase* windowBase, bool visible);
void sfWindowBase_setMouseCursorVisible(sfWindowBase* windowBase, bool visible);
void sfWindowBase_setMouseCursorGrabbed(sfWindowBase* windowBase, bool grabbed);
void sfWindowBase_setMouseCursor(sfWindowBase* windowBase, const sfCursor* cursor);
void sfWindowBase_setKeyRepeatEnabled(sfWindowBase* windowBase, bool enabled);
void sfWindowBase_setJoystickThreshold(sfWindowBase* windowBase, float threshold);
void sfWindowBase_requestFocus(sfWindowBase* windowBase);
bool sfWindowBase_hasFocus(const sfWindowBase* windowBase);
sfWindowHandle sfWindowBase_getNativeHandle(const sfWindowBase* windowBase);
bool sfWindowBase_createVulkanSurface(sfWindowBase* windowBase, const VkInstance* instance, VkSurfaceKHR* surface, const VkAllocationCallbacks* allocator);
```

### Module Graphics (`csfml-graphics`)

Toutes les déclarations ci-dessous sont copiées des headers du tag git `3.0.0` (extraction automatique, voir « Méthode »). [CONFIRMÉ]

#### Graphics — types (48)

| Nom | Nature | Header (tag 3.0.0) |
|---|---|---|
| `sfBlendFactor` | enum | `include/CSFML/Graphics/BlendMode.h` |
| `sfBlendEquation` | enum | `include/CSFML/Graphics/BlendMode.h` |
| `sfBlendMode` | struct (champs publics) | `include/CSFML/Graphics/BlendMode.h` |
| `sfColor` | struct (champs publics) | `include/CSFML/Graphics/Color.h` |
| `sfCoordinateType` | enum | `include/CSFML/Graphics/CoordinateType.h` |
| `sfFontInfo` | struct (champs publics) | `include/CSFML/Graphics/FontInfo.h` |
| `sfGlslVec2` | alias (typedef) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslIvec2` | alias (typedef) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslBvec2` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslVec3` | alias (typedef) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslIvec3` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslBvec3` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslVec4` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslIvec4` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslBvec4` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslMat3` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlslMat4` | struct (champs publics) | `include/CSFML/Graphics/Glsl.h` |
| `sfGlyph` | struct (champs publics) | `include/CSFML/Graphics/Glyph.h` |
| `sfPrimitiveType` | enum | `include/CSFML/Graphics/PrimitiveType.h` |
| `sfFloatRect` | struct (champs publics) | `include/CSFML/Graphics/Rect.h` |
| `sfIntRect` | struct (champs publics) | `include/CSFML/Graphics/Rect.h` |
| `sfRenderStates` | struct (champs publics) | `include/CSFML/Graphics/RenderStates.h` |
| `sfShapeGetPointCountCallback` | pointeur de fonction (callback) | `include/CSFML/Graphics/Shape.h` |
| `sfShapeGetPointCallback` | pointeur de fonction (callback) | `include/CSFML/Graphics/Shape.h` |
| `sfStencilComparison` | enum | `include/CSFML/Graphics/StencilMode.h` |
| `sfStencilUpdateOperation` | enum | `include/CSFML/Graphics/StencilMode.h` |
| `sfStencilValue` | struct (champs publics) | `include/CSFML/Graphics/StencilMode.h` |
| `sfStencilMode` | struct (champs publics) | `include/CSFML/Graphics/StencilMode.h` |
| `sfTextStyle` | enum | `include/CSFML/Graphics/Text.h` |
| `sfTransform` | struct (champs publics) | `include/CSFML/Graphics/Transform.h` |
| `sfCircleShape` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfConvexShape` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfFont` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfImage` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfShader` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfRectangleShape` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfRenderTexture` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfRenderWindow` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfShape` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfSprite` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfText` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfTexture` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfTransformable` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfVertexArray` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfVertexBuffer` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfView` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Graphics/Types.h` |
| `sfVertex` | struct (champs publics) | `include/CSFML/Graphics/Vertex.h` |
| `sfVertexBufferUsage` | enum | `include/CSFML/Graphics/VertexBuffer.h` |

Définition des types non opaques (format compact, une ligne par struct) :

```c
typedef enum {   /* CSFML/Graphics/BlendMode.h, 10 valeurs */
    sfBlendFactorZero, sfBlendFactorOne, sfBlendFactorSrcColor, sfBlendFactorOneMinusSrcColor,
    sfBlendFactorDstColor, sfBlendFactorOneMinusDstColor, sfBlendFactorSrcAlpha,
    sfBlendFactorOneMinusSrcAlpha, sfBlendFactorDstAlpha, sfBlendFactorOneMinusDstAlpha
} sfBlendFactor;
typedef enum {   /* CSFML/Graphics/BlendMode.h, 5 valeurs */
    sfBlendEquationAdd, sfBlendEquationSubtract, sfBlendEquationReverseSubtract, sfBlendEquationMin,
    sfBlendEquationMax
} sfBlendEquation;
typedef struct { sfBlendFactor colorSrcFactor; sfBlendFactor colorDstFactor; sfBlendEquation colorEquation; sfBlendFactor alphaSrcFactor; sfBlendFactor alphaDstFactor; sfBlendEquation alphaEquation; } sfBlendMode;   /* CSFML/Graphics/BlendMode.h */
typedef struct { uint8_t r; uint8_t g; uint8_t b; uint8_t a; } sfColor;   /* CSFML/Graphics/Color.h */
typedef enum {   /* CSFML/Graphics/CoordinateType.h, 2 valeurs */
    sfCoordinateTypeNormalized, sfCoordinateTypePixels
} sfCoordinateType;
typedef struct { const char* family; } sfFontInfo;   /* CSFML/Graphics/FontInfo.h */
typedef sfVector2f sfGlslVec2;   /* CSFML/Graphics/Glsl.h */
typedef sfVector2i sfGlslIvec2;   /* CSFML/Graphics/Glsl.h */
typedef struct { bool x; bool y; } sfGlslBvec2;   /* CSFML/Graphics/Glsl.h */
typedef sfVector3f sfGlslVec3;   /* CSFML/Graphics/Glsl.h */
typedef struct { int x; int y; int z; } sfGlslIvec3;   /* CSFML/Graphics/Glsl.h */
typedef struct { bool x; bool y; bool z; } sfGlslBvec3;   /* CSFML/Graphics/Glsl.h */
typedef struct { float x; float y; float z; float w; } sfGlslVec4;   /* CSFML/Graphics/Glsl.h */
typedef struct { int x; int y; int z; int w; } sfGlslIvec4;   /* CSFML/Graphics/Glsl.h */
typedef struct { bool x; bool y; bool z; bool w; } sfGlslBvec4;   /* CSFML/Graphics/Glsl.h */
typedef struct { float array[3* 3]; } sfGlslMat3;   /* CSFML/Graphics/Glsl.h */
typedef struct { float array[4* 4]; } sfGlslMat4;   /* CSFML/Graphics/Glsl.h */
typedef struct { float advance; sfFloatRect bounds; sfIntRect textureRect; } sfGlyph;   /* CSFML/Graphics/Glyph.h */
typedef enum {   /* CSFML/Graphics/PrimitiveType.h, 6 valeurs */
    sfPoints, sfLines, sfLineStrip, sfTriangles, sfTriangleStrip, sfTriangleFan
} sfPrimitiveType;
typedef struct { sfVector2f position; sfVector2f size; } sfFloatRect;   /* CSFML/Graphics/Rect.h */
typedef struct { sfVector2i position; sfVector2i size; } sfIntRect;   /* CSFML/Graphics/Rect.h */
typedef struct { sfBlendMode blendMode; sfStencilMode stencilMode; sfTransform transform; sfCoordinateType coordinateType; const sfTexture* texture; const sfShader* shader; } sfRenderStates;   /* CSFML/Graphics/RenderStates.h */
typedef size_t (*sfShapeGetPointCountCallback)(void*);   /* CSFML/Graphics/Shape.h */
typedef sfVector2f (*sfShapeGetPointCallback)(size_t, void*);   /* CSFML/Graphics/Shape.h */
typedef enum {   /* CSFML/Graphics/StencilMode.h, 8 valeurs */
    sfStencilComparisonNever, sfStencilComparisonLess, sfStencilComparisonLessEqual,
    sfStencilComparisonGreater, sfStencilComparisonGreaterEqual, sfStencilComparisonEqual,
    sfStencilComparisonNotEqual, sfStencilComparisonAlways
} sfStencilComparison;
typedef enum {   /* CSFML/Graphics/StencilMode.h, 6 valeurs */
    sfStencilUpdateOperationKeep, sfStencilUpdateOperationZero, sfStencilUpdateOperationReplace,
    sfStencilUpdateOperationIncrement, sfStencilUpdateOperationDecrement,
    sfStencilUpdateOperationInvert
} sfStencilUpdateOperation;
typedef struct { unsigned int value; } sfStencilValue;   /* CSFML/Graphics/StencilMode.h */
typedef struct { sfStencilComparison stencilComparison; sfStencilUpdateOperation stencilUpdateOperation; sfStencilValue stencilReference; sfStencilValue stencilMask; bool stencilOnly; } sfStencilMode;   /* CSFML/Graphics/StencilMode.h */
typedef enum {   /* CSFML/Graphics/Text.h, 5 valeurs */
    sfTextRegular = 0, sfTextBold = 1 << 0, sfTextItalic = 1 << 1, sfTextUnderlined = 1 << 2,
    sfTextStrikeThrough = 1 << 3
} sfTextStyle;
typedef struct { float matrix[9]; } sfTransform;   /* CSFML/Graphics/Transform.h */
typedef struct { sfVector2f position; sfColor color; sfVector2f texCoords; } sfVertex;   /* CSFML/Graphics/Vertex.h */
typedef enum {   /* CSFML/Graphics/VertexBuffer.h, 3 valeurs */
    sfVertexBufferStream, sfVertexBufferDynamic, sfVertexBufferStatic
} sfVertexBufferUsage;
```

#### Graphics — constantes

```c
CSFML_GRAPHICS_API const sfBlendMode sfBlendAlpha;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfBlendMode sfBlendAdd;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfBlendMode sfBlendMultiply;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfBlendMode sfBlendMin;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfBlendMode sfBlendMax;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfBlendMode sfBlendNone;   /* CSFML/Graphics/BlendMode.h */
CSFML_GRAPHICS_API const sfColor sfBlack;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfWhite;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfRed;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfGreen;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfBlue;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfYellow;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfMagenta;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfCyan;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfColor sfTransparent;   /* CSFML/Graphics/Color.h */
CSFML_GRAPHICS_API const sfRenderStates sfRenderStates_default;   /* CSFML/Graphics/RenderStates.h */
CSFML_GRAPHICS_API const sfStencilMode sfStencilMode_default;   /* CSFML/Graphics/StencilMode.h */
CSFML_GRAPHICS_API const sfTransform sfTransform_Identity;   /* CSFML/Graphics/Transform.h */
```

#### Graphics — fonctions (467)

##### `include/CSFML/Graphics/CircleShape.h` — 34 fonction(s)

```c
sfCircleShape* sfCircleShape_create(void);
sfCircleShape* sfCircleShape_copy(const sfCircleShape* shape);
void sfCircleShape_destroy(const sfCircleShape* shape);
void sfCircleShape_setPosition(sfCircleShape* shape, sfVector2f position);
void sfCircleShape_setRotation(sfCircleShape* shape, float angle);
void sfCircleShape_setScale(sfCircleShape* shape, sfVector2f scale);
void sfCircleShape_setOrigin(sfCircleShape* shape, sfVector2f origin);
sfVector2f sfCircleShape_getPosition(const sfCircleShape* shape);
float sfCircleShape_getRotation(const sfCircleShape* shape);
sfVector2f sfCircleShape_getScale(const sfCircleShape* shape);
sfVector2f sfCircleShape_getOrigin(const sfCircleShape* shape);
void sfCircleShape_move(sfCircleShape* shape, sfVector2f offset);
void sfCircleShape_rotate(sfCircleShape* shape, float angle);
void sfCircleShape_scale(sfCircleShape* shape, sfVector2f factors);
sfTransform sfCircleShape_getTransform(const sfCircleShape* shape);
sfTransform sfCircleShape_getInverseTransform(const sfCircleShape* shape);
void sfCircleShape_setTexture(sfCircleShape* shape, const sfTexture* texture, bool resetRect);
void sfCircleShape_setTextureRect(sfCircleShape* shape, sfIntRect rect);
void sfCircleShape_setFillColor(sfCircleShape* shape, sfColor color);
void sfCircleShape_setOutlineColor(sfCircleShape* shape, sfColor color);
void sfCircleShape_setOutlineThickness(sfCircleShape* shape, float thickness);
const sfTexture* sfCircleShape_getTexture(const sfCircleShape* shape);
sfIntRect sfCircleShape_getTextureRect(const sfCircleShape* shape);
sfColor sfCircleShape_getFillColor(const sfCircleShape* shape);
sfColor sfCircleShape_getOutlineColor(const sfCircleShape* shape);
float sfCircleShape_getOutlineThickness(const sfCircleShape* shape);
size_t sfCircleShape_getPointCount(const sfCircleShape* shape);
sfVector2f sfCircleShape_getPoint(const sfCircleShape* shape, size_t index);
sfVector2f sfCircleShape_getGeometricCenter(const sfCircleShape* shape);
void sfCircleShape_setRadius(sfCircleShape* shape, float radius);
float sfCircleShape_getRadius(const sfCircleShape* shape);
void sfCircleShape_setPointCount(sfCircleShape* shape, size_t count);
sfFloatRect sfCircleShape_getLocalBounds(const sfCircleShape* shape);
sfFloatRect sfCircleShape_getGlobalBounds(const sfCircleShape* shape);
```

##### `include/CSFML/Graphics/Color.h` — 7 fonction(s)

```c
sfColor sfColor_fromRGB(uint8_t red, uint8_t green, uint8_t blue);
sfColor sfColor_fromRGBA(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
sfColor sfColor_fromInteger(uint32_t color);
uint32_t sfColor_toInteger(sfColor color);
sfColor sfColor_add(sfColor color1, sfColor color2);
sfColor sfColor_subtract(sfColor color1, sfColor color2);
sfColor sfColor_modulate(sfColor color1, sfColor color2);
```

##### `include/CSFML/Graphics/ConvexShape.h` — 33 fonction(s)

```c
sfConvexShape* sfConvexShape_create(void);
sfConvexShape* sfConvexShape_copy(const sfConvexShape* shape);
void sfConvexShape_destroy(const sfConvexShape* shape);
void sfConvexShape_setPosition(sfConvexShape* shape, sfVector2f position);
void sfConvexShape_setRotation(sfConvexShape* shape, float angle);
void sfConvexShape_setScale(sfConvexShape* shape, sfVector2f scale);
void sfConvexShape_setOrigin(sfConvexShape* shape, sfVector2f origin);
sfVector2f sfConvexShape_getPosition(const sfConvexShape* shape);
float sfConvexShape_getRotation(const sfConvexShape* shape);
sfVector2f sfConvexShape_getScale(const sfConvexShape* shape);
sfVector2f sfConvexShape_getOrigin(const sfConvexShape* shape);
void sfConvexShape_move(sfConvexShape* shape, sfVector2f offset);
void sfConvexShape_rotate(sfConvexShape* shape, float angle);
void sfConvexShape_scale(sfConvexShape* shape, sfVector2f factors);
sfTransform sfConvexShape_getTransform(const sfConvexShape* shape);
sfTransform sfConvexShape_getInverseTransform(const sfConvexShape* shape);
void sfConvexShape_setTexture(sfConvexShape* shape, const sfTexture* texture, bool resetRect);
void sfConvexShape_setTextureRect(sfConvexShape* shape, sfIntRect rect);
void sfConvexShape_setFillColor(sfConvexShape* shape, sfColor color);
void sfConvexShape_setOutlineColor(sfConvexShape* shape, sfColor color);
void sfConvexShape_setOutlineThickness(sfConvexShape* shape, float thickness);
const sfTexture* sfConvexShape_getTexture(const sfConvexShape* shape);
sfIntRect sfConvexShape_getTextureRect(const sfConvexShape* shape);
sfColor sfConvexShape_getFillColor(const sfConvexShape* shape);
sfColor sfConvexShape_getOutlineColor(const sfConvexShape* shape);
float sfConvexShape_getOutlineThickness(const sfConvexShape* shape);
size_t sfConvexShape_getPointCount(const sfConvexShape* shape);
sfVector2f sfConvexShape_getPoint(const sfConvexShape* shape, size_t index);
sfVector2f sfConvexShape_getGeometricCenter(const sfConvexShape* shape);
void sfConvexShape_setPointCount(sfConvexShape* shape, size_t count);
void sfConvexShape_setPoint(sfConvexShape* shape, size_t index, sfVector2f point);
sfFloatRect sfConvexShape_getLocalBounds(const sfConvexShape* shape);
sfFloatRect sfConvexShape_getGlobalBounds(const sfConvexShape* shape);
```

##### `include/CSFML/Graphics/Font.h` — 16 fonction(s)

```c
sfFont* sfFont_createFromFile(const char* filename);
sfFont* sfFont_createFromMemory(const void* data, size_t sizeInBytes);
sfFont* sfFont_createFromStream(sfInputStream* stream);
sfFont* sfFont_copy(const sfFont* font);
void sfFont_destroy(const sfFont* font);
sfGlyph sfFont_getGlyph(const sfFont* font, uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness);
bool sfFont_hasGlyph(const sfFont* font, uint32_t codePoint);
float sfFont_getKerning(const sfFont* font, uint32_t first, uint32_t second, unsigned int characterSize);
float sfFont_getBoldKerning(const sfFont* font, uint32_t first, uint32_t second, unsigned int characterSize);
float sfFont_getLineSpacing(const sfFont* font, unsigned int characterSize);
float sfFont_getUnderlinePosition(const sfFont* font, unsigned int characterSize);
float sfFont_getUnderlineThickness(const sfFont* font, unsigned int characterSize);
const sfTexture* sfFont_getTexture(sfFont* font, unsigned int characterSize);
void sfFont_setSmooth(sfFont* font, bool smooth);
bool sfFont_isSmooth(const sfFont* font);
sfFontInfo sfFont_getInfo(const sfFont* font);
```

##### `include/CSFML/Graphics/Image.h` — 18 fonction(s)

```c
sfImage* sfImage_create(sfVector2u size);
sfImage* sfImage_createFromColor(sfVector2u size, sfColor color);
sfImage* sfImage_createFromPixels(sfVector2u size, const uint8_t* pixels);
sfImage* sfImage_createFromFile(const char* filename);
sfImage* sfImage_createFromMemory(const void* data, size_t size);
sfImage* sfImage_createFromStream(sfInputStream* stream);
sfImage* sfImage_copy(const sfImage* image);
void sfImage_destroy(const sfImage* image);
bool sfImage_saveToFile(const sfImage* image, const char* filename);
bool sfImage_saveToMemory(const sfImage* image, sfBuffer* output, const char* format);
sfVector2u sfImage_getSize(const sfImage* image);
void sfImage_createMaskFromColor(sfImage* image, sfColor color, uint8_t alpha);
bool sfImage_copyImage(sfImage* image, const sfImage* source, sfVector2u dest, sfIntRect sourceRect, bool applyAlpha);
void sfImage_setPixel(sfImage* image, sfVector2u coords, sfColor color);
sfColor sfImage_getPixel(const sfImage* image, sfVector2u coords);
const uint8_t* sfImage_getPixelsPtr(const sfImage* image);
void sfImage_flipHorizontally(sfImage* image);
void sfImage_flipVertically(sfImage* image);
```

##### `include/CSFML/Graphics/Rect.h` — 4 fonction(s)

```c
/* --- sfFloatRect --- */
bool sfFloatRect_contains(const sfFloatRect* rect, sfVector2f point);

/* --- sfIntRect --- */
bool sfIntRect_contains(const sfIntRect* rect, sfVector2i point);

/* --- sfFloatRect --- */
bool sfFloatRect_intersects(const sfFloatRect* rect1, const sfFloatRect* rect2, sfFloatRect* intersection);

/* --- sfIntRect --- */
bool sfIntRect_intersects(const sfIntRect* rect1, const sfIntRect* rect2, sfIntRect* intersection);
```

##### `include/CSFML/Graphics/RectangleShape.h` — 33 fonction(s)

```c
sfRectangleShape* sfRectangleShape_create(void);
sfRectangleShape* sfRectangleShape_copy(const sfRectangleShape* shape);
void sfRectangleShape_destroy(const sfRectangleShape* shape);
void sfRectangleShape_setPosition(sfRectangleShape* shape, sfVector2f position);
void sfRectangleShape_setRotation(sfRectangleShape* shape, float angle);
void sfRectangleShape_setScale(sfRectangleShape* shape, sfVector2f scale);
void sfRectangleShape_setOrigin(sfRectangleShape* shape, sfVector2f origin);
sfVector2f sfRectangleShape_getPosition(const sfRectangleShape* shape);
float sfRectangleShape_getRotation(const sfRectangleShape* shape);
sfVector2f sfRectangleShape_getScale(const sfRectangleShape* shape);
sfVector2f sfRectangleShape_getOrigin(const sfRectangleShape* shape);
void sfRectangleShape_move(sfRectangleShape* shape, sfVector2f offset);
void sfRectangleShape_rotate(sfRectangleShape* shape, float angle);
void sfRectangleShape_scale(sfRectangleShape* shape, sfVector2f factors);
sfTransform sfRectangleShape_getTransform(const sfRectangleShape* shape);
sfTransform sfRectangleShape_getInverseTransform(const sfRectangleShape* shape);
void sfRectangleShape_setTexture(sfRectangleShape* shape, const sfTexture* texture, bool resetRect);
void sfRectangleShape_setTextureRect(sfRectangleShape* shape, sfIntRect rect);
void sfRectangleShape_setFillColor(sfRectangleShape* shape, sfColor color);
void sfRectangleShape_setOutlineColor(sfRectangleShape* shape, sfColor color);
void sfRectangleShape_setOutlineThickness(sfRectangleShape* shape, float thickness);
const sfTexture* sfRectangleShape_getTexture(const sfRectangleShape* shape);
sfIntRect sfRectangleShape_getTextureRect(const sfRectangleShape* shape);
sfColor sfRectangleShape_getFillColor(const sfRectangleShape* shape);
sfColor sfRectangleShape_getOutlineColor(const sfRectangleShape* shape);
float sfRectangleShape_getOutlineThickness(const sfRectangleShape* shape);
size_t sfRectangleShape_getPointCount(const sfRectangleShape* shape);
sfVector2f sfRectangleShape_getPoint(const sfRectangleShape* shape, size_t index);
sfVector2f sfRectangleShape_getGeometricCenter(const sfRectangleShape* shape);
void sfRectangleShape_setSize(sfRectangleShape* shape, sfVector2f size);
sfVector2f sfRectangleShape_getSize(const sfRectangleShape* shape);
sfFloatRect sfRectangleShape_getLocalBounds(const sfRectangleShape* shape);
sfFloatRect sfRectangleShape_getGlobalBounds(const sfRectangleShape* shape);
```

##### `include/CSFML/Graphics/RenderTexture.h` — 36 fonction(s)

```c
sfRenderTexture* sfRenderTexture_create(sfVector2u size, const sfContextSettings* settings);
void sfRenderTexture_destroy(const sfRenderTexture* renderTexture);
sfVector2u sfRenderTexture_getSize(const sfRenderTexture* renderTexture);
bool sfRenderTexture_isSrgb(const sfRenderTexture* renderTexture);
bool sfRenderTexture_setActive(sfRenderTexture* renderTexture, bool active);
void sfRenderTexture_display(sfRenderTexture* renderTexture);
void sfRenderTexture_clear(sfRenderTexture* renderTexture, sfColor color);
void sfRenderTexture_clearStencil(sfRenderTexture* renderTexture, sfStencilValue stencilValue);
void sfRenderTexture_clearColorAndStencil(sfRenderTexture* renderTexture, sfColor color, sfStencilValue stencilValue);
void sfRenderTexture_setView(sfRenderTexture* renderTexture, const sfView* view);
const sfView* sfRenderTexture_getView(const sfRenderTexture* renderTexture);
const sfView* sfRenderTexture_getDefaultView(const sfRenderTexture* renderTexture);
sfIntRect sfRenderTexture_getViewport(const sfRenderTexture* renderTexture, const sfView* view);
sfIntRect sfRenderTexture_getScissor(const sfRenderTexture* renderTexture, const sfView* view);
sfVector2f sfRenderTexture_mapPixelToCoords(const sfRenderTexture* renderTexture, sfVector2i point, const sfView* view);
sfVector2i sfRenderTexture_mapCoordsToPixel(const sfRenderTexture* renderTexture, sfVector2f point, const sfView* view);
void sfRenderTexture_drawSprite(sfRenderTexture* renderTexture, const sfSprite* object, const sfRenderStates* states);
void sfRenderTexture_drawText(sfRenderTexture* renderTexture, const sfText* object, const sfRenderStates* states);
void sfRenderTexture_drawShape(sfRenderTexture* renderTexture, const sfShape* object, const sfRenderStates* states);
void sfRenderTexture_drawCircleShape(sfRenderTexture* renderTexture, const sfCircleShape* object, const sfRenderStates* states);
void sfRenderTexture_drawConvexShape(sfRenderTexture* renderTexture, const sfConvexShape* object, const sfRenderStates* states);
void sfRenderTexture_drawRectangleShape(sfRenderTexture* renderTexture, const sfRectangleShape* object, const sfRenderStates* states);
void sfRenderTexture_drawVertexArray(sfRenderTexture* renderTexture, const sfVertexArray* object, const sfRenderStates* states);
void sfRenderTexture_drawVertexBuffer(sfRenderTexture* renderTexture, const sfVertexBuffer* object, const sfRenderStates* states);
void sfRenderTexture_drawVertexBufferRange(sfRenderTexture* renderTexture, const sfVertexBuffer* object, size_t firstVertex, size_t vertexCount, const sfRenderStates* states);
void sfRenderTexture_drawPrimitives(sfRenderTexture* renderTexture, const sfVertex* vertices, size_t vertexCount, sfPrimitiveType type, const sfRenderStates* states);
void sfRenderTexture_pushGLStates(sfRenderTexture* renderTexture);
void sfRenderTexture_popGLStates(sfRenderTexture* renderTexture);
void sfRenderTexture_resetGLStates(sfRenderTexture* renderTexture);
const sfTexture* sfRenderTexture_getTexture(const sfRenderTexture* renderTexture);
unsigned int sfRenderTexture_getMaximumAntiAliasingLevel(void);
void sfRenderTexture_setSmooth(sfRenderTexture* renderTexture, bool smooth);
bool sfRenderTexture_isSmooth(const sfRenderTexture* renderTexture);
void sfRenderTexture_setRepeated(sfRenderTexture* renderTexture, bool repeated);
bool sfRenderTexture_isRepeated(const sfRenderTexture* renderTexture);
bool sfRenderTexture_generateMipmap(sfRenderTexture* renderTexture);
```

##### `include/CSFML/Graphics/RenderWindow.h` — 59 fonction(s)

```c
/* --- sfRenderWindow --- */
sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);
sfRenderWindow* sfRenderWindow_createUnicode(sfVideoMode mode, const sfChar32* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);
sfRenderWindow* sfRenderWindow_createFromHandle(sfWindowHandle handle, const sfContextSettings* settings);
void sfRenderWindow_destroy(const sfRenderWindow* renderWindow);
void sfRenderWindow_close(sfRenderWindow* renderWindow);
bool sfRenderWindow_isOpen(const sfRenderWindow* renderWindow);
sfContextSettings sfRenderWindow_getSettings(const sfRenderWindow* renderWindow);
bool sfRenderWindow_pollEvent(sfRenderWindow* renderWindow, sfEvent* event);
bool sfRenderWindow_waitEvent(sfRenderWindow* renderWindow, sfTime timeout, sfEvent* event);
sfVector2i sfRenderWindow_getPosition(const sfRenderWindow* renderWindow);
void sfRenderWindow_setPosition(sfRenderWindow* renderWindow, sfVector2i position);
sfVector2u sfRenderWindow_getSize(const sfRenderWindow* renderWindow);
bool sfRenderWindow_isSrgb(const sfRenderWindow* renderWindow);
void sfRenderWindow_setSize(sfRenderWindow* renderWindow, sfVector2u size);
void sfRenderWindow_setMinimumSize(sfRenderWindow* renderWindow, const sfVector2u* minimumSize);
void sfRenderWindow_setMaximumSize(sfRenderWindow* renderWindow, const sfVector2u* maximumSize);
void sfRenderWindow_setTitle(sfRenderWindow* renderWindow, const char* title);
void sfRenderWindow_setUnicodeTitle(sfRenderWindow* renderWindow, const sfChar32* title);
void sfRenderWindow_setIcon(sfRenderWindow* renderWindow, sfVector2u size, const uint8_t* pixels);
void sfRenderWindow_setVisible(sfRenderWindow* renderWindow, bool visible);
void sfRenderWindow_setVerticalSyncEnabled(sfRenderWindow* renderWindow, bool enabled);
void sfRenderWindow_setMouseCursorVisible(sfRenderWindow* renderWindow, bool show);
void sfRenderWindow_setMouseCursorGrabbed(sfRenderWindow* renderWindow, bool grabbed);
void sfRenderWindow_setMouseCursor(sfRenderWindow* renderWindow, const sfCursor* cursor);
void sfRenderWindow_setKeyRepeatEnabled(sfRenderWindow* renderWindow, bool enabled);
void sfRenderWindow_setFramerateLimit(sfRenderWindow* renderWindow, unsigned int limit);
void sfRenderWindow_setJoystickThreshold(sfRenderWindow* renderWindow, float threshold);
bool sfRenderWindow_setActive(sfRenderWindow* renderWindow, bool active);
void sfRenderWindow_requestFocus(sfRenderWindow* renderWindow);
bool sfRenderWindow_hasFocus(const sfRenderWindow* renderWindow);
void sfRenderWindow_display(sfRenderWindow* renderWindow);
sfWindowHandle sfRenderWindow_getNativeHandle(const sfRenderWindow* renderWindow);
void sfRenderWindow_clear(sfRenderWindow* renderWindow, sfColor color);
void sfRenderWindow_clearStencil(sfRenderWindow* renderWindow, sfStencilValue stencilValue);
void sfRenderWindow_clearColorAndStencil(sfRenderWindow* renderWindow, sfColor color, sfStencilValue stencilValue);
void sfRenderWindow_setView(sfRenderWindow* renderWindow, const sfView* view);
const sfView* sfRenderWindow_getView(const sfRenderWindow* renderWindow);
const sfView* sfRenderWindow_getDefaultView(const sfRenderWindow* renderWindow);
sfIntRect sfRenderWindow_getViewport(const sfRenderWindow* renderWindow, const sfView* view);
sfIntRect sfRenderWindow_getScissor(const sfRenderWindow* renderWindow, const sfView* view);
sfVector2f sfRenderWindow_mapPixelToCoords(const sfRenderWindow* renderWindow, sfVector2i point, const sfView* view);
sfVector2i sfRenderWindow_mapCoordsToPixel(const sfRenderWindow* renderWindow, sfVector2f point, const sfView* view);
void sfRenderWindow_drawSprite(sfRenderWindow* renderWindow, const sfSprite* object, const sfRenderStates* states);
void sfRenderWindow_drawText(sfRenderWindow* renderWindow, const sfText* object, const sfRenderStates* states);
void sfRenderWindow_drawShape(sfRenderWindow* renderWindow, const sfShape* object, const sfRenderStates* states);
void sfRenderWindow_drawCircleShape(sfRenderWindow* renderWindow, const sfCircleShape* object, const sfRenderStates* states);
void sfRenderWindow_drawConvexShape(sfRenderWindow* renderWindow, const sfConvexShape* object, const sfRenderStates* states);
void sfRenderWindow_drawRectangleShape(sfRenderWindow* renderWindow, const sfRectangleShape* object, const sfRenderStates* states);
void sfRenderWindow_drawVertexArray(sfRenderWindow* renderWindow, const sfVertexArray* object, const sfRenderStates* states);
void sfRenderWindow_drawVertexBuffer(sfRenderWindow* renderWindow, const sfVertexBuffer* object, const sfRenderStates* states);
void sfRenderWindow_drawVertexBufferRange(sfRenderWindow* renderWindow, const sfVertexBuffer* object, size_t firstVertex, size_t vertexCount, const sfRenderStates* states);
void sfRenderWindow_drawPrimitives(sfRenderWindow* renderWindow, const sfVertex* vertices, size_t vertexCount, sfPrimitiveType type, const sfRenderStates* states);
void sfRenderWindow_pushGLStates(sfRenderWindow* renderWindow);
void sfRenderWindow_popGLStates(sfRenderWindow* renderWindow);
void sfRenderWindow_resetGLStates(sfRenderWindow* renderWindow);

/* --- sfMouse --- */
sfVector2i sfMouse_getPositionRenderWindow(const sfRenderWindow* relativeTo);
void sfMouse_setPositionRenderWindow(sfVector2i position, const sfRenderWindow* relativeTo);

/* --- sfTouch --- */
sfVector2i sfTouch_getPositionRenderWindow(unsigned int finger, const sfRenderWindow* relativeTo);

/* --- sfRenderWindow --- */
bool sfRenderWindow_createVulkanSurface(sfRenderWindow* renderWindow, const VkInstance* instance, VkSurfaceKHR* surface, const VkAllocationCallbacks* allocator);
```

##### `include/CSFML/Graphics/Shader.h` — 32 fonction(s)

```c
sfShader* sfShader_createFromFile(const char* vertexShaderFilename, const char* geometryShaderFilename, const char* fragmentShaderFilename);
sfShader* sfShader_createFromMemory(const char* vertexShader, const char* geometryShader, const char* fragmentShader);
sfShader* sfShader_createFromStream(sfInputStream* vertexShaderStream, sfInputStream* geometryShaderStream, sfInputStream* fragmentShaderStream);
void sfShader_destroy(const sfShader* shader);
void sfShader_setFloatUniform(sfShader* shader, const char* name, float x);
void sfShader_setVec2Uniform(sfShader* shader, const char* name, sfGlslVec2 vector);
void sfShader_setVec3Uniform(sfShader* shader, const char* name, sfGlslVec3 vector);
void sfShader_setVec4Uniform(sfShader* shader, const char* name, sfGlslVec4 vector);
void sfShader_setColorUniform(sfShader* shader, const char* name, sfColor color);
void sfShader_setIntUniform(sfShader* shader, const char* name, int x);
void sfShader_setIvec2Uniform(sfShader* shader, const char* name, sfGlslIvec2 vector);
void sfShader_setIvec3Uniform(sfShader* shader, const char* name, sfGlslIvec3 vector);
void sfShader_setIvec4Uniform(sfShader* shader, const char* name, sfGlslIvec4 vector);
void sfShader_setIntColorUniform(sfShader* shader, const char* name, sfColor color);
void sfShader_setBoolUniform(sfShader* shader, const char* name, bool x);
void sfShader_setBvec2Uniform(sfShader* shader, const char* name, sfGlslBvec2 vector);
void sfShader_setBvec3Uniform(sfShader* shader, const char* name, sfGlslBvec3 vector);
void sfShader_setBvec4Uniform(sfShader* shader, const char* name, sfGlslBvec4 vector);
void sfShader_setMat3Uniform(sfShader* shader, const char* name, const sfGlslMat3* matrix);
void sfShader_setMat4Uniform(sfShader* shader, const char* name, const sfGlslMat4* matrix);
void sfShader_setTextureUniform(sfShader* shader, const char* name, const sfTexture* texture);
void sfShader_setCurrentTextureUniform(sfShader* shader, const char* name);
void sfShader_setFloatUniformArray(sfShader* shader, const char* name, const float* scalarArray, size_t length);
void sfShader_setVec2UniformArray(sfShader* shader, const char* name, const sfGlslVec2* vectorArray, size_t length);
void sfShader_setVec3UniformArray(sfShader* shader, const char* name, const sfGlslVec3* vectorArray, size_t length);
void sfShader_setVec4UniformArray(sfShader* shader, const char* name, const sfGlslVec4* vectorArray, size_t length);
void sfShader_setMat3UniformArray(sfShader* shader, const char* name, const sfGlslMat3* matrixArray, size_t length);
void sfShader_setMat4UniformArray(sfShader* shader, const char* name, const sfGlslMat4* matrixArray, size_t length);
unsigned int sfShader_getNativeHandle(const sfShader* shader);
void sfShader_bind(const sfShader* shader);
bool sfShader_isAvailable(void);
bool sfShader_isGeometryAvailable(void);
```

##### `include/CSFML/Graphics/Shape.h` — 31 fonction(s)

```c
sfShape* sfShape_create(sfShapeGetPointCountCallback getPointCount, sfShapeGetPointCallback getPoint, void* userData);
void sfShape_destroy(const sfShape* shape);
void sfShape_setPosition(sfShape* shape, sfVector2f position);
void sfShape_setRotation(sfShape* shape, float angle);
void sfShape_setScale(sfShape* shape, sfVector2f scale);
void sfShape_setOrigin(sfShape* shape, sfVector2f origin);
sfVector2f sfShape_getPosition(const sfShape* shape);
float sfShape_getRotation(const sfShape* shape);
sfVector2f sfShape_getScale(const sfShape* shape);
sfVector2f sfShape_getOrigin(const sfShape* shape);
void sfShape_move(sfShape* shape, sfVector2f offset);
void sfShape_rotate(sfShape* shape, float angle);
void sfShape_scale(sfShape* shape, sfVector2f factors);
sfTransform sfShape_getTransform(const sfShape* shape);
sfTransform sfShape_getInverseTransform(const sfShape* shape);
void sfShape_setTexture(sfShape* shape, const sfTexture* texture, bool resetRect);
void sfShape_setTextureRect(sfShape* shape, sfIntRect rect);
void sfShape_setFillColor(sfShape* shape, sfColor color);
void sfShape_setOutlineColor(sfShape* shape, sfColor color);
void sfShape_setOutlineThickness(sfShape* shape, float thickness);
const sfTexture* sfShape_getTexture(const sfShape* shape);
sfIntRect sfShape_getTextureRect(const sfShape* shape);
sfColor sfShape_getFillColor(const sfShape* shape);
sfColor sfShape_getOutlineColor(const sfShape* shape);
float sfShape_getOutlineThickness(const sfShape* shape);
size_t sfShape_getPointCount(const sfShape* shape);
sfVector2f sfShape_getPoint(const sfShape* shape, size_t index);
sfVector2f sfShape_getGeometricCenter(const sfShape* shape);
sfFloatRect sfShape_getLocalBounds(const sfShape* shape);
sfFloatRect sfShape_getGlobalBounds(const sfShape* shape);
void sfShape_update(sfShape* shape);
```

##### `include/CSFML/Graphics/Sprite.h` — 24 fonction(s)

```c
sfSprite* sfSprite_create(const sfTexture* texture);
sfSprite* sfSprite_copy(const sfSprite* sprite);
void sfSprite_destroy(const sfSprite* sprite);
void sfSprite_setPosition(sfSprite* sprite, sfVector2f position);
void sfSprite_setRotation(sfSprite* sprite, float angle);
void sfSprite_setScale(sfSprite* sprite, sfVector2f scale);
void sfSprite_setOrigin(sfSprite* sprite, sfVector2f origin);
sfVector2f sfSprite_getPosition(const sfSprite* sprite);
float sfSprite_getRotation(const sfSprite* sprite);
sfVector2f sfSprite_getScale(const sfSprite* sprite);
sfVector2f sfSprite_getOrigin(const sfSprite* sprite);
void sfSprite_move(sfSprite* sprite, sfVector2f offset);
void sfSprite_rotate(sfSprite* sprite, float angle);
void sfSprite_scale(sfSprite* sprite, sfVector2f factors);
sfTransform sfSprite_getTransform(const sfSprite* sprite);
sfTransform sfSprite_getInverseTransform(const sfSprite* sprite);
void sfSprite_setTexture(sfSprite* sprite, const sfTexture* texture, bool resetRect);
void sfSprite_setTextureRect(sfSprite* sprite, sfIntRect rectangle);
void sfSprite_setColor(sfSprite* sprite, sfColor color);
const sfTexture* sfSprite_getTexture(const sfSprite* sprite);
sfIntRect sfSprite_getTextureRect(const sfSprite* sprite);
sfColor sfSprite_getColor(const sfSprite* sprite);
sfFloatRect sfSprite_getLocalBounds(const sfSprite* sprite);
sfFloatRect sfSprite_getGlobalBounds(const sfSprite* sprite);
```

##### `include/CSFML/Graphics/Text.h` — 39 fonction(s)

```c
sfText* sfText_create(const sfFont* font);
sfText* sfText_copy(const sfText* text);
void sfText_destroy(const sfText* text);
void sfText_setPosition(sfText* text, sfVector2f position);
void sfText_setRotation(sfText* text, float angle);
void sfText_setScale(sfText* text, sfVector2f scale);
void sfText_setOrigin(sfText* text, sfVector2f origin);
sfVector2f sfText_getPosition(const sfText* text);
float sfText_getRotation(const sfText* text);
sfVector2f sfText_getScale(const sfText* text);
sfVector2f sfText_getOrigin(const sfText* text);
void sfText_move(sfText* text, sfVector2f offset);
void sfText_rotate(sfText* text, float angle);
void sfText_scale(sfText* text, sfVector2f factors);
sfTransform sfText_getTransform(const sfText* text);
sfTransform sfText_getInverseTransform(const sfText* text);
void sfText_setString(sfText* text, const char* string);
void sfText_setUnicodeString(sfText* text, const sfChar32* string);
void sfText_setFont(sfText* text, const sfFont* font);
void sfText_setCharacterSize(sfText* text, unsigned int size);
void sfText_setLineSpacing(sfText* text, float spacingFactor);
void sfText_setLetterSpacing(sfText* text, float spacingFactor);
void sfText_setStyle(sfText* text, uint32_t style);
void sfText_setFillColor(sfText* text, sfColor color);
void sfText_setOutlineColor(sfText* text, sfColor color);
void sfText_setOutlineThickness(sfText* text, float thickness);
const char* sfText_getString(const sfText* text);
const sfChar32* sfText_getUnicodeString(const sfText* text);
const sfFont* sfText_getFont(const sfText* text);
unsigned int sfText_getCharacterSize(const sfText* text);
float sfText_getLetterSpacing(const sfText* text);
float sfText_getLineSpacing(const sfText* text);
uint32_t sfText_getStyle(const sfText* text);
sfColor sfText_getFillColor(const sfText* text);
sfColor sfText_getOutlineColor(const sfText* text);
float sfText_getOutlineThickness(const sfText* text);
sfVector2f sfText_findCharacterPos(const sfText* text, size_t index);
sfFloatRect sfText_getLocalBounds(const sfText* text);
sfFloatRect sfText_getGlobalBounds(const sfText* text);
```

##### `include/CSFML/Graphics/Texture.h` — 31 fonction(s)

```c
sfTexture* sfTexture_create(sfVector2u size);
sfTexture* sfTexture_createSrgb(sfVector2u size);
sfTexture* sfTexture_createFromFile(const char* filename, const sfIntRect* area);
sfTexture* sfTexture_createSrgbFromFile(const char* filename, const sfIntRect* area);
sfTexture* sfTexture_createFromMemory(const void* data, size_t sizeInBytes, const sfIntRect* area);
sfTexture* sfTexture_createSrgbFromMemory(const void* data, size_t sizeInBytes, const sfIntRect* area);
sfTexture* sfTexture_createFromStream(sfInputStream* stream, const sfIntRect* area);
sfTexture* sfTexture_createSrgbFromStream(sfInputStream* stream, const sfIntRect* area);
sfTexture* sfTexture_createFromImage(const sfImage* image, const sfIntRect* area);
sfTexture* sfTexture_createSrgbFromImage(const sfImage* image, const sfIntRect* area);
sfTexture* sfTexture_copy(const sfTexture* texture);
void sfTexture_destroy(const sfTexture* texture);
bool sfTexture_resize(sfTexture* texture, sfVector2u size);
bool sfTexture_resizeSrgb(sfTexture* texture, sfVector2u size);
sfVector2u sfTexture_getSize(const sfTexture* texture);
sfImage* sfTexture_copyToImage(const sfTexture* texture);
void sfTexture_updateFromPixels(sfTexture* texture, const uint8_t* pixels, sfVector2u size, sfVector2u offset);
void sfTexture_updateFromTexture(sfTexture* destination, const sfTexture* source, sfVector2u offset);
void sfTexture_updateFromImage(sfTexture* texture, const sfImage* image, sfVector2u offset);
void sfTexture_updateFromWindow(sfTexture* texture, const sfWindow* window, sfVector2u offset);
void sfTexture_updateFromRenderWindow(sfTexture* texture, const sfRenderWindow* renderWindow, sfVector2u offset);
void sfTexture_setSmooth(sfTexture* texture, bool smooth);
bool sfTexture_isSmooth(const sfTexture* texture);
bool sfTexture_isSrgb(const sfTexture* texture);
void sfTexture_setRepeated(sfTexture* texture, bool repeated);
bool sfTexture_isRepeated(const sfTexture* texture);
bool sfTexture_generateMipmap(sfTexture* texture);
void sfTexture_swap(sfTexture* left, sfTexture* right);
unsigned int sfTexture_getNativeHandle(const sfTexture* texture);
void sfTexture_bind(const sfTexture* texture, sfCoordinateType type);
unsigned int sfTexture_getMaximumSize(void);
```

##### `include/CSFML/Graphics/Transform.h` — 12 fonction(s)

```c
sfTransform sfTransform_fromMatrix(float a00, float a01, float a02, float a10, float a11, float a12, float a20, float a21, float a22);
void sfTransform_getMatrix(const sfTransform* transform, float* matrix);
sfTransform sfTransform_getInverse(const sfTransform* transform);
sfVector2f sfTransform_transformPoint(const sfTransform* transform, sfVector2f point);
sfFloatRect sfTransform_transformRect(const sfTransform* transform, sfFloatRect rectangle);
void sfTransform_combine(sfTransform* transform, const sfTransform* other);
void sfTransform_translate(sfTransform* transform, sfVector2f offset);
void sfTransform_rotate(sfTransform* transform, float angle);
void sfTransform_rotateWithCenter(sfTransform* transform, float angle, sfVector2f center);
void sfTransform_scale(sfTransform* transform, sfVector2f scale);
void sfTransform_scaleWithCenter(sfTransform* transform, sfVector2f scale, sfVector2f center);
bool sfTransform_equal(sfTransform* left, sfTransform* right);
```

##### `include/CSFML/Graphics/Transformable.h` — 16 fonction(s)

```c
sfTransformable* sfTransformable_create(void);
sfTransformable* sfTransformable_copy(const sfTransformable* transformable);
void sfTransformable_destroy(const sfTransformable* transformable);
void sfTransformable_setPosition(sfTransformable* transformable, sfVector2f position);
void sfTransformable_setRotation(sfTransformable* transformable, float angle);
void sfTransformable_setScale(sfTransformable* transformable, sfVector2f scale);
void sfTransformable_setOrigin(sfTransformable* transformable, sfVector2f origin);
sfVector2f sfTransformable_getPosition(const sfTransformable* transformable);
float sfTransformable_getRotation(const sfTransformable* transformable);
sfVector2f sfTransformable_getScale(const sfTransformable* transformable);
sfVector2f sfTransformable_getOrigin(const sfTransformable* transformable);
void sfTransformable_move(sfTransformable* transformable, sfVector2f offset);
void sfTransformable_rotate(sfTransformable* transformable, float angle);
void sfTransformable_scale(sfTransformable* transformable, sfVector2f factors);
sfTransform sfTransformable_getTransform(const sfTransformable* transformable);
sfTransform sfTransformable_getInverseTransform(const sfTransformable* transformable);
```

##### `include/CSFML/Graphics/VertexArray.h` — 11 fonction(s)

```c
sfVertexArray* sfVertexArray_create(void);
sfVertexArray* sfVertexArray_copy(const sfVertexArray* vertexArray);
void sfVertexArray_destroy(const sfVertexArray* vertexArray);
size_t sfVertexArray_getVertexCount(const sfVertexArray* vertexArray);
sfVertex* sfVertexArray_getVertex(sfVertexArray* vertexArray, size_t index);
void sfVertexArray_clear(sfVertexArray* vertexArray);
void sfVertexArray_resize(sfVertexArray* vertexArray, size_t vertexCount);
void sfVertexArray_append(sfVertexArray* vertexArray, sfVertex vertex);
void sfVertexArray_setPrimitiveType(sfVertexArray* vertexArray, sfPrimitiveType type);
sfPrimitiveType sfVertexArray_getPrimitiveType(const sfVertexArray* vertexArray);
sfFloatRect sfVertexArray_getBounds(const sfVertexArray* vertexArray);
```

##### `include/CSFML/Graphics/VertexBuffer.h` — 14 fonction(s)

```c
sfVertexBuffer* sfVertexBuffer_create(size_t vertexCount, sfPrimitiveType type, sfVertexBufferUsage usage);
sfVertexBuffer* sfVertexBuffer_copy(const sfVertexBuffer* vertexBuffer);
void sfVertexBuffer_destroy(const sfVertexBuffer* vertexBuffer);
size_t sfVertexBuffer_getVertexCount(const sfVertexBuffer* vertexBuffer);
bool sfVertexBuffer_update(sfVertexBuffer* vertexBuffer, const sfVertex* vertices, unsigned int vertexCount, unsigned int offset);
bool sfVertexBuffer_updateFromVertexBuffer(sfVertexBuffer* vertexBuffer, const sfVertexBuffer* other);
void sfVertexBuffer_swap(sfVertexBuffer* left, sfVertexBuffer* right);
unsigned int sfVertexBuffer_getNativeHandle(const sfVertexBuffer* vertexBuffer);
void sfVertexBuffer_setPrimitiveType(sfVertexBuffer* vertexBuffer, sfPrimitiveType type);
sfPrimitiveType sfVertexBuffer_getPrimitiveType(const sfVertexBuffer* vertexBuffer);
void sfVertexBuffer_setUsage(sfVertexBuffer* vertexBuffer, sfVertexBufferUsage usage);
sfVertexBufferUsage sfVertexBuffer_getUsage(const sfVertexBuffer* vertexBuffer);
void sfVertexBuffer_bind(const sfVertexBuffer* vertexBuffer);
bool sfVertexBuffer_isAvailable(void);
```

##### `include/CSFML/Graphics/View.h` — 17 fonction(s)

```c
sfView* sfView_create(void);
sfView* sfView_createFromRect(sfFloatRect rectangle);
sfView* sfView_copy(const sfView* view);
void sfView_destroy(const sfView* view);
void sfView_setCenter(sfView* view, sfVector2f center);
void sfView_setSize(sfView* view, sfVector2f size);
void sfView_setRotation(sfView* view, float angle);
void sfView_setViewport(sfView* view, sfFloatRect viewport);
void sfView_setScissor(sfView* view, sfFloatRect scissor);
sfVector2f sfView_getCenter(const sfView* view);
sfVector2f sfView_getSize(const sfView* view);
float sfView_getRotation(const sfView* view);
sfFloatRect sfView_getViewport(const sfView* view);
sfFloatRect sfView_getScissor(const sfView* view);
void sfView_move(sfView* view, sfVector2f offset);
void sfView_rotate(sfView* view, float angle);
void sfView_zoom(sfView* view, float factor);
```

### Module Audio (`csfml-audio`)

Toutes les déclarations ci-dessous sont copiées des headers du tag git `3.0.0` (extraction automatique, voir « Méthode »). [CONFIRMÉ]

#### Audio — types (18)

| Nom | Nature | Header (tag 3.0.0) |
|---|---|---|
| `sfEffectProcessor` | pointeur de fonction (callback) | `include/CSFML/Audio/EffectProcessor.h` |
| `sfListenerCone` | struct (champs publics) | `include/CSFML/Audio/Listener.h` |
| `sfTimeSpan` | struct (champs publics) | `include/CSFML/Audio/Music.h` |
| `sfSoundChannel` | enum | `include/CSFML/Audio/SoundChannel.h` |
| `sfSoundRecorderStartCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/SoundRecorder.h` |
| `sfSoundRecorderProcessCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/SoundRecorder.h` |
| `sfSoundRecorderStopCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/SoundRecorder.h` |
| `sfSoundSourceCone` | struct (champs publics) | `include/CSFML/Audio/SoundSourceCone.h` |
| `sfSoundStatus` | enum | `include/CSFML/Audio/SoundStatus.h` |
| `sfSoundStreamChunk` | struct (champs publics) | `include/CSFML/Audio/SoundStream.h` |
| `sfSoundStreamGetDataCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/SoundStream.h` |
| `sfSoundStreamSeekCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/SoundStream.h` |
| `sfMusic` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |
| `sfSound` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |
| `sfSoundBuffer` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |
| `sfSoundBufferRecorder` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |
| `sfSoundRecorder` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |
| `sfSoundStream` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Audio/Types.h` |

Définition des types non opaques (format compact, une ligne par struct) :

```c
typedef void (*sfEffectProcessor)(const float* inputFrames, unsigned int* inputFrameCount, float* outputFrames, unsigned int* outputFrameCount, unsigned int frameChannelCount);   /* CSFML/Audio/EffectProcessor.h */
typedef struct { float innerAngle; float outerAngle; float outerGain; } sfListenerCone;   /* CSFML/Audio/Listener.h */
typedef struct { sfTime offset; sfTime length; } sfTimeSpan;   /* CSFML/Audio/Music.h */
typedef enum {   /* CSFML/Audio/SoundChannel.h, 20 valeurs */
    sfSoundChannelUnspecified, sfSoundChannelMono, sfSoundChannelFrontLeft,
    sfSoundChannelFrontRight, sfSoundChannelFrontCenter, sfSoundChannelFrontLeftOfCenter,
    sfSoundChannelFrontRightOfCenter, sfSoundChannelLowFrequencyEffects, sfSoundChannelBackLeft,
    sfSoundChannelBackRight, sfSoundChannelBackCenter, sfSoundChannelSideLeft,
    sfSoundChannelSideRight, sfSoundChannelTopCenter, sfSoundChannelTopFrontLeft,
    sfSoundChannelTopFrontRight, sfSoundChannelTopFrontCenter, sfSoundChannelTopBackLeft,
    sfSoundChannelTopBackRight, sfSoundChannelTopBackCenter
} sfSoundChannel;
typedef bool (*sfSoundRecorderStartCallback)(void*);   /* CSFML/Audio/SoundRecorder.h */
typedef bool (*sfSoundRecorderProcessCallback)(const int16_t*, size_t, void*);   /* CSFML/Audio/SoundRecorder.h */
typedef void (*sfSoundRecorderStopCallback)(void*);   /* CSFML/Audio/SoundRecorder.h */
typedef struct { float innerAngle; float outerAngle; float outerGain; } sfSoundSourceCone;   /* CSFML/Audio/SoundSourceCone.h */
typedef enum {   /* CSFML/Audio/SoundStatus.h, 3 valeurs */
    sfStopped, sfPaused, sfPlaying
} sfSoundStatus;
typedef struct { int16_t* samples; unsigned int sampleCount; } sfSoundStreamChunk;   /* CSFML/Audio/SoundStream.h */
typedef bool (*sfSoundStreamGetDataCallback)(sfSoundStreamChunk*, void*);   /* CSFML/Audio/SoundStream.h */
typedef void (*sfSoundStreamSeekCallback)(sfTime, void*);   /* CSFML/Audio/SoundStream.h */
```

#### Audio — fonctions (191)

##### `include/CSFML/Audio/Listener.h` — 12 fonction(s)

```c
void sfListener_setGlobalVolume(float volume);
float sfListener_getGlobalVolume(void);
void sfListener_setPosition(sfVector3f position);
sfVector3f sfListener_getPosition(void);
void sfListener_setDirection(sfVector3f direction);
sfVector3f sfListener_getDirection(void);
void sfListener_setVelocity(sfVector3f velocity);
sfVector3f sfListener_getVelocity(void);
void sfListener_setCone(sfListenerCone cone);
sfListenerCone sfListener_getCone(void);
void sfListener_setUpVector(sfVector3f upVector);
sfVector3f sfListener_getUpVector(void);
```

##### `include/CSFML/Audio/Music.h` — 51 fonction(s)

```c
sfMusic* sfMusic_createFromFile(const char* filename);
sfMusic* sfMusic_createFromMemory(const void* data, size_t sizeInBytes);
sfMusic* sfMusic_createFromStream(sfInputStream* stream);
void sfMusic_destroy(const sfMusic* music);
void sfMusic_setLooping(sfMusic* music, bool loop);
bool sfMusic_isLooping(const sfMusic* music);
void sfMusic_setEffectProcessor(sfMusic* music, sfEffectProcessor effectProcessor);
sfTime sfMusic_getDuration(const sfMusic* music);
sfTimeSpan sfMusic_getLoopPoints(const sfMusic* music);
void sfMusic_setLoopPoints(sfMusic* music, sfTimeSpan timePoints);
void sfMusic_play(sfMusic* music);
void sfMusic_pause(sfMusic* music);
void sfMusic_stop(sfMusic* music);
unsigned int sfMusic_getChannelCount(const sfMusic* music);
unsigned int sfMusic_getSampleRate(const sfMusic* music);
const sfSoundChannel* sfMusic_getChannelMap(const sfMusic* music, size_t* count);
sfSoundStatus sfMusic_getStatus(const sfMusic* music);
sfTime sfMusic_getPlayingOffset(const sfMusic* music);
void sfMusic_setPitch(sfMusic* music, float pitch);
void sfMusic_setPan(sfMusic* music, float pan);
void sfMusic_setVolume(sfMusic* music, float volume);
void sfMusic_setSpatializationEnabled(sfMusic* music, bool enabled);
void sfMusic_setPosition(sfMusic* music, sfVector3f position);
void sfMusic_setDirection(sfMusic* music, sfVector3f direction);
void sfMusic_setCone(sfMusic* music, sfSoundSourceCone cone);
void sfMusic_setVelocity(sfMusic* music, sfVector3f velocity);
void sfMusic_setDopplerFactor(sfMusic* music, float factor);
void sfMusic_setDirectionalAttenuationFactor(sfMusic* music, float factor);
void sfMusic_setRelativeToListener(sfMusic* music, bool relative);
void sfMusic_setMinDistance(sfMusic* music, float distance);
void sfMusic_setMaxDistance(sfMusic* music, float distance);
void sfMusic_setMinGain(sfMusic* music, float gain);
void sfMusic_setMaxGain(sfMusic* music, float gain);
void sfMusic_setAttenuation(sfMusic* music, float attenuation);
void sfMusic_setPlayingOffset(sfMusic* music, sfTime timeOffset);
float sfMusic_getPitch(const sfMusic* music);
float sfMusic_getPan(const sfMusic* music);
bool sfMusic_isSpatializationEnabled(const sfMusic* music);
float sfMusic_getVolume(const sfMusic* music);
sfVector3f sfMusic_getPosition(const sfMusic* music);
sfVector3f sfMusic_getDirection(const sfMusic* music);
sfSoundSourceCone sfMusic_getCone(const sfMusic* music);
sfVector3f sfMusic_getVelocity(const sfMusic* music);
float sfMusic_getDopplerFactor(const sfMusic* music);
float sfMusic_getDirectionalAttenuationFactor(const sfMusic* music);
bool sfMusic_isRelativeToListener(const sfMusic* music);
float sfMusic_getMinDistance(const sfMusic* music);
float sfMusic_getMaxDistance(const sfMusic* music);
float sfMusic_getMinGain(const sfMusic* music);
float sfMusic_getMaxGain(const sfMusic* music);
float sfMusic_getAttenuation(const sfMusic* music);
```

##### `include/CSFML/Audio/Sound.h` — 46 fonction(s)

```c
sfSound* sfSound_create(const sfSoundBuffer* buffer);
sfSound* sfSound_copy(const sfSound* sound);
void sfSound_destroy(const sfSound* sound);
void sfSound_play(sfSound* sound);
void sfSound_pause(sfSound* sound);
void sfSound_stop(sfSound* sound);
void sfSound_setBuffer(sfSound* sound, const sfSoundBuffer* buffer);
const sfSoundBuffer* sfSound_getBuffer(const sfSound* sound);
void sfSound_setLooping(sfSound* sound, bool loop);
bool sfSound_isLooping(const sfSound* sound);
sfSoundStatus sfSound_getStatus(const sfSound* sound);
void sfSound_setPitch(sfSound* sound, float pitch);
void sfSound_setPan(sfSound* sound, float pan);
void sfSound_setVolume(sfSound* sound, float volume);
void sfSound_setSpatializationEnabled(sfSound* sound, bool enabled);
void sfSound_setPosition(sfSound* sound, sfVector3f position);
void sfSound_setDirection(sfSound* sound, sfVector3f direction);
void sfSound_setCone(sfSound* sound, sfSoundSourceCone cone);
void sfSound_setVelocity(sfSound* sound, sfVector3f velocity);
void sfSound_setDopplerFactor(sfSound* sound, float factor);
void sfSound_setDirectionalAttenuationFactor(sfSound* sound, float factor);
void sfSound_setRelativeToListener(sfSound* sound, bool relative);
void sfSound_setMinDistance(sfSound* sound, float distance);
void sfSound_setMaxDistance(sfSound* sound, float distance);
void sfSound_setMinGain(sfSound* sound, float gain);
void sfSound_setMaxGain(sfSound* sound, float gain);
void sfSound_setAttenuation(sfSound* sound, float attenuation);
void sfSound_setPlayingOffset(sfSound* sound, sfTime timeOffset);
void sfSound_setEffectProcessor(sfSound* sound, sfEffectProcessor effectProcessor);
float sfSound_getPitch(const sfSound* sound);
float sfSound_getPan(const sfSound* sound);
float sfSound_getVolume(const sfSound* sound);
bool sfSound_isSpatializationEnabled(const sfSound* sound);
sfVector3f sfSound_getPosition(const sfSound* sound);
sfVector3f sfSound_getDirection(const sfSound* sound);
sfSoundSourceCone sfSound_getCone(const sfSound* sound);
sfVector3f sfSound_getVelocity(const sfSound* sound);
float sfSound_getDopplerFactor(const sfSound* sound);
float sfSound_getDirectionalAttenuationFactor(const sfSound* sound);
bool sfSound_isRelativeToListener(const sfSound* sound);
float sfSound_getMinDistance(const sfSound* sound);
float sfSound_getMaxDistance(const sfSound* sound);
float sfSound_getMinGain(const sfSound* sound);
float sfSound_getMaxGain(const sfSound* sound);
float sfSound_getAttenuation(const sfSound* sound);
sfTime sfSound_getPlayingOffset(const sfSound* sound);
```

##### `include/CSFML/Audio/SoundBuffer.h` — 13 fonction(s)

```c
sfSoundBuffer* sfSoundBuffer_createFromFile(const char* filename);
sfSoundBuffer* sfSoundBuffer_createFromMemory(const void* data, size_t sizeInBytes);
sfSoundBuffer* sfSoundBuffer_createFromStream(sfInputStream* stream);
sfSoundBuffer* sfSoundBuffer_createFromSamples(const int16_t* samples, uint64_t sampleCount, unsigned int channelCount, unsigned int sampleRate, sfSoundChannel* channelMapData, size_t channelMapSize);
sfSoundBuffer* sfSoundBuffer_copy(const sfSoundBuffer* soundBuffer);
void sfSoundBuffer_destroy(const sfSoundBuffer* soundBuffer);
bool sfSoundBuffer_saveToFile(const sfSoundBuffer* soundBuffer, const char* filename);
const int16_t* sfSoundBuffer_getSamples(const sfSoundBuffer* soundBuffer);
uint64_t sfSoundBuffer_getSampleCount(const sfSoundBuffer* soundBuffer);
unsigned int sfSoundBuffer_getSampleRate(const sfSoundBuffer* soundBuffer);
unsigned int sfSoundBuffer_getChannelCount(const sfSoundBuffer* soundBuffer);
sfSoundChannel* sfSoundBuffer_getChannelMap(const sfSoundBuffer* soundBuffer, size_t* count);
sfTime sfSoundBuffer_getDuration(const sfSoundBuffer* soundBuffer);
```

##### `include/CSFML/Audio/SoundBufferRecorder.h` — 10 fonction(s)

```c
sfSoundBufferRecorder* sfSoundBufferRecorder_create(void);
void sfSoundBufferRecorder_destroy(const sfSoundBufferRecorder* soundBufferRecorder);
bool sfSoundBufferRecorder_start(sfSoundBufferRecorder* soundBufferRecorder, unsigned int sampleRate);
void sfSoundBufferRecorder_stop(sfSoundBufferRecorder* soundBufferRecorder);
unsigned int sfSoundBufferRecorder_getSampleRate(const sfSoundBufferRecorder* soundBufferRecorder);
const sfSoundBuffer* sfSoundBufferRecorder_getBuffer(const sfSoundBufferRecorder* soundBufferRecorder);
bool sfSoundBufferRecorder_setDevice(sfSoundBufferRecorder* soundBufferRecorder, const char* name);
const char* sfSoundBufferRecorder_getDevice(sfSoundBufferRecorder* soundBufferRecorder);
void sfSoundBufferRecorder_setChannelCount(sfSoundBufferRecorder* soundBufferRecorder, unsigned int channelCount);
unsigned int sfSoundBufferRecorder_getChannelCount(const sfSoundBufferRecorder* soundBufferRecorder);
```

##### `include/CSFML/Audio/SoundRecorder.h` — 13 fonction(s)

```c
sfSoundRecorder* sfSoundRecorder_create(sfSoundRecorderStartCallback onStart, sfSoundRecorderProcessCallback onProcess, sfSoundRecorderStopCallback onStop, void* userData);
void sfSoundRecorder_destroy(const sfSoundRecorder* soundRecorder);
bool sfSoundRecorder_start(sfSoundRecorder* soundRecorder, unsigned int sampleRate);
void sfSoundRecorder_stop(sfSoundRecorder* soundRecorder);
unsigned int sfSoundRecorder_getSampleRate(const sfSoundRecorder* soundRecorder);
bool sfSoundRecorder_isAvailable(void);
const char* const* sfSoundRecorder_getAvailableDevices(size_t* count);
const char* sfSoundRecorder_getDefaultDevice(void);
bool sfSoundRecorder_setDevice(sfSoundRecorder* soundRecorder, const char* name);
const char* sfSoundRecorder_getDevice(sfSoundRecorder* soundRecorder);
void sfSoundRecorder_setChannelCount(sfSoundRecorder* soundRecorder, unsigned int channelCount);
unsigned int sfSoundRecorder_getChannelCount(const sfSoundRecorder* soundRecorder);
sfSoundChannel* sfSoundRecorder_getChannelMap(const sfSoundRecorder* soundRecorder, size_t* count);
```

##### `include/CSFML/Audio/SoundStream.h` — 46 fonction(s)

```c
sfSoundStream* sfSoundStream_create(sfSoundStreamGetDataCallback onGetData, sfSoundStreamSeekCallback onSeek, unsigned int channelCount, unsigned int sampleRate, sfSoundChannel* channelMapData, size_t channelMapSize, void* userData);
void sfSoundStream_destroy(const sfSoundStream* soundStream);
void sfSoundStream_play(sfSoundStream* soundStream);
void sfSoundStream_pause(sfSoundStream* soundStream);
void sfSoundStream_stop(sfSoundStream* soundStream);
sfSoundStatus sfSoundStream_getStatus(const sfSoundStream* soundStream);
unsigned int sfSoundStream_getChannelCount(const sfSoundStream* soundStream);
unsigned int sfSoundStream_getSampleRate(const sfSoundStream* soundStream);
sfSoundChannel* sfSoundStream_getChannelMap(const sfSoundStream* soundStream, size_t* count);
void sfSoundStream_setPitch(sfSoundStream* soundStream, float pitch);
void sfSoundStream_setPan(sfSoundStream* soundStream, float pan);
void sfSoundStream_setVolume(sfSoundStream* soundStream, float volume);
void sfSoundStream_setSpatializationEnabled(sfSoundStream* soundStream, bool enabled);
void sfSoundStream_setPosition(sfSoundStream* soundStream, sfVector3f position);
void sfSoundStream_setDirection(sfSoundStream* soundStream, sfVector3f direction);
void sfSoundStream_setCone(sfSoundStream* soundStream, sfSoundSourceCone cone);
void sfSoundStream_setVelocity(sfSoundStream* soundStream, sfVector3f velocity);
void sfSoundStream_setDopplerFactor(sfSoundStream* soundStream, float factor);
void sfSoundStream_setDirectionalAttenuationFactor(sfSoundStream* soundStream, float factor);
void sfSoundStream_setRelativeToListener(sfSoundStream* soundStream, bool relative);
void sfSoundStream_setMinDistance(sfSoundStream* soundStream, float distance);
void sfSoundStream_setMaxDistance(sfSoundStream* soundStream, float distance);
void sfSoundStream_setMinGain(sfSoundStream* soundStream, float gain);
void sfSoundStream_setMaxGain(sfSoundStream* soundStream, float gain);
void sfSoundStream_setAttenuation(sfSoundStream* soundStream, float attenuation);
void sfSoundStream_setPlayingOffset(sfSoundStream* soundStream, sfTime timeOffset);
void sfSoundStream_setLooping(sfSoundStream* soundStream, bool loop);
float sfSoundStream_getPitch(const sfSoundStream* soundStream);
float sfSoundStream_getPan(const sfSoundStream* soundStream);
float sfSoundStream_getVolume(const sfSoundStream* soundStream);
bool sfSoundStream_isSpatializationEnabled(const sfSoundStream* soundStream);
sfVector3f sfSoundStream_getPosition(const sfSoundStream* soundStream);
sfVector3f sfSoundStream_getDirection(const sfSoundStream* soundStream);
sfSoundSourceCone sfSoundStream_getCone(const sfSoundStream* soundStream);
sfVector3f sfSoundStream_getVelocity(const sfSoundStream* soundStream);
float sfSoundStream_getDopplerFactor(const sfSoundStream* soundStream);
float sfSoundStream_getDirectionalAttenuationFactor(const sfSoundStream* soundStream);
bool sfSoundStream_isRelativeToListener(const sfSoundStream* soundStream);
float sfSoundStream_getMinDistance(const sfSoundStream* soundStream);
float sfSoundStream_getMaxDistance(const sfSoundStream* soundStream);
float sfSoundStream_getMinGain(const sfSoundStream* soundStream);
float sfSoundStream_getMaxGain(const sfSoundStream* soundStream);
float sfSoundStream_getAttenuation(const sfSoundStream* soundStream);
bool sfSoundStream_isLooping(const sfSoundStream* soundStream);
void sfSoundStream_setEffectProcessor(sfSoundStream* soundStream, sfEffectProcessor effectProcessor);
sfTime sfSoundStream_getPlayingOffset(const sfSoundStream* soundStream);
```

### Module Network (`csfml-network`)

Toutes les déclarations ci-dessous sont copiées des headers du tag git `3.0.0` (extraction automatique, voir « Méthode »). [CONFIRMÉ]

#### Network — types (18)

| Nom | Nature | Header (tag 3.0.0) |
|---|---|---|
| `sfFtpTransferMode` | enum | `include/CSFML/Network/Ftp.h` |
| `sfFtpStatus` | enum | `include/CSFML/Network/Ftp.h` |
| `sfHttpMethod` | enum | `include/CSFML/Network/Http.h` |
| `sfHttpStatus` | enum | `include/CSFML/Network/Http.h` |
| `sfIpAddress` | struct (champs publics) | `include/CSFML/Network/IpAddress.h` |
| `sfSocketStatus` | enum | `include/CSFML/Network/SocketStatus.h` |
| `sfFtpDirectoryResponse` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfFtpListingResponse` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfFtpResponse` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfFtp` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfHttpRequest` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfHttpResponse` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfHttp` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfPacket` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSocketSelector` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfTcpListener` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfTcpSocket` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfUdpSocket` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |

Définition des types non opaques (format compact, une ligne par struct) :

```c
typedef enum {   /* CSFML/Network/Ftp.h, 3 valeurs */
    sfFtpBinary, sfFtpAscii, sfFtpEbcdic
} sfFtpTransferMode;
typedef enum {   /* CSFML/Network/Ftp.h, 43 valeurs */
    sfFtpRestartMarkerReply = 110, sfFtpServiceReadySoon = 120, sfFtpDataConnectionAlreadyOpened =
    125, sfFtpOpeningDataConnection = 150, sfFtpOk = 200, sfFtpPointlessCommand = 202,
    sfFtpSystemStatus = 211, sfFtpDirectoryStatus = 212, sfFtpFileStatus = 213, sfFtpHelpMessage =
    214, sfFtpSystemType = 215, sfFtpServiceReady = 220, sfFtpClosingConnection = 221,
    sfFtpDataConnectionOpened = 225, sfFtpClosingDataConnection = 226, sfFtpEnteringPassiveMode =
    227, sfFtpLoggedIn = 230, sfFtpFileActionOk = 250, sfFtpDirectoryOk = 257, sfFtpNeedPassword =
    331, sfFtpNeedAccountToLogIn = 332, sfFtpNeedInformation = 350, sfFtpServiceUnavailable = 421,
    sfFtpDataConnectionUnavailable = 425, sfFtpTransferAborted = 426, sfFtpFileActionAborted = 450,
    sfFtpLocalError = 451, sfFtpInsufficientStorageSpace = 452, sfFtpCommandUnknown = 500,
    sfFtpParametersUnknown = 501, sfFtpCommandNotImplemented = 502, sfFtpBadCommandSequence = 503,
    sfFtpParameterNotImplemented = 504, sfFtpNotLoggedIn = 530, sfFtpNeedAccountToStore = 532,
    sfFtpFileUnavailable = 550, sfFtpPageTypeUnknown = 551, sfFtpNotEnoughMemory = 552,
    sfFtpFilenameNotAllowed = 553, sfFtpInvalidResponse = 1000, sfFtpConnectionFailed = 1001,
    sfFtpConnectionClosed = 1002, sfFtpInvalidFile = 1003
} sfFtpStatus;
typedef enum {   /* CSFML/Network/Http.h, 5 valeurs */
    sfHttpGet, sfHttpPost, sfHttpHead, sfHttpPut, sfHttpDelete
} sfHttpMethod;
typedef enum {   /* CSFML/Network/Http.h, 23 valeurs */
    sfHttpOk = 200, sfHttpCreated = 201, sfHttpAccepted = 202, sfHttpNoContent = 204,
    sfHttpResetContent = 205, sfHttpPartialContent = 206, sfHttpMultipleChoices = 300,
    sfHttpMovedPermanently = 301, sfHttpMovedTemporarily = 302, sfHttpNotModified = 304,
    sfHttpBadRequest = 400, sfHttpUnauthorized = 401, sfHttpForbidden = 403, sfHttpNotFound = 404,
    sfHttpRangeNotSatisfiable = 407, sfHttpInternalServerError = 500, sfHttpNotImplemented = 501,
    sfHttpBadGateway = 502, sfHttpServiceNotAvailable = 503, sfHttpGatewayTimeout = 504,
    sfHttpVersionNotSupported = 505, sfHttpInvalidResponse = 1000, sfHttpConnectionFailed = 1001
} sfHttpStatus;
typedef struct { char address[16]; } sfIpAddress;   /* CSFML/Network/IpAddress.h */
typedef enum {   /* CSFML/Network/SocketStatus.h, 5 valeurs */
    sfSocketDone, sfSocketNotReady, sfSocketPartial, sfSocketDisconnected, sfSocketError
} sfSocketStatus;
```

#### Network — constantes

```c
CSFML_NETWORK_API const sfIpAddress sfIpAddress_None;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_Any;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_LocalHost;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_Broadcast;   /* CSFML/Network/IpAddress.h */
```

#### Network — fonctions (144)

##### `include/CSFML/Network/Ftp.h` — 34 fonction(s)

```c
/* --- sfFtpListingResponse --- */
void sfFtpListingResponse_destroy(const sfFtpListingResponse* ftpListingResponse);
bool sfFtpListingResponse_isOk(const sfFtpListingResponse* ftpListingResponse);
sfFtpStatus sfFtpListingResponse_getStatus(const sfFtpListingResponse* ftpListingResponse);
const char* sfFtpListingResponse_getMessage(const sfFtpListingResponse* ftpListingResponse);
size_t sfFtpListingResponse_getCount(const sfFtpListingResponse* ftpListingResponse);
const char* sfFtpListingResponse_getName(const sfFtpListingResponse* ftpListingResponse, size_t index);

/* --- sfFtpDirectoryResponse --- */
void sfFtpDirectoryResponse_destroy(const sfFtpDirectoryResponse* ftpDirectoryResponse);
bool sfFtpDirectoryResponse_isOk(const sfFtpDirectoryResponse* ftpDirectoryResponse);
sfFtpStatus sfFtpDirectoryResponse_getStatus(const sfFtpDirectoryResponse* ftpDirectoryResponse);
const char* sfFtpDirectoryResponse_getMessage(const sfFtpDirectoryResponse* ftpDirectoryResponse);
const char* sfFtpDirectoryResponse_getDirectory(const sfFtpDirectoryResponse* ftpDirectoryResponse);
const sfChar32* sfFtpDirectoryResponse_getDirectoryUnicode(const sfFtpDirectoryResponse* ftpDirectoryResponse);

/* --- sfFtpResponse --- */
void sfFtpResponse_destroy(const sfFtpResponse* ftpResponse);
bool sfFtpResponse_isOk(const sfFtpResponse* ftpResponse);
sfFtpStatus sfFtpResponse_getStatus(const sfFtpResponse* ftpResponse);
const char* sfFtpResponse_getMessage(const sfFtpResponse* ftpResponse);

/* --- sfFtp --- */
sfFtp* sfFtp_create(void);
void sfFtp_destroy(const sfFtp* ftp);
sfFtpResponse* sfFtp_connect(sfFtp* ftp, sfIpAddress server, unsigned short port, sfTime timeout);
sfFtpResponse* sfFtp_loginAnonymous(sfFtp* ftp);
sfFtpResponse* sfFtp_login(sfFtp* ftp, const char* name, const char* password);
sfFtpResponse* sfFtp_disconnect(sfFtp* ftp);
sfFtpResponse* sfFtp_keepAlive(sfFtp* ftp);
sfFtpDirectoryResponse* sfFtp_getWorkingDirectory(sfFtp* ftp);
sfFtpListingResponse* sfFtp_getDirectoryListing(sfFtp* ftp, const char* directory);
sfFtpResponse* sfFtp_changeDirectory(sfFtp* ftp, const char* directory);
sfFtpResponse* sfFtp_parentDirectory(sfFtp* ftp);
sfFtpResponse* sfFtp_createDirectory(sfFtp* ftp, const char* name);
sfFtpResponse* sfFtp_deleteDirectory(sfFtp* ftp, const char* name);
sfFtpResponse* sfFtp_renameFile(sfFtp* ftp, const char* file, const char* newName);
sfFtpResponse* sfFtp_deleteFile(sfFtp* ftp, const char* name);
sfFtpResponse* sfFtp_download(sfFtp* ftp, const char* remoteFile, const char* localPath, sfFtpTransferMode mode);
sfFtpResponse* sfFtp_upload(sfFtp* ftp, const char* localFile, const char* remotePath, sfFtpTransferMode mode, bool append);
sfFtpResponse* sfFtp_sendCommand(sfFtp* ftp, const char* command, const char* parameter);
```

##### `include/CSFML/Network/Http.h` — 17 fonction(s)

```c
/* --- sfHttpRequest --- */
sfHttpRequest* sfHttpRequest_create(void);
void sfHttpRequest_destroy(const sfHttpRequest* httpRequest);
void sfHttpRequest_setField(sfHttpRequest* httpRequest, const char* field, const char* value);
void sfHttpRequest_setMethod(sfHttpRequest* httpRequest, sfHttpMethod method);
void sfHttpRequest_setUri(sfHttpRequest* httpRequest, const char* uri);
void sfHttpRequest_setHttpVersion(sfHttpRequest* httpRequest, unsigned int major, unsigned int minor);
void sfHttpRequest_setBody(sfHttpRequest* httpRequest, const char* body);

/* --- sfHttpResponse --- */
void sfHttpResponse_destroy(const sfHttpResponse* httpResponse);
const char* sfHttpResponse_getField(const sfHttpResponse* httpResponse, const char* field);
sfHttpStatus sfHttpResponse_getStatus(const sfHttpResponse* httpResponse);
unsigned int sfHttpResponse_getMajorVersion(const sfHttpResponse* httpResponse);
unsigned int sfHttpResponse_getMinorVersion(const sfHttpResponse* httpResponse);
const char* sfHttpResponse_getBody(const sfHttpResponse* httpResponse);

/* --- sfHttp --- */
sfHttp* sfHttp_create(void);
void sfHttp_destroy(const sfHttp* http);
void sfHttp_setHost(sfHttp* http, const char* host, unsigned short port);
sfHttpResponse* sfHttp_sendRequest(sfHttp* http, const sfHttpRequest* request, sfTime timeout);
```

##### `include/CSFML/Network/IpAddress.h` — 7 fonction(s)

```c
sfIpAddress sfIpAddress_fromString(const char* address);
sfIpAddress sfIpAddress_fromBytes(uint8_t byte0, uint8_t byte1, uint8_t byte2, uint8_t byte3);
sfIpAddress sfIpAddress_fromInteger(uint32_t address);
void sfIpAddress_toString(sfIpAddress address, char* string);
uint32_t sfIpAddress_toInteger(sfIpAddress address);
sfIpAddress sfIpAddress_getLocalAddress(void);
sfIpAddress sfIpAddress_getPublicAddress(sfTime timeout);
```

##### `include/CSFML/Network/Packet.h` — 36 fonction(s)

```c
sfPacket* sfPacket_create(void);
sfPacket* sfPacket_copy(const sfPacket* packet);
void sfPacket_destroy(const sfPacket* packet);
void sfPacket_append(sfPacket* packet, const void* data, size_t sizeInBytes);
size_t sfPacket_getReadPosition(const sfPacket* packet);
void sfPacket_clear(sfPacket* packet);
const void* sfPacket_getData(const sfPacket* packet);
size_t sfPacket_getDataSize(const sfPacket* packet);
bool sfPacket_endOfPacket(const sfPacket* packet);
bool sfPacket_canRead(const sfPacket* packet);
bool sfPacket_readBool(sfPacket* packet);
int8_t sfPacket_readInt8(sfPacket* packet);
uint8_t sfPacket_readUint8(sfPacket* packet);
int16_t sfPacket_readInt16(sfPacket* packet);
uint16_t sfPacket_readUint16(sfPacket* packet);
int32_t sfPacket_readInt32(sfPacket* packet);
uint32_t sfPacket_readUint32(sfPacket* packet);
int64_t sfPacket_readInt64(sfPacket* packet);
uint64_t sfPacket_readUint64(sfPacket* packet);
float sfPacket_readFloat(sfPacket* packet);
double sfPacket_readDouble(sfPacket* packet);
void sfPacket_readString(sfPacket* packet, char* string);
void sfPacket_readWideString(sfPacket* packet, wchar_t* string);
void sfPacket_writeBool(sfPacket* packet, bool);
void sfPacket_writeInt8(sfPacket* packet, int8_t);
void sfPacket_writeUint8(sfPacket* packet, uint8_t);
void sfPacket_writeInt16(sfPacket* packet, int16_t);
void sfPacket_writeUint16(sfPacket* packet, uint16_t);
void sfPacket_writeInt32(sfPacket* packet, int32_t);
void sfPacket_writeUint32(sfPacket* packet, uint32_t);
void sfPacket_writeInt64(sfPacket* packet, int64_t);
void sfPacket_writeUint64(sfPacket* packet, uint64_t);
void sfPacket_writeFloat(sfPacket* packet, float);
void sfPacket_writeDouble(sfPacket* packet, double);
void sfPacket_writeString(sfPacket* packet, const char* string);
void sfPacket_writeWideString(sfPacket* packet, const wchar_t* string);
```

##### `include/CSFML/Network/SocketSelector.h` — 14 fonction(s)

```c
sfSocketSelector* sfSocketSelector_create(void);
sfSocketSelector* sfSocketSelector_copy(const sfSocketSelector* selector);
void sfSocketSelector_destroy(const sfSocketSelector* selector);
void sfSocketSelector_addTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
void sfSocketSelector_addTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
void sfSocketSelector_addUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
void sfSocketSelector_removeTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
void sfSocketSelector_removeTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
void sfSocketSelector_removeUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
void sfSocketSelector_clear(sfSocketSelector* selector);
bool sfSocketSelector_wait(sfSocketSelector* selector, sfTime timeout);
bool sfSocketSelector_isTcpListenerReady(const sfSocketSelector* selector, sfTcpListener* socket);
bool sfSocketSelector_isTcpSocketReady(const sfSocketSelector* selector, sfTcpSocket* socket);
bool sfSocketSelector_isUdpSocketReady(const sfSocketSelector* selector, sfUdpSocket* socket);
```

##### `include/CSFML/Network/TcpListener.h` — 9 fonction(s)

```c
sfTcpListener* sfTcpListener_create(void);
void sfTcpListener_destroy(const sfTcpListener* listener);
void sfTcpListener_setBlocking(sfTcpListener* listener, bool blocking);
bool sfTcpListener_isBlocking(const sfTcpListener* listener);
unsigned short sfTcpListener_getLocalPort(const sfTcpListener* listener);
sfSocketStatus sfTcpListener_listen(sfTcpListener* listener, unsigned short port, sfIpAddress address);
void sfTcpListener_close(sfTcpListener* listener);
sfSocketStatus sfTcpListener_accept(sfTcpListener* listener, sfTcpSocket** connected);
unsigned short sfTcpListener_anyPort(void);
```

##### `include/CSFML/Network/TcpSocket.h` — 14 fonction(s)

```c
sfTcpSocket* sfTcpSocket_create(void);
void sfTcpSocket_destroy(const sfTcpSocket* socket);
void sfTcpSocket_setBlocking(sfTcpSocket* socket, bool blocking);
bool sfTcpSocket_isBlocking(const sfTcpSocket* socket);
unsigned short sfTcpSocket_getLocalPort(const sfTcpSocket* socket);
sfIpAddress sfTcpSocket_getRemoteAddress(const sfTcpSocket* socket);
unsigned short sfTcpSocket_getRemotePort(const sfTcpSocket* socket);
sfSocketStatus sfTcpSocket_connect(sfTcpSocket* socket, sfIpAddress remoteAddress, unsigned short remotePort, sfTime timeout);
void sfTcpSocket_disconnect(sfTcpSocket* socket);
sfSocketStatus sfTcpSocket_send(sfTcpSocket* socket, const void* data, size_t size);
sfSocketStatus sfTcpSocket_sendPartial(sfTcpSocket* socket, const void* data, size_t size, size_t* sent);
sfSocketStatus sfTcpSocket_receive(sfTcpSocket* socket, void* data, size_t size, size_t* received);
sfSocketStatus sfTcpSocket_sendPacket(sfTcpSocket* socket, sfPacket* packet);
sfSocketStatus sfTcpSocket_receivePacket(sfTcpSocket* socket, sfPacket* packet);
```

##### `include/CSFML/Network/UdpSocket.h` — 13 fonction(s)

```c
sfUdpSocket* sfUdpSocket_create(void);
void sfUdpSocket_destroy(const sfUdpSocket* socket);
void sfUdpSocket_setBlocking(sfUdpSocket* socket, bool blocking);
bool sfUdpSocket_isBlocking(const sfUdpSocket* socket);
unsigned short sfUdpSocket_getLocalPort(const sfUdpSocket* socket);
sfSocketStatus sfUdpSocket_bind(sfUdpSocket* socket, unsigned short port, sfIpAddress address);
void sfUdpSocket_unbind(sfUdpSocket* socket);
sfSocketStatus sfUdpSocket_send(sfUdpSocket* socket, const void* data, size_t size, sfIpAddress remoteAddress, unsigned short remotePort);
sfSocketStatus sfUdpSocket_receive(sfUdpSocket* socket, void* data, size_t size, size_t* received, sfIpAddress* remoteAddress, unsigned short* remotePort);
sfSocketStatus sfUdpSocket_sendPacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress remoteAddress, unsigned short remotePort);
sfSocketStatus sfUdpSocket_receivePacket(sfUdpSocket* socket, sfPacket* packet, sfIpAddress* remoteAddress, unsigned short* remotePort);
unsigned int sfUdpSocket_maxDatagramSize(void);
unsigned short sfUdpSocket_anyPort(void);
```



## 5. Statistiques

| Module | Headers avec fonctions | Fonctions (3.0.0) | Fonctions (master) | dont `CSFML_DEPRECATED` (master) | Constantes globales (3.0.0) | Types nommés (3.0.0) |
|---|---|---|---|---|---|---|
| System | 5 | 21 | 22 | 0 | 1 | 12 |
| Window | 12 | 103 | 103 | 3 | 0 | 37 |
| Graphics | 19 | 467 | 488 | 2 | 18 | 48 |
| Audio | 7 | 191 | 200 | 0 | 0 | 18 |
| Network | 8 | 144 | 221 | 35 | 4 | 18 |
| **Total** | **51** | **926** | **1034** | **40** | **23** | **133** |

Fonctions par header (tag 3.0.0) :

| Header | Fonctions |
|---|---|
| `CSFML/System/Alloc.h` | 1 |
| `CSFML/System/Buffer.h` | 4 |
| `CSFML/System/Clock.h` | 9 |
| `CSFML/System/Sleep.h` | 1 |
| `CSFML/System/Time.h` | 6 |
| `CSFML/Window/Clipboard.h` | 4 |
| `CSFML/Window/Context.h` | 7 |
| `CSFML/Window/Cursor.h` | 3 |
| `CSFML/Window/Joystick.h` | 7 |
| `CSFML/Window/Keyboard.h` | 6 |
| `CSFML/Window/Mouse.h` | 5 |
| `CSFML/Window/Sensor.h` | 3 |
| `CSFML/Window/Touch.h` | 3 |
| `CSFML/Window/VideoMode.h` | 3 |
| `CSFML/Window/Vulkan.h` | 3 |
| `CSFML/Window/Window.h` | 32 |
| `CSFML/Window/WindowBase.h` | 27 |
| `CSFML/Graphics/CircleShape.h` | 34 |
| `CSFML/Graphics/Color.h` | 7 |
| `CSFML/Graphics/ConvexShape.h` | 33 |
| `CSFML/Graphics/Font.h` | 16 |
| `CSFML/Graphics/Image.h` | 18 |
| `CSFML/Graphics/Rect.h` | 4 |
| `CSFML/Graphics/RectangleShape.h` | 33 |
| `CSFML/Graphics/RenderTexture.h` | 36 |
| `CSFML/Graphics/RenderWindow.h` | 59 |
| `CSFML/Graphics/Shader.h` | 32 |
| `CSFML/Graphics/Shape.h` | 31 |
| `CSFML/Graphics/Sprite.h` | 24 |
| `CSFML/Graphics/Text.h` | 39 |
| `CSFML/Graphics/Texture.h` | 31 |
| `CSFML/Graphics/Transform.h` | 12 |
| `CSFML/Graphics/Transformable.h` | 16 |
| `CSFML/Graphics/VertexArray.h` | 11 |
| `CSFML/Graphics/VertexBuffer.h` | 14 |
| `CSFML/Graphics/View.h` | 17 |
| `CSFML/Audio/Listener.h` | 12 |
| `CSFML/Audio/Music.h` | 51 |
| `CSFML/Audio/Sound.h` | 46 |
| `CSFML/Audio/SoundBuffer.h` | 13 |
| `CSFML/Audio/SoundBufferRecorder.h` | 10 |
| `CSFML/Audio/SoundRecorder.h` | 13 |
| `CSFML/Audio/SoundStream.h` | 46 |
| `CSFML/Network/Ftp.h` | 34 |
| `CSFML/Network/Http.h` | 17 |
| `CSFML/Network/IpAddress.h` | 7 |
| `CSFML/Network/Packet.h` | 36 |
| `CSFML/Network/SocketSelector.h` | 14 |
| `CSFML/Network/TcpListener.h` | 9 |
| `CSFML/Network/TcpSocket.h` | 14 |
| `CSFML/Network/UdpSocket.h` | 13 |


## 6. Remarques sur les fonctions particulières

### 6.1 Retours à libérer par l'appelant

Tout `sfX_create*` / `sfX_copy` se libère avec `sfX_destroy`. En plus, ces retours sont alloués pour vous
(preuve : header quand il le dit, sinon implémentation `src/` du tag 3.0.0) :

| Fonction | Ce qui est alloué | Libérer avec | Preuve | Statut |
|---|---|---|---|---|
| `const char* sfKeyboard_getDescription(sfScancode code);` | chaîne `strdup` | `free()` ou `sfFree()` (`#include <CSFML/System/Alloc.h>`) | `Window/Keyboard.h` : « owns the string and must be freed » ; `src/CSFML/Window/Keyboard.cpp` l.67 `strdup` | [CONFIRMÉ] |
| `const char* sfFtpDirectoryResponse_getDirectory(const sfFtpDirectoryResponse*);` | chaîne `strdup` | `free()` / `sfFree()` | `Network/Ftp.h` + `src/CSFML/Network/Ftp.cpp` l.146 | [CONFIRMÉ] |
| `const sfChar32* sfFtpDirectoryResponse_getDirectoryUnicode(const sfFtpDirectoryResponse*);` | tableau `malloc` | `free()` / `sfFree()` | `Network/Ftp.h` + `Ftp.cpp` l.53 | [CONFIRMÉ] |
| `sfImage* sfTexture_copyToImage(const sfTexture*);` | `new sfImage` | `sfImage_destroy` | `src/CSFML/Graphics/Texture.cpp` l.204 | [CONFIRMÉ] |
| `sfFtp_connect`, `_loginAnonymous`, `_login`, `_disconnect`, `_keepAlive`, `_changeDirectory`, `_parentDirectory`, `_createDirectory`, `_deleteDirectory`, `_renameFile`, `_deleteFile`, `_download`, `_upload`, `_sendCommand` | `sfFtpResponse*` | `sfFtpResponse_destroy` | `Network/Ftp.h` (type de retour + `sfFtpResponse_destroy`) | [CONFIRMÉ] |
| `sfFtp_getWorkingDirectory` / `sfFtp_getDirectoryListing` | `sfFtpDirectoryResponse*` / `sfFtpListingResponse*` | `sfFtpDirectoryResponse_destroy` / `sfFtpListingResponse_destroy` | `Network/Ftp.h` | [CONFIRMÉ] |
| `sfHttpResponse* sfHttp_sendRequest(sfHttp*, const sfHttpRequest*, sfTime timeout);` | `new sfHttpResponse` | `sfHttpResponse_destroy` | `src/CSFML/Network/Http.cpp` l.164 | [CONFIRMÉ] |
| `sfSocketStatus sfTcpListener_accept(sfTcpListener*, sfTcpSocket** connected);` | nouvelle socket dans `*connected` (NULL si échec) | `sfTcpSocket_destroy` | `Network/TcpListener.h` (« points to a valid sfTcpSocket pointer ») + `src/CSFML/Network/TcpListener.cpp` l.103-109 | [CONFIRMÉ source ; le header ne dit pas qui libère] |
| `bool sfImage_saveToMemory(const sfImage*, sfBuffer* output, const char* format);` | remplit un `sfBuffer` créé par l'appelant | `sfBuffer_destroy` (sur le buffer que vous avez créé) | `Graphics/Image.h`, `System/Buffer.h`, `src/CSFML/Graphics/Image.cpp` l.121-133 | [CONFIRMÉ] |

`sfFree` : « deallocates the memory … using the free function from the C standard library »
([System/Alloc.h](https://github.com/SFML/CSFML/blob/3.0.0/include/CSFML/System/Alloc.h)). Utile sous Windows
quand la DLL et le programme n'ont pas le même runtime C. [CONFIRMÉ header ; usage Windows DÉDUIT]

Attention : `sfFont_getInfo` renvoie un `sfFontInfo` **par valeur** (pas de pointeur à libérer). Son champ
`family` pointe dans la police (`font->getInfo().family.c_str()`, `src/CSFML/Graphics/Font.cpp` l.181). Idem
`sfJoystick_getIdentification` (par valeur, voir 6.2). [CONFIRMÉ]

### 6.2 Pointeurs empruntés : ne pas libérer, durée de validité

| Fonction(s) | Pointe vers | Valide jusqu'à | Preuve (tag 3.0.0) | Statut |
|---|---|---|---|---|
| `sfClipboard_getString`, `sfClipboard_getUnicodeString` | variable **globale statique** de la lib | prochain appel de la même fonction (non thread-safe) | `src/CSFML/Window/Clipboard.cpp` (`ClipboardString`, `ClipboardStringAnsi`) | [CONFIRMÉ source] |
| `sfJoystick_getIdentification(j).name` | `static std::string` | prochain appel (tous joysticks confondus) | `src/CSFML/Window/Joystick.cpp` l.70 | [CONFIRMÉ source] |
| `sfFont_getInfo(f).family` | donnée interne de la police | destruction de la police | `Font.cpp` l.181 + `const Info& getInfo()` (SFML 3.0.2 `Font.hpp` l.210) | [CONFIRMÉ] |
| `sfText_getString` | copie ANSI stockée dans l'objet `sfText` | prochain `getString` / modification / destruction | `src/CSFML/Graphics/Text.cpp` l.254-258 | [CONFIRMÉ source] |
| `sfText_getUnicodeString` | chaîne interne du texte | modification ou destruction du texte | `Text.cpp` l.265-268 + `const String& getString()` (SFML `Text.hpp`) | [CONFIRMÉ] |
| `sfText_getFont`, `sfSprite_getTexture`, `sf*Shape_getTexture`, `sfSound_getBuffer` | l'objet que vous avez passé | sa destruction (par vous) | headers Sprite/Shape/Text/Sound | [CONFIRMÉ] |
| `sfFont_getTexture` | `sfTexture` interne mis en cache par la police | destruction de la police (contenu modifié quand de nouveaux glyphes sont chargés) | `src/CSFML/Graphics/FontStruct.hpp` (`std::map<unsigned int, sfTexture> Textures`) | [CONFIRMÉ source ; mise à jour DÉDUITE] |
| `sfRenderWindow_getView`, `_getDefaultView`, `sfRenderTexture_getView`, `_getDefaultView`, `_getTexture` | objet interne | destruction de la fenêtre / render texture | `RenderWindow.cpp` l.397-401 | [CONFIRMÉ] |
| `sfImage_getPixelsPtr`, `sfBuffer_getData`, `sfSoundBuffer_getSamples`, `sfPacket_getData`, `sfSoundBufferRecorder_getBuffer` | données internes | modification / destruction de l'objet | headers (« read-only pointer ») | [CONFIRMÉ header ; invalidation DÉDUITE] |
| `sfVertexArray_getVertex` | sommet interne (modifiable) | `resize`/`append`/`clear` (réallocation) | `Graphics/VertexArray.h` (« doesn't check index ») | [CONFIRMÉ ; invalidation DÉDUITE] |
| `sfSoundBuffer_getChannelMap`, `sfMusic_getChannelMap`, `sfSoundStream_getChannelMap`, `sfSoundRecorder_getChannelMap` | tableau `Channels` stocké dans l'objet | prochain appel / destruction | `src/CSFML/Audio/*.cpp` | [CONFIRMÉ source] |
| `sfSoundRecorder_getDevice`, `sfSoundBufferRecorder_getDevice` | `std::string` membre de l'objet | prochain appel / destruction | `src/CSFML/Audio/SoundRecorder.cpp` l.122-129 | [CONFIRMÉ source] |
| `sfVideoMode_getFullscreenModes`, `sfVulkan_getGraphicsRequiredInstanceExtensions`, `sfSoundRecorder_getAvailableDevices`, `sfSoundRecorder_getDefaultDevice` | tableau **statique calculé une seule fois** | fin du programme | `src/CSFML/Window/VideoMode.cpp` l.44, `Vulkan.cpp` l.52, `SoundRecorder.cpp` l.86-108 | [CONFIRMÉ source] |
| `sfFtp*Response_getMessage`, `sfFtpListingResponse_getName`, `sfHttpResponse_getField`, `sfHttpResponse_getBody` | chaîne interne de la réponse | destruction de la réponse | `Ftp.cpp`, `Http.cpp` + références `const std::string&` (SFML 3.0.2 `Http.hpp`, `Ftp.hpp`) | [CONFIRMÉ] |

Conséquence du cache statique : un micro branché après le premier appel à
`sfSoundRecorder_getAvailableDevices` n'apparaît jamais dans la liste. [DÉDUIT de la source ; à tester]

### 6.3 Fonctions dangereuses ou piégeuses

- `void sfPacket_readString(sfPacket* packet, char* string);` et `sfPacket_readWideString(…, wchar_t*)` : aucune
  taille de buffer ; la fonction écrit toute la chaîne reçue (`*packet >> string`, `src/CSFML/Network/Packet.cpp`
  l.187-198). Risque de **débordement de buffer** si le pair envoie une chaîne longue. [CONFIRMÉ]
- `void sfIpAddress_toString(sfIpAddress address, char* string);` : `strcpy` depuis `address.address`
  (`char[16]` en 3.0.0) ; il faut un buffer d'au moins 16 octets (46 sur `master`). Taille non documentée.
  [CONFIRMÉ source ; taille DÉDUITE de la struct]
- `sfIpAddress_fromString` résout aussi les noms d'hôte en 3.0.0 (appelle `sf::IpAddress::resolve`,
  `src/CSFML/Network/IpAddress.cpp` l.67-71) : peut bloquer (requête DNS). Sur `master`, elle ne résout plus
  les noms (voir §7). [CONFIRMÉ]
- `sfFtpListingResponse_getName(…, size_t index)` et `sfVertexArray_getVertex` : pas de contrôle d'indice. [CONFIRMÉ
  pour VertexArray (header) ; DÉDUIT pour FTP]
- `sfUdpSocket_send`/`sendPacket` : la taille ne doit pas dépasser `sfUdpSocket_maxDatagramSize()`
  (`Network/UdpSocket.h`). [CONFIRMÉ]
- `sfMusic_createFrom*` : la liste de formats du header (ogg, wav, flac, mp3, aiff, au, raw, paf, svx…) date de
  libsndfile. SFML 3.0.2 n'enregistre que 4 lecteurs : FLAC, MP3, OGG, WAV
  (`src/SFML/Audio/SoundFileFactory.cpp` l.29-32 @ SFML 3.0.2). [CONFIRMÉ ; un `.aiff` devrait échouer → à tester]

### 6.4 Fonctions sans équivalent direct en C++ (propres au binding C)

[DÉDUIT : comparaison avec l'API C++ de SFML 3, pas de source qui liste ces différences]

- Mémoire : `sfFree` (Alloc.h), tout `sfBuffer_*` (Buffer.h) ; tous les `*_destroy` et `*_copy`.
- Héritage remplacé par duplication : chaque drawable a ses propres `setPosition/Rotation/Scale/Origin`,
  `move/rotate/scale`, `getTransform/getInverseTransform` (Sprite, Text, Shape, CircleShape, ConvexShape,
  RectangleShape, Transformable). `sfTransformable` existe aussi comme objet autonome.
- `draw(const Drawable&)` remplacé par une fonction par type : `sfRenderWindow_drawSprite`, `_drawText`,
  `_drawShape`, `_drawCircleShape`, `_drawConvexShape`, `_drawRectangleShape`, `_drawVertexArray`,
  `_drawVertexBuffer`, `_drawVertexBufferRange`, `_drawPrimitives` (et la même série pour `sfRenderTexture`).
- Fonctions du module Window dupliquées pour les types Graphics, **déclarées dans `Graphics/RenderWindow.h`** :
  `sfMouse_getPositionRenderWindow`, `sfMouse_setPositionRenderWindow`, `sfTouch_getPositionRenderWindow`.
  Et pour `sfWindowBase` : `sfMouse_getPositionWindowBase`, `sfMouse_setPositionWindowBase`,
  `sfTouch_getPositionWindowBase`. `sfTexture_updateFromWindow` / `sfTexture_updateFromRenderWindow`.
- Classes dérivables remplacées par des callbacks : `sfShape_create`, `sfSoundStream_create`,
  `sfSoundRecorder_create`, `sfInputStream`.
- Opérateurs remplacés par des fonctions : `sfColor_add/subtract/modulate`, `sfTransform_combine/equal`,
  `sfPacket_read*/write*` (au lieu de `>>`/`<<`), `sfFloatRect_contains/intersects`.
- Surcharges éclatées : `sfShader_set{Float,Vec2,Vec3,Vec4,Color,Int,Ivec2,…,Mat4}Uniform` et `…UniformArray` ;
  `sfShader_setCurrentTextureUniform` remplace `sf::Shader::CurrentTexture` ; `sfTexture_createSrgb*`.
- Chaînes : variantes `*Unicode*` (UTF-32) à côté des variantes `char*`.
- Constantes accessibles par fonction : `sfUdpSocket_maxDatagramSize()`, `sfUdpSocket_anyPort()`,
  `sfTcpListener_anyPort()`.

### 6.5 Fonctions spécifiques OpenGL / Vulkan

- OpenGL (une trentaine de fonctions) : tout `Window/Context.h` (`sfContext_create`, `_destroy`,
  `_isExtensionAvailable`, `_setActive`, `_getFunction`, `_getSettings`, `_getActiveContextId`) ;
  `sfWindow_setActive`, `sfRenderWindow_setActive`, `sfRenderTexture_setActive` ;
  `sfRenderWindow_pushGLStates/popGLStates/resetGLStates` et leurs équivalents `sfRenderTexture_*` ;
  `sfTexture_bind`, `sfTexture_getNativeHandle`, `sfTexture_getMaximumSize`, `sfTexture_generateMipmap`,
  `sfRenderTexture_generateMipmap`, `sfRenderTexture_getMaximumAntiAliasingLevel` ; `sfShader_bind`,
  `sfShader_getNativeHandle`, `sfShader_isAvailable`, `sfShader_isGeometryAvailable` ;
  `sfVertexBuffer_bind`, `sfVertexBuffer_getNativeHandle`, `sfVertexBuffer_isAvailable` ; les `getSettings`
  (`sfContextSettings`). Header d'aide : `CSFML/OpenGL.h`. [CONFIRMÉ, inventaire]
- Vulkan (6 fonctions) : `sfVulkan_isAvailable`, `sfVulkan_getFunction`,
  `sfVulkan_getGraphicsRequiredInstanceExtensions`, `sfWindowBase_createVulkanSurface`,
  `sfWindow_createVulkanSurface`, `sfRenderWindow_createVulkanSurface`. Types `VkInstance`, `VkSurfaceKHR`,
  `VkAllocationCallbacks` redéclarés dans `Window/Vulkan.h`. [CONFIRMÉ]
- `sfWindowBase` est la fenêtre **sans** contexte OpenGL (pour Vulkan ou un rendu externe) ; elle n'a ni
  `display`, ni `setActive`, ni `setFramerateLimit`, ni `setVerticalSyncEnabled`. [CONFIRMÉ, liste des 27 fonctions
  de `Window/WindowBase.h`]

### 6.6 Deprecated

- **Aucune** fonction `CSFML_DEPRECATED` en 3.0.0 (`grep` : seule la définition dans `Config.h`). La release 3.0.0
  dit avoir « removed deprecated interfaces » (https://github.com/SFML/CSFML/releases/tag/3.0.0). [CONFIRMÉ]
- Sur `master`, 40 fonctions sont marquées `CSFML_DEPRECATED` (liste au §7). Elles existent toutes en 3.0.0 sans
  marque. Une doc « 3.0.0 » peut signaler qu'elles seront dépréciées en 3.1. [CONFIRMÉ]

### 6.7 Défauts de la documentation officielle (headers et page d'accueil, tag 3.0.0)

| Endroit | Défaut | Statut |
|---|---|---|
| `doc/mainpage.hpp` (exemple « Short example ») | `sfRenderWindow_create(mode, "SFML window", sfResize \| sfClose, NULL)` : 4 arguments au lieu de 5 (manque `sfWindowState`) ; `sfSprite_create()` sans texture ; `sfText_create()` sans police. Corrigé sur `master` pour `sfText` seulement (commit `9a2a909`). | [CONFIRMÉ lecture ; erreurs de compilation à constater] |
| `Window/Window.h`, `Window/WindowBase.h` | « If \a style contains sfFullscreen » : `sfFullscreen` est maintenant un `sfWindowState`, pas un style ; « The fourth parameter » désigne en fait le 5e (`settings`). | [CONFIRMÉ] |
| `Graphics/Shader.h` (`sfShader_setTextureUniform`) | exemple `sfTexture texture; … &texture` (impossible : type opaque) et `sf::Shader::CurrentTexture` (C++). Corrigé sur `master` (commit `cdae9a2`). | [CONFIRMÉ] |
| `Window/Event.h` | `sfEvtMouseMovedRaw` documenté « data in event.mouseMove » au lieu de `mouseMoveRaw`. | [CONFIRMÉ] |
| `Audio/Music.h` | liste de formats héritée de libsndfile (voir 6.3). | [CONFIRMÉ] |
| `Graphics/Font.h` | « Create a new image font a file in memory » (copier-coller). | [CONFIRMÉ] |
| `Graphics/Text.h` (`sfText_setFont`) | « refers to a texture » au lieu de « font ». | [CONFIRMÉ] |
| `Audio/SoundStream.h` | `onSeek` « can't be NULL » alors que la source le tolère (`if (mySeekCallback)`). | [CONFIRMÉ] |
| `System/InputStream.h` | ne dit pas que les callbacks doivent renvoyer -1 en cas d'erreur. | [CONFIRMÉ] |
| `Graphics/Sprite.h`, `Text.h`, `Sound.h` | ne disent pas que `create` exige un pointeur non NULL (seul l'`assert` le montre). | [CONFIRMÉ] |
| `Network/Http.h` | `sfHttpRangeNotSatisfiable = 407` (le vrai code HTTP est 416) ; valeur héritée de SFML 3.0.2 (`Http.hpp` l.221). | [CONFIRMÉ] |



## 7. Ce qui diffère sur `master` (non publié)

Source : `git diff 3.0.0 master -- include/` sur le clone (master = 20 commits après 3.0.0, dernier commit
2026-11-01 d'après le brief commun), puis la même extraction automatique sur les headers de `master`.
Ces changements **ne sont dans aucune release** au 2026-11-07 : la doc doit les présenter comme « à venir »
(probablement CSFML 3.1). [CONFIRMÉ pour le contenu ; numéro de version DÉDUIT de `CSFML_VERSION_MINOR 1` dans
`Config.h` sur master et de `find_package(SFML 3.1 …)` dans `CMakeLists.txt` sur master]

Résumé :

- Dépendance : `master` exige SFML **3.1** (`find_package(SFML 3.1 …)`, `CMakeLists.txt` l.69), 3.0.0 exige SFML
  3.0 (`find_package(SFML 3.0 …)`). [CONFIRMÉ]
- Nouveaux headers : `Audio/PlaybackDevice.h`, `Network/Dns.h`, `Network/Sftp.h`,
  `System/TimeoutWithPredicate.h`, `System/Version.h` (commits `c64cd68`, `1be2a3d`, `739b120`, `84d65e3`,
  `50f437e`). [CONFIRMÉ]
- FTP déprécié en bloc au profit de `sfSftp` ; `sfTouch_*` dépréciés au profit des événements tactiles ;
  `sfText_findCharacterPos` déprécié (nouvelle API de mise en forme : `sfText_getShapedGlyphs`, alignement,
  orientation) ; `sfIpAddress_resolve` ajouté **et** déprécié d'emblée (remplacé par `sfDns_resolve`). [CONFIRMÉ]
- Changement de comportement : sur `master`, `sfIpAddress_fromString` ne résout plus les noms (« Network names are
  not resolved, use sfDns_resolve or sfIpAddress_resolve for that », `Network/IpAddress.h` @ master). [CONFIRMÉ]
- Ruptures d'ABI : `sfIpAddress.address` passe de `char[16]` à `char[46]` (IPv6) ; `sfFontInfo` gagne `id`,
  `hasKerning`, `hasVerticalMetrics` ; 7 fonctions passent de `void` à `bool`. Un programme compilé contre 3.0.0
  ne doit pas être lié à une lib `master` sans recompilation. [CONFIRMÉ pour les types ; conséquence DÉDUITE]
- Nouvelle règle de libération : `sfDns_resolve` renvoie un tableau `sfIpAddress*` qui « must be freed with
  sfFree » (`Network/Dns.h` l.66 @ master). [CONFIRMÉ]
- Doc : formats d'image `pnm` et `qoi` ajoutés aux commentaires de `Image.h` ; exemples C++ retirés de
  `Shader.h`. [CONFIRMÉ]


Bilan chiffré (extraction automatique sur `master`) : 1034 fonctions sur master contre 926 en 3.0.0 ; 108 ajoutées, 0 supprimées, 7 dont la signature change, 40 marquées `CSFML_DEPRECATED`.

### Signatures modifiées (3.0.0 → master)

```c
/* 3.0.0  */ void sfHttp_setHost(sfHttp* http, const char* host, unsigned short port);
/* master */ bool sfHttp_setHost(sfHttp* http, const char* host, unsigned short port);
/* 3.0.0  */ void sfSocketSelector_addTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
/* master */ bool sfSocketSelector_addTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
/* 3.0.0  */ void sfSocketSelector_addTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
/* master */ bool sfSocketSelector_addTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
/* 3.0.0  */ void sfSocketSelector_addUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
/* master */ bool sfSocketSelector_addUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
/* 3.0.0  */ void sfSocketSelector_removeTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
/* master */ bool sfSocketSelector_removeTcpListener(sfSocketSelector* selector, sfTcpListener* socket);
/* 3.0.0  */ void sfSocketSelector_removeTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
/* master */ bool sfSocketSelector_removeTcpSocket(sfSocketSelector* selector, sfTcpSocket* socket);
/* 3.0.0  */ void sfSocketSelector_removeUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
/* master */ bool sfSocketSelector_removeUdpSocket(sfSocketSelector* selector, sfUdpSocket* socket);
```

### Fonctions marquées `CSFML_DEPRECATED` sur master (40)

- `include/CSFML/Window/Touch.h` : `sfTouch_isDown`, `sfTouch_getPosition`, `sfTouch_getPositionWindowBase`
- `include/CSFML/Graphics/RenderWindow.h` : `sfTouch_getPositionRenderWindow`
- `include/CSFML/Graphics/Text.h` : `sfText_findCharacterPos`
- `include/CSFML/Network/Ftp.h` : `sfFtpListingResponse_destroy`, `sfFtpListingResponse_isOk`, `sfFtpListingResponse_getStatus`, `sfFtpListingResponse_getMessage`, `sfFtpListingResponse_getCount`, `sfFtpListingResponse_getName`, `sfFtpDirectoryResponse_destroy`, `sfFtpDirectoryResponse_isOk`, `sfFtpDirectoryResponse_getStatus`, `sfFtpDirectoryResponse_getMessage`, `sfFtpDirectoryResponse_getDirectory`, `sfFtpDirectoryResponse_getDirectoryUnicode`, `sfFtpResponse_destroy`, `sfFtpResponse_isOk`, `sfFtpResponse_getStatus`, `sfFtpResponse_getMessage`, `sfFtp_create`, `sfFtp_destroy`, `sfFtp_connect`, `sfFtp_loginAnonymous`, `sfFtp_login`, `sfFtp_disconnect`, `sfFtp_keepAlive`, `sfFtp_getWorkingDirectory`, `sfFtp_getDirectoryListing`, `sfFtp_changeDirectory`, `sfFtp_parentDirectory`, `sfFtp_createDirectory`, `sfFtp_deleteDirectory`, `sfFtp_renameFile`, `sfFtp_deleteFile`, `sfFtp_download`, `sfFtp_upload`, `sfFtp_sendCommand`
- `include/CSFML/Network/IpAddress.h` : `sfIpAddress_resolve`

### Fonctions ajoutées sur master (108)

`include/CSFML/System/Version.h` (1) :

```c
sfVersion sfGetVersion(void);
```

`include/CSFML/Graphics/CircleShape.h` (2) :

```c
void sfCircleShape_setMiterLimit(sfCircleShape* shape, float miterLimit);
float sfCircleShape_getMiterLimit(const sfCircleShape* shape);
```

`include/CSFML/Graphics/ConvexShape.h` (2) :

```c
void sfConvexShape_setMiterLimit(sfConvexShape* shape, float miterLimit);
float sfConvexShape_getMiterLimit(const sfConvexShape* shape);
```

`include/CSFML/Graphics/Font.h` (3) :

```c
sfGlyph sfFont_getGlyphById(const sfFont* font, uint32_t id, unsigned int characterSize, bool bold, float outlineThickness);
float sfFont_getAscent(const sfFont* font, unsigned int characterSize);
float sfFont_getDescent(const sfFont* font, unsigned int characterSize);
```

`include/CSFML/Graphics/RectangleShape.h` (2) :

```c
void sfRectangleShape_setMiterLimit(sfRectangleShape* shape, float miterLimit);
float sfRectangleShape_getMiterLimit(const sfRectangleShape* shape);
```

`include/CSFML/Graphics/Shape.h` (2) :

```c
void sfShape_setMiterLimit(sfShape* shape, float miterLimit);
float sfShape_getMiterLimit(const sfShape* shape);
```

`include/CSFML/Graphics/Text.h` (10) :

```c
void sfText_setLineAlignment(sfText* text, sfTextLineAlignment lineAlignment);
void sfText_setTextOrientation(sfText* text, sfTextOrientation textOrientation);
sfTextLineAlignment sfText_getLineAlignment(const sfText* text);
sfTextOrientation sfText_getTextOrientation(const sfText* text);
const sfShapedGlyph* sfText_getShapedGlyphs(const sfText* text, size_t* count);
sfTextClusterGrouping sfText_getClusterGrouping(const sfText* text);
void sfText_setClusterGrouping(sfText* text, sfTextClusterGrouping clusterGrouping);
void sfText_setGlyphPreProcessor(sfText* text, sfGlyphPreProcessor glyphPreProcessor, void* userData);
sfVertex* sfText_getVertexData(const sfText* text, size_t* count);
sfVertex* sfText_getOutlineVertexData(const sfText* text, size_t* count);
```

`include/CSFML/Audio/PlaybackDevice.h` (9) :

```c
const char* const* sfPlaybackDevice_getAvailableDevices(size_t* count);
const char* sfPlaybackDevice_getDefaultDevice(void);
bool sfPlaybackDevice_setDevice(const char* name);
bool sfPlaybackDevice_setDeviceToDefault(void);
bool sfPlaybackDevice_setDeviceToNull(void);
const char* sfPlaybackDevice_getDevice(void);
uint32_t sfPlaybackDevice_getDeviceSampleRate(void);
bool sfPlaybackDevice_isDefaultDevice(void);
void sfPlaybackDevice_setNotificationCallback(sfPlaybackDeviceNotificationCallback callback, void* userData);
```

`include/CSFML/Network/Dns.h` (19) :

```c
/* --- sfDns --- */
sfIpAddress* sfDns_resolve(const char* hostname, const sfIpAddress* servers, size_t serverCount, sfTime timeout, size_t* count);
sfDnsNsRecords* sfDns_queryNs(const char* hostname, const sfIpAddress* servers, size_t serverCount, sfTime timeout);
sfDnsMxRecords* sfDns_queryMx(const char* hostname, const sfIpAddress* servers, size_t serverCount, sfTime timeout);
sfDnsSrvRecords* sfDns_querySrv(const char* hostname, const sfIpAddress* servers, size_t serverCount, sfTime timeout);
sfDnsTxtRecords* sfDns_queryTxt(const char* hostname, const sfIpAddress* servers, size_t serverCount, sfTime timeout);
sfIpAddress sfDns_getPublicAddress(sfTime timeout, sfIpAddressType type);

/* --- sfDnsNsRecords --- */
void sfDnsNsRecords_destroy(const sfDnsNsRecords* records);
size_t sfDnsNsRecords_getCount(const sfDnsNsRecords* records);
const char* sfDnsNsRecords_getRecord(const sfDnsNsRecords* records, size_t index);

/* --- sfDnsMxRecords --- */
void sfDnsMxRecords_destroy(const sfDnsMxRecords* records);
size_t sfDnsMxRecords_getCount(const sfDnsMxRecords* records);
sfDnsMxRecord sfDnsMxRecords_getRecord(const sfDnsMxRecords* records, size_t index);

/* --- sfDnsSrvRecords --- */
void sfDnsSrvRecords_destroy(const sfDnsSrvRecords* records);
size_t sfDnsSrvRecords_getCount(const sfDnsSrvRecords* records);
sfDnsSrvRecord sfDnsSrvRecords_getRecord(const sfDnsSrvRecords* records, size_t index);

/* --- sfDnsTxtRecords --- */
void sfDnsTxtRecords_destroy(const sfDnsTxtRecords* records);
size_t sfDnsTxtRecords_getCount(const sfDnsTxtRecords* records);
size_t sfDnsTxtRecords_getStringCount(const sfDnsTxtRecords* records, size_t index);
const char* sfDnsTxtRecords_getString(const sfDnsTxtRecords* records, size_t index, size_t stringIndex);
```

`include/CSFML/Network/Http.h` (2) :

```c
bool sfHttp_setHostWithAddressType(sfHttp* http, const char* host, unsigned short port, const sfIpAddressType* addressType);
sfHttpResponse* sfHttp_sendRequestWithVerification(const sfHttp* http, const sfHttpRequest* request, sfTime timeout, bool verifyServer);
```

`include/CSFML/Network/IpAddress.h` (8) :

```c
sfIpAddress sfIpAddress_resolve(const char* address);
sfIpAddress sfIpAddress_fromV6Bytes(const uint8_t bytes[16]);
bool sfIpAddress_toV6Bytes(sfIpAddress address, uint8_t bytes[16]);
sfIpAddressType sfIpAddress_getType(sfIpAddress address);
bool sfIpAddress_isV4(sfIpAddress address);
bool sfIpAddress_isV6(sfIpAddress address);
sfIpAddress sfIpAddress_getLocalAddressOfType(sfIpAddressType type);
sfIpAddress sfIpAddress_getPublicAddressOfType(sfTime timeout, const sfIpAddressType* type, bool secure);
```

`include/CSFML/Network/Sftp.h` (37) :

```c
/* --- sfSftpResult --- */
void sfSftpResult_destroy(const sfSftpResult* result);
bool sfSftpResult_isOk(const sfSftpResult* result);
sfSftpResultValue sfSftpResult_getValue(const sfSftpResult* result);
const char* sfSftpResult_getMessage(const sfSftpResult* result);

/* --- sfSftpPathResult --- */
void sfSftpPathResult_destroy(const sfSftpPathResult* result);
bool sfSftpPathResult_isOk(const sfSftpPathResult* result);
sfSftpResultValue sfSftpPathResult_getValue(const sfSftpPathResult* result);
const char* sfSftpPathResult_getMessage(const sfSftpPathResult* result);
const char* sfSftpPathResult_getPath(const sfSftpPathResult* result);

/* --- sfSftpAttributesResult --- */
void sfSftpAttributesResult_destroy(const sfSftpAttributesResult* result);
bool sfSftpAttributesResult_isOk(const sfSftpAttributesResult* result);
sfSftpResultValue sfSftpAttributesResult_getValue(const sfSftpAttributesResult* result);
const char* sfSftpAttributesResult_getMessage(const sfSftpAttributesResult* result);
sfSftpAttributes sfSftpAttributesResult_getAttributes(const sfSftpAttributesResult* result);

/* --- sfSftpListingResult --- */
void sfSftpListingResult_destroy(const sfSftpListingResult* result);
bool sfSftpListingResult_isOk(const sfSftpListingResult* result);
sfSftpResultValue sfSftpListingResult_getValue(const sfSftpListingResult* result);
const char* sfSftpListingResult_getMessage(const sfSftpListingResult* result);
size_t sfSftpListingResult_getCount(const sfSftpListingResult* result);
sfSftpAttributes sfSftpListingResult_getAttributes(const sfSftpListingResult* result, size_t index);

/* --- sfSftp --- */
sfSftp* sfSftp_create(void);
void sfSftp_destroy(const sfSftp* sftp);
sfSftpResult* sfSftp_connect(sfSftp* sftp, sfIpAddress server, unsigned short port, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_disconnect(sfSftp* sftp, sfTimeoutWithPredicate timeout);
bool sfSftp_getSessionInfo(const sfSftp* sftp, sfSftpSessionInfo* info);
sfSftpResult* sfSftp_login(sfSftp* sftp, const char* name, const char* password, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_loginWithKey(sfSftp* sftp, const char* name, const char* publicKeyData, size_t publicKeyLength, const char* privateKeyData, size_t privateKeyLength, const char* privateKeyPassphrase, sfTimeoutWithPredicate timeout);
sfSftpPathResult* sfSftp_resolvePath(sfSftp* sftp, const char* path, sfTimeoutWithPredicate timeout);
sfSftpPathResult* sfSftp_getWorkingDirectory(sfSftp* sftp, sfTimeoutWithPredicate timeout);
sfSftpAttributesResult* sfSftp_getAttributes(sfSftp* sftp, const char* path, bool followLinks, sfTimeoutWithPredicate timeout);
sfSftpListingResult* sfSftp_getDirectoryListing(sfSftp* sftp, const char* path, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_createDirectory(sfSftp* sftp, const char* path, uint32_t permissions, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_deleteDirectory(sfSftp* sftp, const char* path, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_rename(sfSftp* sftp, const char* oldPath, const char* newPath, bool overwrite, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_deleteFile(sfSftp* sftp, const char* path, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_download(sfSftp* sftp, const char* remotePath, sfSftpDownloadCallback callback, void* userData, uint64_t offset, sfTimeoutWithPredicate timeout);
sfSftpResult* sfSftp_upload(sfSftp* sftp, const char* remotePath, sfSftpUploadCallback callback, void* userData, uint32_t permissions, bool truncate, bool append, uint64_t offset, sfTimeoutWithPredicate timeout);
```

`include/CSFML/Network/SocketSelector.h` (7) :

```c
bool sfSocketSelector_addTcpListenerWithReadiness(sfSocketSelector* selector, sfTcpListener* socket, uint32_t readiness, sfSocketSelectorCallback callback, void* userData);
bool sfSocketSelector_addTcpSocketWithReadiness(sfSocketSelector* selector, sfTcpSocket* socket, uint32_t readiness, sfSocketSelectorCallback callback, void* userData);
bool sfSocketSelector_addUdpSocketWithReadiness(sfSocketSelector* selector, sfUdpSocket* socket, uint32_t readiness, sfSocketSelectorCallback callback, void* userData);
bool sfSocketSelector_isTcpListenerReadyFor(const sfSocketSelector* selector, sfTcpListener* socket, uint32_t readiness);
bool sfSocketSelector_isTcpSocketReadyFor(const sfSocketSelector* selector, sfTcpSocket* socket, uint32_t readiness);
bool sfSocketSelector_isUdpSocketReadyFor(const sfSocketSelector* selector, sfUdpSocket* socket, uint32_t readiness);
void sfSocketSelector_dispatchReadyCallbacks(sfSocketSelector* selector);
```

`include/CSFML/Network/TcpSocket.h` (4) :

```c
sfTlsStatus sfTcpSocket_setupTlsClient(sfTcpSocket* socket, const char* hostname, bool verifyPeer);
sfTlsStatus sfTcpSocket_setupTlsClientWithCertificate(sfTcpSocket* socket, const char* hostname, const void* certificateChainData, size_t certificateChainSize);
sfTlsStatus sfTcpSocket_setupTlsServer(sfTcpSocket* socket, const void* certificateChainData, size_t certificateChainSize, const void* privateKeyData, size_t privateKeySize, const void* privateKeyPasswordData, size_t privateKeyPasswordSize);
const char* sfTcpSocket_getCurrentCiphersuiteName(const sfTcpSocket* socket);
```

### Types ajoutés sur master (33)

| Nom | Nature | Header (master) |
|---|---|---|
| `sfTimeoutPredicate` | pointeur de fonction (callback) | `include/CSFML/System/TimeoutWithPredicate.h` |
| `sfTimeoutWithPredicate` | struct (champs publics) | `include/CSFML/System/TimeoutWithPredicate.h` |
| `sfVersion` | struct (champs publics) | `include/CSFML/System/Version.h` |
| `sfTextLineAlignment` | enum | `include/CSFML/Graphics/Text.h` |
| `sfTextClusterGrouping` | enum | `include/CSFML/Graphics/Text.h` |
| `sfTextDirection` | enum | `include/CSFML/Graphics/Text.h` |
| `sfTextOrientation` | enum | `include/CSFML/Graphics/Text.h` |
| `sfShapedGlyph` | struct (champs publics) | `include/CSFML/Graphics/Text.h` |
| `sfGlyphPreProcessor` | pointeur de fonction (callback) | `include/CSFML/Graphics/Text.h` |
| `sfPlaybackDeviceNotification` | enum | `include/CSFML/Audio/PlaybackDevice.h` |
| `sfPlaybackDeviceNotificationCallback` | pointeur de fonction (callback) | `include/CSFML/Audio/PlaybackDevice.h` |
| `sfDnsMxRecord` | struct (champs publics) | `include/CSFML/Network/Dns.h` |
| `sfDnsSrvRecord` | struct (champs publics) | `include/CSFML/Network/Dns.h` |
| `sfIpAddressType` | enum | `include/CSFML/Network/IpAddress.h` |
| `sfSftpResultValue` | enum | `include/CSFML/Network/Sftp.h` |
| `sfSftpFileType` | enum | `include/CSFML/Network/Sftp.h` |
| `sfSftpAttributes` | struct (champs publics) | `include/CSFML/Network/Sftp.h` |
| `sfSftpHostKeyType` | enum | `include/CSFML/Network/Sftp.h` |
| `sfSftpSessionInfo` | struct (champs publics) | `include/CSFML/Network/Sftp.h` |
| `sfSftpDownloadCallback` | pointeur de fonction (callback) | `include/CSFML/Network/Sftp.h` |
| `sfSftpUploadCallback` | pointeur de fonction (callback) | `include/CSFML/Network/Sftp.h` |
| `sfSocketSelectorReadiness` | enum | `include/CSFML/Network/SocketSelector.h` |
| `sfSocketSelectorCallback` | pointeur de fonction (callback) | `include/CSFML/Network/SocketSelector.h` |
| `sfTlsStatus` | enum | `include/CSFML/Network/TcpSocket.h` |
| `sfDnsMxRecords` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfDnsNsRecords` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfDnsSrvRecords` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfDnsTxtRecords` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSftpAttributesResult` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSftpListingResult` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSftpPathResult` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSftpResult` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |
| `sfSftp` | opaque (struct incomplète, manipulée par pointeur) | `include/CSFML/Network/Types.h` |

### Types existants modifiés sur master (2)

```c
/* 3.0.0  */ typedef struct { const char* family; } sfFontInfo;
/* master */ typedef struct { uint64_t id; const char* family; bool hasKerning; bool hasVerticalMetrics; } sfFontInfo;
/* 3.0.0  */ typedef struct { char address[16]; } sfIpAddress;
/* master */ typedef struct { char address[46]; } sfIpAddress;
```

### Constantes globales ajoutées sur master (5)

```c
CSFML_NETWORK_API const sfIpAddress sfIpAddress_AnyV4;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_LocalHostV4;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_BroadcastV4;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_AnyV6;   /* CSFML/Network/IpAddress.h */
CSFML_NETWORK_API const sfIpAddress sfIpAddress_LocalHostV6;   /* CSFML/Network/IpAddress.h */
```


## Décisions prises

1. **Version de référence = tag git `3.0.0`** (dernière release stable, identique au paquet Ubuntu installé).
   `master` est traité à part (§7) et marqué « non publié ».
2. **Format des signatures** : macro d'export omise, espaces normalisés, une ligne par fonction. Le reste est
   identique au header (vérifié automatiquement, 0 écart). Les noms de paramètres sont gardés (utiles pour la doc).
3. **Regroupement** : par module, puis par header, puis par préfixe de type quand un header en contient plusieurs
   (ex. `Http.h` : `sfHttpRequest`, `sfHttpResponse`, `sfHttp`). Les fonctions `sfMouse_*RenderWindow` et
   `sfTouch_getPositionRenderWindow` restent dans Graphics, car c'est là qu'elles sont déclarées et exportées.
4. **Valeurs d'enum** listées en entier (y compris `sfKeyCode` et `sfScancode`) : la doc en aura besoin et le coût
   est faible.
5. **Ownership** : quand le header CSFML ne dit rien, j'ai lu l'implémentation `src/` du même tag (lecture seule)
   et la doc C++ de SFML 3.0.2 (version installée localement). Ces affirmations portent la mention « source » ou
   « C++ » et la plupart sont reprises dans la liste « À tester ».
6. **Pas de compilation** (règle PHASE 1) : tout ce qui demande d'exécuter du code est dans
   `a-tester-03.md`.
7. Les compteurs « types nommés » incluent les callbacks, les alias et les 3 types Vulkan ; `sfChar32`
   (`Config.h`) est compté à part (134 au total).

## Points [INCERTAIN]

1. `sfX_destroy(NULL)` est sans effet : vrai d'après la source (`delete`), mais non documenté. → test.
2. Comportement du paquet Ubuntu quand on passe `NULL` à une fonction protégée par `assert` (lib compilée avec
   ou sans `NDEBUG` ?). → test.
3. Affichage d'une chaîne UTF-8 (« é ») via `sfText_setString` / titre de fenêtre : probablement faux ; vérifier
   et documenter la méthode `sfText_setUnicodeString(U"…")`. → test (important pour une doc en français).
4. Durée de validité réelle des pointeurs empruntés (clipboard, joystick, `sfText_getString`) : déduite de la
   source, à confirmer par un test qui appelle deux fois et compare.
5. `sfSoundStream_create` avec `channelMapData = NULL, channelMapSize = 0` : accepté ou erreur ? (SFML 3 attend une
   « channel map ».)
6. Le buffer passé à `sfSoundBuffer_createFromMemory` / `sfTexture_createFromMemory` / `sfImage_createFromMemory`
   peut-il être libéré juste après ? (oui d'après SFML C++, non vérifié en C.)
7. `sfRenderWindow_create` avec une taille impossible : la fenêtre est-elle non NULL mais fermée
   (`sfRenderWindow_isOpen` = false) ?
8. `SFML_DEFINE_DISCRETE_GPU_PREFERENCE` toujours vide (macro qui teste `SFML_SYSTEM_WINDOWS`) : à vérifier sous
   Windows (hors périmètre Linux de la phase 2).
9. `sfMusic_createFromFile` sur un format listé par le header mais absent de SFML 3 (`.aiff`, `.au`) : NULL attendu.
10. `getRotation` après `setRotation(-90)` : 270 attendu. Déduit de la lecture : SFML 3.0.2 fait
    `m_rotation = angle.wrapUnsigned()` (`src/SFML/Graphics/Transformable.cpp` l.47) et CSFML renvoie
    `getRotation().asDegrees()` (`src/CSFML/Graphics/Sprite.cpp` l.109). Non exécuté.
11. La date de la release GitHub 3.0.0 (« 14 January », année absente sur la page) contre la date du tag
    (2026-01-13) : l'année 2026 est déduite.
