# 00 — Synthèse (phase 1, recherche) — 2026-11-07

Documentation communautaire CSFML en français et en anglais, hébergée sur GitHub Pages. Rien n'a été
compilé ni installé en phase 1. Tout ce qui demande un test réel est dans [A-TESTER.md](A-TESTER.md)
(147 points : 46 P1, 66 P2, 35 P3). Les sources sont dans [sources.md](sources.md).

## Décisions clés

1. **Versions.** CSFML **3.0.0** (release GitHub 2026-01-14) est la dernière stable. Il n'y a pas de 3.1 :
   `master` se déclare 3.1.0, exige SFML 3.1 et n'est pas publié. SFML stable = 3.1.0 (2026-04-16).
   CSFML 3.0.0 se construit avec SFML 3.0.x et, d'après le paquet Debian sid, avec 3.1. [01]
2. **Poids égal 2.6.1 / 3.0.0** (décision de l'utilisateur à la reprise). Fedora est encore en 2.6.1 et
   aucune doc API n'existe en ligne pour 2.6 ni pour 3.0. Deux versions de première classe, mêmes sections,
   sélecteur de version, deux jeux d'exemples compilés en CI. On rédige d'abord en 3.0, puis on dérive 2.6. [08, 09, 10]
3. **Epitech.** L'image `epitechcontent/epitest-docker` est déduite basée sur Ubuntu 26.04, donc CSFML 3.0.0.
   Ce n'est pas encore constaté dans un conteneur (test P1 n°1). [01, 06]
4. **Outil de site : Starlight (Astro)**, repli Docusaurus (47/55 contre 41, 40 et 39 pour Docusaurus,
   MkDocs Material et VitePress). Les notes sont sourcées mais non mesurées. [08]
5. **Contenu.** Dix sections (Démarrer, Concepts, Tutoriels, Recettes, Référence, Migration, Dépannage,
   Epitech, Glossaire, À propos). Référence = une page par header ou type (environ 70 pages), signatures
   **générées** depuis les headers. Fil rouge des tutoriels : un casse-briques, pour ne reproduire aucun
   projet Epitech noté. [09]
6. **Qualité.** libclang produit un JSON de l'API par version. Contrôles automatiques : « toute fonction
   citée existe » et « toute fonction des headers a une page ». Exemples `.c` réels, compilés et exécutés
   sous xvfb + llvmpipe, valgrind, badge « testé sur » généré par la CI. [10]
7. **Angle unique :** CSFML en C, en français d'abord, exemples exécutés en CI, migration 2→3 côté C,
   dépannage Linux/WSL, volet Epitech sans solutions de projets. [07]

## Ce que les agents ont trouvé et qui change la donne

- **Aucune doc API CSFML en ligne** (404 sur les URLs officielles). La doc Doxygen n'existe que dans les
  paquets (`/usr/share/doc/libcsfml-dev/html`). Aucun tutoriel C pour CSFML 3.0. [07]
- **L'exemple de la page d'accueil Doxygen est faux en 3.0** (`sfRenderWindow_create` à 4 arguments,
  `sfSprite_create()` sans texture) et l'erreur reste sur master. D'où une page « Erreurs connues de la doc officielle ». [07]
- **API 3.0.0 : 926 fonctions**, 23 constantes, 134 types (comptes vérifiés trois fois). Master : 1034
  fonctions, 40 marquées dépréciées (tout FTP). `sfIpAddress` passe de 16 à 46 octets sur master (ABI cassée). [03]
- **Cassures 2→3 les plus fréquentes :** chemin `<CSFML/...>`, `sfBool` → `bool`, `sfRenderWindow_create` à
  5 paramètres, `sfSprite_create(texture)` et `sfText_create(font)` obligatoires (NULL = assert),
  rectangles en `position`+`size`, `sfQuads` supprimé. [04]
- **Installation sans sudo :** SFML se compile en statique par défaut alors que CSFML cherche une SFML
  partagée : passer `-DBUILD_SHARED_LIBS=ON`. Prévoir un rpath et `-DCMAKE_INSTALL_LIBDIR=lib`. [02]
- **Audio sans périphérique :** en 3.0 (miniaudio) repli sur un backend nul et le programme continue ; en 2.6
  (OpenAL) deux messages d'erreur, comportement à tester. [05]
- **Piège de CI :** `apt` sur Ubuntu 24.04 donne CSFML 2.6.0, pas 2.6.1. Pour tester 2.6.1, compiler le tag. [10]
- **Licences :** tutoriels sfml-dev.org en CC BY 4.0 (adaptables avec attribution), code CSFML en zlib. [07]

## Risques

| Risque | Gravité | Réponse |
|---|---|---|
| Charge de rédaction doublée par la parité 2.6 / 3.0 | Haute | Rédiger en 3.0 d'abord, dériver 2.6 avec la table de [04](04-migration-2-vers-3.md) ; générer la référence par script |
| Contenu écrit par IA contenant des erreurs | Haute | Tout exemple compilé et exécuté en CI, contrôle automatique des signatures |
| Version Epitech non constatée | Moyenne | Test P1 n°1 (`docker run` sur l'image) |
| Sortie de CSFML 3.1 (ABI et dépréciations) | Moyenne | Workflow programmé qui teste les nouveaux tags, diff de headers |
| Section Epitech perçue comme une aide à la triche | Moyenne | Notions et liens seulement, encadré « ton projet doit être ton travail » |
| Recherche web parfois mince (05/06 : 2 recherches seulement, forum SFML bloqué) | Moyenne | Points signalés [INCERTAIN], à valider en phase 2 |

## Points [INCERTAIN] principaux

- Version CSFML des postes étudiants Epitech et de l'image de correction (déduite, non constatée).
- Comportement de `destroy(NULL)`, des `assert` dans les paquets, de l'UTF-8 dans `sfText_setString`.
- Audio 2.6 (OpenAL) sans périphérique ; WSLg, macOS, noms des DLL Windows ; Wayland sur master / SFML 3.1.
- Règles exactes de la norme Epitech (sources non officielles) et liste actuelle des fonctions autorisées.
- Notes « rapidité » et « accessibilité » du tableau des générateurs de site : non mesurées.
- Noms exacts de la capture d'écran en 2.6.1 et 3.0.0 (utile pour les tests d'image en CI).
- Un chiffre diffère entre agents : 916 fonctions avec `\brief` (07) contre 1083 `\brief` sur master (10) ;
  ce ne sont pas les mêmes comptages (tag 3.0.0 / master), à recompter.

## Cohérence : points relevés à la passe finale

- Parité 2.6 / 3.0 appliquée dans 08 et 09. 01, 02, 03, 04, 05, 06, 07 et 10 sont écrits par des agents qui
  avaient déjà l'instruction « poids égal » (05, 10) ou qui sont neutres sur ce point ; **01, 02, 03, 04 et 07
  n'ont pas été relus après la décision de parité** : leur contenu reste valable mais leurs phrases du type
  « viser 3.0.0 en priorité » sont à nuancer en phase 2.
- Aucune relecture ligne à ligne des 4 500+ lignes n'a été faite. Les comptes de l'inventaire (03) ont été
  vérifiés par l'agent qui l'a écrit, pas par une seconde passe indépendante.
- Les listes [A-TESTER.md](A-TESTER.md) et [sources.md](sources.md) sont fusionnées automatiquement et
  dédoublonnées par URL ; des tests proches peuvent apparaître deux fois.

## Prochaine étape (phase 2)

Exécuter les 46 tests P1 de [A-TESTER.md](A-TESTER.md), en commençant par le n°1 (image Epitech) qui
conditionne le poids réel de 2.6 et de 3.0 dans la doc.

## Index des fichiers

[01 versions](01-versions.md) · [02 installation](02-install.md) · [03 inventaire API](03-api-inventaire.md) ·
[04 migration 2→3](04-migration-2-vers-3.md) · [05 pièges](05-pieges.md) · [06 Epitech](06-epitech.md) ·
[07 existant](07-existant.md) · [08 site](08-architecture-site.md) · [09 contenu](09-architecture-contenu.md) ·
[10 workflow](10-workflow.md) · [sources](sources.md) · [A-TESTER](A-TESTER.md)
