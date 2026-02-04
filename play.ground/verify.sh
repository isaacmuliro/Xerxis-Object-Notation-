#!/bin/bash


# This script verifies the Xon project setup before building for WebAssembly.
# It checks for the presence of the generated parser, lexer source file,
# the Lemon tool, and Emscripten installation.
# Run this script from the play.ground directory.



echo "🔍 Verifying Xon Project..."

# Check parser
if [ ! -f "../build/xon.c" ]; then
    echo "⚠️  Parser not generated. Running lemon..."
    cd ..
    ./tools/lemon src/xon.lemon
    mv xon.c xon.h xon.out build/ 2>/dev/null || true
    cd play.ground
else
    echo "✅ Parser found"
fi

# Check lexer
if [ ! -f "../src/lexer.c" ]; then
    echo "❌ Lexer missing: ../src/lexer.c"
    exit 1
fi
echo "✅ Lexer found"

# Check lemon tool
if [ ! -x "../tools/lemon" ]; then
    echo "❌ Lemon tool not executable: ../tools/lemon"
    exit 1
fi
echo "✅ Lemon tool found"

# Check emscripten
if ! command -v emcc &> /dev/null; then
    echo "⚠️  Emscripten not installed"
    echo "   Install: https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi
echo "✅ Emscripten found: $(emcc --version | head -n1)"

echo ""
echo "✅ All checks passed! Ready to build WASM."
echo "   Run: ./build_wasm.sh"