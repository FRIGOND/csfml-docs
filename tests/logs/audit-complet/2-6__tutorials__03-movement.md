# 2-6/tutorials/03-movement.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/03-movement CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | enchaînement | Extrait `reflect(float *pos, float *vel, ...)` | Pointeurs en paramètres (`*pos = min`) jamais expliqués, alors que c'est la première fois ; `*vel = -*vel` déroutant pour un débutant. | gênant |
| 5 | 2 | `Un test qui donne toujours le même résultat` ... « déterministe » | « déterministe » apparaît dans « Ce que tu as appris » sans définition. | mineur |
| 6 | 6 | `sfClock_restart` ... `sfTime_asSeconds` | Existent dans les deux versions avec la même signature (vérifié JSON) ; la 3.0 ajoute `sfClock_reset` (met en pause) non mentionné : sans danger. | mineur |
| 7 | enchaînement | Code importé `bb.left` / `bb.position.x` (check_ball) | Champs de `sfFloatRect` utilisés avant leur introduction (étape 5), différence 2.6/3.0 non signalée. | mineur |
