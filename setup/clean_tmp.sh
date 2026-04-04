#!/bin/bash
set -e

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/.." || { echo "unable to cd project root"; exit 1; }

echo "running uninstall_local_raylib for "

prjdir="$(pwd)"

tmpdir="$prjdir/tmp"

rm -rf "$tmpdir"

echo "\"${prjdir}\" no more tmp!"