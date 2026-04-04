#!/bin/bash
set -e

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/" || { echo "unable to cd project root"; exit 1; }

./clean_tmp.sh
./uninstall_local_raylib.sh
./install_local_raylib.sh -t 5.5