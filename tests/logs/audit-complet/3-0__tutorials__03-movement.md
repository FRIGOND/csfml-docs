# 3-0/tutorials/03-movement.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/03-movement CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | enchaînement | Extrait `reflect(float *pos, float *vel, ...)` | Pointeurs en paramètres (`*pos = min`) jamais expliqués, alors que c'est la première fois ; `*vel = -*vel` déroutant pour un débutant. | gênant |
| 4 | 2 | `Un test qui donne toujours le même résultat` ... « déterministe » | « déterministe » apparaît dans « Ce que tu as appris » sans définition. | mineur |
| 5 | 6 | `sfClock_restart` ... `sfTime_asSeconds` | Existent dans les deux versions avec la même signature (vérifié JSON) ; la 3.0 ajoute `sfClock_reset` (met en pause) non mentionné : sans danger. | mineur |
| 6 | enchaînement | Code importé `bb.left` / `bb.position.x` (check_ball) | Champs de `sfFloatRect` utilisés avant leur introduction (étape 5), différence 2.6/3.0 non signalée. | mineur |
