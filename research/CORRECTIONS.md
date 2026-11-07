# CORRECTIONS au rapport de phase 1

Ce qui est testé localement prime sur le rapport. Une entrée par correction.

| # | Fichier | Rapport disait | Constat | Preuve |
|---|---|---|---|---|
| 1 | 01, 06, 08 | Epitech actuel : CSFML 3.0.0 (déduit) | Confirmé pour `epitest-docker:latest` | tests/RESULTATS.md A.2 |
| 2 | 01, 09 | L'ancien environnement Epitech serait « 2.6 » | `epitest-docker:ubuntu24` a CSFML **2.6.0** (SFML 2.6.1), headers dans `/usr/include/SFML` | tests/RESULTATS.md A.2 |
