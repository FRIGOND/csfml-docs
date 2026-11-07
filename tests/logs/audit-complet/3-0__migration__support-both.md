# 3-0/migration/support-both.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 1/3 | Bloc `gcc ... -I$HOME/csfml-3.0/include main.c -o app3 ...` | Suppose `compat.h` et `main.c` dans le dossier courant et des préfixes `~/csfml-3.0` / `~/csfml-2.6` (voir from-source, non lié) ; cas du paquet système (sans `-I`) non traité. | gênant |
| 2 | 1 | `compat.h` / `main.c` | Aucune consigne pour créer les fichiers ; `main.c` ne montre pas où est `compat.h`. | mineur |
| 3 | 7 | `testé : aucune erreur` (piège __has_include), « ont été compilés et exécutés » | Test non numéroté dans RESULTATS (voir tools/test-snippets.py) ; préciser. | mineur |
| 4 | 2 | `static inline`, `préfixe`, `garde-fou`, `rpath` | Termes non définis ni liés (rpath est au glossaire). | mineur |
| 5 | 7 | `gcc 15 sous Linux` | Version de test, correct comme limite ; distribution non indiquée. | mineur |
| 6 | autre | `app_event_x(...)` | Exemple annoncé non compilé : signalé honnêtement. | mineur |
