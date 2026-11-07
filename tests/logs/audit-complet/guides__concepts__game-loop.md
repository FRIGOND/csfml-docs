# guides/concepts/game-loop.mdx
Verdict : A CORRIGER (3 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 2 / 6 | « renvoie `false` (3.0) ou `sfFalse` (2.6) » ; « `bool` ... `sfBool` » | OK (versions dites), mais les extraits de code de la page ne montrent aucun onglet : le bloc `while (sfRenderWindow_pollEvent(...))` est valable pour les deux, et c'est bien ce qui est dit. Rien à corriger, sauf que « frame », « file », « CPU », « synchro verticale » ne sont pas dans le glossaire lié. | mineur |
| 2 | 7 | « Sous Xvfb, la synchro verticale n'est pas supportée (message `Setting vertical sync not supported`) » | Source : RESULTATS n°33 (ok) ; mais « La doc SFML conseille de ne pas utiliser les deux ensemble (source : rapport, non retestée) » : source non nommée, pas de research/... cité. | mineur |
| 3 | 1 | « `example_frames()` vient d'un petit fichier de test » | L'exemple affiché contient `#include "test_mode.h"` / `example_frames()` que le lecteur ne peut pas compiler seul : le code complet n'est donc pas copiable tel quel et ce fichier n'est pas fourni. | gênant |
Vérifié : liens (/3-0/cookbook/window/events/, /2-6/cookbook/window/events/, events, time) existent. auto.md : sfBool/sfFalse signalés comme 2.6 uniquement, la page le dit. Mention « research/05 §2.1, marqué déduit » : fichier présent.
