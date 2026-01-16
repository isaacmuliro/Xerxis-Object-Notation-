cat > build.sh << 'EOF'
#!/bin/bash
echo "🔨 Building Xon..."
./tools/lemon src/xon.lemon
mv xon.c xon.h xon.out build/ 2>/dev/null || true
gcc -Wall -Wextra -std=c99 -Ibuild -o xon src/main.c src/lexer.c
echo "✅ Build complete!"
EOF

chmod +x build.sh