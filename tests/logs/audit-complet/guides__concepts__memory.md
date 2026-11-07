# guides/concepts/memory.mdx
Verdict : A CORRIGER (7 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « valgrind --leak-check=full ./app » | Ne dit pas d'installer valgrind (`sudo apt install valgrind`) ni de compiler avec `-g` (sinon pas de numéros de ligne). `./app` supposé déjà compilé. | gênant |
| 2 | 7 | « Résultat mesuré (test n°27, tests/RESULTATS.md) ... `still reachable: 259,204 bytes in 3,671 blocks` » | Faux renvoi : RESULTATS n°27 parle de fenêtre+sprite+texte (1 646 octets perdus), pas de 259 204 octets. Ce chiffre vient de tests/logs/guides-agent.md. De plus « vient des bibliothèques système (OpenGL, X11) » est une affirmation sans preuve. | gênant |
| 3 | 8 | « les 40 fonctions `*_destroy` », « 41 des 42 fonctions », « 259,204 ... 3,671 », « 1 646 octets en 8 blocs » | Chiffres écrits en dur, non recomptés par un script (40/42 viennent des tests n°62/91). Format mixte « 259,204 » / « 259 204 ». | mineur |
| 4 | 1 | « valgrind --leak-check=full --suppressions=tests/valgrind.supp ./app » | `tests/valgrind.supp` est un fichier du dépôt ; le lecteur ne l'a pas (ni contenu ni lien). | gênant |
| 5 | 2 | « Mesa / amdgpu », « FreeType », « still reachable », « definitely lost », « `-DNDEBUG` », « SIGSEGV », « assert », « Release » | Non définis, aucun lien glossaire. | gênant |
| 6 | autre | « mets tes pointeurs à `NULL` seulement après avoir testé qu'ils ne le sont pas déjà » | Phrase incompréhensible/contradictoire (on met à NULL après le destroy). | mineur |
| 7 | 6 | « les 40 fonctions ... (paquet Ubuntu et build local, n°62) » | Résultats 3.0.0 vs 2.6.1 bien distingués. Mais les « asserts » (dernier paragraphe) ne sont décrits qu'en 3.0 ; rien pour 2.6. | mineur |
Vérifié : liens (ownership, headless-xvfb, runtime-crashes) existent ; n°33, n°62, n°91 existent dans RESULTATS.md.
