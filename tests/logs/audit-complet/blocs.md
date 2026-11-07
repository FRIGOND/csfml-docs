# Audit des blocs de code

Généré par `python3 tools/audit-blocks.py`. **importé** = `<Example/>` (vrai fichier d'examples/, compilé et exécuté par `tools/run-all-examples.sh`) ; **extrait** = bloc ```c dont toutes les lignes sont dans un main.c d'examples/ ; **partiel** = au moins la moitié ; **non testé** = écrit à la main ; **shell** = commande (test éventuel dans tests/RESULTATS.md) ; **autre** = texte, sortie, etc.

Totaux : importé 38, extrait 73, partiel 3, non testé 45, shell 48, autre 2

| Page | importés | blocs (ligne:langue:statut) |
|---|---|---|
| 2-6/getting-started/first-window.mdx | 1 | 14:sh:shell |
| 2-6/tutorials/01-window.mdx | 1 | 32:c:extrait, 39:c:extrait, 55:c:extrait, 63:c:extrait, 79:c:extrait, 108:sh:shell |
| 2-6/tutorials/02-shapes.mdx | 1 | 25:c:extrait, 44:c:extrait, 55:c:extrait, 80:sh:shell |
| 2-6/tutorials/03-movement.mdx | 1 | 29:c:extrait, 39:c:extrait, 54:c:extrait, 74:sh:shell |
| 2-6/tutorials/04-input.mdx | 1 | 26:c:extrait, 37:c:extrait, 51:c:extrait, 70:sh:shell |
| 2-6/tutorials/05-collisions.mdx | 1 | 31:c:extrait, 43:c:extrait, 50:c:extrait, 63:c:extrait, 85:sh:shell |
| 2-6/tutorials/06-sprites.mdx | 1 | 36:c:extrait, 44:c:extrait, 54:c:extrait, 61:c:extrait, 74:c:extrait, 84:c:extrait, 95:c:extrait, 130:sh:shell |
| 2-6/tutorials/07-text-score.mdx | 1 | 30:c:extrait, 41:c:extrait, 62:c:extrait, 77:c:extrait, 103:sh:shell |
| 2-6/tutorials/08-sound-states.mdx | 1 | 31:c:extrait, 39:c:extrait, 62:c:extrait, 86:c:extrait, 105:sh:shell |
| 2-6/tutorials/09-final.mdx | 1 | 48:c:extrait, 60:c:extrait, 91:sh:shell |
| 3-0/getting-started/first-window.mdx | 1 | 23:sh:shell |
| 3-0/migration/checklist.mdx | 0 | 26:sh:shell, 34:sh:shell |
| 3-0/migration/events.mdx | 0 | 24:c:non testé, 45:c:non testé, 97:c:non testé, 112:c:non testé, 136:c:non testé, 143:c:non testé, 179:c:non testé, 186:c:non testé |
| 3-0/migration/objects-creation.mdx | 0 | 44:c:non testé, 56:c:non testé, 72:c:non testé, 83:c:non testé, 98:c:non testé, 111:c:non testé, 140:c:non testé, 148:c:non testé, 158:c:non testé |
| 3-0/migration/overview.mdx | 2 | 39:c:extrait, 47:c:extrait, 64:c:non testé, 73:c:non testé, 89:c:non testé, 100:c:non testé, 122:c:non testé, 130:c:non testé, 149:c:non testé, 170:c:non testé, 200:c:non testé, 212:c:non testé, 241:c:non testé, 250:c:non testé, 267:c:non testé, 278:c:non testé, 308:c:non testé, 321:c:non testé, 353:c:non testé, 365:c:non testé |
| 3-0/migration/support-both.mdx | 0 | 33:c:non testé, 133:c:non testé, 171:sh:shell |
| 3-0/migration/types.mdx | 0 | 17:c:non testé, 26:c:non testé, 67:c:non testé, 79:c:non testé, 111:c:non testé, 123:c:non testé |
| 3-0/tutorials/01-window.mdx | 1 | 32:c:extrait, 39:c:extrait, 55:c:extrait, 63:c:extrait, 79:c:extrait, 108:sh:shell |
| 3-0/tutorials/02-shapes.mdx | 1 | 25:c:extrait, 44:c:extrait, 55:c:extrait, 80:sh:shell |
| 3-0/tutorials/03-movement.mdx | 1 | 29:c:extrait, 39:c:extrait, 54:c:extrait, 74:sh:shell |
| 3-0/tutorials/04-input.mdx | 1 | 26:c:extrait, 37:c:extrait, 51:c:extrait, 70:sh:shell |
| 3-0/tutorials/05-collisions.mdx | 1 | 31:c:extrait, 43:c:extrait, 50:c:extrait, 63:c:extrait, 85:sh:shell |
| 3-0/tutorials/06-sprites.mdx | 1 | 36:c:extrait, 44:c:extrait, 54:c:extrait, 61:c:extrait, 74:c:extrait, 84:c:extrait, 95:c:extrait, 130:sh:shell |
| 3-0/tutorials/07-text-score.mdx | 1 | 30:c:extrait, 41:c:extrait, 62:c:extrait, 77:c:extrait, 103:sh:shell |
| 3-0/tutorials/08-sound-states.mdx | 1 | 31:c:extrait, 39:c:extrait, 62:c:extrait, 86:c:extrait, 105:sh:shell |
| 3-0/tutorials/09-final.mdx | 1 | 48:c:extrait, 60:c:extrait, 91:sh:shell |
| en/2-6/getting-started/first-window.mdx | 1 | — |
| en/3-0/getting-started/first-window.mdx | 1 | 22:sh:shell |
| en/index.mdx | 0 | — |
| guides/concepts/api-conventions.mdx | 0 | 25:c:non testé |
| guides/concepts/audio-without-device.mdx | 2 | — |
| guides/concepts/coordinates-views.mdx | 4 | — |
| guides/concepts/events.mdx | 2 | — |
| guides/concepts/game-loop.mdx | 2 | 21:c:extrait |
| guides/concepts/index.mdx | 0 | — |
| guides/concepts/memory.mdx | 0 | 9:sh:shell, 15:sh:shell, 26:sh:shell |
| guides/concepts/ownership.mdx | 0 | — |
| guides/concepts/resources.mdx | 2 | 14:c:partiel, 40:c:partiel |
| guides/concepts/time.mdx | 2 | 22:c:partiel |
| guides/epitech/before-you-submit.mdx | 0 | 15:sh:shell, 31:sh:shell, 41:sh:shell |
| guides/epitech/coding-style.mdx | 0 | 11:sh:shell |
| guides/epitech/environment.mdx | 0 | 31:sh:shell |
| guides/epitech/index.mdx | 0 | — |
| guides/epitech/makefile.mdx | 0 | — |
| guides/epitech/project-notions.mdx | 0 | — |
| guides/help/about.mdx | 0 | — |
| guides/help/faq.mdx | 0 | — |
| guides/help/glossary.mdx | 0 | — |
| guides/index.mdx | 0 | — |
| guides/install/arch.mdx | 0 | 5:sh:shell, 17:sh:shell |
| guides/install/check-your-setup.mdx | 0 | 12:c:non testé, 58:sh:shell, 66:sh:shell |
| guides/install/compiling.mdx | 0 | 9:sh:shell, 39:sh:shell, 47:sh:shell, 60:make:shell, 76:text:autre |
| guides/install/fedora.mdx | 0 | 5:sh:shell |
| guides/install/from-source.mdx | 0 | 26:sh:shell, 50:text:autre, 64:sh:shell |
| guides/install/index.mdx | 0 | — |
| guides/install/macos-windows.mdx | 0 | 11:sh:shell, 19:sh:shell |
| guides/install/ubuntu-debian.mdx | 0 | 9:sh:shell, 27:sh:shell |
| guides/install/wsl.mdx | 0 | — |
| guides/troubleshooting/black-window.mdx | 0 | — |
| guides/troubleshooting/compile-errors.mdx | 0 | — |
| guides/troubleshooting/headless-xvfb.mdx | 0 | 11:sh:shell |
| guides/troubleshooting/index.mdx | 0 | — |
| guides/troubleshooting/link-errors.mdx | 0 | — |
| guides/troubleshooting/no-sound.mdx | 0 | — |
| guides/troubleshooting/runtime-crashes.mdx | 0 | — |
| index.mdx | 0 | — |
