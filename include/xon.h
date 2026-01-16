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
