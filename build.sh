#!/bin/bash
echo "🔨 Building Xon Library and CLI..."

# Generate parser
./tools/lemon src/xon.lemon
mv xon.c xon.h xon.out build/ 2>/dev/null || true

# Detect OS for shared library extension
if [[ "$OSTYPE" == "darwin"* ]]; then
    LIB_EXT="dylib"
    LIB_FLAGS="-dynamiclib"
else
    LIB_EXT="so"
    LIB_FLAGS="-shared -fPIC"
fi

# Build shared library
echo "📚 Building libxon.${LIB_EXT}..."
gcc $LIB_FLAGS -Wall -Wextra -std=c99 -Ibuild -Iinclude \
    -o libxon.${LIB_EXT} \
    src/xon_api.c src/lexer.c

# Build CLI tool
echo "🔧 Building xon CLI..."
gcc -Wall -Wextra -std=c99 -Ibuild \
    -o xon \
    src/main.c src/lexer.c

# Build example program
echo "📝 Building example program..."
gcc -Wall -Wextra -std=c99 -Iinclude \
    -o example_lib \
    examples/use_library.c \
    -L. -lxon

echo ""
echo "✅ Build complete!"
echo "   - libxon.${LIB_EXT} (shared library)"
echo "   - xon (CLI tool)"
echo "   - example_lib (library usage example)"
echo ""
echo "📖 To run the example:"
if [[ "$OSTYPE" == "darwin"* ]]; then
    echo "   DYLD_LIBRARY_PATH=. ./example_lib"
else
    echo "   LD_LIBRARY_PATH=. ./example_lib"
fi
