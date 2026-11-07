# guides/concepts/coordinates-views.mdx
Verdict : A CORRIGER (3 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 2 | « matrice », « pivot », « `sfTransform` », « rect », « mapPixelToCoords » | Termes non définis ni liés au glossaire ; aucune explication de l'ordre d'application translation/rotation/échelle (juste « l'ordre compte »). | mineur |
| 2 | 6 | « Un ancien `{0, 0, 32, 32}` compile encore mais avec `-Wmissing-braces` (RESULTATS, test 26) » | Exact (test 26) mais l'avertissement n'apparaît qu'avec `-Wall` : à préciser. La page signale bien le changement `sfFloatRect` (JSON vérifié : position/size en 3.0, left/top/width/height en 2.6) mais pas `sfVideoMode`. | mineur |
| 3 | autre | « Les angles de rotation sont en degrés » | Vérifié (float en 3.0 et 2.6, JSON). Mais « 10,0 devient 100,70 » n'est pas expliqué (le lecteur ne peut pas le recalculer sans le code de l'exemple). | mineur |
Vérifié : tous les liens (reference/graphics/transformable, view, transform, render-window) existent ; signatures mapPixelToCoords / sfView_createFromRect identiques dans les deux JSON.
