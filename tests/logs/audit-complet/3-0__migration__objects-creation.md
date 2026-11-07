# 3-0/migration/objects-creation.mdx
Verdict : A CORRIGER (5 problèmes)

| # | Catégorie (1-8 ou autre) | Citation | Problème | Gravité (bloquant/gênant/mineur) |
|---|---|---|---|---|
| 1 | 7 | `avec le paquet Ubuntu ... build maison en Release` | Mesure du test n°31 (Ubuntu 26.04.1) ; numéro et version d'Ubuntu absents ; « Mesures (tests locaux) » seulement. | gênant |
| 2 | 5 | Tableau « Si tu passes NULL » : `sfText_create` / `sfSound_create` : `idem` | Présenté comme acquis dans le tableau, alors que le bloc « Non vérifié » dit que ce n'est pas exécuté ; overview.mdx §6 affirme aussi sans réserve. | gênant |
| 3 | 2 | `assertion`, `build Release`, `segfault`, `carte des canaux`, `return 84` | Non définis (assert est au glossaire) ; `84` (code Epitech) non expliqué. | mineur |
| 4 | autre | `duck.png`, `shot.ogg`, `arial.ttf` | Fichiers imaginaires, non signalés ; extraits « partiels » non compilables seuls. | mineur |
| 5 | 8 | `40 fonctions testées` / `une fonction sur 42` | Conforme aux tests n°62/n°91 et audit (40/42), numéros non cités. | mineur |
