# guides/install/check-your-setup.mdx
Verdict : A CORRIGER (6 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « gcc -std=c11 -Wall -Wextra -o check check.c » | Ne dit pas d'enregistrer le programme sous `check.c`, ni dans quel dossier ; ni qu'il faut CSFML installée (aucun lien vers /guides/install/ en début de page). | gênant |
| 2 | 1 / 3 | « Fenetre : OK » | La fenêtre s'ouvre puis se ferme aussitôt (pas de boucle) : jamais dit. Sans bureau graphique il faut Xvfb mais la commande `xvfb-run` n'est donnée nulle part ici (renvoi seulement). | gênant |
| 3 | 7 | « Sortie vérifiée (préfixe local 3.0.0 et paquet système) : `CSFML 3.0.0` puis `Fenetre : OK` » | Aucun n° de test dans tests/RESULTATS.md pour ce programme (grep « Fenetre : OK » dans tests/ et tools/ : rien). Idem « j'ai d'abord essayé `__has_include(<CSFML/Config.h>)` ... il a choisi la 3.0 par erreur » : pas de n°. | gênant |
| 4 | 2 | « `__has_include` », « macros », « header de config », « préfixe », « `-Wl,-rpath` », « Xvfb » | Non définis ni liés au glossaire ; le bloc `#if ... #define CHECK_IS_V3` n'est pas expliqué ligne par ligne. | mineur |
| 5 | autre | Onglet 2.6 : « gcc ... -Wno-error=deprecated-declarations » | Sans `-Werror` dans la commande, l'option ne sert à rien : incohérent avec le texte « avec un avertissement deprecated ». | mineur |
| 6 | 7 | « Ubuntu 26.04 » (encadré final) | OK (RESULTATS A.1) mais « Sur macOS, Windows ou WSL, il n'a pas été essayé » n'est pas relié aux pages wsl/macos. Mineur. | mineur |
Vérifié : liens existants (api-conventions, compiling, headless-xvfb, compile-errors, link-errors). Le code montre bien les deux signatures de `sfRenderWindow_create` (JSON : 5 args en 3.0, 4 en 2.6 ; auto.md confirme).
