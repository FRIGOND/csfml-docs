# guides/troubleshooting/headless-xvfb.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « Xvfb :99 -screen 0 1024x768x24 -nolisten tcp & » | Installation de Xvfb (`sudo apt install xvfb`) seulement dans l'encadré sur xvfb-run ; pas de commande pour arrêter Xvfb (`kill`) ni gestion du cas « :99 déjà utilisé ». | gênant |
| 2 | 3 | Bloc `export DISPLAY=:99` | Ne dit pas que les `export` ne valent que pour ce terminal. | mineur |
| 3 | 7 | « sous Wayland, ça passe par XWayland » | Non testé sur vrai Wayland (RESULTATS) ; ici sans marque (la FAQ en a une). Test « lot 4 » sans n°. | mineur |
| 4 | 2 | « DISPLAY », « X11 », « backend », « Mesa », « SSH », « CI », « stderr », « framebuffer » | Non définis. | mineur |
| 5 | 6 | « Les deux messages SFML ont été vus avec les deux versions » | Les messages de la 2.6 ne sont pas montrés séparément ; RESULTATS n°33 ne confirme que « Failed to create input context ... Setting vertical sync » en général. | mineur |
Vérifié : liens (check-your-setup, audio-without-device, black-window) existent ; n°17, n°33 existent ; comportement abort exit 134 conforme au n°32.
