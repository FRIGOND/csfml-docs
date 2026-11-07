# 2-6/tutorials/07-text-score.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/07-text-score CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 2 | `valgrind`, `FreeType`, `tests/valgrind.supp` | valgrind non défini ; renvoi vers un fichier du dépôt que le lecteur n'a pas forcément. | mineur |
| 5 | 2 | `locale C.UTF-8`, `U+00C9`, `UTF-8` | Termes non expliqués ; pas de lien glossaire. | mineur |
| 6 | autre | `Oublier sfText_setFont en 2.6 : le texte n'affiche rien` | Non testé. | mineur |
| 7 | 6 | Bloc « Non vérifié » sur `sfText_setString` | Bien marqué ; mais le tutoriel 2.6 utilise `U"..."` : cohérent. Rien à corriger. | mineur |
