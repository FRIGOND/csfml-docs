# guides/install/arch.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « sudo pacman -S csfml » | Ne dit pas de synchroniser la base (`-Syu`) ; sur Arch un `-S` seul peut échouer (404) ou créer un état incohérent. `sudo` non expliqué. | mineur |
| 2 | 3 | « pacman -Q csfml sfml » / « pkg-config --modversion csfml-graphics » | Sortie attendue non montrée pour pacman. Pour pkg-config : « Sur la machine de test (Ubuntu), elle affiche `3.0.0` » : RESULTATS.md (pkg-config) ne donne pas cette sortie exacte pour `--modversion`, et aucun n° de test. | mineur |
| 3 | 1 | « Compile depuis les sources : depuis les sources » | La page from-source ne liste que des noms de paquets apt ; aucun nom de dépendances pour Arch. | gênant |
| 4 | 2 | « dépôt `extra` », « dépend de », « headers », « pkg-config » | Non définis, pas de lien glossaire. | mineur |
Vérifié : les 3 liens internes existent ; versions Arch marquées « Non vérifié » avec référence research/01-versions.md §5 (fichier présent).
