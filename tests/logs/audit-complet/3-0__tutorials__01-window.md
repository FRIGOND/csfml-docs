# 3-0/tutorials/01-window.mdx
Verdict : A CORRIGER (8 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/01-window CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | 6 | (code importé) `#include <SFML/Graphics.h>` / `<CSFML/Graphics.h>` | Le texte n'explique jamais que le chemin de l'include change entre 2.6 et 3.0 (SFML/ contre CSFML/), alors que les deux main.c jumeaux diffèrent sur ce point ; seule la migration ou check-your-setup le dit. | gênant |
| 4 | 2/autre | ```c while (sfRenderWindow_isOpen(window) && frames != 0) { handle_events(window); ...``` | Extrait tronqué (accolade jamais fermée) ; `frames` et `handle_events` ne sont pas introduits avant (le mode test est expliqué plus bas) ; les puces suivantes parlent de `pollEvent` absent de l'extrait. | gênant |
| 5 | 2 | `sfTitlebar | sfClose, ..., NULL)` | Le dernier `NULL` (réglages `sfContextSettings`) n'est jamais expliqué ; `sfTitlebar | sfClose` (OU binaire sur des drapeaux) non plus. | mineur |
| 6 | autre | Description et Objectif : « croix ou touche Échap » | La touche Échap (`sfKeyEscape` dans `handle_events`) n'est expliquée nulle part dans le texte. | mineur |
| 7 | 2 | `file d'événements`, `frames`, `pixel` | Termes (event queue, frame) sans lien vers le glossaire. | mineur |
| 8 | 7 | `L'ancienne forme à plat compile encore ... -Wmissing-braces` | Vrai (test n°29/n°26 de RESULTATS.md) mais sans numéro de test cité. | mineur |
