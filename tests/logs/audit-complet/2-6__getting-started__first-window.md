# 2-6/getting-started/first-window.mdx
Verdict : A CORRIGER (5 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `<Example example="window/basic-window" version="2.6" />` puis `gcc -std=c11 -Wall -Wextra -o app main.c -lcsfml-graphics ...` | Le main.c importé contient `#include "test_mode.h"` (outil du dépôt) : copié tel quel puis compilé avec la commande donnée, gcc échoue (`test_mode.h: No such file`). La page n'en parle pas, ni de `example_frames()`. Aucune étape « crée main.c ». | bloquant |
| 2 | 6 | Page entière | Jumelle 3.0 a une section « Ce que fait le programme » et une note de différences ; cette page n'explique ni `NULL`, ni la boucle, ni `{800, 600, 32}`. Include `<SFML/...>` non signalé. | gênant |
| 3 | 2 | `type is deprecated` ... `sfMouseWheelEvent` | Terme « deprecated » (= dépassé, à ne plus utiliser) non expliqué, pas de lien vers le glossaire (/guides/help/glossary/ a « deprecated »). | mineur |
| 4 | 1 | `-Werror` | Non expliqué ; le texte parle de `-Wall` alors que la commande contient aussi `-Wextra`. | mineur |
| 5 | 3 | Bloc `sh` de compilation | Dossier d'exécution et sortie attendue non précisés ; aucune ligne « lancer ./app ». | mineur |
