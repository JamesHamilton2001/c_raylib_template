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

# raygui source data
rg="raygui"
rgname="$rg-$tags"
rgtar="$rgname.tar.gz"
rgurl="https://codeload.github.com/raysan5/raygui/tar.gz/refs/tags/$tags"

# get to script location (project/dev)
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/.." || { echo "unable to cd project root"; exit 1; }

# project directories
prjdir="$(pwd)"
libdir="$prjdir/lib"
libsrcdir="$libdir/src"
libreldir="$libdir/rel"
libdbgdir="$libdir/dbg"

mkdir -p "$libdir" "$libsrcdir" "$libreldir" "$libdbgdir"

echo "\"${prjdir}\" downloading local ${rgname}..."

cd "$libsrcdir" || { echo "unable to cd lib work directory"; exit 1; }
curl -fL -o "$rgtar" "$rgurl"
tar -xzf "$rgtar"
rm -f "$rgtar"

# create impl file
cat > "$libsrcdir/raygui_impl.c" <<EOF
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
EOF

# release
gcc -c -O2 -DNDEBUG -fPIC "$libsrcdir/raygui_impl.c" \
    -I"$libsrcdir/$rgname/src" \
    -o "$libsrcdir/raygui.o"
ar rcs "$libreldir/libraygui.a" "$libsrcdir/raygui.o"
rm -f "$libsrcdir/raygui.o"

# debug
gcc -c -g -O0 -DDEBUG -fPIC "$libsrcdir/raygui_impl.c" \
    -I"$libsrcdir/$rgname/src" \
    -o "$libsrcdir/raygui.o"

ar rcs "$libdbgdir/libraygui.a" "$libsrcdir/raygui.o"
rm -f "$libsrcdir/raygui.o" "$libsrcdir/raygui_impl.c"

echo "$tags" > "$libdir/raygui.version"

echo "\"${prjdir}\" local ${rgname} installed!"