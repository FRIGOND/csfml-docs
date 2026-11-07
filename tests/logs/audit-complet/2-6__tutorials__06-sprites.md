# 2-6/tutorials/06-sprites.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/06-sprites CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 2 | `repérée par valgrind` | valgrind jamais défini ni lié (absent du glossaire). Le constat vient du test n°19. | gênant |
| 5 | 1/3 | Exercice `examples/assets/sfml_logo.png` | Chemin relatif ambigu : le programme se lance depuis son dossier (`../../assets/`), l'exemple_asset n'est expliqué qu'à l'étape 7. | mineur |
| 6 | 2 | `carte graphique`, `échelle`, `teinte` | Pas de lien glossaire (sprite/texture/alpha y sont). | mineur |
| 7 | autre | `Oublier sfSprite_setTexture en 2.6 : le sprite reste blanc` | Non vérifié : un sprite sans texture a un rectangle de texture vide et risque de ne rien afficher plutôt que d'être blanc. | gênant |
