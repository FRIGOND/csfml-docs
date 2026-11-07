# Contribuer

Règle d'or : **aucune affirmation technique sans vérification** (header de la version testée, ou
programme compilé et exécuté). Sinon, écrire « non vérifié ».

- **Corriger une page de référence** : ne pas l'éditer, elle est générée. Corriger `api/*.json` (via
  `tools/gen_api.py`) ou les notes vérifiées `site/src/data/notes.json` (avec l'id du test).
- **Ajouter un exemple** : `examples/<module>/<nom>/{Makefile,meta.json,3.0/main.c,2.6/main.c}`,
  compilable avec `-Wall -Wextra -Werror`, sans fuite sous valgrind. Voir `examples/README.md`.
- **Avant une PR** : `tools/run-all-examples.sh --valgrind`, puis `cd site && npm run build && npm run check`.
- **Écrire** : français simple, termes anglais gardés avec leur traduction à la première occurrence.
- **Epitech** : pas de solutions de projets notés. La section Epitech explique des notions, pas des corrigés.
- Commits courts et atomiques. Jamais de secret dans le dépôt.
