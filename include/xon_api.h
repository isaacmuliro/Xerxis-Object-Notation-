#ifndef XON_API_H
#define XON_API_H

#include <stdio.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// Opaque pointer - actual definition is DataNode from xon.c
struct DataNode;
typedef struct DataNode XonValue;

// Type enumeration for runtime type checking
typedef enum {
    XON_TYPE_NULL,
    XON_TYPE_BOOL,
    XON_TYPE_NUMBER,
    XON_TYPE_STRING,
    XON_TYPE_OBJECT,
    XON_TYPE_LIST
} XonType;

// ============ Core API (Branded) ============

// Parse a .xon file from path (Brand: xonify)
XonValue* xonify(const char* filename);

// Parse .xon from a string (Brand: xonify_string)
XonValue* xonify_string(const char* xon_string);

// Free memory
void xon_free(XonValue* value);

// ============ Type Checking ============

XonType xon_get_type(const XonValue* value);
int xon_is_null(const XonValue* value);
int xon_is_bool(const XonValue* value);
int xon_is_number(const XonValue* value);
int xon_is_string(const XonValue* value);
int xon_is_object(const XonValue* value);
int xon_is_list(const XonValue* value);

// ============ Value Access ============

// Get primitive values (returns 0/NULL on type mismatch)
int xon_get_bool(const XonValue* value);
double xon_get_number(const XonValue* value);
const char* xon_get_string(const XonValue* value);

// ============ Object Operations ============

// Get value by key (returns NULL if not found)
XonValue* xon_object_get(const XonValue* obj, const char* key);

// Check if key exists
int xon_object_has(const XonValue* obj, const char* key);

// Get number of keys
size_t xon_object_size(const XonValue* obj);

// ============ List Operations ============

// Get list element by index (returns NULL if out of bounds)
XonValue* xon_list_get(const XonValue* list, size_t index);

// Get list length
size_t xon_list_size(const XonValue* list);

// ============ Debugging ============

// Print AST structure
void xon_print(const XonValue* value);

#ifdef __cplusplus
}
#endif

#endif // XON_API_H
