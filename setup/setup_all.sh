#!/bin/bash
set -e

#!/bin/bash
set -e

tags=""
tag_provided=false

# parse -t, --tag arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--tag)
            if [[ -z "$2" || "$2" == -* ]]; then
                echo "Error: $1 requires a value"
                exit 1
            fi
            tags="$2"
            tag_provided=true
            shift 2
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
./install_local_raylib.sh -t "$tags"