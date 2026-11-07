# 3-0/tutorials/02-shapes.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/02-shapes CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | enchaînement | `sfRectangleShape_getGlobalBounds` (voir le test) ; code importé `rb.left`/`rb.position.x` | Le test de `check_layout` lit les champs d'un `sfFloatRect` ; ils ne sont expliqués qu'à l'étape 5. L'étape 2 ne dit pas que `sfFloatRect` change entre 2.6 et 3.0. | mineur |
| 4 | 2 | `(sfVector2f){BALL_R, BALL_R}`, `game_t g = {0};` | Littéral composé C et initialisation `{0}` jamais expliqués ; `BRICK_W`, `BRICK_GAP`, `ROW_COLORS` cités sans valeurs (il faut lire le main.c). | mineur |
| 5 | 8 | `50 briques (10 colonnes, 5 lignes)` | Vérifié : COLS=10, ROWS=5 dans main.c (50). Chiffre en dur mais cohérent ; dernière brique x=774 < 800. | mineur |
| 6 | 7 | `détruire un pointeur NULL est sans effet pour toutes les fonctions destroy testées` | Test n°62 (40 fonctions) non cité ; « toutes » est trop fort (40 testées). | mineur |
