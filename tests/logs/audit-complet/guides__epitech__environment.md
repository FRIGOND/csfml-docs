# guides/epitech/environment.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « Si Docker est installé et que tu es dans le groupe `docker` : » | Ni installation de Docker ni commande pour rejoindre le groupe ; sortie attendue de `dpkg -l` non montrée ; « voir plus bas » renvoie à une phrase vague. | gênant |
| 2 | 2 | « Docker », « image », « couches », « registre », « Dockerfile », « `dpkg/status` » | Seul « conteneur » est défini ; pas de lien glossaire. | mineur |
| 3 | 7 | « Epitech publie des images **Docker** » ; « Le Dockerfile public fixe `CC=clang` » | Que `epitechcontent` soit l'organisation officielle d'Epitech n'est pas prouvé ; CC=clang vient de research/01 §6 mais l'énoncé n'est pas marqué non vérifié ici (il l'est dans makefile.mdx). | mineur |
| 4 | 5 | Page vs before-you-submit | environment : « Je ne sais pas quelle image ... utilise ton correcteur » ; before-you-submit : « L'image actuelle a CSFML 3.0.0 ». Incohérent. | gênant |
| 5 | 8 | « 3.0.0-1 », « 2.6.0-3build1 », « gcc 15.2 » | Versions datées 2026-11-07 écrites en dur, non recalculées par script (source RESULTATS A.2 correcte). | mineur |
Vérifié : liens (api-conventions, check-your-setup, from-source, migration/overview) existent ; données du tableau conformes à RESULTATS A.2 ; aucune phrase « Epitech utilise la version X » (formulation prudente).
