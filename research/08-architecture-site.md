# 08 — Architecture du site : choix du générateur, structure du dépôt, versionnage

Recherche du 2026-11-07 (PHASE 1, aucun site construit, aucun `npm install`).
Les versions npm citées ont été lues avec `npm view <paquet> version time` (lecture du registre, sans rien installer).
Les versions des GitHub Actions ont été lues avec l'API `https://api.github.com/repos/<owner>/<repo>/releases/latest`.

## En bref

- **Recommandation : Starlight (Astro).** Repli : **Docusaurus**. Starlight a obtenu 47/55 dans le tableau noté, contre 41 pour Docusaurus, 40 pour MkDocs Material et 39 pour VitePress. [DÉDUIT]
- **MkDocs Material est à écarter pour un projet neuf.** Il est en mode maintenance depuis la 9.7.0 (novembre 2025). Sa fin de vie est repoussée au **5 mai 2027**. MkDocs 1.x est gelé (dernière version stable 1.6.1, août 2024). MkDocs 2.0 (pré-versions `2.0.dev0` à `dev6`) est incompatible. Le successeur **Zensical** n'est qu'en bêta (0.1.0 du 2026-11-05). [CONFIRMÉ]
- Starlight est très actif (0.42.10 du 2026-11-06, Astro 7.4.7). Il fournit nativement la recherche locale **Pagefind**, avec un index par langue et le stemming français. Il fournit aussi la coloration **Shiki/Expressive Code** (grammaire `c`), l'**i18n avec page de repli**, des composants (Tabs, Aside, Badge, Card, Steps, FileTree…) et un déploiement GitHub Pages officiel (`withastro/action@v6` + `actions/deploy-pages@v5`). [CONFIRMÉ]
- Le point faible de Starlight est le **versionnage**. Il n'est pas natif : le plugin communautaire `starlight-versions` (0.10.2) se dit « still in early development ». On n'en a pas besoin, voir la stratégie ci-dessous. [CONFIRMÉ]
- **Attention à Node.js.** Astro 7.4.7 exige `node >=22.23.4`. La machine locale a Node 22.22.1, ce qui est **trop vieux** : la phase 2 doit utiliser Node 24. Le runner `ubuntu-26.04` fournit Node 24.22.2. [CONFIRMÉ]
- **Versionnage retenu (révisé : parité 2.6 / 3.x) :** deux versions de **première classe**, **3.x** (3.0.0, avec badges « 3.1+ ») et **2.6** (2.6.1), avec les mêmes sections (démarrer, guides, référence, exemples) et un **sélecteur de version**. Décision utilisateur : Fedora est encore en 2.6.1 et aucune doc API en ligne n'existe pour 2.6. Pas de doc dupliquée par version mineure 3.x. [DÉCISION]
- **Un exemple de code = un vrai fichier `.c`** dans `examples/`. On l'affiche avec un import `?raw` dans un composant `<Code>`. Il n'est jamais recopié à la main. La CI compile et exécute ce même fichier, voir `10-workflow.md`. [CONFIRMÉ pour le mécanisme `?raw` + `<Code>` ; À TESTER pour l'import hors de `src/`]
- **Contexte Epitech :** l'image officielle `epitechcontent/epitest-docker:latest` est basée sur Ubuntu 26.04 (poussée le 2026-10-07). Son méta-paquet `epitech-cpool` dépend de `libcsfml-dev`, qui est en 3.0.0 sur 26.04. Le public cible est donc maintenant sur CSFML 3.0.0. C'est ce qui justifie de mettre 3.x en principal. [CONFIRMÉ pour l'image et les dépendances ; DÉDUIT pour la version installée]

---

## 1. État actuel des quatre outils (novembre 2026)

| Outil | Dernière version stable | Date | Pré-version / suite | Pile technique | Source |
|---|---|---|---|---|---|
| Starlight (`@astrojs/starlight`) | 0.42.10 | 2026-11-06 | aucune (toujours en 0.x) | Astro 7.4.7 (2026-11-05), Node ≥ 22.23.4 | npm `@astrojs/starlight`, `astro` ; https://github.com/withastro/starlight/releases |
| Docusaurus (`@docusaurus/core`) | 3.10.2 | 2026-07-10 | canary `4.0.0-canary-7078` (2026-11-07) | React, Node ≥ 20 | npm ; https://github.com/meta/docusaurus/releases/tag/v3.10.2 |
| MkDocs Material | 9.8.0 | 2026-10-24 | — (maintenance) | Python ≥ 3.11, MkDocs 1.6.1 | https://github.com/squidfunk/mkdocs-material/releases ; PyPI |
| MkDocs | 1.6.1 | 2024-08-30 | `2.0.dev6` (2026-09-15) | Python | https://pypi.org/project/mkdocs/#history |
| Zensical | 0.1.0 (bêta) | 2026-11-05 | bêta « pour les trois prochains mois » | Python ≥ 3.11 (+ Rust) | https://github.com/zensical/zensical/releases |
| VitePress | 1.6.4 | 2025-08-05 | `2.0.0-alpha.21` (2026-10-25) | Vue 3, Vite 5, Shiki 2 | npm `vitepress` ; https://github.com/vuejs/vitepress/releases |

Remarques :
- Le dépôt Docusaurus redirige maintenant de `facebook/docusaurus` vers `meta/docusaurus`. L'API GitHub renvoie `https://github.com/meta/docusaurus/releases/tag/v3.10.2`. [CONFIRMÉ, 2026-11-07]
- La branche stable de VitePress n'a rien publié depuis le 2025-08-05. Le travail se fait sur la 2.0, toujours en alpha au bout de 21 versions. C'est un risque pour un projet qui veut une base stable. [DÉDUIT]

### 1.1 MkDocs Material, MkDocs et Zensical : la situation en détail

- **Mode maintenance.** L'annonce épinglée https://github.com/squidfunk/mkdocs-material/issues/8523 a été ouverte le 2025-11-06. Elle s'intitule maintenant « End of life extended to May 5, 2027 » (titre changé le 2026-09-15). Elle dit que le projet est dans sa « final maintenance period » : il reçoit encore les correctifs critiques et de sécurité, et les nouvelles fonctionnalités vont dans Zensical. Elle précise aussi « no need to migrate immediately ». [CONFIRMÉ]
- **Historique des versions** (https://github.com/squidfunk/mkdocs-material/releases) [CONFIRMÉ] :
  - la 9.7.0 a lancé le mode maintenance (dernière version avec de nouvelles fonctionnalités) ;
  - la 9.7.2 a ajouté un avertissement sur l'incompatibilité avec MkDocs 2.0 ;
  - la 9.7.5 limite `mkdocs<2` ;
  - la 9.7.7 annonçait une fin de vie au 5 novembre 2026 ;
  - la 9.8.0 (2026-10-24) la repousse au 5 mai 2027.
- **MkDocs 2.0.** L'article https://squidfunk.github.io/mkdocs-material/blog/2026/02/18/mkdocs-2.0/ date du 2026-02-18 et a été mis à jour le 2026-10-02. Il annonce plusieurs changements [CONFIRMÉ, source = équipe Material, donc partie prenante] :
  - réécriture complète ;
  - **plus de système de plugins** ;
  - configuration en **TOML** au lieu de `mkdocs.yml`, sans chemin de migration ;
  - navigation passée aux thèmes en HTML pré-rendu ;
  - modèle de contribution fermé.

  Le même article dit aussi que le dépôt MkDocs 1.x est fermé aux contributions externes depuis le 2026-10-02. Sur PyPI, la dernière stable est toujours 1.6.1 (2024-08-30), et les pré-versions vont de `2.0.dev0` (2026-08-28) à `2.0.dev6` (2026-09-15). [CONFIRMÉ]
- **Zensical.** La 0.1.0 du 2026-11-05 entre en « beta for the next three months ». Elle reprend `mkdocs.yml` et beaucoup de plugins MkDocs. Plusieurs points manquent encore [CONFIRMÉ, https://github.com/zensical/zensical/releases et https://zensical.org/docs/compatibility/mkdocs/mike/] :
  - le **versionnage natif** n'existe pas encore : il est « on the roadmap » et passe en attendant par un fork transitoire de mike (`squidfunk/mike`, basé sur mike 2.2.0) ;
  - aucun support i18n n'est mentionné dans les notes de version.
- **Conclusion.** Partir sur MkDocs Material en novembre 2026, c'est construire sur une base dont la fin de vie est dans six mois. Partir sur Zensical, c'est construire sur une bêta qui n'a ni versionnage ni i18n documentés. Les deux options sont à écarter pour ce projet, mais Zensical est à réévaluer mi-2027. [DÉDUIT]

---

## 2. Tableau noté (1 = mauvais, 5 = excellent)

Les notes sont [DÉDUIT] à partir des sources citées dans la section 3. Elles ne sont pas mesurées.

| Critère | Starlight | Docusaurus | MkDocs Material | VitePress |
|---|:-:|:-:|:-:|:-:|
| Déploiement GitHub Pages gratuit (action officielle, base path) | 5 | 4 | 4 | 4 |
| Recherche locale sans service externe | 5 | 3 | 4 | 4 |
| Coloration syntaxique du C | 5 | 3 | 4 | 4 |
| Inclusion d'un vrai fichier `.c` dans la page | 4 | 3 | 5 | 5 |
| Versionnage 2.x / 3.x | 2 | 5 | 3 | 1 |
| i18n FR/EN (natif, repli) | 5 | 4 | 2 | 3 |
| Rapidité (build et site) | 4 | 3 | 4 | 5 |
| Facilité de maintenance par une IA | 4 | 3 | 4 | 4 |
| Accessibilité et mobile (non mesurée) | 4 | 4 | 4 | 4 |
| Composants (onglets, asides, badges, cartes) | 5 | 4 | 5 | 3 |
| Écosystème et pérennité | 4 | 5 | 1 | 2 |
| **Total / 55** | **47** | **41** | **40** | **39** |
| Dépendance | Node ≥ 22.23.4 | Node ≥ 20 | Python ≥ 3.11 | Node (Vite 5) |

---

## 3. Commentaires par critère (sourcés)

### 3.1 Déploiement GitHub Pages gratuit
- **Conditions GitHub Pages.** C'est gratuit pour un dépôt public avec GitHub Free, d'après https://docs.github.com/en/pages/getting-started-with-github-pages/about-github-pages (recherche du 2026-11-07, formulation constante depuis 2021). Les limites sont listées sur https://docs.github.com/en/pages/getting-started-with-github-pages/github-pages-limits :
  - site publié de 1 Go maximum ;
  - bande passante de 100 Go/mois (limite souple) ;
  - déploiement de 10 minutes maximum ;
  - la limite de 10 builds/heure ne s'applique pas à un workflow Actions personnalisé.

  [CONFIRMÉ]
- **Starlight.** Le guide officiel est https://docs.astro.build/en/guides/deploy/github/. Il utilise `actions/checkout@v7`, `withastro/action@v6` et `actions/deploy-pages@v5`, avec les permissions `contents: read`, `pages: write` et `id-token: write`. La configuration se résume à `site: 'https://<user>.github.io'` et `base: '/<repo>'`. [CONFIRMÉ]
  - L'action `withastro/action` v6.1.4 (2026-10-23) détecte le gestionnaire de paquets grâce au lockfile, utilise Node 24 par défaut, met en cache `node_modules/.astro` et appelle `actions/upload-pages-artifact@v5` (lu dans son `action.yml`). [CONFIRMÉ]
  - Avec un `base`, les liens internes écrits à la main doivent inclure le préfixe. C'est un piège à vérifier en phase 2. [CONFIRMÉ, guide Astro]
- **Docusaurus.** La doc https://docusaurus.io/docs/deployment explique les champs `url`, `baseUrl: '/<repo>/'` et `trailingSlash`. Mais son exemple de workflow utilise des versions anciennes (`checkout@v4`, `upload-pages-artifact@v3`, `deploy-pages@v4`), alors que les dernières sont v7, v5 et v5. Il faudrait l'adapter. [CONFIRMÉ]
- **VitePress.** La doc https://vitepress.dev/guide/deploy fournit un exemple avec `base: '/<repo>/'`, mais lui aussi avec des actions anciennes (`checkout@v5`, `configure-pages@v4`, `upload-pages-artifact@v3`, `deploy-pages@v4`). [CONFIRMÉ]
- **MkDocs Material.** On publie avec `mkdocs gh-deploy` ou avec mike, qui poussent sur une branche `gh-pages`, ou bien avec un workflow Actions. [CONFIRMÉ pour mike, https://squidfunk.github.io/mkdocs-material/setup/setting-up-versioning/]

### 3.2 Recherche locale sans service externe
- **Starlight.** Il utilise **Pagefind** par défaut (https://starlight.astro.build/guides/site-search/). On peut exclure une page avec `pagefind: false` ou un bloc avec `data-pagefind-ignore`. [CONFIRMÉ]
  - Pagefind crée **un index par langue**, à partir de l'attribut `lang` de la balise `<html>`. Le **français a le stemming** et une interface traduite (https://pagefind.app/docs/multilingual/). [CONFIRMÉ]
  - Pagefind 1.6.0 date du 2026-11-01. [CONFIRMÉ]
  - L'index n'est créé qu'au build : la recherche ne marche pas en `astro dev`. [DÉDUIT, À TESTER]
- **VitePress.** Il intègre **MiniSearch**, un index plein texte dans le navigateur, configurable par locale (https://vitepress.dev/reference/default-theme-search). [CONFIRMÉ] Aucun stemming français n'est documenté. [INCERTAIN]
- **Docusaurus.** Il n'a pas de recherche locale native : l'option officielle est Algolia DocSearch, qui est externe. Il faut le plugin tiers `@easyops-cn/docusaurus-search-local` (0.55.3, 2026-07-29). [CONFIRMÉ pour la version ; DÉDUIT pour l'absence de recherche locale native]
- **MkDocs Material.** Son plugin `search` est intégré et s'exécute côté client. [DÉDUIT, connaissance générale de l'outil]
- **Algolia DocSearch** est un service externe dans les quatre cas. Starlight le propose aussi en option (`@astrojs/starlight-docsearch` 0.8.1). On ne le retient pas. [CONFIRMÉ]

### 3.3 Coloration syntaxique du C
- **Starlight** utilise **Expressive Code**, construit sur Shiki : `astro-expressive-code` ^0.44.0 dans les dépendances de Starlight 0.42.10. [CONFIRMÉ, npm]
  - La grammaire Shiki `c` est celle de VS Code. `cmake`, `make`/`makefile`, `shellscript`/`bash` et `glsl` existent aussi (https://github.com/shikijs/textmate-grammars-themes, README de `tm-grammars`). [CONFIRMÉ]
  - En Markdown pur, on dispose de `title="main.c"`, du marquage de lignes `{2-3}`, de `ins=`/`del=` et du mode `diff` (https://starlight.astro.build/guides/authoring-content/). [CONFIRMÉ]
  - `glsl` sera utile pour les pages sur les shaders.
- **VitePress** utilise lui aussi Shiki (https://vitepress.dev/guide/markdown). [CONFIRMÉ]
- **Docusaurus** utilise Prism via `prism-react-renderer` 2.4.1.
  - Les langages livrés par défaut incluent `cpp` mais pas `c` explicitement (`languagesToBundle` dans https://github.com/FormidableLabs/prism-react-renderer). [CONFIRMÉ]
  - `cpp` dépend de la grammaire `c` de Prism, donc `c` devrait marcher sans configuration. [DÉDUIT, À TESTER] Sinon, il suffit de l'ajouter dans `additionalLanguages`.
- **MkDocs Material** utilise Pygments, qui gère `c`, `cmake`, `make`, `bash` et `glsl` (https://pygments.org/languages/). [CONFIRMÉ]

### 3.4 Inclure un vrai fichier `.c` (pas de copie à la main)
- **Starlight.** Dans une page `.mdx`, on écrit `import code from '<chemin>?raw'` puis `<Code code={code} lang="c" title="main.c" />` (https://starlight.astro.build/components/code/). [CONFIRMÉ]
  - Cela oblige à écrire les pages concernées en `.mdx`.
  - Il faudra vérifier que Vite accepte un chemin hors de `src/`, par exemple `examples/` à la racine du projet. [À TESTER]
  - On encapsulera ce mécanisme dans un composant maison `<Example id="graphics/sprite-basic" />`, décrit dans `10-workflow.md`.
- **VitePress.** `<<< @/examples/sprite.c{3-5}` est natif et accepte les régions `#region`. **Un fichier manquant provoque une erreur de build** (https://vitepress.dev/guide/markdown). C'est le meilleur mécanisme des quatre. [CONFIRMÉ]
- **MkDocs Material.** L'extension `pymdownx.snippets` (`--8<-- "file.c"`) fait la même chose. [DÉDUIT, connaissance générale]
- **Docusaurus.** Il n'y a rien de natif dans la doc des blocs de code (https://docusaurus.io/docs/markdown-features/code-blocks). Il faut passer par `raw-loader` ou un plugin. [CONFIRMÉ pour l'absence dans la doc]

### 3.5 Versionnage 2.x / 3.x
- **Docusaurus** est le seul à avoir un versionnage natif (https://docusaurus.io/docs/versioning). [CONFIRMÉ]
  - `docs:version X` fige une copie dans `versioned_docs/version-X/`, avec `versioned_sidebars/` et `versions.json`.
  - La doc conseille de rester « below 10 » versions et de ne pas versionner les correctifs.
- **MkDocs Material** passe par **mike**, avec un dossier par version sur la branche `gh-pages` et un sélecteur de version. mike 2.2.0 date du 2026-04-14. Les anciennes versions ne sont jamais reconstruites (https://squidfunk.github.io/mkdocs-material/setup/setting-up-versioning/). [CONFIRMÉ]
- **Starlight** passe par le plugin communautaire `starlight-versions` (0.10.2, 2026-10-30, auteur HiDeoo, peer `@astrojs/starlight >=0.39.0`). [CONFIRMÉ]
  - Le plugin se décrit comme « opinionated » et « still in early development ». Il archive l'état courant des docs comme version figée (https://starlight-versions.vercel.app/getting-started/).
  - Pour nous, l'**alternative native** est une section `v2.6/` de même structure que la racine (parité 2.6 / 3.x). Pour avoir une barre latérale séparée, on peut ajouter `starlight-sidebar-topics` (0.9.0, 2026-09-03). [CONFIRMÉ pour les versions]
- **VitePress** n'a aucun versionnage. Il faudrait des dossiers séparés. [DÉDUIT, absent de la doc]

### 3.6 i18n FR/EN
- **Starlight** gère l'i18n nativement (https://starlight.astro.build/guides/i18n/). [CONFIRMÉ]
  - On déclare `locales` et `defaultLocale`. Une locale `root` permet d'avoir des URL sans préfixe.
  - Si une page n'est pas traduite, le lecteur voit **le contenu de la langue par défaut avec un avis** (« This content is not available in your language yet »).
  - Les chaînes d'interface sont fournies en français. On peut les surcharger avec `src/content/i18n/*.json`.
  - Le titre du site peut changer selon la langue.
- **Docusaurus** gère l'i18n nativement, mais **chaque locale est une application séparée**. On ne peut pas lancer toutes les locales en même temps en développement (`--locale fr`) (https://docusaurus.io/docs/i18n/tutorial). [CONFIRMÉ] Le repli vers la langue par défaut n'est pas décrit dans les pages lues. [INCERTAIN]
- **VitePress** gère l'i18n nativement avec un dossier par langue (https://vitepress.dev/guide/i18n). [CONFIRMÉ] Mais :
  - `/` ne redirige pas vers une langue sans configuration côté serveur ;
  - aucun repli n'est documenté. [CONFIRMÉ pour l'absence dans la page]
- **MkDocs Material** traduit son interface, mais le contenu multilingue passe par le plugin tiers `mkdocs-static-i18n` (1.3.1, 2026-02-20). [CONFIRMÉ pour la version PyPI]
- **Suivi des traductions.** Pour savoir quelles traductions sont en retard, **Lunaria** (`@lunariajs/core` 0.2.1) est cité comme compatible avec Starlight et VitePress (https://www.lunaria.dev/). [CONFIRMÉ pour l'existence ; usage À TESTER]

### 3.7 Rapidité
- **Starlight/Astro** génère du HTML statique et n'envoie du JavaScript que pour les composants interactifs (îlots). **VitePress** est une SPA Vue légère. **Docusaurus** est une SPA React, plus lourde à l'hydratation et au build. [DÉDUIT, connaissance générale des architectures, non mesuré]
- La phase 2 doit mesurer le temps de build et le poids des pages. [À TESTER]

### 3.8 Maintenance par une IA
- **Starlight.** Les points forts :
  - le contenu est en Markdown/MDX ;
  - le frontmatter est **validé par un schéma Zod**, qu'on peut étendre avec `docsSchema({ extend })` (https://starlight.astro.build/reference/frontmatter/) : un champ `csfml.tested` mal écrit fait échouer le build, ce qui est utile pour une IA [CONFIRMÉ] ;
  - la configuration tient dans un seul fichier `astro.config.mjs` ;
  - les asides s'écrivent en Markdown pur (`:::note`, `:::tip`, `:::caution`, `:::danger`). [CONFIRMÉ]

  Le risque est la version 0.x : les changements cassants sont possibles d'une mineure à l'autre. Il faut figer les versions et passer par Renovate. [DÉDUIT]
- **Docusaurus.** Personnaliser oblige souvent à faire du « swizzling » de composants React, ce qui fait plus de code à maintenir. [DÉDUIT]
- **MkDocs.** Le format YAML + Markdown est très simple, mais la plateforme est en fin de vie (section 1.1). [DÉDUIT]

### 3.9 Accessibilité et mobile
- La page d'accueil de Starlight se présente comme « accessible » (https://starlight.astro.build/). Les quatre outils ont des thèmes adaptés au mobile avec mode sombre. On ne départage pas sans mesure. [DÉDUIT]
- Proposition pour la phase 2 : passer `@axe-core/cli` (4.14.0) ou `pa11y-ci` (5.0.0) sur le site construit. [À TESTER]

### 3.10 Composants utiles
- **Starlight** (https://starlight.astro.build/components/using-components/) propose : Tabs, Aside, Badge, Card, CardGrid, LinkCard, LinkButton, Steps, FileTree, Code et Icon. Le frontmatter `sidebar.badge` permet un badge « 3.1+ » dans la barre latérale. [CONFIRMÉ]
- **VitePress** propose les conteneurs `::: tip|warning|danger|details`, les alertes GitHub `> [!NOTE]` et `::: code-group` pour les onglets de code. [CONFIRMÉ]
- **Docusaurus** propose les admonitions, Tabs et tous les composants React en MDX. **MkDocs Material** propose les admonitions, les onglets de contenu, les annotations et les grilles. [DÉDUIT, connaissance générale]

### 3.11 Écosystème
- **Starlight.** Les plugins utiles sont listés sur https://starlight.astro.build/resources/plugins/ [CONFIRMÉ] :
  - `starlight-links-validator` (0.27.0) vérifie les liens internes au build ;
  - `starlight-llms-txt` (0.12.0) génère un `llms.txt` ;
  - `starlight-image-zoom` (0.16.2) ;
  - `starlight-sidebar-topics` (0.9.0) ;
  - `starlight-versions` (0.10.2).

---

## 4. Recommandation

### Choix : Starlight (Astro)
Raisons, par ordre d'importance :
1. **Projet vivant.** Plusieurs versions par semaine en octobre et novembre 2026 : 0.42.8 le 10-30, 0.42.9 le 11-04, 0.42.10 le 11-06. MkDocs Material est en fin de vie et la branche stable de VitePress est figée. [CONFIRMÉ]
2. **Recherche locale gratuite et multilingue.** Pagefind est intégré, avec le stemming français. Aucun service externe. [CONFIRMÉ]
3. **i18n FR/EN native avec repli.** Une page pas encore traduite reste accessible, avec un avis. C'est indispensable pour une doc rédigée progressivement. [CONFIRMÉ]
4. **Coloration C de qualité** grâce à Expressive Code : titres de fichiers, lignes marquées, diff. C'est utile pour montrer les migrations 2.6 → 3.0. [CONFIRMÉ]
5. **Déploiement GitHub Pages officiel et à jour** : `withastro/action@v6` et `deploy-pages@v5`. [CONFIRMÉ]
6. **Frontmatter typé (Zod).** Les métadonnées « testé sur CSFML x.y.z » et « depuis 3.1 » sont contrôlées au build. [CONFIRMÉ]

Faiblesses acceptées :
- pas de versionnage natif, contourné par la stratégie de la section 6 ;
- version 0.x, à figer avec un lockfile et Renovate ;
- Node ≥ 22.23.4 obligatoire.

### Repli : Docusaurus
On bascule si Starlight devient bloquant (changement cassant majeur, abandon) ou si un vrai versionnage multi-arbres devient nécessaire. Docusaurus a le **versionnage et l'i18n natifs** et une pérennité élevée (Meta, 3.10.2, 4.0 en canary). Il coûte cependant :
- un plugin tiers pour la recherche locale ;
- Prism, moins riche ;
- l'inclusion de fichiers via raw-loader ou un plugin.

Le contenu reste en MDX, donc la migration Starlight → Docusaurus serait surtout mécanique. [DÉDUIT]

*Mention :* VitePress serait le plus agréable pour inclure du code (`<<<`, erreur si le fichier manque). Mais sa stable est figée depuis août 2025 et il n'a ni versionnage ni repli i18n. On ne le retient pas. [DÉDUIT]

---

## 5. Structure de dépôt envisagée

Le projet Astro est placé **à la racine du dépôt**. Ainsi `examples/` est dans la racine Vite, ce qui simplifie les imports `?raw`. [DÉDUIT, À TESTER]

```
csfml-docs/
├── astro.config.mjs            # site, base, starlight({ locales, plugins })
├── package.json / package-lock.json   # versions figées, Node >= 22.23.4 (engines)
├── src/
│   ├── content.config.ts       # docsSchema({ extend: csfml.{tested, since, examples} })
│   ├── content/
│   │   ├── docs/               # FR = locale racine (URL sans préfixe)
│   │   │   ├── index.mdx
│   │   │   ├── demarrer/       # (3.x, racine) installation (Ubuntu 26.04, Epitech, Windows…), premier programme
│   │   │   ├── guides/         # fenêtre, événements, sprites, texte, audio, réseau…
│   │   │   ├── reference/      # 1 page par « classe » C : graphics/sprite.mdx, audio/music.mdx…
│   │   │   ├── migration/      # 2.6 → 3.0, et plus tard 3.0 → 3.1
│   │   │   ├── v2.6/           # CSFML 2.6 : mêmes sections que la 3.x (voir §6)
│   │   │   └── en/…            # traduction EN, même arborescence
│   │   └── i18n/fr.json, en.json   # chaînes UI ajoutées (« Testé sur », « Depuis »)
│   └── components/
│       ├── Example.astro       # lit examples/<id>/main.c (?raw) -> <Code>, + badge résultat CI
│       └── Since.astro         # badge « 3.1+ »
├── examples/
│   ├── 3.x/<module>/<slug>/main.c      # code exact affiché dans la page
│   ├── 3.x/<module>/<slug>/example.json  # modules à lier, nb de frames, capture ?, audio ?
│   ├── 2.6/<module>/<slug>/main.c      # en-têtes <SFML/...> (différents de <CSFML/...>)
│   └── assets/                         # images/polices/sons sous licence libre (+ LICENSES.md)
├── tests/
│   ├── harness/doc_wrap_3x.c, doc_wrap_26.c   # --wrap de pollEvent/display (voir 10-workflow)
│   ├── run_examples.sh                 # compile gcc+clang -Wall -Wextra -Werror, xvfb-run, valgrind
│   ├── compare_images.py
│   ├── references/<csfml-version>/<slug>.png
│   └── suppressions/valgrind-mesa.supp
├── tools/
│   ├── extract_api.py          # en-têtes -> api/csfml-<ver>.json
│   ├── check_doc_api.py        # fonctions citées ⊂ API ; couverture API -> pages
│   ├── diff_api.py             # diff entre deux tags -> pages à mettre à jour
│   └── make_badge.py           # JSON endpoint shields.io
├── api/csfml-3.0.0.json, api/csfml-2.6.1.json   # instantanés générés pour LES DEUX versions, versionnés dans git
├── .github/workflows/ci.yml, deploy.yml, links.yml, watch-csfml.yml
└── renovate.json (ou .github/dependabot.yml)
```

Points importants :
- **Les en-têtes diffèrent entre versions.** Les exemples 3.x incluent `<CSFML/Graphics.h>` et ceux de 2.6 incluent `<SFML/Graphics.h>`. Dans le dépôt, `include/CSFML/...` existe au tag 3.0.0 et `include/SFML/...` au tag 2.6.1. Un même fichier `.c` ne peut donc pas servir aux deux versions. Il faut deux arbres d'exemples. [CONFIRMÉ, `git archive` des tags 3.0.0 et 2.6.1]
- **Les en-têtes système correspondent au dépôt.** Les en-têtes installés dans `/usr/include/CSFML` (paquet `libcsfml-dev` 3.0.0-1) sont **identiques** à `include/CSFML` du tag 3.0.0 (`diff -r -q`, aucune différence). [CONFIRMÉ, 2026-11-07]

---

## 6. Stratégie de versionnage 2.6 / 3.x retenue (révisée : parité)

**Décision : deux versions de première classe, 3.x (URL racine) et 2.6 (préfixe `/v2.6/`), même structure, sélecteur de version, pas de plugin de versionnage.**

Pourquoi la parité (décision de l'utilisateur) :
1. **2.6 est encore installée.** Fedora est en 2.6.1, Ubuntu 24.04 fournit 2.6.0-3build1 (https://packages.ubuntu.com/search?keywords=libcsfml-dev). Ubuntu 26.04 et l'image `epitest-docker:latest` (base `ubuntu:26.04`, 2026-10-07) sont en 3.0.0-1 ; `epitech-cpool` dépend de `libcsfml-dev`. [CONFIRMÉ pour ces faits ; DÉDUIT : le public est partagé entre les deux versions]
2. **Aucune doc API en ligne pour 2.6.** La doc Doxygen ne vient qu'avec le paquet `libcsfml-doc` ou un build avec `CSFML_BUILD_DOC`. Notre site comble ce vide. [DÉDUIT, voir 10-workflow.md §1.1]
3. **2.6 est figée** (dernière : 2.6.1, release du 2024-05-19) : le coût est ponctuel, pas continu. [CONFIRMÉ pour la date ; DÉDUIT pour la suite]
4. **Les en-têtes diffèrent** (`<SFML/...>` en 2.6, `<CSFML/...>` en 3.0, §5), donc de toute façon deux arbres d'exemples. La référence est **générée** par `tools/extract_api.py` pour les deux versions (`api/csfml-2.6.1.json` et `api/csfml-3.0.0.json`), ce qui réduit le coût de la parité. [DÉDUIT]
5. **Les mineures 3.x s'ajoutent sans rien retirer.** `master` est en `project(CSFML VERSION 3.1.0)` : environ +108 fonctions, 0 retirée, 40 déclarations `CSFML_DEPRECATED` par rapport à 3.0.0 (script regex sur `git archive master include`, 926 fonctions pour 3.0.0, identique à `nm -D`). Une même doc « 3.x » avec badges `Depuis 3.1` et `Déprécié en 3.1` suffit. [CONFIRMÉ pour le tag et le décompte local ; INCERTAIN : la 3.1.0 finale peut différer de `master`]

Mise en oeuvre :
- **Sélecteur de version :** un petit composant (surcharge `Header`/`ThemeSelect` de Starlight, ou `starlight-sidebar-topics` avec deux « topics » : 3.x et 2.6) en haut de page. Il renvoie à la **page équivalente** de l'autre version quand elle existe (même slug), sinon à l'accueil de l'autre version. Le plugin `starlight-versions` (0.10.2, « early development ») n'est pas utilisé. [DÉDUIT, À TESTER : sélecteur et slugs équivalents]
- **Sections identiques dans les deux versions :** démarrer (installation, premier programme), guides, référence (une page par type/header), exemples testés, dépannage. La page de migration 2.6 → 3.0 est partagée (`migration/`) et liée depuis les deux.
- **Contenu 2.6 :** rédigé comme la 3.x, mais d'abord pour les écarts : fonctions absentes de 2.6.0 (voir ci-dessous), types différents (`sfEvent`, `sfVector2`, etc., voir 04-migration).
- **Génération :** `extract_api.py` pour les deux tags ; `check_doc_api.py` vérifie chaque page contre l'API de sa version (voir 10-workflow.md §2).
- **CI à poids égal :** matrice 3.0.0 / 2.6.1, cellules bloquantes toutes deux (10-workflow.md §4). Badge « testé sur » par version.
- **Pages jumelles :** le frontmatter porte `tested: ["3.0.0"]` ou `["2.6.1"]` et un lien `twin:` vers la page de l'autre version, utilisé par le sélecteur.

Attention 2.6.0 / 2.6.1 : 2.6.1 ajoute par rapport à 2.6.0 notamment `sfFree`, `sfFont_hasGlyph`, `sfMouse_getPositionWindowBase`, `sfMouse_setPositionWindowBase`, `sfTouch_getPositionWindowBase` et `sf*_createVulkanSurface`. La signature de `sfVulkan_getGraphicsRequiredInstanceExtensions` change aussi (`git diff 2.6.0 2.6.1 -- include/`). Les pages 2.6 portent donc un badge `2.6.1+` pour ces fonctions, car Ubuntu 24.04 est en 2.6.0. [CONFIRMÉ]

Plus tard, pour une CSFML 4, on archivera la 3.x avec `starlight-versions` ou on basculera sur Docusaurus. [DÉDUIT]

### i18n retenue
- La locale **`fr` est la racine** (URL sans préfixe) et **`en`** est sous `/en/`. Le français est rédigé d'abord, car le public principal est Epitech. Les pages anglaises manquantes retombent sur le français avec un avis. [DÉDUIT]
- C'est réversible : on pourra passer `en` en racine plus tard, ce qui ne change que des URL et des redirections.

---

## Décisions prises

1. **Générateur : Starlight**, repli **Docusaurus**. MkDocs Material est écarté (fin de vie au 2027-05-05) et Zensical aussi (bêta 0.1.0, à réévaluer mi-2027).
2. Le projet Astro est **à la racine du dépôt**, avec `examples/`, `tests/`, `tools/` et `api/` à côté.
3. **Versionnage (révisé, parité) :** deux versions de première classe, 3.x (racine) et 2.6 (`/v2.6/`), mêmes sections, sélecteur de version, référence générée pour les deux, CI à poids égal (3.0.0 et 2.6.1) ; mineures 3.x signalées par des badges, sans arbre séparé ; pas de plugin `starlight-versions` pour l'instant.
4. **i18n :** FR en locale racine, EN sous `/en/`, avec le repli natif.
5. **Exemples :** un vrai `main.c` par exemple, affiché par un composant `<Example>` (import `?raw` + `<Code>`), jamais recopié.
6. **Recherche :** Pagefind, sans Algolia.
7. **Node 24 LTS** en CI et en local, à cause de l'exigence d'Astro 7 (`node >=22.23.4`).
8. Plugins prévus : `starlight-links-validator` (obligatoire) ; `starlight-sidebar-topics` (une barre latérale par version, probable) ; `starlight-llms-txt` (optionnel).

## Points [INCERTAIN]

- L'import `?raw` d'un fichier hors de `src/` (`examples/` à la racine) n'a pas été vérifié avec Astro 7 / Vite. Le mode dev peut aussi poser problème (`server.fs.allow`). [À TESTER]
- Une locale racine `fr` combinée à `en` en sous-dossier, et le comportement exact de `/en/` pour une page non traduite, sont à vérifier sur un prototype.
- Le comportement de repli i18n de Docusaurus (option de repli seulement) n'a pas été trouvé dans les pages lues.
- Les notes « rapidité » et « accessibilité » ne sont pas mesurées.
- Le contenu final de CSFML 3.1.0 est inconnu : les chiffres viennent de `master` au 2026-11-01.
- La présence effective de CSFML 3.0.0 dans `epitest-docker:latest` est déduite des dépendances et du dépôt Ubuntu, pas constatée dans le conteneur.
- L'article sur MkDocs 2.0 vient de l'équipe Material, qui est partie prenante. Les affirmations « plus de plugins » et « TOML » sont à recouper avec la doc officielle de MkDocs 2.0 quand elle sera publiée.
- `prism-react-renderer` (Docusaurus) : le support de `c` via la dépendance de `cpp` n'a pas été testé.
