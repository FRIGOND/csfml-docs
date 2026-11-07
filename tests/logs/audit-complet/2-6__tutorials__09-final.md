# 2-6/tutorials/09-final.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/09-final CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 8 | `3 vies`, `1 %`, `240 pas`, `score vaut 500`, `30 images`, `20 lignes` | Vérifiés dans main.c (LIVES 3, SPEED_UP 1.01, TEST_SPEEDUP 240, 50x10, 30 images) mais chiffres en dur sans recomptage ; « 20 lignes ou moins » non contrôlé par script ici. | mineur |
| 5 | 2 | `valgrind` | Non défini (absent du glossaire). | mineur |
| 6 | autre | Tableau des sections | Omet la section « Réglages du jeu » du main.c. | mineur |
| 7 | autre | `environ la moitié du temps disponible` | Affirmation non vérifiée. | mineur |
