#!/usr/bin/env python3
"""Génère api/csfml-<version>.json depuis les headers installés (libclang).

Usage : PYTHONPATH=~/local/py/libclang-18.1.1.data/platlib python3 tools/gen_api.py \
          <préfixe d'installation> <CSFML|SFML> <version> [sortie.json]
Le préfixe contient include/<CSFML|SFML>/ (3.0.0 : CSFML ; 2.6.x : SFML).
Contenu : fonctions (signature exacte, paramètres, commentaire Doxygen), types (struct/enum/union/typedef),
variables globales, macros de version. Rien n'est écrit à la main : tout vient des headers.
"""
import glob, json, os, re, subprocess, sys, tempfile
from clang import cindex as ci

prefix, ns, version = sys.argv[1], sys.argv[2], sys.argv[3]
out = sys.argv[4] if len(sys.argv) > 4 else f"api/csfml-{version}.json"
inc = os.path.join(prefix, "include")
root = os.path.join(inc, ns)
headers = sorted(glob.glob(root + "/*/*.h") + [p for p in glob.glob(root + "/*.h")])
sub = [h for h in headers if os.path.dirname(h) != root]          # headers des modules (pas les parapluies)
umbrellas = [h for h in headers if os.path.dirname(h) == root]

gcc_inc = subprocess.check_output(["gcc", "-print-file-name=include"], text=True).strip()
args = ["-x", "c", "-std=c11", f"-I{inc}", f"-isystem{gcc_inc}", "-Wno-everything"]
with tempfile.NamedTemporaryFile("w", suffix=".c", delete=False) as t:
    for h in sub:
        t.write(f'#include "{h}"\n')
    tu_path = t.name
index = ci.Index.create()
tu = index.parse(tu_path, args=args, options=ci.TranslationUnit.PARSE_DETAILED_PROCESSING_RECORD)
errs = [d for d in tu.diagnostics if d.severity >= ci.Diagnostic.Error]
for d in errs[:10]:
    print("ERREUR clang:", d, file=sys.stderr)
os.unlink(tu_path)

def rel(path):
    return os.path.relpath(path, inc) if path and path.startswith(inc) else None

def clean_comment(raw):
    if not raw:
        return ""
    lines = []
    for l in raw.splitlines():
        l = l.strip()
        if re.fullmatch(r"/{3,}", l):
            continue
        l = re.sub(r"^/{2,3} ?", "", l)
        lines.append(l)
    return "\n".join(lines).strip()

def parse_doc(text):
    brief, params, ret, rest = "", {}, "", []
    cur = None
    for l in text.splitlines():
        m = re.match(r"\\(brief|param|return|returns)\b\s*(.*)", l)
        if m:
            k, v = m.groups()
            if k == "brief":
                brief, cur = v, ("brief", None)
            elif k == "param":
                pm = re.match(r"(\w+)\s*(.*)", v)
                if pm:
                    params[pm.group(1)] = pm.group(2); cur = ("param", pm.group(1))
            else:
                ret, cur = v, ("ret", None)
            continue
        if l.startswith("\\"):
            cur = None; rest.append(l); continue
        if cur and l:
            if cur[0] == "brief": brief += " " + l
            elif cur[0] == "param": params[cur[1]] += " " + l
            else: ret += " " + l
        elif not l:
            cur = None if not rest else cur
    return brief.strip(), {k: v.strip() for k, v in params.items()}, ret.strip()

def is_deprecated(c):
    return any(ch.kind == ci.CursorKind.UNEXPOSED_ATTR or "deprecated" in (ch.spelling or "").lower()
               for ch in c.get_children()) or "DEPRECATED" in "".join(t.spelling for t in c.get_tokens())

functions, types, variables, macros = [], [], [], []
for c in tu.cursor.get_children():
    path = c.location.file.name if c.location.file else None
    r = rel(path)
    if not r:
        continue
    module = r.split("/")[1] if r.count("/") >= 2 else ""
    doc = clean_comment(c.raw_comment)
    brief, pdoc, rdoc = parse_doc(doc)
    k = c.kind
    if k == ci.CursorKind.FUNCTION_DECL:
        params = [{"name": a.spelling, "type": a.type.spelling, "doc": pdoc.get(a.spelling, "")} for a in c.get_arguments()]
        sig = f'{c.result_type.spelling} {c.spelling}(' + (", ".join(f'{p["type"]} {p["name"]}'.strip() for p in params) or "void") + ")"
        functions.append({"name": c.spelling, "module": module, "header": r, "line": c.location.line,
                          "returns": c.result_type.spelling, "return_doc": rdoc, "params": params,
                          "signature": sig, "brief": brief, "doc": doc, "deprecated": is_deprecated(c)})
    elif k in (ci.CursorKind.STRUCT_DECL, ci.CursorKind.UNION_DECL, ci.CursorKind.ENUM_DECL, ci.CursorKind.TYPEDEF_DECL):
        if k == ci.CursorKind.TYPEDEF_DECL:
            under = c.underlying_typedef_type.spelling
            ent = {"name": c.spelling, "kind": "typedef", "underlying": under}
            decl = c.underlying_typedef_type.get_declaration()
            if decl.kind in (ci.CursorKind.STRUCT_DECL, ci.CursorKind.UNION_DECL, ci.CursorKind.ENUM_DECL) and decl.spelling == "" or decl.is_anonymous():
                ent["kind"] = {ci.CursorKind.STRUCT_DECL: "struct", ci.CursorKind.UNION_DECL: "union", ci.CursorKind.ENUM_DECL: "enum"}.get(decl.kind, "typedef")
                if decl.kind == ci.CursorKind.ENUM_DECL:
                    ent["values"] = [{"name": v.spelling, "value": v.enum_value, "doc": parse_doc(clean_comment(v.raw_comment))[0]} for v in decl.get_children() if v.kind == ci.CursorKind.ENUM_CONSTANT_DECL]
                else:
                    ent["fields"] = [{"name": f.spelling, "type": f.type.spelling, "doc": clean_comment(f.raw_comment)} for f in decl.get_children() if f.kind == ci.CursorKind.FIELD_DECL]
        else:
            if not c.spelling:
                continue
            ent = {"name": c.spelling, "kind": {ci.CursorKind.STRUCT_DECL: "struct", ci.CursorKind.UNION_DECL: "union", ci.CursorKind.ENUM_DECL: "enum"}[k],
                   "opaque": not c.is_definition()}
            if k == ci.CursorKind.ENUM_DECL:
                ent["values"] = [{"name": v.spelling, "value": v.enum_value, "doc": parse_doc(clean_comment(v.raw_comment))[0]} for v in c.get_children() if v.kind == ci.CursorKind.ENUM_CONSTANT_DECL]
            elif c.is_definition():
                ent["fields"] = [{"name": f.spelling, "type": f.type.spelling, "doc": clean_comment(f.raw_comment)} for f in c.get_children() if f.kind == ci.CursorKind.FIELD_DECL]
        ent.update({"module": module, "header": r, "line": c.location.line, "brief": brief})
        types.append(ent)
    elif k == ci.CursorKind.VAR_DECL:
        variables.append({"name": c.spelling, "type": c.type.spelling, "module": module, "header": r, "line": c.location.line, "brief": brief})
    elif k == ci.CursorKind.MACRO_DEFINITION and re.match(r"CSFML_VERSION_|SFML_VERSION_", c.spelling):
        macros.append({"name": c.spelling, "header": r})
# macros de version (liste des tokens)
for m in tu.cursor.get_children():
    if m.kind == ci.CursorKind.MACRO_DEFINITION and re.match(r"CSFML_VERSION_(MAJOR|MINOR|PATCH)$", m.spelling):
        macros.append({"name": m.spelling, "value": "".join(t.spelling for t in list(m.get_tokens())[1:]), "header": rel(m.location.file.name)})

# dédoublonnage (typedef + struct de même nom restent distincts par kind)
seen, ftab = set(), []
for f in functions:
    if f["name"] not in seen:
        seen.add(f["name"]); ftab.append(f)
data = {"csfml_version": version, "namespace": ns, "generated_by": "tools/gen_api.py (libclang " + ci.conf.lib.clang_getClangVersion.__name__ + ")",
        "headers": [rel(h) for h in headers], "counts": {"functions": len(ftab), "types": len(types), "variables": len(variables),
        "documented_functions": sum(1 for f in ftab if f["brief"]), "deprecated_functions": sum(1 for f in ftab if f["deprecated"])},
        "clang_errors": [str(e) for e in errs], "functions": sorted(ftab, key=lambda f: (f["module"], f["header"], f["name"])),
        "types": types, "variables": variables, "macros": [m for m in macros if "value" in m]}
os.makedirs(os.path.dirname(out) or ".", exist_ok=True)
json.dump(data, open(out, "w", encoding="utf-8"), indent=1, ensure_ascii=False)
print(out, json.dumps(data["counts"]), "erreurs clang:", len(errs))
