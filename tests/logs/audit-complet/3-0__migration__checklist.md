# 3-0/migration/checklist.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `sed -i 's|#include <SFML/|#include <CSFML/|' src/*.c include/*.h` | Suppose une arborescence `src/` + `include/` à la racine ; `sed -i` modifie les fichiers sans retour (la page dit « copie » mais pas « git »). Dossier de lancement non précisé. | gênant |
| 2 | 3 | `ls /usr/include/CSFML` | Sortie attendue non donnée ; ne marche que pour un paquet système. | mineur |
| 3 | 2 | `gcc 15`, `valgrind --leak-check=full`, `FreeType`, « -Wmissing-braces » | Termes non définis ; messages en français (`Aucun fichier ou dossier`) selon la locale. | mineur |
| 4 | 8 | `16 extraits ... 8 « avant » ... 8 « après »` et `1 646 octets en 8 blocs` | Chiffres cohérents avec RESULTATS (n°26, n°27) mais non recomptés par script ni numéros cités. | mineur |
| 5 | 7 | `Avec gcc 14 ou plus récent ... est une erreur` | Conforme à research/04 et test n°86, sans renvoi explicite. | mineur |
| 6 | autre | Étape 8 | « avec `grep` » : aucune commande donnée. | mineur |
