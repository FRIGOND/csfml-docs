# guides/troubleshooting/black-window.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 | « `sfTexture_updateFromRenderWindow` (en 3.0 et 2.6) » | Signature différente (JSON) : 3.0 `(texture, window, sfVector2u offset)`, 2.6 `(texture, window, unsigned x, unsigned y)`. Non signalé. | gênant |
| 2 | 3 | « `LIBGL_ALWAYS_SOFTWARE=1 ./app` » | Effet attendu non décrit ; « WSL, machine virtuelle » non testé (couvert par l'encadré). | mineur |
| 3 | 2 | « champ de vision », « rectangle de texture », « Mesa », « OpenGL », « Xvfb » | Non définis (glossaire non lié). | mineur |
| 4 | 7 | « Exemple vu en test 3.0.0 : `Failed to load image` ... » | Pas de n° de test. | mineur |
Vérifié : liens (headless-xvfb, game-loop, coordinates-views, resources, runtime-crashes, cookbook shapes 3.0 et 2.6) existent ; n°33 existe ; marque « Non vérifié » présente.
