# Xon C API Reference

## Core Parsing

### `XonValue* xonify(const char* filename)`
Parse a `.xon` (or JSON) file from disk.

### `XonValue* xonify_string(const char* xon_string)`
Parse Xon content directly from a string.

### `XonValue* xon_eval(const XonValue* value)`
Evaluate parsed runtime expressions with variable resolution, conditionals, and function calls.

- Supports `let`/`const` declarations and identifier references.
- Supports short-circuit boolean ops (`&&`, `||`) and null-coalescing (`??`).
- Supports anonymous function expressions and call evaluation.
- Resolves built-in helpers (`abs`, `len`, `max`, `min`, `str`, `upper`, `lower`, `keys`, `has`, `env`).
- Returns `NULL` on evaluation error.
  Caller must free the returned value with `xon_free`.

Supported runtime operators:

- arithmetic: `+`, `-`, `*`, `/`, `%`
- comparison: `==`, `!=`, `<`, `<=`, `>`, `>=`
- boolean: `!`, `&&`, `||`
- conditional: `if (...) ... else ...` and `?:`
- nullish coalescing: `??`

Built-ins and contracts:

- `abs(x)` requires one numeric argument.
- `len(x)` requires one list, string, or object argument.
- `min(x...)`, `max(x...)` requires at least one numeric argument.
- `str(x)` requires one argument, returns a string representation.
- `upper(x)`, `lower(x)` require one string argument.
- `keys(obj)` requires one object argument.
- `has(obj, key)` requires object + string key.
- `env(name)` requires one string argument.

### `void xon_free(XonValue* value)`
Free parsed values recursively.

## Type and Primitive Access

### `XonType xon_get_type(const XonValue* value)`
Returns one of:
- `XON_TYPE_NULL`
- `XON_TYPE_BOOL`
- `XON_TYPE_NUMBER`
- `XON_TYPE_STRING`
- `XON_TYPE_OBJECT`
- `XON_TYPE_LIST`

### Helpers
- `int xon_is_null(const XonValue* value)`
- `int xon_is_bool(const XonValue* value)`
- `int xon_is_number(const XonValue* value)`
- `int xon_is_string(const XonValue* value)`
- `int xon_is_object(const XonValue* value)`
- `int xon_is_list(const XonValue* value)`

### Primitive getters
- `int xon_get_bool(const XonValue* value)`
- `double xon_get_number(const XonValue* value)`
- `const char* xon_get_string(const XonValue* value)`

## Object Operations

- `XonValue* xon_object_get(const XonValue* obj, const char* key)`
- `int xon_object_has(const XonValue* obj, const char* key)`
- `size_t xon_object_size(const XonValue* obj)`
- `const char* xon_object_key_at(const XonValue* obj, size_t index)`
- `XonValue* xon_object_value_at(const XonValue* obj, size_t index)`

## List Operations

- `XonValue* xon_list_get(const XonValue* list, size_t index)`
- `size_t xon_list_size(const XonValue* list)`

## Serialization

### `char* xon_to_json(const XonValue* value, int pretty)`
Serialize parsed values to JSON text. `pretty=1` pretty prints.

### `char* xon_to_xon(const XonValue* value, int pretty)`
Serialize parsed values back to Xon text. `pretty=1` pretty prints.

### `void xon_string_free(char* str)`
Free strings returned by `xon_to_json` / `xon_to_xon`.

## Debugging

### `void xon_print(const XonValue* value)`
Prints AST structure in a readable tree format.

## Logging

### `int xon_set_log_directory(const char* directory)`
Set custom log directory. Directory is created if missing.

### `void xon_set_log_level(XonLogLevel level)`
Set minimum log verbosity.

### `void xon_enable_stderr_logging(int enabled)`
Enable or disable warning/error mirroring to stderr.

### `void xon_shutdown_logging(void)`
Flush and close logger resources.
