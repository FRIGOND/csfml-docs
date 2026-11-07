#!/usr/bin/env bash
# Usage: tools/build-csfml.sh <csfml-tag> <sfml-tag> <prefix> [extra cmake args for CSFML...]
# Compile SFML puis CSFML (bibliothèques partagées) dans <prefix>, sans sudo.
# Sources et logs : <prefix>/src. Exemple : tools/build-csfml.sh 3.0.0 3.0.2 "$HOME/csfml-3.0"
set -euo pipefail
CSFML_TAG=$1; SFML_TAG=$2; PREFIX=$3; shift 3
SRC="$PREFIX/src"; mkdir -p "$SRC"; cd "$SRC"
J=$(nproc)
common=(-DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX="$PREFIX"
        -DCMAKE_INSTALL_LIBDIR=lib '-DCMAKE_INSTALL_RPATH=$ORIGIN')
# CMake 4 refuse les projets dont cmake_minimum_required < 3.5 (cas de SFML 2.6.x) :
if [[ $SFML_TAG == 2.* ]]; then common+=(-DCMAKE_POLICY_VERSION_MINIMUM=3.5); fi
[[ -d SFML ]]  || git clone -q --depth 1 --branch "$SFML_TAG"  https://github.com/SFML/SFML.git
[[ -d CSFML ]] || git clone -q --depth 1 --branch "$CSFML_TAG" https://github.com/SFML/CSFML.git
cmake -S SFML -B SFML/build "${common[@]}" > sfml-configure.log 2>&1
cmake --build SFML/build -j"$J" > sfml-build.log 2>&1
cmake --install SFML/build > sfml-install.log 2>&1
cmake -S CSFML -B CSFML/build "${common[@]}" -DSFML_DIR="$PREFIX/lib/cmake/SFML" "$@" > csfml-configure.log 2>&1
cmake --build CSFML/build -j"$J" > csfml-build.log 2>&1
cmake --install CSFML/build > csfml-install.log 2>&1
echo "OK: CSFML $CSFML_TAG + SFML $SFML_TAG dans $PREFIX"
