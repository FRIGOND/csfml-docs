#!/usr/bin/env python3
"""Ajoute le `base` GitHub Pages (/<dépôt>) aux liens racine du contenu Markdown/MDX.
Usage : python3 tools/fix-base.py <dist> <base>     (sans effet si base vaut « / » ou est vide)
Les liens écrits « /3-0/... » dans le contenu n'ont pas le base ; Starlight ne le met pas pour nous.
Les liens déjà préfixés (générés par Starlight) sont laissés tels quels. tools/check-site.py contrôle le résultat."""
import os, re, sys
dist, base = sys.argv[1], (sys.argv[2] if len(sys.argv) > 2 else "/").rstrip("/")
if not base: sys.exit(0)
pat = re.compile(r'\b(href|src)="(/(?!/)[^"]*)"')
def sub(m):
    v = m.group(2)
    return m.group(0) if v == base or v.startswith(base + "/") else f'{m.group(1)}="{base}{v}"'
n = 0
for root, _, files in os.walk(dist):
    for f in files:
        if f.endswith(".html"):
            p = os.path.join(root, f); s = open(p, encoding="utf-8").read(); t = pat.sub(sub, s)
            if t != s: open(p, "w", encoding="utf-8").write(t); n += 1
print(f"fix-base : {n} fichiers corrigés (base {base})")
