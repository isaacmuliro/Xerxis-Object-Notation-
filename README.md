# Xon Language

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/language-C-blue.svg)](https://en.wikipedia.org/wiki/C_(programming_language))

**Xon** is a modern, developer-friendly configuration language that extends JSON with practical features for real-world use cases. Built in C with a hand-written lexer and LALR(1) parser.

## ✨ Features

Xon provides all the structure of JSON, plus:

- **Comments** - C-style `//` comments for documentation
- **Hexadecimal Numbers** - Native support for hex literals (`0xFF`, `0x1A`)
- **Trailing Commas** - No more syntax errors from trailing commas
- **Unquoted Keys** - Clean syntax: `name: "value"` instead of `"name": "value"`
- **Escape Sequences** - Full support for `\n`, `\t`, `\"`, `\\`, etc.
- **Line-Based Error Reporting** - Precise error messages with line numbers

## 📋 Quick Example

```javascript
{
    // Server configuration
    server_name: "XonServer",
    max_memory: 0xFF,           // 255 in hex
    is_active: true,
    features: [
        "fast",
        "simple",
        "powerful",             // trailing comma OK!
    ],
    database: {
        host: "localhost",
        port: 5432,
        ssl: true,
    }
}
```

## 🚀 Getting Started

### Prerequisites

- GCC compiler (or any C99-compatible compiler)
- Make (optional, for build automation)
- Lemon Parser Generator (included in `tools/`)

### Building from Source

```bash
# Clone the repository
git clone https://github.com/yourusername/xon.git
cd xon

# Generate the parser
./tools/lemon xon.lemon

# Compile
gcc -o xon main.c lexer.c -Wall -Wextra

# Run
./xon test.xon
```

### Using a Makefile (Recommended)

Create a `Makefile` in your project root:

```makefile
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LEMON = ./tools/lemon

SOURCES = main.c lexer.c xon.c
TARGET = xon

all: $(TARGET)

xon.c: xon.lemon
    $(LEMON) xon.lemon

$(TARGET): xon.c $(SOURCES)
    $(CC) $(CFLAGS) -o $(TARGET) main.c lexer.c

clean:
    rm -f xon.c xon.h xon.out $(TARGET)

test: $(TARGET)
    ./$(TARGET) test.xon

.PHONY: all clean test
```

Then simply run:
```bash
make          # Build the project
make test     # Run tests
make clean    # Clean build artifacts
```

## 📖 Language Specification

### Data Types

| Type | Example | Description |
|------|---------|-------------|
| **String** | `"Hello\nWorld"` | UTF-8 text with escape sequences |
| **Number** | `42`, `3.14`, `0xFF` | Decimal, floating-point, or hexadecimal |
| **Boolean** | `true`, `false` | Logical values |
| **Null** | `null` | Absence of value |
| **Object** | `{ key: value }` | Key-value mappings |
| **List** | `[1, 2, 3]` | Ordered collections |

### Syntax Rules

1. **Keys** can be quoted (`"name"`) or unquoted (`name`) if they're valid identifiers
2. **Comments** start with `//` and continue to end of line
3. **Trailing commas** are allowed in objects and lists
4. **Hex numbers** must start with `0x` or `0X`
5. **Escape sequences**: `\n` (newline), `\t` (tab), `\"` (quote), `\\` (backslash), `\r` (carriage return)

### Grammar Overview

```
root       ::= object | list
object     ::= '{' pair_list? '}'
list       ::= '[' value_list? ']'
pair       ::= (STRING | IDENTIFIER) ':' value
value      ::= STRING | NUMBER | BOOL | NULL | object | list
```

## 🔧 API Reference

### Parsing a File

```c
#include <stdio.h>
#include "lexer.h"
#include "xon.c"

int main() {
    FILE* f = fopen("config.xon", "r");
    void* parser = xonParserAlloc(malloc);
    DataNode* root = NULL;
    
    // ... token loop (see main.c for complete example)
    
    xonParserFree(parser, free);
    fclose(f);
    
    if (root) {
        // Use the data
        free_xon_ast(root);
    }
    
    return 0;
}
```

### Visitor Pattern

Query parsed data dynamically:

```c
// Get a value from an object by key
DataNode* xon_get_key(DataNode* obj, const char* key);

// Example usage
DataNode* root = /* parsed AST */;
DataNode* server = xon_get_key(root, "server_name");
if (server && server->type == TYPE_STRING) {
    printf("Server: %s\n", server->data.s_val);
}
```

### AST Structure

```c
typedef struct DataNode {
    enum DataType type;         // TYPE_OBJECT, TYPE_LIST, TYPE_STRING, etc.
    struct DataNode *next;      // Sibling in linked list
    union {
        char *s_val;            // String data
        double n_val;           // Numeric data
        int b_val;              // Boolean data (1 or 0)
        struct {
            struct DataNode *key;
            struct DataNode *value;
        } aggregate;            // Object/List container
    } data;
} DataNode;
```

### Data Types

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

### Memory Management

Always free the AST after use to prevent memory leaks:

```c
void free_xon_ast(DataNode* node);  // Recursive cleanup
```

### Utility Functions

```c
// Print AST structure (debugging)
void print_ast(DataNode* node, int depth);

// Create a new node
DataNode* new_node(enum DataType type);

// Link nodes in a list
DataNode* link_node(DataNode* head, DataNode* item);
```

## 🧪 Testing

### Running Tests

```bash
# Compile
gcc -o xon main.c lexer.c

# Run test file
./xon test.xon
```

### Expected Output

```text
Parsing Successful! AST Structure:
OBJECT
  Key: server_name
    STRING: "XonServer"
  Key: max_memory
    NUMBER: 255.000000
  Key: features
    LIST
      STRING: "fast"
      STRING: "simple"

--- Visitor Demo (Dynamic Traversal) ---
Found Key: server_name     -> String: "XonServer"
Found Key: max_memory      -> Number: 255
Found Key: features        -> [List]
Memory cleaned up.
```

### Creating Test Files

Example test file (`test.xon`):

```javascript
{
    // Test all features
    string_test: "Hello\nWorld",
    number_test: 42,
    hex_test: 0xFF,
    bool_test: true,
    null_test: null,
    list_test: [1, 2, 3,],
    object_test: {
        nested: "value",
    }
}
```

## 📂 Project Structure

```
xon/
├── main.c          # CLI driver and AST utilities
├── lexer.c         # Tokenizer implementation
├── lexer.h         # Lexer interface
├── xon.lemon       # Grammar specification
├── xon.c           # Generated parser (from xon.lemon)
├── xon.h           # Generated parser header
├── xon.out         # Parser state machine output
├── test.xon        # Test configuration file
├── tools/
│   └── lemon       # Lemon parser generator
└── README.md       # This file
```

## 🤝 Contributing

We welcome contributions! Here's how you can help:

### Reporting Bugs

Open an issue with:
- Clear title describing the problem
- Steps to reproduce
- Expected vs actual behavior
- Your environment (OS, compiler version)
- Sample `.xon` file that triggers the bug

### Suggesting Features

Open an issue or discussion with:
- Use case description
- Proposed syntax/behavior
- Examples of how it would work

### Pull Requests

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Regenerate parser if grammar changed: `./tools/lemon xon.lemon`
5. Test your changes: `./xon test.xon`
6. Commit with clear messages (`git commit -m 'Add amazing feature'`)
7. Push to your fork (`git push origin feature/amazing-feature`)
8. Open a Pull Request

### Code Style

- Use 4 spaces for indentation
- Follow existing naming conventions
- Add comments for complex logic
- Update documentation for new features
- Ensure code compiles without warnings (`-Wall -Wextra`)

## 🛣️ Roadmap

### Version 1.0 (Current)
- [x] Basic JSON-compatible parsing
- [x] Comments support
- [x] Hexadecimal numbers
- [x] Trailing commas
- [x] Unquoted keys
- [x] Visitor pattern API
- [x] Line-based error reporting

### Future Versions
- [ ] Error recovery (continue parsing after errors)
- [ ] Multi-line strings (triple quotes `"""`)
- [ ] Include/import system
- [ ] Schema validation
- [ ] Binary number literals (`0b1010`)
- [ ] Octal number literals (`0o755`)
- [ ] Scientific notation (`1.23e-4`)
- [ ] Unicode escape sequences (`\u{1F600}`)
- [ ] Date/Time literals
- [ ] Shared library build (`libxon.so`)
- [ ] Language bindings (Python, JavaScript, Rust)
- [ ] VSCode syntax highlighting extension
- [ ] Online playground/REPL
- [ ] Pretty-printer/formatter
- [ ] JSON converter (`xon2json`, `json2xon`)

## 🔍 Comparison with Other Formats

| Feature | JSON | JSON5 | TOML | YAML | **Xon** |
|---------|------|-------|------|------|---------|
| Comments | ❌ | ✅ | ✅ | ✅ | ✅ |
| Trailing Commas | ❌ | ✅ | N/A | N/A | ✅ |
| Unquoted Keys | ❌ | ✅ | ✅ | ✅ | ✅ |
| Hex Numbers | ❌ | ✅ | ✅ | ❌ | ✅ |
| Multi-line Strings | ❌ | ✅ | ✅ | ✅ | 🔜 |
| Native C API | ❌ | ❌ | ❌ | ❌ | ✅ |
| Simple Grammar | ✅ | ✅ | ❌ | ❌ | ✅ |

## 📜 License

This project is licensed under the MIT License - see below for details:

```
MIT License

Copyright (c) 2025 Xon Contributors

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
```

## 🙏 Acknowledgments

- **Lemon Parser Generator** by D. Richard Hipp (creator of SQLite)
- Inspired by JSON, JSON5, and TOML
- Built with guidance from compiler design principles
- Community contributors (thank you!)

## 📞 Contact & Support

- **Issues**: [GitHub Issues](https://github.com/yourusername/xon/issues)
- **Discussions**: [GitHub Discussions](https://github.com/yourusername/xon/discussions)
- **Documentation**: See `xon.docs/` directory
- **Email**: your.email@example.com

## ⭐ Show Your Support

If you find Xon useful, please consider:
- Giving it a star on GitHub ⭐
- Sharing it with others who might benefit
- Contributing code, documentation, or ideas
- Reporting bugs and suggesting improvements

---

**Made with ❤️ by Isaac Muliro**

*"Configuration should be simple, readable, and human-friendly."*