# guides/concepts/ownership.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 / 7 | Tableau « Test réel (RESULTATS) » n°19, 20, 21 ; n°22, 23, 24 ; « n°67 », « n°72 » | Tous ces tests ont été faits en CSFML 3.0.0 (RESULTATS : build ~/csfml-3.0 / paquet 3.0.0). La page n'est pas par onglet et n'indique pas la version testée ; rien sur la 2.6 (ex. `sfSprite_setTexture`/`sfSound_setBuffer` en 2.6 : mêmes gardes de pointeur ?). | gênant |
| 2 | 2 | « thread audio », « glyphe », « `strdup` », « emprunté », « comportement indéfini », « valgrind » (premier emploi) | Non définis, aucun lien glossaire ; valgrind expliqué seulement via un lien final. | mineur |
| 3 | 1 | « Écrire `char *d = ...` donne `-Wdiscarded-qualifiers` » | Ne dit pas qu'il s'agit d'un avertissement de gcc avec `-Wall -Wextra` ni comment le voir ; `free((void *)d)` : cast non expliqué. `sfKeyboard_getDescription` ne reçoit pas d'argument montré. | mineur |
| 4 | 5 | « `sfFont_createFromFile` et `sfMusic_createFromFile` relisent probablement le fichier plus tard » | Contredit resources.mdx qui dit seulement `sfMusic` lit en continu (« Longues musiques : sfMusic ... le fichier doit rester accessible ») sans parler de `sfFont_createFromFile` ; à harmoniser. | mineur |
Vérifié : liens (reference, cookbook sprite-texture 3.0 et 2.6, resources, runtime-crashes, memory) existent ; signatures getTexture/getFont/getString/saveToMemory conformes aux JSON ; n°19-24, 67, 72 existent dans RESULTATS.md.
