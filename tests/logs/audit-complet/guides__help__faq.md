# guides/help/faq.mdx
Verdict : A CORRIGER (6 problèmes)
| # | Catégorie | Citation | Problème | Gravité |
|---|---|---|---|---|
| 1 | 6 | « Pourquoi `sfSprite_create(NULL)` plante ? En 3.0, il faut une texture valide. » | Ne dit pas qu'en 2.6 `sfSprite_create(void)` n'a pas de paramètre (JSON) : la question n'a pas de sens en 2.6. | mineur |
| 2 | 6 | « `sfText_setString` décode mal l'UTF-8 (test réel) » | Test fait en 3.0.0 seulement (n°18) ; 2.6 non dit. | mineur |
| 3 | 7 | « SFML 3.0.2 n'a que le backend X11 ... sous Wayland, ça passe par XWayland si `DISPLAY` est défini » | Marqué « non vérifié » et « non testé » : conforme. Test cité sans n° (lot 4). | mineur |
| 4 | 3 | « Sur Ubuntu : `dpkg -l libcsfml-dev` » | Fonctionne seulement si le paquet est installé ; message sinon non décrit. | mineur |
| 5 | 2 | « environnement de rendu », « backend », « XWayland », « stderr », « delta time », « valgrind » | Non définis dans la FAQ ; le glossaire n'est pas lié. | gênant |
| 6 | 7 | « une partie vient d'OpenGL/X11 (« still reachable »)... » | Non prouvé par test (voir memory.mdx) ; pas de marque. | mineur |
Vérifié : tous les liens internes (guides, /3-0/migration/overview/, /3-0/reference/, /2-6/reference/) existent. La phrase sur l'image `latest` est prudente (« non vérifié ») : pas de formulation interdite.
