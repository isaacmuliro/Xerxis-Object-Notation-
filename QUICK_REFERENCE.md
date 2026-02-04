# Xon Quick Reference

## Installation

### C Library
```bash
# Clone and build
git clone https://github.com/yourusername/xon.git
cd xon
./build.sh

# Use in your project
gcc -o myapp myapp.c -L/path/to/xon -lxon -I/path/to/xon/include
```

### Node.js
```bash
npm install xon
```

---

## C API (Branded)

### Parse Files
```c
#include <xon_api.h>

// Branded: xonify() - Parse from file
XonValue* config = xonify("config.xon");

// Branded: xonify_string() - Parse from string
XonValue* data = xonify_string("{ key: \"value\" }");
```

### Access Values
```c
// Get object field
XonValue* server = xon_object_get(config, "server");
XonValue* port = xon_object_get(server, "port");

// Get list item
XonValue* features = xon_object_get(config, "features");
XonValue* first = xon_list_get(features, 0);

// Extract primitive values
double num = xon_get_number(port);
const char* str = xon_get_string(first);
int bool_val = xon_get_bool(debug_flag);
```

### Type Checking
```c
if (xon_is_number(value)) {
    double n = xon_get_number(value);
}

if (xon_is_string(value)) {
    const char* s = xon_get_string(value);
}

if (xon_is_object(value)) {
    XonValue* field = xon_object_get(value, "fieldname");
}
```

### Memory Management
```c
xon_free(config);  // Always free when done!
```

---

## JavaScript API (Branded)

### Parse Files
```javascript
const xon = require('xon');

// Branded: xonify() - Parse from file
const config = xon.xonify('config.xon');

// Branded: xonifyString() - Parse from string
const data = xon.xonifyString('{ name: "test", value: 42 }');
```

### Access Values
```javascript
// Direct property access
console.log(config.server.port);        // 8080
console.log(config.app_name);           // "XonApp"
console.log(config.features[0]);        // "auth"

// Arrays
config.features.forEach(f => console.log(f));
```

---

## Xon Syntax

### Basic Types
```javascript
{
    string: "Hello",
    number: 42,
    float: 3.14,
    hex: 0xFF,              // Hex numbers!
    bool: true,
    null_val: null
}
```

### Objects
```javascript
{
    // Unquoted keys
    server: {
        host: "localhost",
        port: 8080,         // Trailing comma OK!
    }
}
```

### Lists
```javascript
{
    items: [
        "first",
        "second",
        "third",            // Trailing comma OK!
    ]
}
```

### Comments
```javascript
{
    // Single-line comments
    name: "value",  // Inline comments
}
```

### Escape Sequences
```javascript
{
    text: "Line 1\nLine 2\tTabbed",
    path: "C:\\Windows\\System32",
    quote: "Say \"hello\""
}
```

---

## CLI Usage

```bash
# Parse and print AST
./xon config.xon

# Parse and validate
./xon mydata.xon && echo "Valid!"
```

---

## Build from Source

```bash
# Generate parser
./tools/lemon src/xon.lemon

# Build library
gcc -dynamiclib -o libxon.dylib src/xon_api.c src/lexer.c -Ibuild

# Build CLI
gcc -o xon src/main.c src/lexer.c -Ibuild

# Or use build script
./build.sh
```

---

## Complete Example

### config.xon
```javascript
{
    app_name: "MyApp",
    version: "1.0.0",
    
    // Server configuration
    server: {
        host: "0.0.0.0",
        port: 8080,
        max_connections: 0x64,  // 100 in hex
    },
    
    features: ["auth", "logging", "cache"]
}
```

### C Program
```c
#include <stdio.h>
#include <xon_api.h>

int main() {
    XonValue* cfg = xonify("config.xon");
    
    XonValue* app = xon_object_get(cfg, "app_name");
    printf("App: %s\n", xon_get_string(app));
    
    XonValue* port = xon_object_get(
        xon_object_get(cfg, "server"),
        "port"
    );
    printf("Port: %.0f\n", xon_get_number(port));
    
    xon_free(cfg);
    return 0;
}
```

### JavaScript Program
```javascript
const xon = require('xon');

const cfg = xon.xonify('config.xon');

console.log('App:', cfg.app_name);
console.log('Port:', cfg.server.port);
console.log('Features:', cfg.features.join(', '));
```

---

## API Reference

### Core Functions (C)
| Function | Description |
|----------|-------------|
| `xonify(path)` | Parse file (branded) |
| `xonify_string(str)` | Parse string (branded) |
| `xon_free(v)` | Free memory |
| `xon_print(v)` | Debug print |

### Type Checking (C)
| Function | Returns |
|----------|---------|
| `xon_get_type(v)` | XonType enum |
| `xon_is_null(v)` | bool |
| `xon_is_bool(v)` | bool |
| `xon_is_number(v)` | bool |
| `xon_is_string(v)` | bool |
| `xon_is_object(v)` | bool |
| `xon_is_list(v)` | bool |

### Value Access (C)
| Function | Returns |
|----------|---------|
| `xon_get_bool(v)` | int (1 or 0) |
| `xon_get_number(v)` | double |
| `xon_get_string(v)` | const char* |

### Objects (C)
| Function | Description |
|----------|-------------|
| `xon_object_get(obj, key)` | Get value by key |
| `xon_object_has(obj, key)` | Check if key exists |
| `xon_object_size(obj)` | Get key count |

### Lists (C)
| Function | Description |
|----------|-------------|
| `xon_list_get(list, index)` | Get item by index |
| `xon_list_size(list)` | Get item count |

---

## License

MIT License - See LICENSE file

## Links

- GitHub: https://github.com/yourusername/xon
- Docs: https://github.com/yourusername/xon/tree/main/docs
- Examples: https://github.com/yourusername/xon/tree/main/examples
