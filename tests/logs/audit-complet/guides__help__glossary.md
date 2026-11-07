# guides/help/glossary.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 8 | description « Plus de 40 mots du métier » | Nombre écrit en dur (55 termes comptés ici) ; non recompté par un script. | mineur |
| 2 | 2 | (termes absents du glossaire) | Manquent des mots employés dans les guides : préfixe, pkg-config, stderr, valgrind, Docker/image, sudo, flag, Xrandr/X11, Mesa, OpenGL, WSL, banana/epiclang, CMake, symbole, `-Werror`. | gênant |
| 3 | 6 | « mutex / thread : Absents de CSFML 3.0 (supprimés) ; utilise `<threads.h>` ou pthread » | Exact pour 3.0 (JSON : aucun sfMutex/sfThread) mais 2.6.1 en a (`sfMutex_create`...) : à dire. `<threads.h>` non vérifié. | mineur |
| 4 | 5 | « assert : ... (`Assertion 'texture' failed`) » | Ne précise pas que les asserts n'existent que dans certains builds (voir memory.mdx). | mineur |
| 5 | autre | « header / library / linker ... » | Termes présents mais sans lien vers install/compiling pour « linker/library » (lien vers link-errors seulement). | mineur |
Vérifié : liens (link-errors, api-conventions, memory, runtime-crashes, compiling, ownership, makefile, events, game-loop, headless-xvfb, coordinates-views, time, /guides/, faq) existent.
