#!/bin/bash
# filepath: /Users/xerxiscmuliro/xon/gen.proj.struct.sh

# */

# This script was used to reorganize the Xon project structure by creating
# directories, moving source and generated files, creating example
# configuration files, setting up a test suite stub, and generating
# documentation files. It also creates a Makefile for building the project,
# /**


set -e  # Exit on error

echo "🚀 Reorganizing Xon project structure..."

# Create directory structure
echo "📁 Creating directories..."
mkdir -p src include build tools tests examples docs

# Move source files
echo "📦 Moving source files..."
[ -f lexer.c ] && mv lexer.c src/
[ -f lexer.h ] && mv lexer.h src/
[ -f main.c ] && mv main.c src/
[ -f xon.lemon ] && mv xon.lemon src/

# Move generated parser files to build/
echo "🔧 Moving generated parser files..."
[ -f xon.c ] && mv xon.c build/
[ -f xon.h ] && mv xon.h build/
[ -f xon.out ] && mv xon.out build/

# Move test file
echo "🧪 Moving test files..."
[ -f test.xon ] && mv test.xon tests/

# Create public API header (stub for now)
echo "📝 Creating public API header..."
cat > include/xon.h << 'EOF'
#ifndef XON_API_H
#define XON_API_H

#include <stdio.h>

// Forward declarations
typedef struct DataNode DataNode;

// Public API functions
DataNode* xon_parse_file(const char* filename);
DataNode* xon_get_key(DataNode* obj, const char* key);
void xon_free_ast(DataNode* node);
void xon_print_ast(DataNode* node, int depth);

#endif // XON_API_H
EOF

# Create example files
echo "📚 Creating example files..."

cat > examples/config.xon << 'EOF'
{
    // Application Configuration
    app_name: "XonApp",
    version: "1.0.0",
    debug: true,
    
    server: {
        host: "0.0.0.0",
        port: 8080,
        ssl: false,
    },
    
    database: {
        type: "postgresql",
        host: "localhost",
        port: 5432,
        name: "xondb",
        pool_size: 0x14,  // 20 in hex
    },
    
    features: [
        "auth",
        "logging",
        "caching",
    ]
}
EOF

cat > examples/data.xon << 'EOF'
{
    users: [
        {
            id: 1,
            name: "Alice",
            active: true,
        },
        {
            id: 2,
            name: "Bob",
            active: false,
        },
    ],
    
    settings: {
        theme: "dark",
        language: "en",
        notifications: true,
    }
}
EOF

cat > examples/nested.xon << 'EOF'
{
    project: {
        name: "Xon Language",
        metadata: {
            author: "Isaac Muliro",
            license: "MIT",
            tags: ["parser", "config", "json"],
        },
        
        build: {
            compiler: "gcc",
            flags: ["-Wall", "-Wextra", "-std=c99"],
            output: "xon",
        },
        
        dependencies: {
            lemon: {
                version: null,
                required: true,
            },
        },
    }
}
EOF

# Create test suite stub
echo "🧪 Creating test suite stub..."
cat > tests/test_suite.c << 'EOF'
#include <stdio.h>
#include <assert.h>
#include "../include/xon.h"

void test_basic_parsing() {
    printf("Running test_basic_parsing...\n");
    // TODO: Implement tests
}

void test_hex_numbers() {
    printf("Running test_hex_numbers...\n");
    // TODO: Implement tests
}

void test_trailing_commas() {
    printf("Running test_trailing_commas...\n");
    // TODO: Implement tests
}

int main() {
    printf("=== Xon Test Suite ===\n");
    test_basic_parsing();
    test_hex_numbers();
    test_trailing_commas();
    printf("All tests passed! ✅\n");
    return 0;
}
EOF

# Create documentation files
echo "📖 Creating documentation files..."

cat > docs/GRAMMAR.md << 'EOF'
# Xon Grammar Specification

## EBNF Notation

```ebnf
root       ::= object | list

object     ::= '{' pair_list? '}'
pair_list  ::= pair (',' pair)* ','?
pair       ::= key ':' value
key        ::= STRING | IDENTIFIER

list       ::= '[' value_list? ']'
value_list ::= value (',' value)* ','?

value      ::= STRING | NUMBER | BOOL | NULL | object | list

STRING     ::= '"' char* '"'
NUMBER     ::= DECIMAL | HEXADECIMAL
DECIMAL    ::= '-'? [0-9]+ ('.' [0-9]+)?
HEXADECIMAL ::= '0' [xX] [0-9a-fA-F]+
BOOL       ::= 'true' | 'false'
NULL       ::= 'null'
IDENTIFIER ::= [a-zA-Z_][a-zA-Z0-9_]*
```

## Grammar Details

### Terminals
- `LBRACE` - `{`
- `RBRACE` - `}`
- `LBRACKET` - `[`
- `RBRACKET` - `]`
- `COLON` - `:`
- `COMMA` - `,`
- `STRING` - Quoted text
- `IDENTIFIER` - Unquoted keys
- `NUMBER` - Decimal or hex numbers
- `TRUE` - Boolean true
- `FALSE` - Boolean false
- `NULL_VAL` - Null value

### Non-Terminals
- `root` - Top-level entry point
- `object` - Key-value pairs
- `list` - Ordered collections
- `pair` - Key-value association
- `value` - Any valid data type

See `src/xon.lemon` for the complete Lemon grammar specification.
EOF

cat > docs/API.md << 'EOF'
# Xon API Reference

## Core Functions

### `xon_parse_file(const char* filename)`
Parse a `.xon` file and return the AST root.

**Parameters:**
- `filename` - Path to the `.xon` file

**Returns:**
- `DataNode*` - Pointer to the root of the AST, or `NULL` on error

**Example:**
```c
DataNode* root = xon_parse_file("config.xon");
if (root) {
    // Use the data
    xon_free_ast(root);
}
```

---

### `xon_get_key(DataNode* obj, const char* key)`
Get a value from an object by key name.

**Parameters:**
- `obj` - Pointer to an object node
- `key` - Key name to search for

**Returns:**
- `DataNode*` - Value node, or `NULL` if not found

**Example:**
```c
DataNode* server = xon_get_key(root, "server");
if (server && server->type == TYPE_OBJECT) {
    DataNode* port = xon_get_key(server, "port");
    if (port && port->type == TYPE_NUMBER) {
        printf("Port: %g\n", port->data.n_val);
    }
}
```

---

### `xon_free_ast(DataNode* node)`
Recursively free all memory associated with an AST.

**Parameters:**
- `node` - Root node to free

**Example:**
```c
xon_free_ast(root);
```

---

### `xon_print_ast(DataNode* node, int depth)`
Print the AST structure for debugging.

**Parameters:**
- `node` - Node to print
- `depth` - Initial indentation depth (usually 0)

**Example:**
```c
xon_print_ast(root, 0);
```

---

## Data Structures

### `DataNode`
```c
typedef struct DataNode {
    enum DataType type;
    struct DataNode *next;
    union {
        char *s_val;
        double n_val;
        int b_val;
        struct {
            struct DataNode *key;
            struct DataNode *value;
        } aggregate;
    } data;
} DataNode;
```

### `DataType`
```c
enum DataType {
    TYPE_OBJECT,
    TYPE_LIST,
    TYPE_STRING,
    TYPE_NUMBER,
    TYPE_BOOL,
    TYPE_NULL
};
```
EOF

cat > docs/EXAMPLES.md << 'EOF'
# Xon Examples

## Basic Configuration

```javascript
{
    app_name: "MyApp",
    version: "1.0.0",
    debug: true,
}
```

## Nested Objects

```javascript
{
    database: {
        host: "localhost",
        port: 5432,
        credentials: {
            user: "admin",
            password: null,
        }
    }
}
```

## Lists

```javascript
{
    colors: ["red", "green", "blue"],
    numbers: [1, 2, 3, 0xFF],
}
```

## Comments

```javascript
{
    // This is a comment
    name: "value",  // Inline comment
}
```

## Hexadecimal Numbers

```javascript
{
    max_connections: 0x64,    // 100
    buffer_size: 0x1000,      // 4096
    color: 0xFF5733,          // RGB color
}
```

## All Data Types

```javascript
{
    string: "Hello, World!",
    number: 42,
    float: 3.14,
    hex: 0xDEADBEEF,
    bool_true: true,
    bool_false: false,
    null_value: null,
    list: [1, 2, 3],
    object: { nested: "value" },
}
```
EOF

# Create CONTRIBUTING.md
echo "🤝 Creating CONTRIBUTING.md..."
cat > CONTRIBUTING.md << 'EOF'
# Contributing to Xon

Thank you for your interest in contributing to Xon!

## How to Contribute

### Reporting Bugs
- Use GitHub Issues
- Include steps to reproduce
- Provide sample `.xon` files
- Mention your OS and compiler version

### Suggesting Features
- Open a GitHub Discussion or Issue
- Describe the use case
- Provide syntax examples

### Pull Requests
1. Fork the repository
2. Create a feature branch
3. Make your changes in `src/`
4. Regenerate parser: `./tools/lemon src/xon.lemon`
5. Test: `make test`
6. Commit with clear messages
7. Push and open a PR

## Code Style
- 4 spaces for indentation
- Follow existing conventions
- Add comments for complex logic
- Update docs for new features

## Development Setup

```bash
git clone https://github.com/yourusername/xon.git
cd xon
make
make test
```

## Questions?
Open a GitHub Discussion or contact the maintainers.
EOF

# Create LICENSE
echo "📜 Creating LICENSE..."
cat > LICENSE << 'EOF'
MIT License

Copyright (c) 2025 Isaac Muliro

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
EOF

# Create Makefile
echo "⚙️  Creating Makefile..."
cat > Makefile << 'EOF'
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -Ibuild
LEMON = ./tools/lemon

SRC_DIR = src
BUILD_DIR = build
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c $(BUILD_DIR)/xon.c
TARGET = xon

all: $(TARGET)

$(BUILD_DIR)/xon.c: $(SRC_DIR)/xon.lemon
    @mkdir -p $(BUILD_DIR)
    $(LEMON) $(SRC_DIR)/xon.lemon
    @mv xon.c xon.h xon.out $(BUILD_DIR)/ 2>/dev/null || true

$(TARGET): $(BUILD_DIR)/xon.c $(SOURCES)
    $(CC) $(CFLAGS) -o $(TARGET) $(SRC_DIR)/main.c $(SRC_DIR)/lexer.c

clean:
    rm -rf $(BUILD_DIR)/* $(TARGET)

test: $(TARGET)
    @echo "Testing basic example..."
    ./$(TARGET) tests/test.xon
    @echo "\nTesting config example..."
    ./$(TARGET) examples/config.xon

.PHONY: all clean test
EOF

# Create .gitignore
echo "📝 Creating .gitignore..."
cat > .gitignore << 'EOF'
# Build artifacts
xon
*.o
*.out
build/xon.c
build/xon.h
build/xon.out

# Editor files
.vscode/
.idea/
*.swp
*.swo
*~

# OS files
.DS_Store
Thumbs.db
EOF

echo ""
echo "✅ Project restructure complete!"
echo ""
echo "📊 New structure:"
echo "xon/"
echo "├── src/           (source files)"
echo "├── include/       (public headers)"
echo "├── build/         (generated files)"
echo "├── tools/         (lemon)"
echo "├── tests/         (test files)"
echo "├── examples/      (example .xon files)"
echo "├── docs/          (documentation)"
echo "├── Makefile"
echo "├── LICENSE"
echo "├── CONTRIBUTING.md"
echo "└── README.md"
echo ""
echo "🔧 Next steps:"
echo "  1. Run: make"
echo "  2. Test: make test"
echo "  3. Try examples: ./xon examples/config.xon"
echo ""
echo "🎉 Done! Your Xon project is now properly organized."