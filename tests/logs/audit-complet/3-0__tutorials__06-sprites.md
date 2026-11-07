# 3-0/tutorials/06-sprites.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/06-sprites CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | 2 | `repérée par valgrind` | valgrind jamais défini ni lié (absent du glossaire). Le constat vient du test n°19. | gênant |
| 4 | 1/3 | Exercice `examples/assets/sfml_logo.png` | Chemin relatif ambigu : le programme se lance depuis son dossier (`../../assets/`), l'exemple_asset n'est expliqué qu'à l'étape 7. | mineur |
| 5 | 2 | `carte graphique`, `échelle`, `teinte` | Pas de lien glossaire (sprite/texture/alpha y sont). | mineur |
| 6 | 7 | `dans le paquet Ubuntu, et plante dans un build maison (test n°31)` | Cité (n°31, Ubuntu 26.04) ; version d'Ubuntu non précisée. Le plantage de « build maison » n'a lieu qu'avec `-DNDEBUG`. | mineur |
