# guides/install/from-source.mdx
Verdict : A CORRIGER (8 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « Tu veux ... ou tu n'as pas `sudo` ? » puis « Prérequis ... headers de développement de : X11 ... » | Contradiction : installer `libx11-dev`, `libgl-dev`... exige sudo. Un étudiant sans sudo ne peut pas suivre la recette. | bloquant |
| 2 | 1 | « Un compilateur C et C++, `cmake`, `git` » / « Sur Ubuntu : `libx11-dev libxrandr-dev ...` » | Aucune commande `sudo apt install ...` complète ; noms `build-essential`, `cmake`, `git`, `g++` non donnés ; OpenAL pour la 2.6 : paquet non nommé. | gênant |
| 3 | 3 | Bloc `PREFIX=... git clone ... cmake ...` | Besoin d'Internet non dit ; durée, sortie attendue, marque de succès non dites. À taper dans un seul terminal (PREFIX est perdu si on ouvre un autre). | gênant |
| 4 | autre (dangereux) | « Pour tout supprimer : `rm -rf "$PREFIX"` » | Variable `$PREFIX` définie seulement plus haut dans le même shell ; dans un nouveau terminal elle est vide. Aussi supprime les sources. À écrire avec le chemin explicite et un avertissement. | gênant |
| 5 | 2 | « préfixe », « rpath », « `$ORIGIN` », « tag », « Release », « partagé/statique », « SFML_DIR », « libdir » | Non définis, pas de lien glossaire (« préfixe » est expliqué seulement par « un dossier à toi »). | gênant |
| 6 | 7 | « a été rejouée dans un dossier temporaire : elle s'est terminée sans erreur » ; « CSFML 3.0.0 demande CMake 3.22 ou plus » | Pas de n° dans tests/RESULTATS.md pour la rejouée ; CMake 3.22 vient du rapport mais n'est pas dans l'encadré Non vérifié. | mineur |
| 7 | 4 | « la recette du script `tools/build-csfml.sh` du dépôt » | Script non lié ; la 2.6 n'est décrite qu'en une phrase (pas de commande), alors que la page sert aux deux versions. | mineur |
| 8 | 6 | « Pour la 2.6 : tags `2.6.2` (SFML) et `2.6.1` (CSFML) ... `-DCMAKE_POLICY_VERSION_MINIMUM=3.5` » | Bien signalé, mais la recette ne dit pas que la 2.6 produit `include/SFML` alors que la commande finale utilise `-lcsfml-*` identique : ok. Rien de grave ; noté pour la 2.6.0 d'Ubuntu absente. | mineur |
Vérifié : liens (compiling) existent ; tests n°8, n°9, n°11 cités et présents dans RESULTATS.md ; la commande `-DSFML_DIR` correspond à A.3.
