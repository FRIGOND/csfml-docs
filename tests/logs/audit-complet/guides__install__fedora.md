# guides/install/fedora.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « Les paquets `-devel` des dépendances (X11, freetype, FLAC, vorbis, OpenGL, udev) doivent être installés ; leurs noms exacts ... ne sont pas vérifiés » | L'étape « obtenir la 3.0 » est impossible pour un débutant : aucun nom de paquet dnf, même hypothétique ; et on ne dit pas d'installer cmake/git/g++. | gênant |
| 2 | 1 | « sans `sudo` pour l'installation » | Les paquets -devel exigent `sudo` : contradiction pratique avec le promis « sans sudo ». | mineur |
| 3 | 2 | « libdir », « lib64 », « `.pc` », « CMake », « -devel » | Non définis, aucun lien glossaire. | mineur |
| 4 | 4/1 | « (c'est ce que fait notre script) » | `tools/build-csfml.sh` du dépôt jamais lié ni nommé ; le lecteur n'a pas le dépôt. | mineur |
| 5 | 7 | « Fedora 43 et suivantes fournissent **CSFML 2.6.1** » | Source research/01 §5 citée + encadré Non vérifié : conforme. Seul bémol : le « piège lib64 » est affirmé avant l'encadré dans une liste à puces comme un fait. | mineur |
Vérifié : liens (api-conventions, from-source, compiling, check-your-setup, /2-6/getting-started/first-window/) existent. Le renvoi « tests/RESULTATS.md, A.2 » (pas d'image Fedora) est exact.
