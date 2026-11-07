# 3-0/tutorials/08-sound-states.mdx
Verdict : A CORRIGER (8 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/08-sound-states CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | autre | Tableau des états : `« Victoire ! »`, `« Perdu… »` | Le main.c affiche « Victoire ! Entrée pour rejouer » et « Perdu... Entrée pour rejouer » : texte différent. | mineur |
| 4 | 2 | `sfMusic` ... `sfPlaying`/`sfStopped` | `sfMusic` jamais introduit dans le tutoriel ; statuts non expliqués. Constat conforme au test n°34, numéro non cité. | mineur |
| 5 | 2 | `énumération`, `enum`, `TEST_SPEEDUP`, conteneur | `enum` non expliqué ; « conteneur » (Docker) non défini. | mineur |
| 6 | 8 | `Le fichier de l'exemple dure 24 secondes` | Vérifié (24,00 s avec ffprobe) mais chiffre non recompté par script. | mineur |
| 7 | autre | `Si le son joue déjà, il recommence depuis le début` | Non testé. | mineur |
| 8 | 5 | « `sfSound_create(buffer)` exige un buffer valide (`NULL` interdit) » | Affirmé sans réserve ; objects-creation dit que `sfSound_create(NULL)` n'a pas été exécuté. | gênant |
