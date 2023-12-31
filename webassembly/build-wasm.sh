SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
echo SCRIPT_DIR=$SCRIPT_DIR

BUILD_DIR="$SCRIPT_DIR"/../dist/webassembly
echo BUILD_DIR=$BUILD_DIR
rm -rf "$BUILD_DIR"

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

emcmake cmake "$SCRIPT_DIR"
emmake make clean
emmake make
