#!/bin/bash


# This script builds the Xon project to WebAssembly using Emscripten.
# It first generates the parser using Lemon, then compiles the source files
# to produce the WASM module and JavaScript glue code.
# Make sure Emscripten is installed and configured in your environment before running this script.
# Run `setup_emscripten.sh` if Emscripten is not installed.
# Run this script from the play.ground directory.




echo "🌐 Building Xon for WebAssembly..."

# Check if Emscripten is installed
if ! command -v emcc &> /dev/null; then
    echo "❌ Emscripten not found. Install from: https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

# Generate parser first (go up one directory)
echo "📝 Generating parser..."
cd ..
./tools/lemon src/xon.lemon
mv xon.c xon.h xon.out build/ 2>/dev/null || true
cd play.ground

# Compile to WebAssembly
echo "🔨 Compiling to WASM..."
emcc ../src/lexer.c ../build/xon.c \
    -o xon.js \
    -s WASM=1 \
    -s EXPORTED_FUNCTIONS='["_malloc","_free"]' \
    -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap","FS","UTF8ToString","stringToUTF8"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s MODULARIZE=1 \
    -s EXPORT_NAME="XonModule" \
    -s INVOKE_RUN=0 \
    -I../build -I../src \
    -O3 \
    --no-entry

echo ""
echo "✅ WASM build complete!"
echo "   Output: play.ground/xon.js and play.ground/xon.wasm"
echo ""
echo "📖 To test the playground:"
echo "   cd play.ground"
echo "   python3 -m http.server 8000"
echo "   open http://localhost:8000"