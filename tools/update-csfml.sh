#!/usr/bin/env bash
# Met à jour la doc pour une nouvelle version de CSFML.
# Usage : tools/update-csfml.sh <csfml-tag> <sfml-tag> [prefix]
# Exemple : tools/update-csfml.sh 3.1.0 3.1.0
# Étapes : 1) compile SFML+CSFML  2) régénère api/csfml-<v>.json  3) diff de l'API avec la version précédente
#          4) rejoue tous les exemples  5) liste ce qu'il faut traiter à la main (rien n'est publié automatiquement).
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"; cd "$ROOT"
CSFML=$1; SFML=$2; PREFIX=${3:-$HOME/csfml-$CSFML}
case $CSFML in 2.*) NS=SFML;; *) NS=CSFML;; esac   # 2.x : headers <SFML/...>, 3.x : <CSFML/...>
[ -d "$PREFIX/include" ] || tools/build-csfml.sh "$CSFML" "$SFML" "$PREFIX"
NEW=api/csfml-$CSFML.json
python3 tools/gen_api.py "$PREFIX" "$NS" "$CSFML" "$NEW"
PREV=$(ls api/csfml-*.json | grep -v "$NEW" | sort -V | tail -1)
echo "== Diff API : $PREV -> $NEW"
python3 - "$PREV" "$NEW" <<'PY'
import json, sys
a, b = (json.load(open(p)) for p in sys.argv[1:3])
fa = {f["name"]: f for f in a["functions"]}; fb = {f["name"]: f for f in b["functions"]}
print("supprimées :", len(set(fa) - set(fb)), "| ajoutées :", len(set(fb) - set(fa)))
chg = [n for n in fa if n in fb and fa[n].get("signature") != fb[n].get("signature")]
print("signature modifiée :", len(chg))
for n in sorted(set(fa) - set(fb))[:50]: print("  -", n)
for n in sorted(set(fb) - set(fa))[:50]: print("  +", n)
PY
echo "== Exemples (nécessite des meta/main.c pour la nouvelle version)"
MV=${CSFML%.*}; export "CSFML_PREFIX_${MV/./_}=$PREFIX"
tools/run-all-examples.sh --version "$MV" || echo "Des exemples échouent : à corriger à la main."
echo "== À faire : ajouter la version dans site/src/data/tested.json et astro.config.mjs, relire notes.json, CORRECTIONS.md."
