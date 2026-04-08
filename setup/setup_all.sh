#!/bin/bash
set -e

raylib_tag=""
raygui_tag=""
tag_provided=false

# parse -t, --tag arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--tag|--tags)
            if [[ -z "$2" || -z "$3" || "$2" == -* || "$3" == -* ]]; then
                echo "Error: $1 requires two values: <raylib_tag> <raygui_tag>"
                exit 1
            fi
            raylib_tag="$2"
            raygui_tag="$3"
            tag_provided=true
            shift 3
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

# enforce required argument
if [[ "$tag_provided" = false ]]; then
    echo "Error: --tag or -t is required"
    exit 1
fi

script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/" || { echo "unable to cd project root"; exit 1; }

./uninstall_local_raylib.sh
./uninstall_local_raygui.sh

./install_local_raylib.sh -t "$raylib_tag"
./install_local_raygui.sh -t "$raygui_tag"