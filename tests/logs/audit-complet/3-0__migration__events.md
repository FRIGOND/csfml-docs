# 3-0/migration/events.mdx
Verdict : A CORRIGER (6 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | autre | `shoot(...)`, `resize(...)`, `use(1, 1)`, `start()` | Fonctions imaginaires dans les extraits, non signalées ; `win`, `paused` non déclarés. | mineur |
| 2 | 2 | `union`, `timeout`, `alias`, `Xvfb` | Pas de lien vers le glossaire (union, Xvfb y figurent). | mineur |
| 3 | 5 | « Les correspondances ... viennent des headers, pas du guide C++ de SFML qui contient deux erreurs » | Affirmation sans source citée dans la page. | mineur |
| 4 | 7 | `pas de souris sous Xvfb` / `compilent la 2.6 avec -Wno-error=...` « de ce dépôt » | Bloc marqué Non vérifié : OK. « ce dépôt » non défini pour le lecteur. | mineur |
| 5 | 8 | `environ 207 ms`, `plus de 3 secondes` | Conforme au test n°17, numéro non cité. | mineur |
| 6 | autre | Tableau types d'événements | Oublie que `sfMouseButtonCount` (2.6) disparaît ; sans conséquence pour un débutant. | mineur |
