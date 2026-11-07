# 3-0/tutorials/04-input.mdx
Verdict : A CORRIGER (7 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `make -C examples/tutorials/04-input CSFML=3.0 CSFML_PREFIX=system` | Suppose que le lecteur a le dossier `examples/` du dépôt du site. Aucune page ne dit comment l'obtenir (le lien GitHub du site est `https://github.com/`). `make -C`, `CSFML_PREFIX=system`, « dépôt » ne sont pas expliqués ; dossier de départ non précisé. | gênant |
| 2 | 3 | Pour compiler à la main, ajoute `-I examples/common` ... et lie : `-lcsfml-graphics ...` | Pas de commande complète (nom du .c, -o, -std). « lie » non expliqué (glossaire : linker). Rien sur `-I`/`-L` pour un préfixe local. | mineur |
| 3 | 2 | `sous Xvfb (écran virtuel)` | Xvfb non défini ici (présent dans le glossaire, non lié). Bloc « Non vérifié » correct sinon. | mineur |
| 4 | enchaînement | `return frame < 45 ? 1 : -1;` et `dir` | `frame`, `dir` non présentés dans le texte ; `paddle_t` nommé sans extrait. | mineur |
| 5 | autre | Exercice « touches `A` et `D` » | Contredit l'erreur fréquente sur l'AZERTY (ne pas supposer l'emplacement des lettres) sans avertir que A/D sont des touches physiques/clavier-dépendantes. | mineur |
| 6 | autre | `la raquette avance par à-coups (répétition clavier)` | Affirmation non testée (pas de vraie touche possible sous Xvfb, cf. bloc Non vérifié). | mineur |
| 7 | 6 | `sont les mêmes en 3.0` | Exact pour `sfKeyLeft/Right` ; la 3.0 ajoute `sfKeyboard_isScancodePressed` non cité (ok pour un débutant). | mineur |
