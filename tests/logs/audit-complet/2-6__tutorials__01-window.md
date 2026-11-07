# 2-6/tutorials/01-window.mdx
Verdict : A CORRIGER (9 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/01-window CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 6 | (code importé) `#include <SFML/Graphics.h>` / `<CSFML/Graphics.h>` | Le texte n'explique jamais que le chemin de l'include change entre 2.6 et 3.0 (SFML/ contre CSFML/), alors que les deux main.c jumeaux diffèrent sur ce point ; seule la migration ou check-your-setup le dit. | gênant |
| 5 | 2/autre | ```c while (sfRenderWindow_isOpen(window) && frames != 0) { handle_events(window); ...``` | Extrait tronqué (accolade jamais fermée) ; `frames` et `handle_events` ne sont pas introduits avant (le mode test est expliqué plus bas) ; les puces suivantes parlent de `pollEvent` absent de l'extrait. | gênant |
| 6 | 2 | `sfTitlebar | sfClose, ..., NULL)` | Le dernier `NULL` (réglages `sfContextSettings`) n'est jamais expliqué ; `sfTitlebar | sfClose` (OU binaire sur des drapeaux) non plus. | mineur |
| 7 | autre | Description et Objectif : « croix ou touche Échap » | La touche Échap (`sfKeyEscape` dans `handle_events`) n'est expliquée nulle part dans le texte. | mineur |
| 8 | 2 | `file d'événements`, `frames`, `pixel` | Termes (event queue, frame) sans lien vers le glossaire. | mineur |
| 9 | 7 | `CSFML_PREFIX=system   # ou CSFML_PREFIX=$HOME/csfml-2.6` | Avec CSFML=2.6, `system` suppose un paquet 2.6 : Ubuntu 26.04 (testé) fournit la 3.0 ; Ubuntu 24.04 fournit la 2.6.0 et non la 2.6.1 testée (guides/install/index). `~/csfml-2.6` vient de from-source, non cité. | mineur |
