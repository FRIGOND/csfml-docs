# Exemples

Chaque exemple est un dossier `examples/<module>/<nom>/` :

```
Makefile        include ../../common/example.mk
meta.json       title, summary, functions (noms de fonctions CSFML utilisées), assets
3.0/main.c      version CSFML 3.0.0   (includes <CSFML/...>)
2.6/main.c      version CSFML 2.6.1   (includes <SFML/...>)
```

Règles : `-std=c11 -Wall -Wextra -Werror`, aucun avertissement, aucune fuite (valgrind), toutes les
fonctions CSFML utilisées vérifiées dans `api/csfml-<version>.json`, retour de chaque `create*` testé.
`#include "test_mode.h"` : `example_frames()` lit `EXAMPLE_FRAMES` (fermeture après N images ; -1 si non défini).
Assets partagés : `examples/assets/` (chemin relatif au dossier d'exécution : l'exemple les cherche via `../../../assets/` ou
reçoit le dossier par la variable `EXAMPLE_ASSETS`, défaut `../../../assets`).

Lancer : `make -C examples/window/basic-window CSFML=3.0 && examples/window/basic-window/build/3.0/example`
Tous : `tools/run-all-examples.sh [--valgrind] [--only motif] [--version 3.0|2.6]`.
