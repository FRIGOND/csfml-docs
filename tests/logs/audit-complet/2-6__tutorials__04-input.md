# 2-6/tutorials/04-input.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/04-input CSFML=2.6 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 5/6 | Ce programme compile avec `-std=c11 -Wall -Wextra -Werror` | Faux pour la 2.6 à la main : le header 2.6.1 émet `type is deprecated` et `-Werror` échoue sans `-Wno-error=deprecated-declarations` (voir 2-6/getting-started/first-window et examples/common/example.mk, qui ajoute ce flag). | gênant |
| 3 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 4 | 2 | `sous Xvfb (écran virtuel)` | Xvfb non défini ici (présent dans le glossaire, non lié). Bloc « Non vérifié » correct sinon. | mineur |
| 5 | enchaînement | `return frame < 45 ? 1 : -1;` et `dir` | `frame`, `dir` non présentés dans le texte ; `paddle_t` nommé sans extrait. | mineur |
| 6 | autre | Exercice « touches `A` et `D` » | Contredit l'erreur fréquente sur l'AZERTY (ne pas supposer l'emplacement des lettres) sans avertir que A/D sont des touches physiques/clavier-dépendantes. | mineur |
| 7 | autre | `la raquette avance par à-coups (répétition clavier)` | Affirmation non testée (pas de vraie touche possible sous Xvfb, cf. bloc Non vérifié). | mineur |
