#!/bin/bash
set -e

tags="${1:-}"

if [[ -z "$tags" ]]; then
    echo "Usage: $0 <raylib tag>"
    exit 1
fi

# raylib source data
rl="raylib"
rlname="$rl-$tags"
rltar="$rlname.tar.gz"
rlurl="https://github.com/raysan5/raylib/archive/refs/tags/$tags.tar.gz"

# get to script location
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/.." || { echo "unable to cd project root"; exit 1; }

# project directories
prjdir="$(pwd)"
libdir="$prjdir/lib"
libsrcdir="$libdir/src"
libreldir="$libdir/rel"
libdbgdir="$libdir/dbg"

mkdir -p "$libdir" "$libsrcdir" "$libreldir" "$libdbgdir"

echo "downloading local ${rlname}..."

cd "$libsrcdir" || { echo "unable to cd lib work directory"; rm -rf "$libdir"; exit 1; }

curl -fL -o "$rltar" "$rlurl"
tar -xzf "$rltar"
rm -f "$rltar"

cd "$rlname/src"

copy_library()
{
    local dest="$1"

    local rlib
    rlib="$(find .. -maxdepth 2 -name libraylib.a | head -n1)"

    if [[ -z "$rlib" ]]; then
        echo "Failed to locate libraylib.a"
        exit 1
    fi

    cp -a "$rlib" "$dest/"
}

# Build release
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=RELEASE
copy_library "$libreldir"
make clean

# Build debug
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=DEBUG
copy_library "$libdbgdir"
make clean

echo "$tags" > "$libdir/raylib.version"

echo "\"${prjdir}\" local raylib ${tags} installed!"