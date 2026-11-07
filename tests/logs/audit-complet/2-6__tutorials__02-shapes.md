# 2-6/tutorials/02-shapes.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/02-shapes CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | enchaînement | `sfRectangleShape_getGlobalBounds` (voir le test) ; code importé `rb.left`/`rb.position.x` | Le test de `check_layout` lit les champs d'un `sfFloatRect` ; ils ne sont expliqués qu'à l'étape 5. L'étape 2 ne dit pas que `sfFloatRect` change entre 2.6 et 3.0. | mineur |
| 5 | 2 | `(sfVector2f){BALL_R, BALL_R}`, `game_t g = {0};` | Littéral composé C et initialisation `{0}` jamais expliqués ; `BRICK_W`, `BRICK_GAP`, `ROW_COLORS` cités sans valeurs (il faut lire le main.c). | mineur |
| 6 | 8 | `50 briques (10 colonnes, 5 lignes)` | Vérifié : COLS=10, ROWS=5 dans main.c (50). Chiffre en dur mais cohérent ; dernière brique x=774 < 800. | mineur |
| 7 | 7 | `sfRenderTexture_destroy(NULL)` plante (testé) | Test n°91 de RESULTATS.md, mais numéro non cité dans la page. | mineur |
