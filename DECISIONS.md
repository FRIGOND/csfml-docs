# DECISIONS

Journal des décisions prises pendant l'implémentation (phase 2 et suivantes).

- 2026-11-07 : dépôt git initialisé à la racine ./lib_csfml (branche main). research/ y est versionné.
- 2026-11-07 (phase A) : SFML 2.6.2 choisi pour construire CSFML 2.6.1 (dernière 2.6.x), alors que le paquet Ubuntu a SFML 2.6.1 et CSFML 2.6.0.
- 2026-11-07 : Xvfb et pkgconf installés sans sudo depuis des .deb extraits dans `~/local/root` (`apt-get download` + `dpkg -x`) ; binding Python libclang = roue PyPI extraite dans `~/local/py` (pas de venv ni pip).
- 2026-11-07 : l'accès docker étant refusé, la version CSFML d'Epitech est lue dans les couches de l'image via l'API du registre Docker Hub.
- 2026-11-07 (phase B) : chaque exemple = `examples/<module>/<nom>/{Makefile,meta.json,3.0/main.c,2.6/main.c}` ; `-Wall -Wextra -Werror` partout, sauf `-Wno-error=deprecated-declarations` en 2.6 (le header 2.6.1 `Event.h` émet un avertissement de dépréciation à chaque inclusion). Mode test : `EXAMPLE_FRAMES=N`. Les exemples 2.6 évitent les fonctions dépréciées (ex. `sfRenderWindow_capture` → `sfTexture_updateFromRenderWindow`).
- 2026-11-07 (phase C) : **Node 24.22.2** installé dans `~/local/node24` (checksum vérifié). Le rapport disait « Astro 7 exige Node 24 » : en réalité Astro 7.4.7 exige Node ≥ 22.23.4 (test : Node 22.22.1 refusé).
- 2026-11-07 (phase C) : **URLs de version `/3-0/` et `/2-6/`** (pas `/3.0/`) : Starlight/Astro supprime le point des slugs (`3.0` devient `30`). Les deux versions sont symétriques (poids égal), pas de version « à la racine » comme proposé dans research/08 §6.
- 2026-11-07 (phase C) : sélecteur de version = plugin `starlight-sidebar-topics` (deux « topics » CSFML 3.0 / CSFML 2.6 en haut de la barre latérale), liens « même page en 2.6 / 3.0 » écrits dans les pages jumelles. Composants maison : `ApiFunction` (fiche générée depuis `api/csfml-*.json`, **le build échoue si la fonction n'existe pas**), `Example` (inclut `examples/**/main.c`, échoue si absent), `TestedBadge` (valeurs dans `site/src/data/tested.json`).
- 2026-11-07 : alias `@/` (tsconfig) pour importer les composants depuis les `.mdx` (les chemins relatifs dépendent de la profondeur).
- 2026-11-07 (phase B) : les `meta.json` listent parfois des noms 3.0 et 2.6 mélangés (ex. `setLooping` / `setLoop`) ; en phase D, séparer `functions` par version (`functions_3_0`, `functions_2_6`) pour vérifier chaque liste contre le bon `api/*.json`.
- 2026-11-07 (phase B) : comportements différents 2.6 / 3.0 sans périphérique audio : 3.0 → `sfMusic_play` donne Playing ; 2.6 → Stopped, `sfSoundBuffer_getSampleRate` = 0. `sfIpAddress_fromString` invalide : 3.0 chaîne vide, 2.6 `0.0.0.0`. `sfHttpRequest` : aucun getter (2 versions). Source : tests/logs/examples-system-audio-network.md.
