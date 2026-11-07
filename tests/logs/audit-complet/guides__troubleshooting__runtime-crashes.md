# guides/troubleshooting/runtime-crashes.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 | « Pareil pour `sfText_create(font)` et `sfSound_create(buffer)` » | Exact seulement en 3.0 (JSON) ; en 2.6 ces `create` n'ont pas de paramètre. La section « Assertion » n'est pas dans un onglet et le test n°31 est 3.0 seulement. | gênant |
| 2 | 1 | « avec `gdb` ou `valgrind` » | gdb non expliqué ni lié ; pas de `-g`. | mineur |
| 3 | 2 | « abort », « segfault », « `-DNDEBUG` », « `__assert_fail` », « Release » | Non définis (glossaire non lié, mais « assert » y est). | mineur |
| 4 | 7 | « (sans message, exit 0, vérifié en 3.0.0 et 2.6.1) » pour `sfSprite_setTexture(NULL, NULL, true)` | Pas de n° dans RESULTATS.md (l'extrait lu n'en montre pas). | mineur |
Vérifié : liens (memory, resources, headless-xvfb, ownership) existent ; n°19-23, 31, 32, 62, 91 existent ; le message Sprite.cpp:42 conforme à RESULTATS n°31.
