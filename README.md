# Documentation CSFML (non officielle)

Documentation communautaire, gratuite et en français de **CSFML** (le binding C de SFML),
pour **CSFML 3.0** et **CSFML 2.6**. Hébergée sur GitHub Pages.

> **À lire.** Ce projet n'est pas officiel et n'est pas affilié à l'équipe SFML.
> Il est **écrit avec l'aide d'une IA** et **vérifié par des tests** : chaque signature vient des headers
> (extraits avec libclang), chaque exemple est compilé (`-Wall -Wextra -Werror`) et exécuté en CI.
> Ce qui n'a pas été vérifié est marqué « non vérifié ». Une erreur ? Ouvrez une issue.

## Contenu

- **Référence** : une page par header, une entrée par fonction (signature, ce qu'elle fait, propriété de la mémoire).
- **Exemples** : 31 programmes C complets par version, compilés et exécutés.
- **Tutoriels** : fil rouge d'un casse-briques.
- **Migration 2.6 → 3.0**, **guides** (installation, concepts, dépannage, Epitech).

## Prévisualiser en local

```sh
cd site
npm ci
npm run dev        # http://localhost:4321/csfml-docs/
npm run build && npm run check   # build + vérification des liens
```

Node ≥ 22.23.4 est requis (Astro 7). Les pages de référence sont **générées** depuis `api/*.json`
par `tools/gen_site_pages.py` (lancé par `npm run dev` / `build`).

## Tester les exemples

```sh
tools/build-csfml.sh 3.0.0 3.0.2 ~/csfml-3.0    # CSFML sans sudo
tools/build-csfml.sh 2.6.1 2.6.1 ~/csfml-2.6
tools/run-all-examples.sh [--valgrind]
```

## Publier sur GitHub Pages (gratuit)

1. Dépôt **public** (GitHub Pages gratuit l'exige).
2. Settings → Pages → Source : **GitHub Actions**.
3. Un push sur `main` lance `.github/workflows/pages.yml` : build, vérification des liens, déploiement.
   L'URL est `https://<utilisateur>.github.io/<dépôt>/` (le `base` est calculé automatiquement).

Aucun service payant ou externe : site 100 % statique.

## Licences

- Documentation (texte) : [CC BY 4.0](LICENSE). Exemples (code) : licence zlib, voir `LICENSE`.
- Les tutoriels adaptés de sfml-dev.org (CC BY 4.0) le mentionnent. CSFML/SFML : licence zlib.
