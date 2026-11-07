# Exemples system / audio / network — compte rendu

15 exemples x 2 versions (3.0.0 et 2.6.1) : `tools/run-all-examples.sh --only <module>/` OK (30/30), `--valgrind` OK (aucune fuite).
Chaque fonction utilisée a été vérifiée dans `api/csfml-<version>.json` (existence, non dépréciée, signature) ; meta.json contrôlé par script (fonctions du code = fonctions du meta).

Exemples : system/{clock-time,vectors,input-stream}, audio/{sound-buffer-play,music-stream,listener-spatial,sound-no-device,sound-recorder-capture}, network/{ip-address,udp-loopback,tcp-loopback,packet,http-parse-only}.

## Surprises / différences
- **`sfIpAddress_fromString` invalide** : 3.0.0 renvoie une adresse à octets nuls, `toString` donne une chaîne **vide** ; 2.6.1 donne `"0.0.0.0"` (sfIpAddress_None). L'exemple teste les deux comportements séparément.
- **Pas de getters sur `sfHttpRequest`** (ni en 2.6 ni en 3.0) : impossible de relire méthode/URI depuis l'objet. L'exemple http-parse-only construit la requête et affiche ce qu'on y a mis (texte fixe) ; aucun accès réseau (`sfHttp_sendRequest` jamais appelé).
- **`sfIpAddress_toString`** : buffer de 16 octets suffit (`255.255.255.255\0`), vérifié (cf. RESULTATS n°78).
- **Port libre** : 3.0 a `sfUdpSocket_anyPort()` / `sfTcpListener_anyPort()` ; 2.6.1 n'a pas ces fonctions → on passe `0` (valeur « any port » de SFML). `getLocalPort` donne le port choisi.
- **Audio sans périphérique** (testé avec `PULSE_SERVER=unix:/nonexistent ALSA_CONFIG_PATH=/dev/null XDG_RUNTIME_DIR=/nonexistent`) : tous les exemples audio sortent 0.
  - 3.0.0 : messages miniaudio `[PulseAudio] Failed to connect PulseAudio context`, `Failed to get audio playback devices: Resource does not exist`, `Failed to initialize sound: No engine available`, + bruit JACK ; `sfMusic_play` -> statut 2 (Playing) quand même ; `getPlayingOffset` reste 0 (`Failed to get sound cursor: Invalid operation`) ; durée/échantillons/canaux du buffer corrects (44100 Hz, 2 canaux) ; `sfSoundRecorder_isAvailable` reste vrai.
  - 2.6.1 : `Failed to open the audio device` ; `sfMusic_play` -> statut 0 (Stopped) ; `sfSoundBuffer_getSampleRate`/`getChannelCount` renvoient **0** (buffer sans contexte) ; `sfMusic_getVolume` renvoie 0 (au lieu de 40 réglé) ; `sfSound_getPosition`/`getMinDistance` renvoient 0 ; `sfListener` (global) reste correct ; `sfMusic_setPlayingOffset(200ms)` relu = 200 ms sans arrêt puis non remis à 0 par stop.
  - 2.6.1 : `sfSoundRecorder_isAvailable` = vrai mais `sfSoundBufferRecorder_start` échoue (« Failed to open the audio capture device ») -> l'exemple affiche et sort 0 dans ce cas.
  - Conséquence : listener-spatial n'impose pas la relecture de la position du son (sinon exit 1 en 2.6 sans périphérique) ; il vérifie seulement la position de l'auditeur.
- Avec périphérique (ou backend nul) : musique/sons ont statut 2 pendant la lecture, offset avance (~90 ms après 100 ms).
- `input-stream` : 3.0 callbacks `int64_t read(void*, size_t, void*)`, `seek(size_t, void*)` ; 2.6 `sfInt64` partout (paramètres taille/position en `sfInt64`). sfml_logo.png = 373 x 113 dans les deux versions.
- Les listes `functions` des meta.json mélangent 3.0 et 2.6 quand les noms diffèrent (`sfMusic_setLooping`/`setLoop`, `sfSound_create`+`setBuffer`, `anyPort`).
- Le Makefile commun recompile avec `re` : `run-all-examples.sh` ne laisse que la dernière version dans `build/`. Le script écrit aussi `tests/logs/examples.log`.

## Vérifications faites par exemple
- clock-time : conversions (1.5 s = 1500 ms = 1500000 us), `sfClock_restart` renvoie le temps écoulé (>= 45 ms après sfSleep 50 ms), le temps écoulé après restart est plus petit.
- vectors : sum, norme (sqrtf), produit vectoriel 3D écrits à la main ; vecteurs passés par valeur.
- input-stream : lecture complète de sfml_logo.png via callbacks.
- sound-buffer-play : attente max 300 ms ; son détruit avant buffer.
- udp/tcp-loopback : 127.0.0.1, port libre, sockets non bloquantes + 100 essais x 5 ms max (aucun blocage possible) ; socket renvoyée par accept détruite.
- packet : Int32/Float/String/Bool, `endOfPacket` vrai après lecture ; exit 1 si faux.
