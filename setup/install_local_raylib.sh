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

# get to script location (project/dev)
script_dir="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
cd "$script_dir/.." || { echo "unable to cd project root"; exit 1; }

# project directories
prjdir="$(pwd)"
libdir="$prjdir/lib"
libsrcdir="$libdir/src"
libreldir="$libdir/rel"
libdbgdir="$libdir/dbg"

# setup local raylib space and work dir for download and compile
mkdir -p "$libdir" "$libsrcdir" "$libreldir" "$libdbgdir"

echo "downloading local ${rlname}..."

# download raylib source, unpack, cd to src and copy to local lib src
cd "$libsrcdir" || { echo "unable to cd lib work directory"; rm -rf "$libdir"; exit 1; }
curl -fL -o "$rltar" "$rlurl"
tar -xzf "$rltar"
rm "$libsrcdir/$rltar"

cd "$rlname/src/"

# build release lib
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=RELEASE
cp -a libraylib.a "$libreldir/"
make clean

# build debug lib
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=DEBUG
cp -a libraylib.a "$libdbgdir/"
make clean

echo "$tags" > "$libdir/raylib.version"

echo "\"${prjdir}\" local raylib$tags installed!"