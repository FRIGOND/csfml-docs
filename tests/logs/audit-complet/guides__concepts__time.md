# guides/concepts/time.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | `float dt = sfTime_asSeconds(sfClock_restart(clock)); pos.x += SPEED * dt;` | `clock`, `pos`, `SPEED` non déclarés dans l'extrait ; seul « tel que dans l'exemple ci-dessous » renvoie à l'exemple. | mineur |
| 2 | 6 | « `sfClock_reset` n'existe pas en 2.6.1 » | Exact (JSON). Mais ne dit pas que la version à onglets du reste (exemple) n'a pas de variante 2.6 pour reset ; ok. Aussi `sfMilliseconds(int32_t / sfInt32)` exact. Rien à corriger sauf la référence à « `api/csfml-2.6.1.json` » : fichier du dépôt, inconnu du lecteur. | mineur |
| 3 | 7 | « (conseil du rapport, non testé) » | Marqué, mais « rapport » non défini et pas de research/... cité. | mineur |
| 4 | 8 | « écart moins de 0,1 s » | Seuil écrit en dur dans la page, copié du code de l'exemple : à vérifier contre examples/graphics/clock-delta-time si le code change. | mineur |
Vérifié : liens (cookbook system/clock-time, reference/system/clock) existent ; n°87 existe dans RESULTATS.md ; sfClock_reset (header : remet à zéro, renvoie le temps, pause) conforme au JSON 3.0.
