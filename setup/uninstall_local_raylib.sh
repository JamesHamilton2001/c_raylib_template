#!/bin/bash
set -e

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/.." || { echo "unable to cd project root"; exit 1; }

echo "running uninstall_local_raylib for "

prjdir="$(pwd)"

dstdir="$prjdir/lib/raylib"

rm -rf "$dstdir"

echo "\"${prjdir}\" local raylib uninstalled!"