# guides/epitech/coding-style.mdx
Verdict : A CORRIGER (6 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « epiclang -c fichier.c -o /dev/null » | Ne dit pas où se trouve/s'installe `epiclang` (présent sur la machine de test, pas forcément chez le lecteur). | gênant |
| 2 | 6 | « `sfDefaultStyle, sfWindowed, NULL);` sur la ligne suivante » | `sfWindowed` n'existe qu'en 3.0 (JSON ; auto.md) : page commune, non signalé (en 2.6 l'appel a 4 arguments). | mineur |
| 3 | 5 | « Les exemples du dépôt respectent ... fonctions de 20 lignes ou moins » | Le `main` de examples/window/basic-window/3.0/main.c compte 21 lignes de corps (mesure ici) : à recontrôler ; et la règle est vérifiée par un script maison, pas par `epiclang`. | mineur |
| 4 | 8 | « 20261006101948 » (version banana) | Valeur en dur propre à une machine ; peut changer. | mineur |
| 5 | 2 | « norme », « greffon », « snake_case », « static », « garde d'inclusion » | Non définis ; aucun lien glossaire. | mineur |
| 6 | 7 | « Chaque ligne a été provoquée volontairement » | Source = tests/logs/guides-agent.md, pas un n° de RESULTATS.md ; marque « Non vérifié » présente en tête (acceptable). | mineur |
Vérifié : lien /3-0/cookbook/window/events/ et before-you-submit existent.
