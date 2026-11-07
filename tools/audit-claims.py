#!/usr/bin/env python3
"""Inventaire des affirmations sur l'environnement (distributions, paquets, versions, Epitech, WSL, macOS, Wayland...).
Chaque paragraphe contenant un mot-clé doit avoir une preuve (n° de test, tests/RESULTATS, research/, lien source) ou la marque « non vérifié ».
Sinon il est listé « SANS MARQUE ». Interdit : « Epitech utilise la version X » (phrase affirmative sans source).
Usage : python3 tools/audit-claims.py [--md sortie.md]   (code 1 s'il reste des SANS MARQUE)"""
import os, re, subprocess, sys
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GENERATED = re.compile(r"/(reference|cookbook)/|/3-0/migration/api-changes\.mdx$")
KEY = re.compile(r"\b(Ubuntu|Debian|Fedora|Arch|Manjaro|openSUSE|macOS|Homebrew|brew|WSL2?|WSLg|Windows|MSYS2?|vcpkg|Wayland|X11|Xorg|Xvfb|Epitech|epitest|Docker|apt|dnf|pacman|Mesa|PulseAudio|PipeWire|OpenAL|Repology|Launchpad|nix|Nix)\b")
PROOF = re.compile(r"n°\s?\d+|tests?/|RESULTATS|research/|\bsource\b|Sources?\s*:|https?://|repology|packages\.|launchpad|[Nn]on v[ée]rifi|[Nn]on test|NON VERIFI|non retest|non confirm|d.après|selon|`apt|vérifi|testé|constaté|observé", re.I)
FORBID = re.compile(r"Epitech\s+(utilise|installe|fournit|impose|a|est en)\b[^.]*\b(2\.6|3\.0|CSFML|SFML)\b", re.I)
pages = [p for p in subprocess.run(["git", "ls-files", "site/src/content/docs"], cwd=ROOT, capture_output=True, text=True).stdout.split()
         if p.endswith((".md", ".mdx")) and not GENERATED.search("/" + p)]
rows, bad = [], 0
for p in pages:
    rel = p.split("docs/", 1)[1]
    text = open(os.path.join(ROOT, p), encoding="utf-8").read()
    text = re.sub(r"^---\n.*?\n---\n", "", text, count=1, flags=re.S)
    text = re.sub(r"^```.*?^```", lambda m: "\n" * m.group(0).count("\n"), text, flags=re.S | re.M)
    base = 0
    for para in re.split(r"\n\s*\n", text):
        line = text[: text.find(para)].count("\n") + 1 if para else 0
        ks = sorted({m.group(1) for m in KEY.finditer(para)})
        if ks and not para.lstrip().startswith(("import ", "<Tabs", "</Tabs", "#")):
            st = "preuve" if PROOF.search(para) else "SANS MARQUE"
            if FORBID.search(para): st = "INTERDIT"
            if st != "preuve": bad += 1
            rows.append((rel, line, st, ",".join(ks), re.sub(r"\s+", " ", para)[:160]))
out = ["# Inventaire des affirmations sur l'environnement", "",
       "Généré par `python3 tools/audit-claims.py`. preuve = le paragraphe cite un test (n°), un fichier research/, une source ou porte la marque « non vérifié ». "
       "SANS MARQUE = à corriger ; INTERDIT = affirme « Epitech utilise la version X ».", "",
       f"{len(rows)} paragraphes, {sum(r[2]=='preuve' for r in rows)} avec preuve, {bad} à traiter.", "", "| Page:ligne | Statut | Mots | Début du paragraphe |", "|---|---|---|---|"]
for rel, line, st, ks, t in rows: out.append(f"| {rel}:{line} | {st} | {ks} | {t.replace('|','\\|')} |")
txt = "\n".join(out) + "\n"
if "--md" in sys.argv: open(sys.argv[sys.argv.index("--md") + 1], "w", encoding="utf-8").write(txt)
else: print(txt)
print(f"{len(rows)} paragraphes, {bad} à traiter", file=sys.stderr)
sys.exit(1 if bad else 0)
