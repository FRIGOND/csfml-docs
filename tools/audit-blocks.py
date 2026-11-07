#!/usr/bin/env python3
"""Audit des blocs de code des pages écrites à la main.
Un bloc ```c est « extrait » si chacune de ses lignes (espaces normalisés) se trouve dans un main.c d'examples/ de la bonne version
(donc compilé et exécuté par tools/run-all-examples.sh). Sinon il est « non testé ».
Les blocs sh/make/etc. sont listés à part : leur test est dans tests/RESULTATS.md ou absent.
Les <Example .../> importent le vrai fichier : ils sont comptés comme « importés ».
Usage : python3 tools/audit-blocks.py [--md sortie.md]"""
import glob, os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS = os.path.join(ROOT, "site/src/content/docs")
GENERATED = re.compile(r"/(reference|cookbook)/|/3-0/migration/api-changes\.mdx$")
ws = lambda s: re.sub(r"\s+", " ", s).strip()

def corpus(v):
    t = ""
    for f in glob.glob(os.path.join(ROOT, "examples/*/*/%s/*.[ch]" % v)) + glob.glob(os.path.join(ROOT, "examples/common/*")):
        try: t += "\n" + open(f, encoding="utf-8").read()
        except (IsADirectoryError, UnicodeDecodeError): pass
    return {ln for ln in map(ws, t.splitlines()) if ln}

CORP = {"3.0": corpus("3.0"), "2.6": corpus("2.6")}
pages = [p for p in subprocess.run(["git", "ls-files", "site/src/content/docs"], cwd=ROOT, capture_output=True, text=True).stdout.split()
         if p.endswith((".md", ".mdx")) and not GENERATED.search("/" + p)]
rows, tot = [], {"importé": 0, "extrait": 0, "partiel": 0, "non testé": 0, "shell": 0, "autre": 0}
for p in pages:
    rel = p.split("docs/", 1)[1]
    text = open(os.path.join(ROOT, p), encoding="utf-8").read()
    text = re.sub(r"^---\n.*?\n---\n", "", text, count=1, flags=re.S)
    vers = ["3.0"] if rel.startswith(("3-0/", "en/3-0/")) else ["2.6"] if rel.startswith(("2-6/", "en/2-6/")) else ["3.0", "2.6"]
    imp = len(re.findall(r"<Example\b", text))
    tot["importé"] += imp
    items = []
    # un bloc dans un onglet est jugé sur la version de l'onglet
    tabs = [(m.start(), m.end(), [v for v in ("3.0", "2.6") if v in m.group(1)]) for m in re.finditer(r'<TabItem\s+label="([^"]*)"', text)]
    for m in re.finditer(r"^```(\w*)[^\n]*\n(.*?)^```", text, re.S | re.M):
        lang, code = m.group(1), m.group(2)
        line = text.count("\n", 0, m.start()) + 2
        bv = vers
        for s0, e0, vs in tabs:
            if s0 < m.start() and vs and m.start() - s0 < 6000 and "</TabItem>" not in text[s0:m.start()]: bv = vs
        if lang in ("c", "h", "cpp"):
            lines = [ws(l) for l in code.splitlines() if ws(l)]
            code_lines = [l for l in lines if not l.startswith(("//", "/*", "*", "#include"))] or lines
            inn = [any(l in CORP[v] for v in bv) for l in code_lines]
            ratio = sum(inn) / len(code_lines)
            st = "extrait" if ratio == 1 else "partiel" if ratio >= 0.5 else "non testé"
        elif lang in ("sh", "bash", "console", "make", "makefile", "cmake"): st = "shell"
        else: st = "autre"
        tot[st] += 1
        items.append((line, lang or "-", st, len(code.splitlines())))
    rows.append((rel, imp, items))
out = ["# Audit des blocs de code", "",
       "Généré par `python3 tools/audit-blocks.py`. **importé** = `<Example/>` (vrai fichier d'examples/, compilé et exécuté par `tools/run-all-examples.sh`) ; "
       "**extrait** = bloc ```c dont toutes les lignes sont dans un main.c d'examples/ ; **partiel** = au moins la moitié ; **non testé** = écrit à la main ; "
       "**shell** = commande (test éventuel dans tests/RESULTATS.md) ; **autre** = texte, sortie, etc.", "",
       "Totaux : " + ", ".join(f"{k} {v}" for k, v in tot.items()), "", "| Page | importés | blocs (ligne:langue:statut) |", "|---|---|---|"]
for rel, imp, items in rows:
    out.append(f"| {rel} | {imp} | " + (", ".join(f"{l}:{g}:{s}" for l, g, s, _ in items) or "—") + " |")
txt = "\n".join(out) + "\n"
if "--md" in sys.argv: open(sys.argv[sys.argv.index("--md") + 1], "w", encoding="utf-8").write(txt)
else: print(txt)
print(tot, file=sys.stderr)
