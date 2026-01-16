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
