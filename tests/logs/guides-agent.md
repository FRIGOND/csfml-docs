# Compte rendu agent GUIDES

## Pages finies
- TOUTES les pages demandées (36 .mdx) : guides/index, install/* (9), concepts/* (10), troubleshooting/* (7), epitech/* (6), help/* (3).
- Build `npm run build` OK, `npm run check` : 517 pages, 64 457 liens, 0 problème.
- Notes de forme : guides/index.mdx utilise `template: splash` (sinon starlight-sidebar-topics refuse la page : "Failed to find the topic for the guides page" ; la config n'est pas à moi). Frontmatter title/description entre guillemets (YAML : les « : » cassaient le parse).
- Pages courtes (< 40 lignes sources, lignes longues) : concepts/index, epitech/index, troubleshooting/index, install/wsl, arch, fedora, black-window, no-sound, project-notions, guides/index.

## Vérifications faites par l'agent (rejouables)
- check-your-setup : programme compilé et lancé sous Xvfb avec ~/csfml-3.0, ~/csfml-2.6 et le paquet système : OK. Piège trouvé : `__has_include(<CSFML/Config.h>)` (proposé par research/06 §5) choisit les headers du paquet système 3.0 même avec `-I$P2/include` => programme affiche "CSFML 3.0.0" alors que lié à la 2.6. Test corrigé : `__has_include(<SFML/Config.h>)` d'abord.
- Recette from-source rejouée en entier (SFML 3.0.2 + CSFML 3.0.0, préfixe temporaire, supprimé) : OK, ldd résout dans le préfixe.
- Messages exacts rejoués (gcc 15.2, LC_ALL=C.UTF-8) : SFML/Graphics.h introuvable, sfBool/sfTrue, missing-braces, too few arguments, implicit declaration sfSound_setLoop, undefined reference, "error while loading shared libraries: libcsfml-system.so.2.6", -Werror=deprecated-declarations (Event.h:133).
- epiclang testé sur fichiers de test (règles C-G1, C-F2, C-F3, C-F4, C-F5, C-F6, C-G2, C-G4, C-C1, C-L1..L4, C-L6, C-O3, C-A3, C-F8, C-H2).
- valgrind sur le programme de diagnostic (3.0.0, fenêtre seule) : definitely lost 0, still reachable 259 204 octets / 3 671 blocs, 0 erreur.

## Erreurs / contradictions trouvées dans le rapport
- research/06 §5 : `#if __has_include(<CSFML/Graphics.h>)` : piège si le paquet système 3.0 coexiste avec un préfixe 2.6 (voir ci-dessus).
- research/05 §4.1 (déjà dans CORRECTIONS n°7) : "NULL audio backend" non reproduit.

## Points "Non vérifié" laissés
- Debian/Ubuntu 24.04/Fedora/Arch/macOS/Windows/WSL : non testés (marqués dans les pages).
- Différences 2.6.0 vs 2.6.1 : non étudiées. Avertissement deprecated non confirmé sur 2.6.0.

## Sources utilisées
tests/RESULTATS.md (A.2, A.3, lots 1-5, P2), research/CORRECTIONS.md, api/*.json (script de recherche de signatures), examples/** (meta, main.c), tests/logs/examples-*.md, research/01 §5-6, 02, 04, 05, 06 (contexte), tools/build-csfml.sh, examples/common/example.mk, tests/valgrind.supp.

## Tests supplémentaires faits par l'agent
- Messages de chargement de ressources inexistantes (3.0.0 : chemin absolu affiché ; 2.6.1 : message plus court), 4 create renvoient NULL.
- Makefile minimal (no relink, re, fclean) : comportement de make vérifié.
- banana-check-repo : C-O1 (.o, .a) et C-O4 (nom non snake_case) vérifiés ; il ne gère pas (non testé) .gitignore.
- epiclang : C-F4 signale la 21e ligne ; C-C1 profondeur 3 ; C-F2 refuse les noms trop courts ; l'outil signale aussi les headers CSFML (C-F3, C-H2).

## Non vérifié restant (marqué dans les pages)
Debian, Ubuntu 24.04 (hors lecture d'image), Fedora, Arch, macOS, Windows, WSL ; xvfb-run ; SFML 3.1 ; sfClock_reset (non testé) ; symptôme visuel sprite/texture détruite ; volume/formats sons ; licence assets ("à re-vérifier" dans examples/assets/LICENSE.md) ; différences 2.6.0/2.6.1 ; chiffre valgrind 259 204 octets propre à la machine.
