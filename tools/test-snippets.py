#!/usr/bin/env python3
"""Compile (gcc -fsyntax-only -std=c11 -Wall -Wextra) les blocs ```c des pages écrites à la main qui ne sont PAS des extraits d'examples/.
Un bloc d'onglet « CSFML 2.6 » est compilé contre ~/csfml-2.6, un bloc « CSFML 3.0 » contre ~/csfml-3.0 ; hors onglet, la version vient de la page
(2-6/ ou 3-0/) ou des #include (<SFML/ = 2.6, <CSFML/ = 3.0) ; à défaut les deux versions sont essayées (il faut au moins une réussite).
Les extraits sans fonction sont placés dans un corps de fonction, avec des déclarations factices communes (win, event, spr, tex...).
Les blocs qui montrent volontairement une ERREUR (« avant » sous la mauvaise version) sont listés dans EXPECT_FAIL.
Pré-requis : P2/P3 (voir tools/env.sh). Usage : python3 tools/test-snippets.py [--md sortie.md]   (code 1 si un bloc échoue sans être attendu)"""
import os, re, subprocess, sys, tempfile
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
P = {"3.0": os.environ.get("P3", os.path.expanduser("~/csfml-3.0")), "2.6": os.environ.get("P2", os.path.expanduser("~/csfml-2.6"))}
DOCS = os.path.join(ROOT, "site/src/content/docs")
sys.argv_ = sys.argv
exec(open(os.path.join(ROOT, "tools/audit-blocks.py")).read().split("out = [")[0].replace("sys.argv", "[]"))   # réutilise corpus() / classification
FORCE_VERS = {('guides/install/check-your-setup.mdx', 12): ['3.0']}   # 2.6 : piège __has_include avec un CSFML 3.0 système (voir la page) ; le diagnostic complet est compilé et exécuté en 2.6 par ailleurs
EXPECT_FAIL = {}   # (page, ligne) -> raison ; rempli après examen
DUMMY = {
 "3.0": "extern sfRenderWindow *win, *window; extern sfEvent event; extern sfSprite *spr, *sprite; extern sfTexture *tex, *texture; extern const sfTexture *tileset;\n"
        "extern sfFont *font; extern sfText *text; extern sfClock *clock, *clock_; extern bool paused; extern sfVertexArray *va;\n"
        "void shoot(int x, int y); void resize(unsigned w, unsigned h); void zoom(float d); void start(void); void hit(void); void handle(sfEvent e); void use(); void use2(); void use3(); extern sfVector2f pos; extern float dt; extern sfFloatRect inter; extern sfImage *image; extern sfColor color; extern unsigned int size, x, y; extern struct { sfText *title_text; sfText *score_text; sfRenderWindow *window; } *g;\n#define SPEED 200.f\n#define WIN_W 800\n#define WIN_H 600\n\n",
 "2.6": "extern sfRenderWindow *win, *window; extern sfEvent event; extern sfSprite *spr, *sprite; extern sfTexture *tex, *texture; extern const sfTexture *tileset;\n"
        "extern sfFont *font; extern sfText *text; extern sfClock *clock, *clock_; extern sfBool paused; extern sfVertexArray *va;\n"
        "void shoot(int x, int y); void resize(unsigned w, unsigned h); void zoom(int d); void start(void); void hit(void); void handle(sfEvent e); void use(); void use2(); void use3(); extern sfVector2f pos; extern float dt; extern sfFloatRect inter; extern sfImage *image; extern sfColor color; extern unsigned int size, x, y; extern struct { sfText *title_text; sfText *score_text; sfRenderWindow *window; } *g;\n#define SPEED 200.f\n#define WIN_W 800\n#define WIN_H 600\n\n",
}
STD = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n#include <stdint.h>\n#include <stdbool.h>\n#include <math.h>\n"
def includes(v):
    d = "CSFML" if v == "3.0" else "SFML"
    return "".join(f"#include <{d}/{m}.h>\n" for m in ("Graphics", "Audio", "Network"))
def build(code, v):
    hoist = not re.search(r"^#\s*(if|ifdef|ifndef)\b", code, re.M)      # include dans un #if : on ne le déplace pas
    inc = [l for l in code.splitlines() if l.startswith("#include")] if hoist else []
    rest = "\n".join(l for l in code.splitlines() if not (hoist and l.startswith("#include")))
    file_level = bool(re.search(r"^(#ifndef|#if |#define|typedef)|^(?!if\b|while\b|for\b|switch\b|else\b|return\b)[A-Za-z_][\w\s\*]*\([^;{]*\)\s*\{?\s*$", rest.strip(), re.M))
    head = "\n".join(inc) + "\n" if inc else ("" if not hoist else includes(v))
    if file_level: return head + STD + rest
    return head + STD + DUMMY[v] + "static int snippet_(void)\n{\n" + rest + "\nreturn 0;\n}\n"
def gcc(src, v):
    with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as f: f.write(src); name = f.name
    try:
        r = subprocess.run(["gcc", "-std=c11", "-Wall", "-Wextra", "-Wno-unused-variable", "-Wno-unused-but-set-variable", "-fsyntax-only", f"-I{P[v]}/include", "-I" + os.path.join(tempfile.gettempdir(), "compat"), name], capture_output=True, text=True, env={**os.environ, "LC_ALL": "C.UTF-8"})
        msgs = [l for l in r.stderr.splitlines() if ("error" in l or "warning" in l) and not l.startswith(P[v])]   # avertissements des headers CSFML exclus (voir RESULTATS : Event.h déprécié)
        return (1 if msgs else 0), "\n".join(msgs)[:300].replace(name, "snippet.c")
    finally: os.unlink(name)

import json
api_funcs = {v: {f['name']: f for f in json.load(open(os.path.join(ROOT, 'api', fn)))['functions']} for v, fn in (('3.0', 'csfml-3.0.0.json'), ('2.6', 'csfml-2.6.1.json'))}
res, fails, nb = [], 0, 0
for p in pages:
    rel = p.split("docs/", 1)[1]
    text = open(os.path.join(ROOT, p), encoding="utf-8").read()
    text = re.sub(r"^---\n.*?\n---\n", "", text, count=1, flags=re.S)
    pv = ["3.0"] if rel.startswith(("3-0/", "en/3-0/")) else ["2.6"] if rel.startswith(("2-6/", "en/2-6/")) else ["3.0", "2.6"]
    tabs = [(m.start(), [v for v in ("3.0", "2.6") if v in m.group(1)]) for m in re.finditer(r'<TabItem\s+label="([^"]*)"', text)]
    for m in re.finditer(r"^```(c|h)\b[^\n]*\n(.*?)^```", text, re.S | re.M):
        code = m.group(2); line = text.count("\n", 0, m.start()) + 2
        lines = [ws(l) for l in code.splitlines() if ws(l)]
        cl = [l for l in lines if not l.startswith(("//", "/*", "*", "#include"))] or lines
        if all(any(l in CORP[v] for v in ("3.0", "2.6")) for l in cl) and len(cl) > 1: continue   # extrait d'un exemple testé (n'importe quelle version : onglets jumeaux)          # déjà extrait d'un exemple testé
        vs = pv
        for pos, tv in tabs:
            if pos < m.start() and tv and "</TabItem>" not in text[pos:m.start()]: vs = tv
        vs = FORCE_VERS.get((rel, line), vs)
        if vs is pv and "<SFML/" in code: vs = ["2.6"]
        elif vs is pv and "<CSFML/" in code: vs = ["3.0"]
        if "compat.h" in code and rel.endswith("support-both.mdx"):
            compat = re.search(r"^```c[^\n]*\n(#ifndef COMPAT_H.*?)^```", text, re.S | re.M).group(1)
            os.makedirs(os.path.join(tempfile.gettempdir(), "compat"), exist_ok=True)
            open(os.path.join(tempfile.gettempdir(), "compat", "compat.h"), "w").write(compat)
            code = code
        sigs = [l.strip() for l in code.splitlines() if l.strip()]
        if sigs and all(re.match(r"^(?:const\s+)?\w+\s*\**\s*\*?\s*sf\w+_\w+\(.*\)\s*;?$", l) and not l.startswith(("return", "else")) and "=" not in l.split("(")[0] for l in sigs):
            nm = lambda t: re.sub(r"\s*\*\s*", "*", ws(t.rstrip(";"))).replace("( ", "(")
            known = {nm(f["signature"]) for v in ("3.0", "2.6") for f in api_funcs[v].values()}
            okk = all(nm(l) in known for l in sigs)
            nb += 1; fails += 0 if okk else 1
            res.append((rel, line, vs, okk, None, {} if okk else {"sig": "signature absente des JSON"})); continue
        out = {v: gcc(build(code, v), v) for v in vs}
        ok = any(rc == 0 for rc, _ in out.values()) if len(vs) > 1 else out[vs[0]][0] == 0
        exp = EXPECT_FAIL.get((rel, line))
        nb += 1
        if not ok and not exp: fails += 1
        res.append((rel, line, vs, ok, exp, {v: e for v, (rc, e) in out.items() if rc}))
o = ["# Compilation des extraits non issus d'examples/", "",
     "Généré par `python3 tools/test-snippets.py` (gcc -fsyntax-only -std=c11 -Wall -Wextra). OK = compile sans erreur ni avertissement dans la ou les versions visées.", "",
     f"{nb} blocs, {fails} échec(s) non attendu(s).", "", "| Page:ligne | Versions | Résultat | Détail |", "|---|---|---|---|"]
for rel, line, vs, ok, exp, errs in res:
    o.append(f"| {rel}:{line} | {','.join(vs)} | {'OK' if ok else ('ÉCHEC ATTENDU : ' + exp if exp else 'ÉCHEC')} | {' ; '.join(f'{v}: {e}' for v, e in errs.items()).replace('|', '/')[:240]} |")
txt = "\n".join(o) + "\n"
if "--md" in sys.argv_: open(sys.argv_[sys.argv_.index("--md") + 1], "w", encoding="utf-8").write(txt)
else: print(txt)
print(f"{nb} blocs, {fails} échec(s)", file=sys.stderr)
sys.exit(1 if fails else 0)
