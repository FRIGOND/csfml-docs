# guides/epitech/before-you-submit.mdx
Verdict : A CORRIGER (7 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 7 | « L'image actuelle a **CSFML 3.0.0** » (section 2, titre « Même version de CSFML que le correcteur ») | Phrase interdite : affirme la version du correcteur. environment.mdx dit lui-même « Je ne sais pas quelle image utilise ton correcteur ». Contradiction interne. | bloquant |
| 2 | 7 | « Si tu développes avec la 2.6, ton code ne compilera pas » | Conséquence affirmée pour le correcteur, sans preuve ; dépend de l'image (ubuntu24 = 2.6.0). | gênant |
| 3 | 1 | « valgrind --leak-check=full ./ton_programme » | Installation de valgrind et compilation avec `-g` non dites. « tests/valgrind.supp » est un fichier du dépôt du site, pas du lecteur. | gênant |
| 4 | 1 | « banana-check-repo . » | Ne dit pas d'où vient l'outil (installé avec le paquet banana : sur quelle machine ?) ni ce qu'il affiche en cas de succès. | gênant |
| 5 | 3 | « git clone <ton-dépôt> /tmp/test_rendu » puis « make && ./ton_programme » | Pour une lib sans `main` (cas du lecteur : livraison sans main) `./ton_programme` n'existe pas ; ne dit pas de construire `lib/my` avant ; `<ton-dépôt>` non expliqué. | gênant |
| 6 | 2 | « snake_case », « still reachable », « FreeType », « assets », « flags » | Non définis, pas de lien glossaire. | mineur |
| 7 | 7 | « Le "still reachable" de plusieurs centaines de Ko vient des bibliothèques graphiques » | Sans n° de test ni marque (memory.mdx cite un chiffre mal sourcé). | mineur |
Vérifié : liens (makefile, environment, check-your-setup, memory, resources) existent. C-O1/C-O4 : cités comme « vu en test » sans n° RESULTATS (source tests/logs/guides-agent.md).
