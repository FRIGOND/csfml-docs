# CORRECTIONS au rapport de phase 1

Ce qui est testé localement prime sur le rapport. Une entrée par correction.

| # | Fichier | Rapport disait | Constat | Preuve |
|---|---|---|---|---|
| 1 | 01, 06, 08 | Epitech actuel : CSFML 3.0.0 (déduit) | Confirmé pour `epitest-docker:latest` | tests/RESULTATS.md A.2 |
| 2 | 01, 09 | L'ancien environnement Epitech serait « 2.6 » | `epitest-docker:ubuntu24` a CSFML **2.6.0** (SFML 2.6.1), headers dans `/usr/include/SFML` | tests/RESULTATS.md A.2 |
| 3 | 04 §3 point 4, A-TESTER n°29 | `sfVideoMode m = {800, 600, 32}` « compile mais donne un mauvais résultat » | Faux : compile avec `-Wmissing-braces` et donne `size=800,600 bpp=32` (élision d'accolades). Écrire `{{800, 600}, 32}` pour supprimer l'avertissement. | tests/RESULTATS.md n°29 |
| 4 | 04 En bref n°6, §3 point 1 ; A-TESTER n°31 | NULL → `assert` (paquet Ubuntu avec asserts actifs) ; 2.6.1 « SFML warning: trying to use a null » | Paquet Ubuntu : `assert` + abort pour `sfSprite_create(NULL)` ✅. Build Release local : segfault (NDEBUG). `sfSprite_setTexture(NULL,…)` : sans effet ni message en 3.0.0 et en 2.6.1. Message « trying to use a null » non observé en 2.6.1. | tests/RESULTATS.md n°31 |
| 5 | 03 (convention retours à libérer), 05 §5.1 | « sfKeyboard_getDescription : à libérer avec `free` » ; sans DISPLAY : « NULL ou assert » (INCERTAIN) | Header : `const char*` (strdup côté source) → `free` nécessaire mais cast requis pour éviter `-Wdiscarded-qualifiers`. Sans DISPLAY : message puis **abort** (134), jamais NULL. | tests/RESULTATS.md lot 3, n°24, n°32 |
| 6 | 05 §1.3, A-TESTER n°91 | `destroy(NULL)` sans effet (3.0.0), inconnu en 2.6 | 3.0.0 : sûr pour les 40 fonctions. 2.6.1 : `sfRenderTexture_destroy(NULL)` segfault (1 sur 42). | tests/RESULTATS.md P2 n°62, 91 |
| 7 | 05 §4.1, A-TESTER n°34 | Sans périphérique audio, 3.0.0 affiche « Using NULL audio backend » | Observé : `Failed to initialize sound: No engine available` (+ messages miniaudio PulseAudio/ALSA) ; objets créés non NULL ; pas de crash. « NULL audio backend » non reproduit. | tests/RESULTATS.md lot 4, n°34 |
