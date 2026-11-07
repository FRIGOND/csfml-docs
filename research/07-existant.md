# 07 — Ce qui existe déjà, ce qui manque, et l'angle de la future doc

> Mission 7 (docs existantes). Recherche du 2026-11-07. Sources primaires : clone de
> https://github.com/SFML/CSFML (tags 2.5 → 3.0.0, master `cdae9a2` du 2026-11-01), dépôt du site
> https://github.com/SFML/SFML-Website (dernier commit 2026-10-20), wiki https://github.com/SFML/SFML/wiki
> (dernier commit 2026-09-11), paquet local `libcsfml-doc 3.0.0-1`, archives officielles téléchargées
> depuis https://www.sfml-dev.org/files/. Les ressources non officielles sont marquées [source secondaire].

## En bref

- **Il n'existe aucune doc API CSFML en ligne officielle**, ni pour 2.6, ni pour 3.0. Les URLs
  `sfml-dev.org/documentation/csfml/…` répondent 404. Le site n'héberge que la doc C++. [CONFIRMÉ]
- **La page de téléchargement dit que la doc est « incluse dans les archives ». C'est faux** : les
  archives 3.0.0 (Windows 64 bits, macOS arm64), 2.6.1 et 2.5.1 (Windows 64 bits) ne contiennent
  aucun fichier HTML. Il ne reste que les paquets Linux (`libcsfml-doc`) ou la génération Doxygen à la main. [CONFIRMÉ]
- **La référence Doxygen est complète mais sèche** : les 916 fonctions des headers 3.0.0 ont un `\brief`,
  mais il n'y a que 12 blocs `\code` dans tous les headers, et une seule page hors référence (l'accueil). [CONFIRMÉ]
- **L'exemple de la page d'accueil Doxygen ne compile pas en 3.0** : trois appels sont restés en
  signature 2.x au tag 3.0.0. Le commit `9a2a909` du 2026-10-03 en corrige un sur master. Deux restent faux. [CONFIRMÉ par lecture des headers ; compilation à faire en phase 2]
- **Exemples officiels : un seul fichier** (`examples/example.c`, apparu en 2.6.0). Il est compilé en CI, et
  ses assets sont libres (logo et police dans le domaine public, son sous Unlicense). [CONFIRMÉ]
- **Aucun tutoriel CSFML officiel.** Le readme et la page de téléchargement renvoient vers les tutoriels
  C++. Les tutoriels SFML 3.x existent **seulement en anglais**. Les tutoriels 2.6 existent en français. [CONFIRMÉ]
- **Aucun guide de migration CSFML 2 → 3.** Le guide officiel 2 → 3 ne parle que de C++ et ne cite pas
  CSFML. Les notes de release 3.0.0 sont une liste de PR. [CONFIRMÉ]
- **L'existant non officiel date de l'ère 2.x (2017–2024)** : docs d'étudiants Epitech (EN), vidéos YouTube
  françaises (vers 2020–2023), un article Medium. Nous n'avons trouvé **aucun contenu tutoriel pour CSFML 3.0**. [DÉDUIT de nombreuses recherches]
- **Versions installées chez les lecteurs** : Ubuntu 22.04 = 2.5, Ubuntu 24.04 = 2.6.0, Debian 13 = 2.6.1,
  **Fedora 43 et rawhide = 2.6.1**, Ubuntu 26.04 et Arch = 3.0.0. Une grande partie du public est donc encore en 2.x. [CONFIRMÉ]
- **Licences** : le code CSFML et les commentaires Doxygen sont sous zlib/png. Les tutoriels de
  sfml-dev.org sont sous **CC BY 4.0** (fichier `LICENSE` du dépôt SFML-Website). On peut donc les adapter
  avec attribution. [CONFIRMÉ]

---

## 1. Documentation officielle

### 1.1 Référence API CSFML (Doxygen)

**En ligne : rien d'officiel.** [CONFIRMÉ, 2026-11-07]

| URL testée | Résultat |
|---|---|
| https://www.sfml-dev.org/documentation/ | Liste SFML C++ 1.6 → 3.1.0. Pas un mot sur CSFML. |
| https://www.sfml-dev.org/documentation/csfml/ | HTTP 404 |
| https://www.sfml-dev.org/documentation/csfml/3.0.0/ | HTTP 404 |
| https://sfml.github.io/CSFML/ | HTTP 404 (l'API GitHub indique `has_pages: false` pour SFML/CSFML) |

La page https://www.sfml-dev.org/download/csfml/ (source : `pages/download/csfml/index.en.md` du dépôt
SFML-Website) dit que la doc API, générée depuis le code, est « incluse dans les archives ».
**Vérification faite le 2026-11-07, en téléchargeant les archives et en listant leur contenu :** [CONFIRMÉ]

| Archive officielle | Taille | Contenu | Doc HTML ? |
|---|---|---|---|
| `CSFML-3.0.0-windows-64-bit.zip` | 342 Ko | `bin/`, `include/`, `lib/`, `license.md`, `readme.md` | **Non** (0 fichier .html, 0 .chm) |
| `CSFML-3.0.0-macOS-clang-arm64.tar.gz` | 180 Ko | `include/`, `lib/`, `license.md`, `readme.md` | **Non** |
| `CSFML-2.6.1-windows-64-bit.zip` | 1,74 Mo | `bin/`, `include/`, `lib/`, licence, readme | **Non** |
| `CSFML-2.5.1-windows-64-bit.zip` | 1,7 Mo | `bin/`, `include/`, `lib/`, licence, readme | **Non** |

Conclusion : en 2026, pour lire la doc CSFML il faut soit un paquet Linux, soit la générer avec Doxygen,
soit lire les headers. [DÉDUIT]

**Dans le dépôt** (tag 3.0.0) : le dossier `doc/` contient `doxyfile.in`, `mainpage.hpp`, `header.html.in`,
`footer.html` et des CSS. L'option CMake `CSFML_BUILD_DOC` (OFF par défaut) crée la cible `doc`
(source : `CMakeLists.txt` l. 36 et 73, `doc/CMakeLists.txt` @ master). Le job `docs` de
`.github/workflows/ci.yml` (master) **construit** le site Doxygen sur macOS, mais **ne le publie pas** :
pas d'étape de déploiement, pas de GitHub Pages. [CONFIRMÉ]

**Paquets de distribution (doc API hors ligne)** : [CONFIRMÉ, consulté le 2026-11-07]

| Distribution | Paquet doc | Version |
|---|---|---|
| Ubuntu 22.04 / 24.04 / 26.04 | `libcsfml-doc` | 2.5-1.1 / 2.6.0-3build1 / 3.0.0-1 ([packages.ubuntu.com](https://packages.ubuntu.com/search?keywords=libcsfml-doc&searchon=names&suite=all&section=all)) |
| Debian 12 / 13 / testing | `libcsfml-doc` | 2.5-1.1 / 2.6.1-1 / 3.0.0-1 ([packages.debian.org](https://packages.debian.org/search?keywords=libcsfml-doc&searchon=names&suite=all&section=all)) |
| Fedora 43 / rawhide | `CSFML-doc` | 2.6.1 (mdapi : `2.6.1-5.fc43`, `2.6.1-8.fc45`) ([mdapi](https://mdapi.fedoraproject.org/rawhide/pkg/CSFML)) |
| Alpine edge (testing) | `csfml-doc` | 2.5.2-r0 ([pkgs.alpinelinux.org](https://pkgs.alpinelinux.org/packages?name=csfml*&branch=edge)) |
| Arch (extra) | `csfml` | 1:3.0.0-1, mis à jour le 2026-04-18 ([archlinux.org](https://archlinux.org/packages/?q=csfml)). Présence de la doc dans le paquet : [INCERTAIN] |

**Paquet local (`libcsfml-doc 3.0.0-1`, mainteneur James Cowgill, changelog du 2026-01-17)** : [CONFIRMÉ]
- Piège : le HTML est installé dans **`/usr/share/doc/libcsfml-dev/html/`**, pas dans
  `/usr/share/doc/libcsfml-doc/` (qui ne contient que `changelog.Debian.gz` et `copyright`). Source :
  `dpkg -L libcsfml-doc` et `/usr/share/doc-base/libcsfml-doc.libcsfml-doc`.
- 402 fichiers, dont 296 pages HTML. Titre « CSFML 3.0.0 ». Généré par Doxygen 1.15.0.
- Une page par header (`Sprite_8h.html`, etc.), une par struct (`structsfVector2f.html`…), index des
  fonctions (`globals_func.html`), recherche JavaScript.

**Qualité de la référence 3.0.0** (mesures sur `/usr/include/CSFML`, paquet 3.0.0-1) :
- Complétude : **916 fonctions `CSFML_*_API`, toutes avec un `\brief`**. [CONFIRMÉ, script de comptage]
- Exemples : **12 blocs `\code` seulement**, dans 7 headers (VertexBuffer, Texture, Shader, RenderWindow,
  Transform, WindowBase, Window). [CONFIRMÉ]
- Pages conceptuelles : **aucune**. Le seul contenu hors référence est la page d'accueil (« Welcome » +
  « Short example »). [CONFIRMÉ, `search/pages_*.js`]
- Le commit `cdae9a2` (2026-11-01, master) « Replace C++ examples in shader texture uniform docs » montre
  que des exemples C++ traînaient encore dans la doc C. [CONFIRMÉ]
- Les descriptions sont courtes et reprennent souvent le texte C++. Elles ne disent presque rien sur
  « qui libère quoi » (ownership). [DÉDUIT d'un échantillon ; à mesurer en phase 2, cf. a-tester n°13]

**Erreur dans l'exemple de la page d'accueil** (`doc/mainpage.hpp`) : [CONFIRMÉ par comparaison avec les headers du même tag]

| Ligne de l'exemple | Header 3.0.0 | État au tag 3.0.0 | État sur master (2026-11-01) |
|---|---|---|---|
| `sfRenderWindow_create(mode, "SFML window", sfResize \| sfClose, NULL)` (4 arguments) | `sfRenderWindow* sfRenderWindow_create(sfVideoMode mode, const char* title, uint32_t style, sfWindowState state, const sfContextSettings* settings);` (`include/CSFML/Graphics/RenderWindow.h` l. 59–64) | Faux (manque `state`) | **Toujours faux** |
| `sprite = sfSprite_create();` | `sfSprite* sfSprite_create(const sfTexture* texture);` (`Graphics/Sprite.h` l. 48) | Faux | **Toujours faux** |
| `text = sfText_create();` puis `sfText_setFont(text, font);` | `sfText* sfText_create(const sfFont* font);` (`Graphics/Text.h` l. 62) | Faux | Corrigé par `9a2a909` (2026-10-03) |

Cette page est exactement celle que voit un débutant qui ouvre la doc locale. C'est une bonne
illustration du besoin d'**exemples compilés en CI**. [DÉDUIT]

### 1.2 Exemples officiels du dépôt CSFML

`git ls-tree` sur le clone : [CONFIRMÉ]

| Tag | `examples/` |
|---|---|
| 2.5, 2.5.2 | absent |
| 2.6.0, 2.6.1 | `CMakeLists.txt`, `example.c`, `doodle_pop.ogg`, `sfml_logo.png`, `tuffy.ttf` |
| 3.0.0-rc.1, 3.0.0, master | mêmes 5 fichiers (`example.c` = 2336 octets, identique de 3.0.0 à master) |

- Le dossier vient du commit `bf29112` (2023-12-19, « Add example program »).
- `example.c` @ 3.0.0 = la version **correcte** de l'exemple d'accueil : fenêtre + sprite + texte + musique,
  avec libération des ressources à chaque erreur. Il utilise `sfRenderWindow_create(mode, …, sfResize | sfClose, sfWindowed, NULL)`,
  `sfSprite_create(texture)`, `sfText_create(font)`.
- Il est **compilé en CI** : le preset `dev` (`CMakePresets.json`) active `CSFML_BUILD_EXAMPLES=ON` et
  `CSFML_WARNINGS_AS_ERRORS=ON`, et `ci.yml` utilise `cmake --preset dev`. Rien n'indique qu'il soit **exécuté**. [CONFIRMÉ / DÉDUIT]
- C'est **le seul** exemple C officiel. Le dossier `test/` contient des tests C++ (Catch2), qui ne sont pas des exemples
  lisibles par un étudiant en C. [CONFIRMÉ]
- Licences des assets (fichier `copyright` Debian, `/usr/share/doc/libcsfml-doc/copyright`) : logo SFML =
  domaine public, `tuffy.ttf` = domaine public, `doodle_pop.ogg` = Unlicense. **Réutilisables dans nos exemples.** [CONFIRMÉ, source secondaire Debian qui cite les sources amont]

### 1.3 Tutoriels SFML C++ (sfml-dev.org)

Le site est généré par MkDocs Material (sur « ProperDocs ») depuis `pages/` du dépôt SFML-Website
([README](https://github.com/SFML/SFML-Website/blob/master/README.md)). Fichiers comptés dans l'arbre git du 2026-11-07 : [CONFIRMÉ]

| Version | Fichiers | Langues | Remarques |
|---|---|---|---|
| 3.1 (https://www.sfml-dev.org/tutorials/3.1/) | 34 | **anglais seulement** (la page `/fr/tutorials/3.1/` a l'interface en français mais la liste en anglais) | ajoute Android, « migration/sfml-2 », « migration/sfml-3.0 » |
| 3.0 (https://www.sfml-dev.org/tutorials/3.0/) | 32 | anglais seulement | guide 2 → 3 sous `getting-started/migrate/` |
| 2.6 (https://www.sfml-dev.org/tutorials/2.6/) | 30 EN + 30 FR | **anglais et français** | contient encore « thread » (retiré en 3) |

**Ce qui se transpose en C (tutoriels 3.1 face aux headers CSFML 3.0.0)** — chaque équivalent C cité
existe dans `/usr/include/CSFML` (paquet 3.0.0-1). [CONFIRMÉ pour l'existence des fonctions ; DÉDUIT pour le jugement]

| Tutoriel 3.1 | Équivalent C | Transposable ? | Remarque |
|---|---|---|---|
| Migration 3.0 → 3.1 | — | Non | CSFML 3.1 pas encore sorti (master suit SFML 3.1.0, commit `48e07cb`) |
| Migration 2 → 3 | — | Partiel | Optional, variant, enums scoped, exceptions : pas en C. Vecteurs, états de fenêtre, constructeurs obligatoires (sprite ← texture, texte ← police), renommages : oui. **À réécrire pour C.** |
| CMake template | — | Partiel | CSFML 3.0.0 n'installe **pas** de `CSFMLConfig.cmake` (pas d'`install(EXPORT)` dans `CMakeLists.txt` / `cmake/Macros.cmake`), seulement des `.pc` |
| Visual Studio, Code::Blocks, Linux, Xcode | — | Partiel | Même logique, autres noms de libs (`csfml-graphics`…) |
| Android, iOS | — | Non / [INCERTAIN] | Pas de support mobile documenté côté CSFML |
| Build from source | — | Partiel | Il faut construire SFML puis CSFML (readme CSFML) |
| Time | `sfClock_create`, `sfTime` | Oui | |
| Angle | — | Non | Pas de type angle en C : `float` en degrés (`Sprite.h` l. 89) |
| User data streams | `sfInputStream` (struct de callbacks) | Oui | Héritage C++ → callbacks |
| Window | `sfRenderWindow_create`, `sfWindowState` | Oui | |
| Events | `sfEvent` (union, champ `type`) | Oui, **modèle différent** | Le tutoriel **2.6** (union + `type`) est plus proche du C que le tutoriel 3.x (`std::optional`, `getIf<T>`) |
| Inputs | `sfKeyboard_*`, `sfMouse_*`, `sfJoystick_*` | Oui | |
| OpenGL | `sfContext_create`, `<CSFML/OpenGL.h>` | Partiel | |
| Draw, Sprites, Shapes, Text, View | `sfRenderWindow_draw*`, `sfSprite`, `sfShape_create` (callbacks), `sfText`, `sfView` | Oui | |
| Transform | `sfTransform`, `sfTransformable` | Partiel | Les « entités custom » par héritage ne se transposent pas |
| Vertex arrays | `sfVertexArray_create` | Partiel | Idem (classe custom) |
| Shaders | `sfShader_createFromFile` | Oui | |
| Sounds, Recording, Spatialization, Streams | `sfSound`, `sfMusic`, `sfSoundBufferRecorder_create`, `sfSoundRecorder_create`, `sfListener_setPosition`, `sfSoundStream_create` | Oui | Héritage → callbacks |
| Sockets, HTTP, FTP | `sfTcpSocket`, `sfUdpSocket`, `sfHttp_create`, `sfFtp_create` | Oui | master ajoute `sfSftp` (commit `739b120`) |
| Packets | `sfPacket_writeInt32`… | Partiel | Surcharge d'opérateurs `<<` / `>>` : pas en C |

Guide de migration officiel 2 → 3 (https://www.sfml-dev.org/tutorials/3.1/migration/sfml-2/) : 21 sections,
toutes en C++. **Aucune mention de CSFML.** [CONFIRMÉ]

### 1.4 Wiki SFML (https://github.com/SFML/SFML/wiki)

Clone du wiki (`SFML.wiki.git`, dernier commit `561b104` du 2026-09-11, 142 pages) : [CONFIRMÉ]
- **Aucune page CSFML.** « csfml » n'apparaît que dans `Projects.md` (vieille liste de bindings qui pointe vers
  `download.php`), dans la checklist de release (`SD:-Release-Checklist.md`) et dans deux tutoriels SFML.Net.
- Les tutoriels du wiki sont en C++ (game loop, animations, views, etc.). Certains concepts sont réutilisables.
- Licence : la page `Home.md` dit que **le code source** du wiki est dans le domaine public, sauf mention
  contraire de l'auteur. Rien n'est dit sur le texte. [CONFIRMÉ]

### 1.5 Pages « CSFML » et « Bindings » de sfml-dev.org

- https://www.sfml-dev.org/download/csfml/ : le texte dit qu'il n'y a pas de tutoriel CSFML et qu'on peut
  adapter facilement les tutoriels C++. Il liste 3.0.0 → 2.0. Il ne donne aucune date. [CONFIRMÉ]
- https://www.sfml-dev.org/download/bindings/ : CSFML est listé « SFML 3.0 ». **Nouveau : `SSW_SFML3`**
  (https://github.com/unwanted666dkjf/SSW_SFML3), un binding C **non officiel** pour SFML 3.1/3.2. Créé le
  2026-07-05, dernier push le 2026-09-29, 0 étoile. Son API est différente (`sf_RenderWindow`, `sf_…_del`), il ne couvre pas
  le réseau, et son auteur dit lui-même que ce n'est pas un remplacement de CSFML. [CONFIRMÉ / source secondaire pour le contenu du README]
- Readme CSFML (tag 3.0.0, section « Learn ») : il dit qu'il n'existe pas de tutoriel CSFML et renvoie
  aux ressources C++, au wiki et au forum. [CONFIRMÉ]

### 1.6 Notes de version

- Release GitHub 3.0.0 publiée le **2026-01-14T09:12Z** (le tag git date du 2026-01-13), corps de 14 890
  caractères : une liste de PR classées (« Updates for SFML 3 », « General Build Code Improvements »…),
  **pas un guide de migration**. https://github.com/SFML/CSFML/releases/tag/3.0.0 [CONFIRMÉ]
- Le forum (https://en.sfml-dev.org/forums/) est protégé par Anubis (anti-bot) : pas lisible
  automatiquement. Nous n'avons trouvé aucun fil d'annonce CSFML 3.0. [INCERTAIN]

---

## 2. Docs non officielles

Dates « dernier push / commit » lues via l'API GitHub le 2026-11-07. Toutes ces ressources sont des [sources secondaires].

| Ressource | URL | Langue | Version CSFML | Dernière MAJ | Qualité | Points forts / faibles |
|---|---|---|---|---|---|---|
| EPITECH Documentation (EPITOOLS) | https://epitech-documentation.readthedocs.io/en/latest/tools_cheatsheet/csfml/ — repo `epitools2024/EPITECH-DOCUMENTATION` | EN | 2.x (`<SFML/Graphics.h>`, `sfRenderWindow_create` à 4 arguments) | page CSFML : 2021-06-24 ; repo : 2022-09-24 | Faible | + structure d'un jeu (init / boucle / free). − pas de date, pas de vérif de `NULL`, ton familier, affirme qu'il n'existe aucun tutoriel. Pas de licence. |
| « CSFML : Graphical Programming » (Epitech 2022) | https://epitech-2022-technical-documentation.readthedocs.io/en/latest/csfml.html — repo `jess-sys/22Docs` (**404 aujourd'hui**) | EN | 2.x (`{800, 600, 32}`, 4 arguments) | inconnue (≈ 2021–2022) | Faible | + tableaux de fonctions fenêtre / dessin. − se dit « bientôt complète », coquilles, points-virgules manquants. Source disparue. |
| QuickStart CSFML (Enichy) | https://enichy.github.io/QuikckStart_CSFML/ | EN | non précisée (lien vers la doc 2.5.1) | 2024-02-09 | Très faible | + plan raisonnable (débutant → jeu → dépannage). − la plupart des sections sont des plans vides (« Provide code examples… »), phrases coupées. Pas de licence. |
| Epidoc (marttop) | https://github.com/marttop/Epidoc | EN | 2.x (supposée) | 2021-07-16 | Faible | Docusaurus, jamais publié (pas de Pages). |
| csfml-docs-ru (Alex-xp) | https://github.com/Alex-xp/csfml-docs-ru | RU | 2.x (`Thread.h`, `Mutex.h`, supprimés en 3) | 2020-11-05 | Moyenne (traduction des headers) | Utile seulement comme modèle de « référence par header ». |
| Medium, « Introduction to CSFML graphical programming » (Sami H.) | https://medium.com/@sami.hamansourt/introduction-to-csfml-graphical-programming-c8aa995c7b18 | EN | 2.x (`{800, 600, 32}`, `sfSprite_create()` sans argument, d'après les extraits de recherche) | ≈ oct. 2024 [INCERTAIN] | Introductive | Vise les 1re année Epitech : fenêtre, événements, sprite, ligne `gcc … -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio`. Page en 403 pour l'outil : non lue en entier. |
| YouTube « Tuto CSFML » (The Elite of Segfault) | https://www.youtube.com/watch?v=utHMKoBjU8Y , https://www.youtube.com/watch?v=xT-2Xd7V49U | FR | 2.x | ≈ 2020 (« 6 ans ») | Inconnue | 2 épisodes (fenêtre, premier sprite), ≈ 8 000 vues chacun. |
| YouTube « Let's Code – Initiation à la CSFML » (Karim Dridi) | https://www.youtube.com/watch?v=WKGqcIrpKRY , https://www.youtube.com/watch?v=hkyw9MRfgME | FR | 2.x | ≈ 2021 (« 5 ans ») | Inconnue | Initiation + fractales. |
| YouTube « CSFML #01 … #07 » (Mathieu) | https://www.youtube.com/watch?v=LQCiefkZDpY (#01 Runner), https://www.youtube.com/watch?v=CHVhfXS2UrE (#07 Norme) | FR | 2.x | ≈ 2022–2023 | Inconnue | Série la plus vue (≈ 10 000 vues pour #01). Couvre runner, menu, animation, **norme Epitech**. Montre que le public français et Epitech existe. |
| YouTube « SFML in C part 1 » (General Vagueness) | https://www.youtube.com/watch?v=UIHEXy1xaeU | EN | 2.x | ≈ 2020 | Inconnue | Seule vidéo anglaise trouvée sur CSFML. |
| CSFML-Workshop (Floriantoine) | https://github.com/Floriantoine/CSFML-Workshop | — | 2.x | 2020-11-25 | — | Exercices ; pas de README. |
| CSFML-Demo (ImOverlord) | https://github.com/ImOverlord/CSFML-Demo | — | 2.x | 2017-12-15 | — | Exemples, MIT. |
| CSFML_frag_shader_example | https://github.com/PierreMauger/CSFML_frag_shader_example | EN | 2.x | 2021-05-27 | — | Exemple de shader ; rare sur ce sujet. |
| EpiVNC (matteoepitech) | https://github.com/matteoepitech/epivnc | EN | celle de l'image `epitechcontent/epitest-docker` [INCERTAIN] | 2026-02-01 | Outil | Docker + Xvfb + noVNC pour lancer du CSFML depuis macOS. Seule ressource récente sur le thème « faire tourner CSFML hors Linux ». MIT. |
| Wrappers (CleanSFML, BillyScene) | https://github.com/NemesisX1/CleanSFML , https://github.com/JulienTD/BillyScene | EN | 2.x | 2020 / 2019 | — | Bibliothèques, pas des docs. |

**Docs d'autres bindings (inspiration seulement)** [source secondaire] :
- **CrSFML** (Crystal) https://oprypin.github.io/crsfml/ : référence API reprise de SFML, **tutoriels traduits** dans le
  langage du binding, MkDocs Material, lien « Edit this page ». C'est le modèle le plus proche de notre projet.
- **SFML.Net** : même position que CSFML (« pas de tutoriel, adaptez le C++ »). La doc est fournie dans les paquets
  (https://www.sfml-dev.org/download/sfml.net/).
- **ruby-sfml** (https://www.rubydoc.info/gems/ruby-sfml/SFML/C/Window) s'appuie sur CSFML 3 via FFI.
  Sa doc montre indirectement les constantes CSFML 3. Non vérifié.

**Recherches sans résultat** (2026-11-07, WebSearch FR et EN) : aucun tutoriel CSFML sur dev.to,
OpenClassrooms, Zeste de Savoir, GitBook ou Notion. Aucune doc CSFML hébergée sur `github.io` en dehors
d'Enichy. Aucun contenu tutoriel qui utilise `sfWindowed` ou les signatures 3.0. Aucune page de dépannage CSFML
pour WSL : il n'y a que des tickets WSL génériques (ex. https://github.com/microsoft/WSL/issues/8899).
L'API de recherche GitHub ne donne **0 dépôt** pour « csfml tutorial », « csfml guide » et « csfml 3.0 ». [CONFIRMÉ pour ces requêtes ; l'absence en général reste DÉDUITE]

---

## 3. Analyse des manques

| Besoin du lecteur | Ce qui existe | Ce qui manque |
|---|---|---|
| Référence API CSFML **en ligne** | Rien d'officiel. Doxygen hors ligne via `libcsfml-doc` (Linux) ; headers sur GitHub ; https://sources.debian.org/src/libcsfml/3.0.0-1/ | Une référence en ligne, versionnée (2.6 / 3.0), consultable sans installer de paquet. **C'est le manque le plus simple à combler.** |
| Référence avec exemples | 12 blocs `\code` dans 916 fonctions | Un exemple court par type ou header, compilé |
| Tutoriels en **C** | Aucun officiel ; non officiels en 2.x, incomplets ou en vidéo | Parcours complet en C : fenêtre → boucle → sprites → texte → son → vues → shaders → réseau |
| Contenu **CSFML 3.0** | `example.c` (1 fichier), headers, notes de release (liste de PR) | Quasi tout : aucun tutoriel ni article 3.0 trouvé |
| Exemples C **qui compilent** | `example.c` (compilé en CI, non exécuté) ; l'exemple de la page d'accueil Doxygen est **faux** | Tous les exemples compilés **et exécutés** en CI (avec un serveur X virtuel), sur 3.0 (et 2.6 pour la partie legacy) |
| **Migration 2 → 3 en C** | Guide C++ (21 sections, C++ seulement) ; diff des headers à faire soi-même | Un guide C : tableau « ancien appel → nouvel appel », erreurs de compilation typiques et leur cause |
| Doc **en français** | Tutoriels SFML 2.6 en FR (C++) ; vidéos FR 2.x | Doc écrite en FR, à jour 3.0. Les tutoriels SFML 3.x n'existent qu'en anglais. |
| **Installation / dépannage Linux, WSL, macOS** | Tutoriel « SFML and Linux » (C++) ; FAQ « Troubleshooting » (C++, Windows surtout) ; EpiVNC | Lien `-lcsfml-*`, `pkg-config`, versions par distribution (Ubuntu 22.04 = 2.5 !), WSLg / `DISPLAY`, son sous WSL, « undefined reference » propres au C |
| Savoir **quelle version on a** | Rien | Une page « Quelle version ai-je ? » (Ubuntu 22.04 / 24.04 / 26.04, Fedora 43, Arch, image Epitech) |
| Ownership / libération mémoire | Implicite (`*_destroy`) ; pas de page concept | Règles claires : qui crée, qui détruit, ce qui reste lié (texture ↔ sprite, police ↔ texte) |
| Volet **Epitech** | Docs d'étudiants 2020–2022, vidéos « norme » | Notions + pièges, à jour, sans fournir de solution de projet noté |

---

## 4. Angle unique proposé

### Formule (3–5 phrases)

> **La référence et le guide CSFML 3 écrits pour le C, en français d'abord puis en anglais.**
> Chaque extrait de code est un vrai fichier `.c` qui est compilé **et exécuté** en CI contre la version
> annoncée. Une page « Erreurs connues de la doc officielle » liste les écarts trouvés.
> La doc ajoute ce que personne ne fournit : la référence API en ligne et versionnée, un guide de migration
> 2 → 3 **côté C**, et un dépannage Linux / WSL / macOS.
> Un volet Epitech explique les notions et les pièges, sans jamais donner de solution de projet.

### Public cible
1. Étudiants francophones qui découvrent la programmation graphique en C (Epitech en premier, puis
   autres écoles et autodidactes).
2. Développeurs C qui passent de CSFML 2.x à 3.0 (ou qui doivent jongler entre les deux selon la machine).
3. Auteurs de bindings qui s'appuient sur CSFML (Nim, Zig, Ada, Ruby, Python…) et veulent une référence en ligne.

### Promesse
- **Exact** : chaque signature vient d'un header d'un tag précis, chaque exemple est compilé (`-Wall -Wextra -Werror`) et exécuté en CI.
- **Daté et versionné** : chaque page indique la version CSFML testée et la date du dernier test.
- **Accessible** : FR d'abord, phrases simples, termes anglais expliqués. EN en second.
- **Complet sur le C** : couvre ce que les tutoriels C++ ne transposent pas (callbacks au lieu de l'héritage,
  union `sfEvent`, `*_destroy`, pas d'exceptions, pas d'angles typés).

### Ce que la doc NE fera PAS
- Pas de solutions, ni de squelettes complets, des projets Epitech notés (my_hunter, my_runner, my_radar, my_world, my_rpg…).
- Pas de copie des tutoriels C++ : on **adapte** en C, avec attribution CC BY 4.0, ou on **renvoie** vers eux.
- Pas de doc C++ SFML, ni de doc des autres bindings.
- Pas de promesse sur CSFML master / 3.1 tant qu'il n'y a pas de release (seulement un encadré « à venir »).
- Pas de « doc officielle » : bandeau clair « communautaire, non affilié à l'équipe SFML ». On n'utilise pas
  la marque SFML d'une façon qui ferait croire à un site officiel.
- Pas de reproduction de contenu non licencié (docs d'étudiants, Medium, vidéos) : seulement des liens.

### Licences : ce qu'on peut réutiliser

| Source | Licence | Ce qu'on peut faire | Attribution / obligations |
|---|---|---|---|
| Code CSFML : headers, commentaires Doxygen, `examples/example.c`, `doc/mainpage.hpp` | zlib/png (`license.md` @ master, « Copyright (C) 2007-2026 Laurent Gomila ») [CONFIRMÉ] | Reprendre et adapter les descriptions d'API, générer une référence depuis les headers | Ne pas se présenter comme l'auteur ; **marquer clairement les versions modifiées** ; garder la notice dans toute copie de source. Crédit recommandé. |
| Code et doc Doxygen de SFML C++ | zlib/png (https://www.sfml-dev.org/license/) [CONFIRMÉ] | Idem | Idem |
| Tutoriels et pages de sfml-dev.org | **CC BY 4.0** (fichier `LICENSE` de https://github.com/SFML/SFML-Website : « SFML Website by the SFML Team is licensed under a Creative Commons Attribution 4.0 International License ») [CONFIRMÉ] | Traduire, adapter en C, réutiliser des passages, y compris pour un usage commercial | Crédit « SFML Team », lien vers la page source et vers la licence CC BY 4.0, **indiquer les modifications**. Pas de restriction technique (DRM) ajoutée. Remarque : le pied de page du site n'affiche que « Copyright © Laurent Gomila ». |
| Logo SFML | Domaine public (copyright Debian, renvoi vers un message du forum) [CONFIRMÉ via Debian, source secondaire] | L'utiliser dans les exemples | Rien d'obligatoire. Éviter de suggérer un caractère officiel. |
| Assets `tuffy.ttf` / `doodle_pop.ogg` | Domaine public / Unlicense [CONFIRMÉ via Debian] | Les réutiliser dans nos exemples | Rien d'obligatoire. Mentionner l'origine par honnêteté. |
| Code du wiki SFML | Domaine public, sauf mention de l'auteur (`Home.md`) [CONFIRMÉ] | Réutiliser le code | Vérifier la mention de chaque page. Le **texte** du wiki n'a pas de licence explicite : on se limite aux liens. |
| Docs d'étudiants, Medium, YouTube | Pas de licence (droits réservés par défaut) [DÉDUIT] | Lien seulement | — |

---

## Décisions prises

- **Angle retenu** : « CSFML 3 en C, FR d'abord, exemples compilés et exécutés en CI, référence en ligne
  versionnée, migration 2 → 3 côté C, dépannage Linux/WSL, volet Epitech sans solutions ». Raison : c'est
  l'intersection exacte des manques mesurés (§3). Personne d'autre n'occupe ce créneau.
- **La référence API en ligne est une priorité haute.** Elle n'existe nulle part. Elle peut être générée
  depuis les headers (zlib) puis enrichie. C'est le contenu qui servira aussi aux auteurs de bindings.
- **Page « Erreurs connues de la doc officielle »** : nous signalons les écarts constatés (exemple de la
  page d'accueil, archives sans doc), avec la source et le commit. Si possible, nous proposons ensuite une PR amont.
  Raison : crédibilité, et utile à la communauté.
- **Tutoriels SFML (CC BY 4.0) adaptés et non copiés** : chaque page adaptée porte un encadré
  d'attribution (auteur, URL source, licence, « adapté en C et traduit »).
- **Assets d'exemple** : reprendre `sfml_logo.png`, `tuffy.ttf`, `doodle_pop.ogg` (domaine public / Unlicense),
  sous réserve du test n°7.
- **Pour les événements**, s'inspirer du tutoriel **2.6** (union + `type`), plus proche du C que le tutoriel 3.x.
- **Ressources non officielles** : on les cite dans une page « Autres ressources », avec version et date,
  sans copier leur contenu.
- **Ne pas lire la page Medium en contournant le blocage** : on garde seulement les extraits des moteurs de recherche, marqués [INCERTAIN].

## Points [INCERTAIN]

- **Contradiction possible avec `09-architecture-contenu.md`** : ce fichier prévoit de « renvoyer vers la
  doc officielle 2.6 si elle est en ligne ». **Elle n'est pas en ligne**, et elle n'est pas non plus dans les
  archives 2.6.1. Elle n'existe que via `libcsfml-doc` 2.6.x (Debian 13, Ubuntu 24.04) ou `CSFML-doc` (Fedora).
  De plus, **Fedora 43 et rawhide sont encore en CSFML 2.6.1**. Si l'environnement Epitech est basé sur
  Fedora, la section 2.6 compte davantage que prévu. → à arbitrer par l'agent principal.
- Date exacte et contenu complet de l'article Medium (page en 403 pour l'outil).
- Dates exactes des vidéos YouTube (seulement « il y a N ans » relevé le 2026-11-07).
- Version de CSFML dans l'image `epitechcontent/epitest-docker` (utilisée par EpiVNC) : non vérifiée.
- Le paquet Arch `csfml` contient-il la doc HTML ? Non vérifié.
- Le dépôt SFML-Website contient aussi `scripts/highlight/LICENSE` (bibliothèque de coloration). La
  CC BY 4.0 couvre le contenu du site. Aucune autre exception n'a été trouvée, mais nous n'avons lu que le `LICENSE` racine.
- La licence du logo SFML (domaine public) est connue par le fichier `copyright` de Debian, qui cite un
  message du forum. Le forum n'a pas pu être lu (anti-bot).
- Le jugement « descriptions courtes, peu d'info sur l'ownership » vient d'un échantillon, pas d'une mesure complète.
- Absence de tout tutoriel CSFML 3.0 sur le web : déduite de nombreuses recherches négatives, pas prouvée.
