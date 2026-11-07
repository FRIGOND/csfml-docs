# 2-6/tutorials/05-collisions.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/05-collisions CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 2 | `BALL_SPEED`, `alive`, `inter`, `rel` | Constantes et champs (BALL_SPEED=400, brick_t.alive) non présentés dans le texte, seulement dans le main.c. | mineur |
| 5 | 6 | Exercice `sfFloatRect_contains` avec `x, y` | Signature différente : 2.6 `(&r, x, y)`, 3.0 `(&r, sfVector2f)` (JSON). L'exercice ne le signale pas. | mineur |
| 6 | 2 | `rectangles englobants (bounding boxes)`, `théorème de Pythagore`, `-lm` | `-lm` expliqué (bien), mais « bounding box » et `math.h` sans lien glossaire. | mineur |
