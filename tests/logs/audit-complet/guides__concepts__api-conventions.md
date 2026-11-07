# guides/concepts/api-conventions.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 5 / autre | « Les fonctions `create…` qui lisent un fichier renvoient `NULL` en cas d'échec : **teste toujours** le retour. » | Ne dit pas que `sfRenderWindow_create` sans écran (DISPLAY absent) ne renvoie pas NULL mais abort, exit 134 (RESULTATS n°32 ; headless-xvfb.mdx). Piège fréquent pour un débutant. | gênant |
| 2 | 6 / 7 | « En 3.0 le paquet Ubuntu 26.04 déclenche une `assert` (test n°31...) » | Ne dit rien de la 2.6 (test non fait ?) ni du build Release local (segfault, n°31 nuancé) ; ne dit pas non plus que `destroy(NULL)` est accepté en 3.0 mais plante en 2.6 pour `sfRenderTexture_destroy` (n°62/91, memory.mdx) : la phrase « Passer NULL ... est une erreur » est trop absolue. | mineur |
| 3 | 2 | « opaque », « assert », « header parapluie », « fuite mémoire », « `const` », « `sizeof` » | « assert » et « parapluie » jamais définis ; aucun lien glossaire. | mineur |
| 4 | 5 | Tableau « Includes » : `/usr/include/CSFML/` | Dossiers valables seulement pour le paquet système ; avec un préfixe local (page from-source) c'est `$PREFIX/include`. Non précisé. | mineur |
| 5 | 6 | « Résumé des différences de forme » | Résumé incomplet : ne mentionne ni `sfFloatRect/sfIntRect` (champs `position`/`size`), ni les événements, ni `sfSound_setLoop` (2.6) vs `sfSound_setLooping` (3.0), ni `sfKeyReturn` (2.6) vs `sfKeyEnter` (JSON 3.0 n'a que Enter) ; ne renvoie qu'à la migration. Signatures citées (`destroy` const, `sfSprite_create`, `waitEvent`) vérifiées OK dans les JSON. | mineur |
Vérifié : liens (memory, runtime-crashes, /3-0/migration/overview/) existent. Les noms signalés par auto.md (sfBool, sfInt64, sfUint32, sfWindowState) sont dits 2.6-only/3.0-only dans la page (onglets ou tableau).
