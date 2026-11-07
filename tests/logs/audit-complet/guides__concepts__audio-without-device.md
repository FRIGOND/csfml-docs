# guides/concepts/audio-without-device.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 3 | « On rend PulseAudio et ALSA inaccessibles : `PULSE_SERVER=unix:/nonexistent ALSA_CONFIG_PATH=/dev/null XDG_RUNTIME_DIR=/nonexistent`. » | Variables d'environnement données sans la ligne de commande complète (`VAR=... ./app`), sans dossier ni prérequis. | gênant |
| 2 | 7 | « Tous les exemples audio du dépôt sortent alors avec le code 0 (tests/logs/examples-system-audio-network.md) » ; table « Résultats » | Source = un log, pas un n° de RESULTATS.md (le n°34 couvre le même cas mais n'est pas cité). Lecteur sans le dépôt ne peut pas le vérifier. | mineur |
| 3 | 2 | « PulseAudio », « ALSA », « miniaudio », « stderr », « backend », « périphérique », « CI », « Xvfb », « le rapport de recherche » | Non définis, pas de lien glossaire ; « le rapport » inconnu du lecteur. | gênant |
| 4 | 8 | « (deux fois par objet) », « 44100 Hz, 2 canaux », « renvoie 0 au lieu de 40 réglé » | Valeurs mesurées une fois, en dur, non recomptées par un script ; la ligne « sfMusic_getVolume (non noté) » en 3.0 montre une mesure incomplète. | mineur |
| 5 | 6 | Tableau : « `sfMusic_getStatus` après play : 2 (Playing) / 0 (Stopped) » | Valeurs numériques d'enum au lieu des noms (`sfPlaying`, `sfStopped`) : un débutant ne sait pas les relier ; valeurs non vérifiées dans le JSON ici. | mineur |
Vérifié : liens (no-sound) existent ; research/CORRECTIONS.md n°7 existe et dit bien que « Using NULL audio backend » n'a pas été observé ; l'exemple audio/sound-no-device existe en 3.0 et 2.6.
