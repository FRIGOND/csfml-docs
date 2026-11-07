# en/3-0/getting-started/first-window.mdx
Verdict : A CORRIGER (4 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | autre / 5 | « It returns `NULL` on failure: **always check** the result. » | Sans écran (DISPLAY absent) `sfRenderWindow_create` n'est pas renvoyé NULL : abort, exit 134 (RESULTATS n°32, headless-xvfb.mdx). La phrase est trompeuse pour ce cas fréquent. | gênant |
| 2 | 1 | « gcc -std=c11 -Wall -Wextra -o app main.c -lcsfml-... » | Ne dit pas dans quel dossier (où est `main.c`), ni qu'il faut avoir installé CSFML avant (aucun lien vers /guides/install/ ; équivalent FR idem), ni ce qui s'affiche (rien = succès ; une fenêtre noire s'ouvre). | gênant |
| 3 | 6 | (absent) | La page FR contient l'encadré « Différence avec la 2.6 » (5 paramètres, `{{800, 600}, 32}`) ; la version EN l'a supprimé. | mineur |
| 4 | 2 | « drains », « pending events », « frees » | Vocabulaire anglais sans traduction pour un débutant ; aucun lien glossaire (guides/help/glossary). | mineur |
Liens : aucun lien interne.
