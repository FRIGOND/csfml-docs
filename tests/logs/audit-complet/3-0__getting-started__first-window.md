# 3-0/getting-started/first-window.mdx
Verdict : A CORRIGER (5 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | `<Example example="window/basic-window" version="3.0" />` puis `gcc ... -o app main.c ...` | Même problème : le main.c importé inclut `"test_mode.h"`, absent de la commande de compilation (pas de `-I`) : échec `test_mode.h: No such file`. Non signalé. Aucune étape « crée main.c ». | bloquant |
| 2 | 6 | Bloc `Différence avec la 2.6` | Liste create à 5 paramètres et `{{800, 600}, 32}` mais oublie la différence d'include (`<CSFML/Graphics.h>` contre `<SFML/Graphics.h>` en 2.6), pourtant visible dans le code importé. | gênant |
| 3 | 2 | `NULL`, « événements », `sfWindowState` | `sfWindowState` mentionné sans dire ce que c'est (fenêtre ou plein écran) ; événement/pollEvent sans lien vers le glossaire. | mineur |
| 4 | autre | `import { Tabs, TabItem }` | Import inutilisé (aucun Tabs dans la page). Sans effet pour le lecteur. | mineur |
| 5 | 3 | Bloc `sh` | Sortie attendue / commande pour lancer `./app` absentes ; `libcsfml-dev` à installer avant : renvoi vers /guides/install/ absent. | mineur |
