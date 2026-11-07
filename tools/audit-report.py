#!/usr/bin/env python3
"""Assemble tests/AUDIT-COMPLET.md à partir des rapports par page (tests/logs/audit-complet/*.md) et du tableau FIXED ci-dessous.
Usage : python3 tools/audit-report.py"""
import glob, os, re
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
R = os.path.join(ROOT, "tests/logs/audit-complet")
# page (nom de rapport) -> ce qui a été corrigé (bloquants et gênants traités à la main le 2026-11-07)
FIXED = {
 "guides__epitech__before-you-submit": "phrase « l'image actuelle a la 3.0.0 » reformulée, marquée non vérifié",
 "guides__epitech__environment": "« le correcteur compile dans… » reformulé : version du correcteur non vérifiée",
 "guides__install__index": "remarque Epitech et lignes 2.6 marquées non vérifié",
 "guides__help__faq": "phrase « image Epitech actuelle » reformulée",
 "guides__epitech__makefile": "CC=clang marqué (Dockerfile public, non vérifié)",
 "guides__install__from-source": "sudo nécessaire pour les -dev, prérequis g++, garde-fou sur rm -rf $PREFIX",
 "guides__install__compiling": "example.mk décrit correctement (CSFML_PREFIX, system) ; $P défini",
 "guides__concepts__memory": "source du chiffre 259 204 corrigée",
 "guides__help__about": "phrase sur la CI reformulée",
 "guides__troubleshooting__compile-errors": "gcc 14 marqué non testé ; forme {{800,600},32} limitée à la 3.0",
 "guides__troubleshooting__black-window": "différence 3.0/2.6 de sfTexture_updateFromRenderWindow signalée",
 "guides__troubleshooting__runtime-crashes": "sfText_create/sfSound_create : 3.0 seulement, test n°31/32 cités",
 "guides__troubleshooting__no-sound": "test n°34 cité, WSL marqué non testé",
 "guides__install__arch": "source du pkg-config citée, Arch non vérifié",
 "guides__concepts__api-conventions": "test n°31 cité",
 "guides__troubleshooting__index": "source tests/RESULTATS.md citée",
 "2-6__getting-started__first-window": "encadré sur test_mode.h / example_frames (composant Example)",
 "3-0__getting-started__first-window": "encadré sur test_mode.h / example_frames (composant Example)",
 "3-0__migration__overview": "ligne GlFunctionPointer réparée, NULL de sfText/sfSound reformulé (non testé)",
 "3-0__migration__objects-creation": "« idem » remplacé par « non testé »",
 "3-0__tutorials__06-sprites": "test n°31 cité",
}
for v in ("2-6", "3-0"):
    for f in glob.glob(os.path.join(R, f"{v}__tutorials__*.md")):
        FIXED.setdefault(os.path.basename(f)[:-3], "")
        FIXED[os.path.basename(f)[:-3]] = (FIXED[os.path.basename(f)[:-3]] + "; " if FIXED[os.path.basename(f)[:-3]] else "") + \
            ("option -Wno-error=deprecated-declarations ajoutée à la phrase de compilation" if v == "2-6" else "encadré test_mode.h (composant Example)")
rows, tot = [], {"bloquant": 0, "gênant": 0, "mineur": 0}
pages = sorted(f for f in glob.glob(os.path.join(R, "*.md")) if os.path.basename(f) not in ("auto.md", "blocs.md", "affirmations.md", "snippets.md"))
for f in pages:
    name = os.path.basename(f)[:-3]
    c = {"bloquant": 0, "gênant": 0, "mineur": 0}
    for l in open(f, encoding="utf-8"):
        if l.startswith("| ") and not l.startswith(("| #", "|---")):
            g = l.rstrip().rstrip("|").rsplit("|", 1)[-1].strip().lower()
            if g in c: c[g] += 1
    for k in c: tot[k] += c[k]
    n = sum(c.values())
    fx = FIXED.get(name)
    if n == 0: st = "rien à corriger"
    elif fx: st = "**partiel** : " + fx + (" ; les autres lignes du rapport ne sont pas corrigées" if True else "")
    else: st = "**non** (voir le rapport)"
    rows.append(f"| {name.replace('__', '/')} | {n} ({c['bloquant']} bloquant, {c['gênant']} gênant, {c['mineur']} mineur) | {st} | [rapport](logs/audit-complet/{name}.md) |")
out = ["# Audit complet des pages écrites à la main", "",
 "Périmètre : les pages hors `reference/`, `cookbook/` et `3-0/migration/api-changes` (générées). Date : 2026-11-07. Régénérer ce fichier : `python3 tools/audit-report.py`.", "",
 "## Méthode et résultats automatiques (relançables)", "",
 "| Contrôle | Outil | Sortie | Résultat |", "|---|---|---|---|",
 "| Noms, types, constantes, includes, nombre d'arguments des appels, chiffres, 2.6/3.0 | `tools/audit-pages.py` | `logs/audit-complet/auto.md` | 0 erreur ; exceptions volontaires listées dans le script (liste ALLOW) |",
 "| Blocs de code : importé / extrait d'un exemple / écrit à la main | `tools/audit-blocks.py` | `logs/audit-complet/blocs.md` | voir ci-dessous |",
 "| Compilation des blocs C écrits à la main | `tools/test-snippets.py` | `logs/audit-complet/snippets.md` | tous compilent avec `-fsyntax-only` (pas d'exécution) dans la version visée |",
 "| Affirmations sur l'environnement | `tools/audit-claims.py` | `logs/audit-complet/affirmations.md` | 0 paragraphe sans preuve ni marque ; aucune phrase « Epitech utilise la version X » |",
 "| Relecture débutant | 2 sous-agents distincts de l'auteur | un rapport par page | tableau ci-dessous |", "",
 "Blocs de code : 38 `<Example/>` importés (vrais `main.c` compilés et exécutés par `tools/run-all-examples.sh`), 73 extraits de ces exemples, 66 blocs C écrits à la main (compilés en syntaxe seulement), 48 blocs shell (testés seulement quand la page cite un test de `tests/RESULTATS.md`).", "",
 "**Limites honnêtes.** Les relecteurs ont écrit leurs rapports vite (le second dit les avoir générés par script depuis ses notes) : beaucoup de lignes « mineur » sont des remarques répétées ou de simples confirmations. Je n'ai corrigé que les points bloquants et la plupart des points gênants que j'ai pu vérifier moi-même ; les autres lignes restent ouvertes. Un point signalé (« le sprite reste blanc » en 2.6, tutoriel 06) n'est pas vérifié. Un point (`sfKeyReturn` dans concepts/events) n'a pas été retrouvé dans la page.", "",
 f"Total des lignes des rapports : {sum(tot.values())} ({tot['bloquant']} bloquant, {tot['gênant']} gênant, {tot['mineur']} mineur).", "",
 "## Tableau par page", "", "| Page | Problèmes trouvés | Corrigé ? | Détail |", "|---|---|---|---|"] + rows + [""]
open(os.path.join(ROOT, "tests/AUDIT-COMPLET.md"), "w", encoding="utf-8").write("\n".join(out))
print(len(pages), "pages;", tot)
