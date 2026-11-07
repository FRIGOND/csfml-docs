# guides/install/index.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 2 | « Choisis la version de **ton environnement de rendu** » | « rendu » (= ce qu'on rend/delivery d'Epitech) non défini ; aucun lien glossaire. Idem « Homebrew », « Repology », « Launchpad », « paquet », « dépôt ». | gênant |
| 2 | 5 | « Versions annoncées pour Fedora, Debian 13 et Ubuntu 24.04 (2.6.0) : non vérifié » | Contredit en partie le paragraphe suivant (« les deux images Epitech ... ont été vérifiées ») : la 2.6.0 d'Ubuntu 24.04 a été lue dans l'image ubuntu24 (RESULTATS A.2). | mineur |
| 3 | 7 | « Version contenue dans l'image publique `epitest-docker:latest` lue le 2026-11-07 ... Quelle image utilise ton correcteur : non vérifié. » | Conforme (source A.2, marque non vérifié). Aucun problème d'interdit. | mineur |
| 4 | 3 | « `dpkg -l libcsfml-dev` donne `3.0.0-1` » | Commande donnée sans dire qu'elle ne marche que sous Debian/Ubuntu. | mineur |
| 5 | 6 | Tableau « Booléens : `bool` / `sfBool` » | Correct (test n°16). Le tableau devrait aussi dire que la 3.0 exige `sfWindowState` ; renvoi vers api-conventions OK. | mineur |
Vérifié : tous les liens (/guides/epitech/environment/, api-conventions, /3-0/migration/overview/, les 6 cartes, compiling, check-your-setup) existent. Versions du tableau : recoupées avec ubuntu-debian, arch, fedora, macos (cohérentes).
