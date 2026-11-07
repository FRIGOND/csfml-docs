# guides/install/compiling.mdx
Verdict : A CORRIGER (8 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « gcc -I"$P/include" -o app main.c -L"$P/lib" ... » | `$P` n'est jamais défini sur la page ; la page from-source utilise `$PREFIX`. Incohérence de nom + variable non expliquée. | gênant |
| 2 | 5 / autre | « ajoute `-I`, `-L` et `-Wl,-rpath` quand `CSFML_PREFIX` désigne un préfixe local » | examples/common/example.mk fait l'inverse : par défaut `CSFML_PREFIX=$(HOME)/csfml-$(CSFML)` (local) et n'omet -I/-L que si `CSFML_PREFIX=system`. Un Makefile copié tel quel échoue sans préfixe local. | gênant |
| 3 | 4/5 | « `examples/common/example.mk` » et « les règles `all/clean/fclean/re` » | Fichier du dépôt non lié ; l'extrait montre `-Werror` sans le `-Wno-error=...` que le même fichier ajoute pour la 2.6 ; example.mk n'a pas de règle `fclean`. | mineur |
| 4 | 3 | Ligne de base `gcc ... main.c -lcsfml-...` | Dossier/fichier `main.c` supposé ; sortie attendue (rien = succès) non dite ; pas de renvoi vers /guides/install/ pour l'installation préalable. | mineur |
| 5 | 2 | « rpath », « RUNPATH », « readelf -d », « objdump -p », « ldd », « LD_LIBRARY_PATH », « préfixe local » | Non définis, aucun lien glossaire. | gênant |
| 6 | 7 | « Test réel ... n°6 », « test n°5 », « test n°15 » | OK. Mais « (testé : exit 0) » pour LD_LIBRARY_PATH, « vérifié » pour `error: type is deprecated [-Werror=...]` et pour le message `libcsfml-system.so.2.6: cannot open shared object file` n'ont pas de n° dans RESULTATS.md. | mineur |
| 7 | 5 | « renvoie `-I…/include -I/usr/include/freetype2 -I/usr/include/libpng16 -L…/lib ...` » | Ne dit pas pour quelle version (RESULTATS : ce sont les .pc 3.0.0). Le lecteur 2.6 ne sait pas si c'est identique. | mineur |
| 8 | 6 | Titre « Les avertissements de la 2.6 » | Contient un onglet 3.0 ; le texte « Aucun avertissement ... (test n°15, paquet 3.0.0) » est correct mais le titre trompe. Tableau des bibliothèques : même noms de fonctions dans les deux versions (JSON vérifié). | mineur |
Vérifié : liens existants (link-errors, makefile, check-your-setup). « Cette ligne a été testée avec le paquet système 3.0.0 (Ubuntu 26.04) » : cohérent avec RESULTATS n°3/5/6.
