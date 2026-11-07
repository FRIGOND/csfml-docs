# guides/troubleshooting/no-sound.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 | « Un `sfSound` garde un pointeur vers son `sfSoundBuffer` » ; « `sfSound_setVolume` » | Page commune non tabulée : en 3.0 `sfSound_create(buffer)`, en 2.6 `sfSound_create(void)` puis `setBuffer` (JSON). Non dit. | mineur |
| 2 | 7 | « Messages réels (3.0.0) : ... `Failed to open music from file` » | Pas de n° de test (le n°34 est cité plus bas pour l'audio sans périphérique seulement). | mineur |
| 3 | 2 | « thread », « périphérique audio », « stderr », « PulseAudio/ALSA », « formats » | Non définis. | mineur |
| 4 | 1 | « WSL sans son : non testé » | Pas de renvoi vers wsl.mdx pour la suite. | mineur |
Vérifié : liens (resources, cookbook sound-buffer-play 3.0 et 2.6, ownership, audio-without-device) existent ; n°34 conforme.
