# guides/troubleshooting/compile-errors.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 7 | « Avec gcc 14 et plus, c'est une **erreur** (avant, un simple avertissement) » | Seul gcc 15.2 et clang 21 ont été testés (RESULTATS n°86) ; « gcc 14 » non vérifié et non marqué. Important car le correcteur peut avoir un autre gcc. | gênant |
| 2 | autre | « (tests/RESULTATS.md, lots 1 et P2, et rejouer en direct pour cette page) » | Phrase incomplète/incompréhensible (« et rejouer en direct »). | mineur |
| 3 | 3 | « `sfVideoMode m = {{800, 600}, 32};` » et autres solutions | Sections sans dire si c'est 3.0 seulement : la page est commune ; en 2.6 la forme `{{800,600},32}` ne convient pas. | gênant |
| 4 | 2 | « header », « initializer », « inclusion indirecte », « pointeur de fonction », « `sfInputStream` » | Non définis ; pas de lien glossaire. | mineur |
| 5 | 6 | « Erreur sur le pointeur de fonction d'un `sfInputStream` (types `size_t` et `int64_t` en 3.0, `sfInt64` en 2.6) » | Affirmation non détaillée ; renvoi vers l'exemple seulement. | mineur |
Vérifié : `sfWindowed`/`sfFullscreen` existent en 3.0 (sfWindowState) ; sfSound_setLooping (3.0) et sfSound_setLoop (2.6) conformes aux JSON ; n°4, 16, 30, 82, 86, 101 existent ; liens (api-conventions, render-window, function-index, migration/overview, compiling, cookbook input-stream) existent.
