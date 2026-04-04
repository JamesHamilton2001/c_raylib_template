#!/bin/bash
set -e

tags="5.5"

# parse -t, --tag arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -t|--tag)
            if [[ -z "$2" || "$2" == -* ]]; then
                echo "Error: $1 requires a value"
                exit 1
            fi
            tags="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done

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
tmpdir="$prjdir/tmp"
libdir="$prjdir/lib"
raydir="$libdir/$rl"
srcdir="$raydir/src"
dbgdir="$raydir/debug"
reldir="$raydir/release"

echo "\"${prjdir}\" downloading local ${rlname}..."

# setup local raylib space and work dir for download and compile
rm -rf "$srcdir"
rm -rf "$tmpdir"
mkdir -p "$tmpdir" "$libdir" "$raydir" "$srcdir" "$dbgdir" "$reldir"

# download raylib source, unpack, cd to src and copy to local lib src
cd "$tmpdir" || { echo "unable to cd lib work directory"; rm -rf "$tmpdir"; exit 1; }
curl -fL -o "$rltar" "$rlurl"
tar -xzf "$rltar"
cd "$tmpdir/$rlname/src" || { echo "unable to cd to local raylib src"; exit 1; }
cp -a ./. "$srcdir"

# # build release lib
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=RELEASE
cp -a libraylib.a "$reldir/"
make clean

# build debug lib
make PLATFORM=PLATFORM_DESKTOP RAYLIB_BUILD_MODE=DEBUG
cp -a libraylib.a "$dbgdir/"
make clean

# clean work dir
cd "$libdir" || { echo "unable to cd to project lib dir"; exit 1; }
rm -rf "$tmpdir"

echo "\"${prjdir}\" local raylib$tags installed!"