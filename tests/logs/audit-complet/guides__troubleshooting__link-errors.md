# guides/troubleshooting/link-errors.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 3 | « `nm -D /usr/lib/x86_64-linux-gnu/libcsfml-system.so | grep sfClock_create` » | Chemin propre à x86_64 Debian/Ubuntu (dit « ajuste ») ; sortie attendue non montrée. | mineur |
| 2 | 7 | « Message exact vu en test, exit 127 » ; « test réel : sans `-rpath`, la lib du système a été prise » | Aucun n° dans RESULTATS.md (n°10 couvre seulement le cas avec rpath). | mineur |
| 3 | 2 | « éditeur de liens », « rpath », « `RUNPATH` », « `libm` », « `readelf` », « préfixe », « `.so` » | Non définis ici (glossaire non lié). | gênant |
| 4 | 1 | « 3. **Mauvais `-L`.** La lib n'est pas là où tu le dis. » | Aucune manière de la trouver (`find`, `ldconfig -p`) ni solution. | mineur |
Vérifié : liens (compiling, from-source, check-your-setup) existent ; n°5 et n°6 conformes à RESULTATS.
