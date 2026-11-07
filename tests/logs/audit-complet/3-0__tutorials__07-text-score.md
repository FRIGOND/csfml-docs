# 3-0/tutorials/07-text-score.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/07-text-score CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | 2 | `valgrind`, `FreeType`, `tests/valgrind.supp` | valgrind non défini ; renvoi vers un fichier du dépôt que le lecteur n'a pas forcément. | mineur |
| 4 | 2 | `locale C.UTF-8`, `U+00C9`, `UTF-8` | Termes non expliqués ; pas de lien glossaire. | mineur |
| 5 | 5 | « `NULL` interdit ; l'assertion ... pas testée pour `sfText_create` » / Erreurs : « Passer `NULL` à `sfText_create` ... : arrêt du programme » | Contradiction interne : le haut avoue ne pas avoir testé, la liste d'erreurs l'affirme. | gênant |
| 6 | 7 | `Le test (CSFML 3.0.0, locale C.UTF-8) montre` | C'est le test n°18 de RESULTATS.md, numéro non cité. | mineur |
