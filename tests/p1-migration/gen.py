"""Extrait les 16 blocs de research/04-migration-2-vers-3.md §4, génère un
programme par bloc (gen/s4.X_{avant,apres}.c) et le compile (tests 25 et 26).
Usage : python3 -I gen.py   (depuis n'importe où, env.sh déjà sourcé)"""
import os, re, subprocess, json
HERE = os.path.dirname(os.path.abspath(__file__))
MD = os.path.join(HERE, "..", "..", "research", "04-migration-2-vers-3.md")
OUT = os.path.join(HERE, "gen"); os.makedirs(OUT, exist_ok=True)
P2, P3 = os.environ["P2"], os.environ["P3"]
txt = open(MD, encoding="utf-8").read()
sec4 = txt.split("## 4. Mini-exemples")[1].split("\n## 5.")[0]
blocks = []  # (section, kind, code)
for m in re.finditer(r"### (4\.\d)[^\n]*\n(.*?)(?=\n### 4\.|\Z)", sec4, re.S):
    cs = re.findall(r"```c\n(.*?)```", m.group(2), re.S)
    assert len(cs) == 2, m.group(1)
    blocks += [(m.group(1), "avant", cs[0]), (m.group(1), "apres", cs[1])]
assert len(blocks) == 16

# Contexte factice commun (ajouté par le wrapper, jamais dans l'extrait).
PRE = {
 "avant": "extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;\n"
          "extern int paused; extern const sfTexture *tileset;\n"
          "void shoot(int x, int y); void resize(unsigned w, unsigned h);\n"
          "void zoom(int d); void start(void); void hit(void);\n",
 "apres": "extern sfRenderWindow *win; extern sfEvent event; extern sfSprite *spr;\n"
          "extern bool paused; extern const sfTexture *tileset;\n"
          "void shoot(int x, int y); void resize(unsigned w, unsigned h);\n"
          "void zoom(float d); void start(void); void hit(void);\n",
}
def build(sec, kind, code, hdr):
    lines = code.split("\n")
    inc = [l for l in lines if l.startswith("#include")]
    body = [l for l in lines if not l.startswith("#include")]
    if hdr == "SFML": inc = [l.replace("<CSFML/", "<SFML/") for l in inc]
    if hdr == "CSFML": inc = [l.replace("<SFML/", "<CSFML/") for l in inc]
    if not inc:  # sections sans include : on l'ajoute dans le wrapper
        inc = ["#include <%s/Graphics.h>" % hdr, "#include <%s/Audio.h>" % hdr]
    # bool/stdbool pour les APRÈS (le wrapper ne le fournit pas si le header ne le fait pas)
    return ("/* wrapper généré — l'extrait est copié tel quel entre les marqueurs */\n"
            + "\n".join(inc) + "\n#include <stdbool.h>\n" + PRE[kind]
            + "int block(void)\n{\n/* ---- DEBUT EXTRAIT ---- */\n" + "\n".join(body)
            + "\n/* ---- FIN EXTRAIT ---- */\nreturn 0;\n}\n")

def compile_(path, P):
    r = subprocess.run(["gcc","-std=c11","-Wall","-Wextra","-I%s/include"%P,"-c",path,"-o","/dev/null"],
                       capture_output=True, text=True, env={**os.environ,"LC_ALL":"C.UTF-8"})
    return r.returncode, r.stderr
res = []
for sec, kind, code in blocks:
    hdr = "SFML" if kind == "avant" else "CSFML"
    p = os.path.join(OUT, "s%s_%s.c" % (sec, kind)); open(p,"w").write(build(sec,kind,code,hdr))
    # test 25 : version native ; test 26 : l'autre version / avant sous 3.0 avec include changé
    nat = P2 if kind == "avant" else P3
    rc, err = compile_(p, nat)
    row = {"sec": sec, "kind": kind, "nat_rc": rc, "nat_err": err}
    if kind == "avant":
        p2 = os.path.join(OUT, "s%s_avant_inc3.c" % sec)
        open(p2,"w").write(build(sec,kind,code,"CSFML"))
        rc2, err2 = compile_(p2, P3)
        row.update(x_rc=rc2, x_err=err2)
    else:
        p2 = os.path.join(OUT, "s%s_apres_inc2.c" % sec)
        open(p2,"w").write(build(sec,kind,code,"SFML"))
        rc2, err2 = compile_(p2, P2)
        row.update(x_rc=rc2, x_err=err2)
    res.append(row)
json.dump(res, open(os.path.join(HERE,"gen","results.json"),"w"), indent=1)
with open(os.path.join(HERE,"..","logs","migration.log"),"w") as f:
    for r in res:
        f.write("=== %s %s  natif rc=%d\n%s\n--- croisé rc=%d\n%s\n" % (r["sec"],r["kind"],r["nat_rc"],r["nat_err"],r["x_rc"],r["x_err"]))
for r in res: print(r["sec"], r["kind"], "natif", "OK" if r["nat_rc"]==0 else "KO", "| croisé", "OK" if r["x_rc"]==0 else "KO")
