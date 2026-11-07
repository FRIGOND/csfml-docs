# guides/install/wsl.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « permet de suivre les pages Linux de ce site » | Comment installer WSL (`wsl --install`), quelle distribution (Ubuntu ?), quelle version Windows : rien. Quelle version d'Ubuntu par défaut donc quelle CSFML (24.04 = 2.6.0, 26.04 = 3.0.0) : rien. | gênant |
| 2 | 3 | « `echo $DISPLAY` doit afficher quelque chose comme `:0` » | Ne dit pas que c'est dans le terminal WSL (pas PowerShell/cmd). | mineur |
| 3 | 2 | « WSLg », « serveur d'affichage », « OpenGL », « PulseAudio », « GPU », « `/mnt/c` » | Non définis (seul WSL est défini). | gênant |
| 4 | 7 | « Pour ouvrir une fenêtre, il faut un serveur d'affichage. **WSLg** (fourni avec WSL2 récent) ... WSL1 n'a pas WSLg » | Sous un titre « (non vérifié) » + encadré : conforme. Ces affirmations restent invérifiées (RESULTATS : WSL non testé). | mineur |
| 5 | 4 | « `research/05-pieges.md` §5.3 » | Le fichier existe ; non lié depuis le site (lecteur sans le dépôt). | mineur |
Vérifié : liens (ubuntu-debian, about, no-sound, check-your-setup, headless-xvfb) existent.
