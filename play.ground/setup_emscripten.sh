#!/bin/bash

# This automated script installs Emscripten on macOS or Linux systems.
# It first attempts to use Homebrew if available, otherwise it falls back to
# installing via the official emsdk method.
# It also adds the necessary environment setup to the user's shell configuration.
# Make sure to restart your terminal or source the shell config after installation.




echo "🚀 Setting up Emscripten..."

# Check if Homebrew is available
if command -v brew &> /dev/null; then
    echo "📦 Installing via Homebrew..."
    brew install emscripten
    
    if command -v emcc &> /dev/null; then
        echo "✅ Emscripten installed successfully!"
        emcc --version
        exit 0
    fi
fi

# Fallback to manual installation
echo "📥 Installing via emsdk..."
cd ~
if [ ! -d "emsdk" ]; then
    git clone https://github.com/emscripten-core/emsdk.git
fi

cd emsdk
./emsdk install latest
./emsdk activate latest

# Add to shell config
SHELL_RC="$HOME/.zshrc"
if [ ! -f "$SHELL_RC" ]; then
    SHELL_RC="$HOME/.bash_profile"
fi

if ! grep -q "emsdk_env.sh" "$SHELL_RC"; then
    echo "source ~/emsdk/emsdk_env.sh" >> "$SHELL_RC"
    echo "✅ Added Emscripten to $SHELL_RC"
fi

# Activate for current session
source ~/emsdk/emsdk_env.sh

echo ""
echo "✅ Emscripten installed!"
echo "   Version: $(emcc --version | head -n1)"
echo ""
echo "⚠️  Please restart your terminal or run:"
echo "   source ~/emsdk/emsdk_env.sh"