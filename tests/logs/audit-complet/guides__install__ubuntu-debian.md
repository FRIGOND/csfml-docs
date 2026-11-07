# guides/install/ubuntu-debian.mdx
Verdict : A CORRIGER (5 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 1 | « sudo apt install libcsfml-dev » | `sudo apt update` non mentionné (sur un Ubuntu/WSL fraîchement installé l'installation échoue souvent sans) ; « sudo », « apt » non expliqués ; aucune sortie attendue. | gênant |
| 2 | 2 | « headers », « paquet », « dpkg », « `.pc` », « pkg-config » | Non définis, aucun lien glossaire (« -dev » est seulement expliqué). | mineur |
| 3 | 5 | « Je n'ai pas testé l'installation sur Debian ni sur Ubuntu 24.04 » vs tableau « Ubuntu 24.04 ... `/usr/include/SFML/` » | Le chemin pour 24.04 est annoncé « venant des couches Docker » (vérifié) puis déclaré non testé : mélange à clarifier. | mineur |
| 4 | 3 | « `dpkg -l libcsfml-dev` » | Sortie (« ii ... 3.0.0-1 ») non montrée ; si le paquet n'est pas installé : message d'erreur non décrit. | mineur |
| 5 | 6 | Ligne « Ubuntu 24.04 : 2.6.0 » | Dit que le code 3.0 du site ne compile pas avec 24.04 ? Non dit ici (renvoi à la page d'index seulement) : un lecteur sur Ubuntu 24.04 ne sait pas qu'il doit utiliser les onglets 2.6. | gênant |
Vérifié : liens (index, from-source, compiling, headless-xvfb, check-your-setup) existent. `ls /usr/lib/x86_64-linux-gnu/pkgconfig` : chemin spécifique à x86_64, non précisé.
