#include "../include/xon_api.h"
#include "lexer.h"
#include "../build/xon.c"
#include <string.h>
#include <stdlib.h>

// ============ Helper Functions ============

// Get a value from an object by key
DataNode* xon_get_key(DataNode* obj, const char* key) {
    if (!obj || obj->type != TYPE_OBJECT) return NULL;
    
    DataNode* current = obj->data.aggregate.value;
    while (current) {
        if (current->data.aggregate.key && 
            strcmp(current->data.aggregate.key->data.s_val, key) == 0) {
            return current->data.aggregate.value;
        }
        current = current->next;
    }
    return NULL;
}

void print_ast(DataNode* node, int depth) {
    if (!node) return;
    for (int i = 0; i < depth; i++) printf("  ");

    switch (node->type) {
        case TYPE_OBJECT:
            printf("OBJECT\n");
            DataNode* current = node->data.aggregate.value;
            while (current) {
                for (int i = 0; i < depth + 1; i++) printf("  ");
                if (current->data.aggregate.key) {
                    printf("Key: %s\n", current->data.aggregate.key->data.s_val);
                }
                print_ast(current->data.aggregate.value, depth + 2);
                current = current->next;
            }
            break;
        case TYPE_LIST:
            printf("LIST\n");
            DataNode* item = node->data.aggregate.value;
            while (item) {
                print_ast(item, depth + 1);
                item = item->next;
            }
            break;
        case TYPE_STRING:
            printf("STRING: \"%s\"\n", node->data.s_val);
            break;
        case TYPE_NUMBER:
            printf("NUMBER: %f\n", node->data.n_val);
            break;
        case TYPE_BOOL:
            printf("BOOL: %s\n", node->data.b_val ? "true" : "false");
            break;
        case TYPE_NULL:
            printf("NULL\n");
            break;
    }
}

void free_xon_ast(DataNode* node) {
    if (!node) return;

    if (node->type == TYPE_STRING) {
        if (node->data.s_val) free(node->data.s_val);
    }
    else if (node->type == TYPE_OBJECT) {
        if (node->data.aggregate.key) free_xon_ast(node->data.aggregate.key);
        if (node->data.aggregate.value) free_xon_ast(node->data.aggregate.value);
    }
    else if (node->type == TYPE_LIST) {
        if (node->data.aggregate.value) free_xon_ast(node->data.aggregate.value);
    }

    if (node->next) {
        free_xon_ast(node->next);
    }

    free(node);
}

// ============ Core API (Branded) ============

XonValue* xonify(const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return NULL;

    void* pParser = xonParserAlloc(malloc);
    DataNode* root = NULL;
    XonTokenData tokenData;
    char* errMsg = NULL;
    int tokenID;
    int currentLine = 1;

    while ((tokenID = xon_get_token(f, &tokenData, &errMsg, &currentLine)) != 0) {
        if (tokenID == -1) {
            fprintf(stderr, "Lexer Error at line %d: %s\n", currentLine, errMsg);
            free(errMsg);
            break;
        }
        Token parserToken;
        parserToken.s_val = tokenData.sVal;
        parserToken.n_val = tokenData.nVal;
        parserToken.line = currentLine;
        xonParser(pParser, tokenID, parserToken, &root);
    }

    Token emptyToken = {0};
    emptyToken.line = currentLine;
    xonParser(pParser, 0, emptyToken, &root);

    xonParserFree(pParser, free);
    fclose(f);
    return root;
}

XonValue* xonify_string(const char* str) {
    if (!str) return NULL;
    
    // Create a temporary file from the string
    FILE* temp_file = tmpfile();
    if (!temp_file) return NULL;
    
    fputs(str, temp_file);
    rewind(temp_file);
    
    void* pParser = xonParserAlloc(malloc);
    DataNode* root = NULL;
    XonTokenData tokenData;
    char* errMsg = NULL;
    int tokenID;
    int currentLine = 1;

    while ((tokenID = xon_get_token(temp_file, &tokenData, &errMsg, &currentLine)) != 0) {
        if (tokenID == -1) {
            fprintf(stderr, "Lexer Error at line %d: %s\n", currentLine, errMsg);
            free(errMsg);
            break;
        }
        Token parserToken;
        parserToken.s_val = tokenData.sVal;
        parserToken.n_val = tokenData.nVal;
        parserToken.line = currentLine;
        xonParser(pParser, tokenID, parserToken, &root);
    }

    Token emptyToken = {0};
    emptyToken.line = currentLine;
    xonParser(pParser, 0, emptyToken, &root);

    xonParserFree(pParser, free);
    fclose(temp_file);
    return root;
}

void xon_free(XonValue* value) {
    free_xon_ast(value);
}

// ============ Type Checking ============

XonType xon_get_type(const XonValue* value) {
    if (!value) return XON_TYPE_NULL;
    switch (value->type) {
        case TYPE_NULL: return XON_TYPE_NULL;
        case TYPE_BOOL: return XON_TYPE_BOOL;
        case TYPE_NUMBER: return XON_TYPE_NUMBER;
        case TYPE_STRING: return XON_TYPE_STRING;
        case TYPE_OBJECT: return XON_TYPE_OBJECT;
        case TYPE_LIST: return XON_TYPE_LIST;
        default: return XON_TYPE_NULL;
    }
}

int xon_is_null(const XonValue* value) { 
    return value && value->type == TYPE_NULL; 
}

int xon_is_bool(const XonValue* value) { 
    return value && value->type == TYPE_BOOL; 
}

int xon_is_number(const XonValue* value) { 
    return value && value->type == TYPE_NUMBER; 
}

int xon_is_string(const XonValue* value) { 
    return value && value->type == TYPE_STRING; 
}

int xon_is_object(const XonValue* value) { 
    return value && value->type == TYPE_OBJECT; 
}

int xon_is_list(const XonValue* value) { 
    return value && value->type == TYPE_LIST; 
}

// ============ Value Access ============

int xon_get_bool(const XonValue* value) {
    return (value && value->type == TYPE_BOOL) ? value->data.b_val : 0;
}

double xon_get_number(const XonValue* value) {
    return (value && value->type == TYPE_NUMBER) ? value->data.n_val : 0.0;
}

const char* xon_get_string(const XonValue* value) {
    return (value && value->type == TYPE_STRING) ? value->data.s_val : NULL;
}

// ============ Object Operations ============

XonValue* xon_object_get(const XonValue* obj, const char* key) {
    if (!obj || obj->type != TYPE_OBJECT || !key) return NULL;
    return xon_get_key((DataNode*)obj, key);
}

int xon_object_has(const XonValue* obj, const char* key) {
    return xon_object_get(obj, key) != NULL;
}

size_t xon_object_size(const XonValue* obj) {
    if (!obj || obj->type != TYPE_OBJECT) return 0;
    size_t count = 0;
    DataNode* current = obj->data.aggregate.value;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// ============ List Operations ============

XonValue* xon_list_get(const XonValue* list, size_t index) {
    if (!list || list->type != TYPE_LIST) return NULL;
    DataNode* current = list->data.aggregate.value;
    size_t i = 0;
    while (current) {
        if (i == index) return current;
        current = current->next;
        i++;
    }
    return NULL;
}

size_t xon_list_size(const XonValue* list) {
    if (!list || list->type != TYPE_LIST) return 0;
    size_t count = 0;
    DataNode* current = list->data.aggregate.value;
    while (current) {
        count++;
        current = current->next;
    }
    return count;
}

// ============ Debugging ============

void xon_print(const XonValue* value) {
    print_ast((DataNode*)value, 0);
}
