# 3-0/migration/overview.mdx
Verdict : A CORRIGER (8 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | autre | Tableau §9 : ligne `sfGlFunctionPointer (était GlFunctionPointer)` | Colonne « Nouveau » vide et nom 3.0 placé dans « Ancien » : ligne incompréhensible. | gênant |
| 2 | autre | §7 : `sfRenderWindow_mapPixelToCoords(win, event.mouseButton.position, NULL)` | `NULL` comme vue : compilé seulement (RESULTATS-migration 4.4), jamais exécuté ; le header 3.0 ne dit pas que `NULL` est permis. Risque de plantage non vérifié. | gênant |
| 3 | 5 | §6 : `Lui passer NULL arrête le programme (assertion) ou plante` | Généralisé à text/sound alors que objects-creation dit que seul `sfSprite_create` a été testé. | gênant |
| 4 | 1/3 | Fin de page : `Le même programme dans les deux versions` (basic-window) | Le code importé inclut `test_mode.h`, absent du lecteur : copie non compilable. | gênant |
| 5 | 8 | `140 fonctions`, `16 extraits`, `les 8 « avant »`, `10 changements` | 140 recompté par script (OK) ; 16 et 8 non recomptés. | mineur |
| 6 | 2 | `union`, `littéral`, `initialisation à plat`, `élision`, `stencil`, `assertion`, `héritage` | Plusieurs termes sans lien glossaire. | mineur |
| 7 | 7 | `gcc le traite en erreur depuis la version 14` | Source research/04 (gcc 14 porting guide), non citée. | mineur |
| 8 | autre | `return 84;` dans les extraits | Code retour Epitech non expliqué. | mineur |
