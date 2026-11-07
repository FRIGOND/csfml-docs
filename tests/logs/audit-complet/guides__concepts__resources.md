# guides/concepts/resources.mdx
Verdict : A CORRIGER (6 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 / 7 | « Test réel (RESULTATS n°18) : `sfText_setString(text, "été")` ... 5 points de code dont quatre valent `U+FFFFFFFF` » | Test fait en 3.0.0 uniquement ; le titre « bug UTF-8 de sfText_setString » est affirmé sans dire ce qu'il en est en 2.6. | gênant |
| 2 | 1 | `static const uint32_t ETE[] = {0xE9, 't', 0xE9, 0};` | `#include <stdint.h>` absent de l'extrait ; `text` non défini ; ne dit pas dans quelle version c'est utilisable (3.0 : `sfChar32` = `uint32_t` OK, JSON). | mineur |
| 3 | 7 | « Messages réels observés avec des fichiers qui n'existent pas » | Aucun n° de test dans RESULTATS.md (vient de tests/logs/guides-agent.md). | mineur |
| 4 | 2 | « asset », « `stderr` », « dossier courant du processus », « point de code », « streaming », « glyphe » | Non définis ; « dossier courant » est expliqué, pas « processus ». | mineur |
| 5 | 1 | « Nos exemples construisent leurs chemins avec une petite fonction `example_asset` : par défaut `../../assets/<nom>` » | Fichier/fonction du dépôt inconnus du lecteur ; `tuffy.ttf` : où le trouver ? (examples/assets/ du dépôt, licence « à re-vérifier » dans RESULTATS). | gênant |
| 6 | 7 | « (fuite et lenteur : constat du rapport, non mesuré) » | Marqué honnêtement ; mais « rapport » inconnu du lecteur, pas de research/... cité. | mineur |
Vérifié : liens (ownership, audio-without-device, 3 cookbooks) existent ; `sfTexture_createFromFile(const char*, const sfIntRect*)` identique 3.0/2.6 (JSON) ; `sfChar32` 3.0 / `sfUint32` 2.6 dits dans la page.
