#!/usr/bin/env python3
"""Contrôle du site construit (site/dist) : tout lien interne et toute ancre doivent exister.
Usage : python3 tools/check-site.py [dist] [base]    (base = SITE_BASE, ex. /lib_csfml ; défaut /)
Sort avec le code 1 s'il y a un lien mort ou une ancre manquante."""
import html.parser, os, re, sys, urllib.parse
dist = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(__file__), "..", "site", "dist")
base = (sys.argv[2] if len(sys.argv) > 2 else "/").rstrip("/")

class P(html.parser.HTMLParser):
    def __init__(self):
        super().__init__(); self.ids, self.links = set(), []
    def handle_starttag(self, tag, attrs):
        a = dict(attrs)
        if "id" in a: self.ids.add(a["id"])
        if tag == "a" and a.get("href"): self.links.append(a["href"])

pages = {}
for root, _, files in os.walk(dist):
    for f in files:
        if f.endswith(".html"):
            path = os.path.join(root, f)
            p = P(); p.feed(open(path, encoding="utf-8").read())
            rel = "/" + os.path.relpath(path, dist).replace(os.sep, "/")
            url = rel[: -len("index.html")] if rel.endswith("/index.html") else rel
            pages[url] = (p.ids, p.links)
bad, n = [], 0
for url, (ids, links) in pages.items():
    for href in links:
        if re.match(r"^(https?:|mailto:|tel:|javascript:)", href): continue
        u = urllib.parse.urlparse(href); n += 1
        path = urllib.parse.unquote(u.path) or url
        if base and path.startswith(base + "/"): path = path[len(base):]
        if not path.startswith("/"): path = urllib.parse.urljoin(url, path)
        if path.endswith(".html") or "." in path.rsplit("/", 1)[-1]:   # fichiers statiques
            if not os.path.exists(os.path.join(dist, path.lstrip("/"))): bad.append((url, href, "fichier absent"))
            continue
        if not path.endswith("/"): path += "/"
        if path not in pages: bad.append((url, href, "page absente")); continue
        if u.fragment and urllib.parse.unquote(u.fragment) not in pages[path][0]:
            bad.append((url, href, "ancre absente"))
print(f"{len(pages)} pages, {n} liens internes vérifiés, {len(bad)} problèmes")
for b in bad[:40]: print("  ", *b)
sys.exit(1 if bad else 0)
