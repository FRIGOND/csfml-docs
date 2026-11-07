# guides/concepts/events.mdx
Verdict : A CORRIGER (3 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 | « si `event.type == sfEvtKeyPressed` : lis `event.key.code` » | Le tableau des différences oublie les codes de touches : `sfKeyReturn` existe en 2.6 mais plus en 3.0 (JSON : seul `sfKeyEnter`), et l'enum `sfKeyCode` passe de 110 à 102 valeurs (voir tutoriel 08). Lecteur qui copie `sfKeyReturn` casse en 3.0. | gênant |
| 2 | 2 | « union », « file (queue) », « Xvfb » | « Xvfb » utilisé sans définition ni lien (headless-xvfb) ; pas de lien glossaire. | mineur |
| 3 | 7 | « À la création d'une fenêtre sous Xvfb, deux événements arrivent d'office (n°17) » | Vrai pour le test ; ne dit pas qu'en 2.6 ce n'a pas été testé. Tout le paragraphe « Vérifié par test » est 3.0 uniquement (waitEvent à 3 paramètres). | mineur |
Vérifié : table des différences conforme aux JSON (sfEvtFocusLost/Gained, size, mouseButton.position, mouseMove, MouseWheel, MouseMovedRaw, waitEvent) ; liens existent (game-loop, compiling, cookbook keyboard-realtime et mouse-position, reference/window/event).
