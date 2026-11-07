#!/usr/bin/env python3
"""Audit automatique des pages écrites à la main contre les JSON de l'API.
Pour chaque page : extrait les noms sf*/CSFML_*, les -lcsfml-*, les headers <CSFML/..>, les options CMake (-D...),
et vérifie leur existence en 3.0 et en 2.6. Vérifie aussi le nombre d'arguments des appels sfXxx_yyy(...) dans les blocs de code.
Usage : python3 tools/audit-pages.py [--md sortie.md]      (relançable ; code 1 s'il reste une erreur « absent des deux versions »)
Contexte de version d'une page : 3-0/... -> 3.0 ; 2-6/... -> 2.6 ; sinon « les deux » (guides)."""
import json, os, re, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS = os.path.join(ROOT, "site/src/content/docs")
VERS = {"3.0": "csfml-3.0.0.json", "2.6": "csfml-2.6.1.json"}
# Pages générées : hors périmètre (contrôlées par gen_site_pages.py)
GENERATED = re.compile(r"/(reference|cookbook)/|/3-0/migration/api-changes\.mdx$")

api = {}
for v, f in VERS.items():
    d = json.load(open(os.path.join(ROOT, "api", f)))
    names = {x["name"] for x in d["functions"]} | {x["name"] for x in d["types"]} | {x["name"] for x in d["variables"]}
    names |= {m["name"] for m in d["macros"]}
    for t in d["types"]:
        for val in t.get("values", []) or []:
            names.add(val["name"])
        for fld in t.get("fields", []) or []:
            pass
    api[v] = dict(names=names, funcs={x["name"]: x for x in d["functions"]},
                  headers={h if isinstance(h, str) else h.get("path", "") for h in d["headers"]})

def pages():
    out = subprocess.run(["git", "ls-files", "site/src/content/docs"], cwd=ROOT, capture_output=True, text=True).stdout.split()
    return [p for p in out if p.endswith((".md", ".mdx")) and not GENERATED.search("/" + p)]

def version_ctx(path):
    rel = path.split("docs/", 1)[1]
    if "/migration/" in "/" + rel: return ["3.0", "2.6"]
    if rel.startswith("3-0/") or rel.startswith("en/3-0/"): return ["3.0"]
    if rel.startswith("2-6/") or rel.startswith("en/2-6/"): return ["2.6"]
    return ["3.0", "2.6"]

ID = re.compile(r"\b(sf[A-Z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*_?|CSFML_VERSION_[A-Z]+)\b")
# macros de la 2.6 que libclang ne liste pas (#define dans SFML/Config.h)
api["2.6"]["names"] |= {"sfTrue", "sfFalse"}
TAB = re.compile(r"<TabItem\s+label=\"([^\"]*)\"[^>]*>(.*?)</TabItem>", re.S)
def scoped(body):
    """Découpe la page en (texte, versions imposées par un onglet ou None)."""
    parts, last = [], 0
    for m in TAB.finditer(body):
        parts.append((body[last:m.start()], None))
        lab = m.group(1)
        vs = [v for v in ("3.0", "2.6") if v in lab]
        parts.append((m.group(2), vs or None)); last = m.end()
    parts.append((body[last:], None))
    return parts
LIB = re.compile(r"-l(csfml-[a-z]+)|\b(csfml-(?:all|system|window|graphics|audio|network))\b")
HDR = re.compile(r"[<\"](C?SFML/[A-Za-z0-9_/]+\.h)[>\"]")
DOPT = re.compile(r"-D\s?([A-Z][A-Z0-9_]+)(?:=([^\s\\`'\"]+))?")
CALL = re.compile(r"\b(sf[A-Z][A-Za-z0-9]*_[A-Za-z0-9]+)\s*\(")

def split_args(s):
    """Arguments de premier niveau d'un appel dont s est le texte après '('. Renvoie None si pas fermé sur le texte."""
    depth, cur, args, i = 0, "", [], 0
    while i < len(s):
        c = s[i]
        if c in "([{": depth += 1
        elif c in ")]}":
            if depth == 0:
                if cur.strip() or args: args.append(cur.strip())
                return args
            depth -= 1
        if c == "," and depth == 0: args.append(cur.strip()); cur = ""
        else: cur += c
        i += 1
    return None

def fenced_blocks(text):
    for m in re.finditer(r"^```(\w*)[^\n]*\n(.*?)^```", text, re.S | re.M):
        yield m.group(1), m.group(2)

# Mentions volontaires qui ne sont pas des erreurs : (fin du chemin de la page, nom) -> raison
ALLOW = {
    ("2-6/tutorials/07-text-score.mdx", "sfChar32"): "dit que le type n'existe pas en 2.6",
    ("3-0/migration/overview.mdx", "sfShader_set"): "famille sfShader_set*Parameter (motif avec *)",
    ("3-0/migration/overview.mdx", "sfTexture_update"): "famille sfTexture_update* (motif avec *)",
    ("3-0/tutorials/08-sound-states.mdx", "sfKeyReturn"): "dit que l'ancien nom a été supprimé en 3.0",
    ("guides/concepts/api-conventions.mdx", "sfX_create"): "nom générique (motif)",
    ("guides/concepts/api-conventions.mdx", "sfX_createFromFile"): "nom générique (motif)",
    ("guides/concepts/api-conventions.mdx", "sfX_destroy"): "nom générique (motif)",
    ("guides/concepts/api-conventions.mdx", "sfType_action"): "nom générique (motif)",
    ("guides/concepts/api-conventions.mdx", "sfBool"): "onglet 3.0 : dit que sfBool n'existe plus",
    ("guides/concepts/api-conventions.mdx", "sfTrue"): "onglet 3.0 : dit que sfTrue n'existe plus",
    ("guides/concepts/api-conventions.mdx", "sfFalse"): "onglet 3.0 : dit que sfFalse n'existe plus",
    ("guides/install/check-your-setup.mdx", "sfRenderWindow_create"): "branches #if/#else : 3.0 puis 2.6",
    ("guides/concepts/game-loop.mdx", "sfRenderWindow_draw"): "famille sfRenderWindow_draw… (motif)",
}
rows, problems = [], 0
for p in pages():
    text = open(os.path.join(ROOT, p), encoding="utf-8").read()
    ctx = version_ctx(p)
    body = re.sub(r"^---\n.*?\n---\n", "", text, count=1, flags=re.S)
    issues = []
    # 1. identifiants
    seen = {}
    for part, vs in scoped(body):
        for m in ID.finditer(part):
            seen.setdefault((m.group(1), tuple(vs or ctx)), 0)
    migr = "/migration/" in p
    for name, vctx in sorted(seen):
        vctx = list(vctx)
        if name.endswith("_"):   # préfixe (sfRenderWindow_) : doit préfixer au moins une fonction
            ok = {v: any(n.startswith(name) for n in api[v]["names"]) for v in api}
        else:
            ok = {v: name in api[v]["names"] for v in api}
        if not any(ok.values()):
            issues.append(("ERREUR", f"`{name}` absent des deux versions")); continue
        for v in vctx:
            if not ok[v]:
                other = [w for w in api if w != v][0]
                if migr: continue    # page de migration : les deux versions sont citées exprès
                lvl = "ERREUR" if len(vctx) == 1 else "A VERIFIER"
                issues.append((lvl, f"`{name}` n'existe qu'en {other}" + ("" if len(vctx) == 1 else " (page commune : doit le dire)") + (f" [onglet {vctx[0]}]" if vctx != ctx else "")))
    # 2. bibliothèques et headers
    for m in LIB.finditer(body):
        lib = m.group(1) or m.group(2)
        if lib not in {"csfml-all", "csfml-system", "csfml-window", "csfml-graphics", "csfml-audio", "csfml-network"}:
            issues.append(("ERREUR", f"bibliothèque inconnue `{lib}`"))
    for m in HDR.finditer(body):
        h = m.group(1)                         # CSFML/... = écriture 3.0 ; SFML/... = écriture 2.6
        claimed = "3.0" if h.startswith("CSFML/") else "2.6"
        if h not in api[claimed]["headers"]:
            issues.append(("ERREUR", f"header `{h}` absent de l'API {claimed}"))
        elif len(ctx) == 1 and claimed not in ctx and "/migration/" not in p:
            issues.append(("ERREUR", f"header `{h}` est l'écriture {claimed}, la page est en {ctx[0]}"))
    for m in DOPT.finditer(body):
        issues.append(("INFO", f"option CMake `-D{m.group(1)}` (non contrôlable avec les JSON : voir tests/RESULTATS.md)"))
    # 3. nombre d'arguments des appels dans les blocs de code
    blocks = [(l, c, vs) for part, vs in scoped(body) for l, c in fenced_blocks(part)]
    for lang, code, vs in blocks:
        bctx = vs or ctx
        if lang in ("sh", "bash", "console", "text", "cmake", "make", "makefile", "diff", "ini", "toml"): continue
        for m in CALL.finditer(code):
            fn = m.group(1)
            args = split_args(code[m.end():])
            if args is None or any("..." in a or "…" in a for a in args): continue
            for v in bctx:
                if len(bctx) > 1 and "/migration/" in p: continue
                f = api[v]["funcs"].get(fn)
                if f is None: continue
                want = len([x for x in f["params"]])
                line_start = code.rfind("\n", 0, m.start()) + 1
                prefix = code[line_start:m.start()]
                if re.search(r"\b(?:void|int|float|sf\w+|unsigned|const|char|size_t)\s+\**\s*$", prefix):
                    continue  # déclaration / prototype, pas un appel
                if len(args) != want:
                    issues.append(("ERREUR", f"appel `{fn}` avec {len(args)} argument(s), {want} attendu(s) en {v}"))
    kept = []
    for lvl, msg in issues:
        m = re.match(r"`([^`]+)`", msg)
        key = (p.split("docs/", 1)[1], m.group(1)) if m else None
        m2 = re.match(r"appel `([^`]+)`", msg)
        if m2: key = (p.split("docs/", 1)[1], m2.group(1))
        if lvl == "ERREUR" and key in ALLOW:
            kept.append(("OK", msg + f" — voulu : {ALLOW[key]}"))
        else: kept.append((lvl, msg))
    issues = kept
    rows.append((p, ctx, issues))
    problems += sum(1 for l, _ in issues if l == "ERREUR")


# ---- chiffres cités dans les pages, recomptés depuis les JSON ----
def _norm(x):
    tm = {r"\bsfBool\b": "bool", r"\bsfTrue\b": "true", r"\bsfFalse\b": "false", r"\bsfUint(8|16|32|64)\b": r"uint\1_t", r"\bsfInt(8|16|32|64)\b": r"int\1_t", r"\b_Bool\b": "bool"}
    for k, v in tm.items(): x = re.sub(k, v, x)
    return re.sub(r"\s+", " ", x).strip()
def _typeonly():
    fa, fb = api["2.6"]["funcs"], api["3.0"]["funcs"]
    n = 0
    for name in set(fa) & set(fb):
        if fa[name]["signature"] != fb[name]["signature"] and _norm(fa[name]["signature"]) == _norm(fb[name]["signature"]): n += 1
    return n
destroy = {v: len([n for n in api[v]["funcs"] if n.endswith("_destroy")]) for v in api}
# (page, regex avec 1 groupe numérique, valeur recomptée, commentaire)
COUNT_CLAIMS = [
    ("3-0/migration/types.mdx", r"\((\d+) fonctions\)", _typeonly(), "fonctions dont seul le type change (sfBool→bool, sfUint8→uint8_t…)"),
    ("3-0/migration/overview.mdx", r"il touche (\d+) fonctions", _typeonly(), "fonctions dont seul le type change"),
    ("3-0/migration/objects-creation.mdx", r"sans effet pour les (\d+) fonctions testées", destroy["3.0"], "fonctions *_destroy 3.0"),
    ("guides/help/about.mdx", r"csfml-3\.0\.0\.json` \((\d+) fonctions\)", len(api["3.0"]["funcs"]), "fonctions 3.0"),
    ("guides/help/about.mdx", r"csfml-2\.6\.1\.json` \((\d+) fonctions\)", len(api["2.6"]["funcs"]), "fonctions 2.6"),
    ("guides/concepts/memory.mdx", r"les (\d+) fonctions `\*_destroy` appelées avec `NULL`", destroy["3.0"], "fonctions *_destroy 3.0"),
    ("guides/concepts/memory.mdx", r"\d+ des (\d+) fonctions `\*_destroy`", destroy["2.6"], "fonctions *_destroy 2.6"),
    ("guides/troubleshooting/runtime-crashes.mdx", r"Les (\d+) fonctions `\*_destroy`", destroy["3.0"], "fonctions *_destroy 3.0"),
]
count_out = ["", "## Chiffres recomptés depuis les JSON", "", "| Page | Dans la page | Recompté | Sens | Verdict |", "|---|---|---|---|---|"]
for page, rx, real, what in COUNT_CLAIMS:
    t = open(os.path.join(DOCS, page), encoding="utf-8").read()
    m = re.search(rx, t)
    got = int(m.group(1)) if m else None
    ok = got == real
    if not ok: problems += 1
    count_out.append(f"| {page} | {got if m else 'introuvable'} | {real} | {what} | {'OK' if ok else 'ERREUR'} |")

out = ["# Audit automatique des noms et signatures", "",
       "Généré par `python3 tools/audit-pages.py` (relançable). ERREUR = à corriger ; A VERIFIER = vérifier à la main ; INFO = non contrôlable ici.", ""]
for p, ctx, issues in rows:
    out.append(f"## {p.split('docs/',1)[1]}  (contexte : {', '.join(ctx)})")
    if not issues: out.append("- aucun problème détecté"); out.append(""); continue
    for lvl, msg in issues: out.append(f"- **{lvl}** {msg}")
    out.append("")
out += count_out
out.append("")
out.append(f"Total : {len(rows)} pages, {problems} ERREUR.")
text = "\n".join(out) + "\n"
dest = None
if "--md" in sys.argv: dest = sys.argv[sys.argv.index("--md") + 1]
if dest: open(dest, "w", encoding="utf-8").write(text)
else: print(text)
print(f"{len(rows)} pages, {problems} ERREUR", file=sys.stderr)
sys.exit(1 if problems else 0)
