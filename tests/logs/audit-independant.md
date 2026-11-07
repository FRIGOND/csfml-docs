# Audit independant (headers reels CSFML 3.0.0 / 2.6.1)

Methode : lecture des headers installes (~/csfml-3.0, ~/csfml-2.6), grep de chaque identifiant sf* des pages (aucun manquant hors anciens noms 2.6 cites volontairement dans la migration), compilation reelle de check-your-setup.mdx (3.0 et 2.6, Xvfb) et de compat.h/main.c de support-both.mdx (3.0 et 2.6, -Werror), tirage de 40 fonctions par JSON (seed 1 et 2) comparees au header (signature, params, brief, deprecated, ligne).

## Pages (27)
| Page | Verdict | Remarque |
|---|---|---|
| concepts/memory | OK (reserve) | destroy(NULL) 2.6/3.0 non verifiable dans les headers, mais tests cites + marque |
| concepts/ownership | A corriger | l.R3 "createFromFile relisent le fichier" non marque, non verifiable header ; sfBuffer/getDescription (strdup/free) OK avec header ("must be freed") |
| concepts/events | OK | tableau 2.6/3.0 verifie champ par champ dans Event.h |
| concepts/time | OK | sfClock_reset 3.0 = pause (header), absent 2.6 : exact |
| concepts/coordinates-views | OK | signatures mapPixelToCoords identiques ; rect 3.0 exact |
| concepts/audio-without-device | OK | resultats = tests, non verifiables par header, caution presente |
| troubleshooting/link-errors | OK | |
| troubleshooting/no-sound | OK | volume 0-100 : le header dit seulement "default 100" ; la page l'attribue a la doc SFML (marque non retesté) |
| troubleshooting/black-window | OK | marque Non verifie |
| install/check-your-setup | OK | compile + execute en 3.0 et 2.6 (1 warning deprecated en 2.6, annonce) |
| tutorials 3-0/03, 3-0/05, 3-0/07, 2-6/02, 2-6/05, 2-6/08 | OK | sfFloatRect_intersects (3e arg NULL), sfSound_play restart, sfKeyReturn deprecie 2.6, setUnicodeString sfChar32 : exacts ; 3-0/07 et 2-6/08 "NULL interdit : assertion" non verifiable header |
| migration/overview | OK (reserve) | ~35 signatures ancien/nouveau verifiees : toutes exactes ; "140 fonctions" non reproduit (recompte approximatif 115) |
| migration/events | OK | tous champs/aliases verifies |
| migration/types | OK (reserve) | sfRenderStates ordre, InputStream, Rect, Transform : exacts ; "140 fonctions" idem |
| migration/objects-creation | OK | createFromSamples/SoundStream_create/RenderTexture exacts |
| migration/checklist | OK | |
| migration/support-both | A corriger | l.10 "tout ce qui est decrit a ete compile" contredit l.~150 (app_event_x non compile). Code reel compile et tourne (CSFML 3 / 2 : 16) |
| epitech/before-you-submit | A corriger | "sfFont_createFromFile ne cherche pas dans le systeme" non marque, absent du header |
| epitech/project-notions | A corriger | phrase sur sfQuads : sfQuads n'apparait pas dans le tableau ; "deux triangles par carre dans les deux versions" non verifie |
| epitech/makefile | OK | warning deprecated Event.h:133 reproduit |
| epitech/environment, coding-style | OK | contenu externe (Docker, epiclang), correctement marque Non verifie |
| help/faq | A corriger | Wayland : "verifie" pour SFML 3.0.2 impossible a recouper avec les headers CSFML ; non marque comme tel |

## Reference generee (80 fonctions)
Resultat : 0 fonction inexistante, 0 erreur de param/nom, deprecated exact (11 en 2.6, 0 en 3.0 : coherent avec CSFML_DEPRECATED), lignes exactes. Defauts systematiques :
1. 3.0 : 127 signatures affichent `_Bool` au lieu de `bool` (ex. sfWindowBase_setKeyRepeatEnabled(..., _Bool enabled)) : ce n'est pas le texte du header (libclang).
2. 2.6 : sfThread_create affiche `void (*)(void *) function` : C invalide ; header = `void (*function)(void*)`.
3. Le champ "brief" (affiche "Doc officielle") n'est pas le \brief mais \brief + 1er paragraphe (388 fonctions 3.0 > 120 car.), avec residus Doxygen (`\a mode`) et troncatures (sfFtp_sendCommand "...specified in", sfShader_setCurrentTextureParameter "Example:", sfTcpListener_listen "pass sfIpAddress_Any").
4. 46 (3.0) / 44 (2.6) fonctions "resume absent" : reel (sfPacket_*, sfSocketSelector_*), correct.

## Erreurs reelles classees
1. (systematique) `_Bool` dans 127 signatures 3.0 de la reference.
2. (systematique) "brief" = brief+paragraphe, avec `\a`, troncatures.
3. sfThread_create 2.6 : signature C invalide.
4. support-both : affirmation "tout compile" fausse (1 extrait non compile, signale plus bas).
5. project-notions : phrase orpheline sfQuads + affirmation VertexArray non verifiee.
6. Affirmations techniques non marquees et non verifiables par header : createFromFile relit le fichier (ownership), sfFont ne cherche pas dans le systeme (before-you-submit), "NULL interdit : assert" pour sfSprite/sfText/sfSound_create dans les commentaires de code (overview, types, objects-creation, tutoriels 07/08 ; objects-creation le marque partiellement), Wayland (faq), "140 fonctions".
Francais : pas de faute genante ; coherence 3.0/2.6 correcte.

## Taux d'erreur
Pages : 6/27 a corriger (~22 %), toutes mineures (aucune signature de code ou de page manuelle fausse). Reference : 0/80 fonctions erronees sur le fond ; 127/926 signatures 3.0 (14 %) avec `_Bool` et 1/850 2.6 invalide ; ~40 % des "brief" depassent le \brief reel.
