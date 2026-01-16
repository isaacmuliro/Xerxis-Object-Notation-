#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "xon.c"

// --- VISITOR FUNCTIONS ---

// Get a value from an object by key
DataNode* xon_get_key(DataNode* obj, const char* key) {
    if (!obj || obj->type != TYPE_OBJECT) return NULL;
    
    // The object wrapper contains the list of pairs in 'value'
    DataNode* current = obj->data.aggregate.value;
    while (current) {
        // Each item in the list is a PAIR (TYPE_OBJECT)
        if (current->data.aggregate.key && 
            strcmp(current->data.aggregate.key->data.s_val, key) == 0) {
            return current->data.aggregate.value;
        }
        current = current->next;
    }
    return NULL;
}

// --- UTILITIES ---

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
                current = current->next; // Use new next pointer
            }
            break;
        case TYPE_LIST:
            printf("LIST\n");
            DataNode* item = node->data.aggregate.value;
            while (item) {
                print_ast(item, depth + 1);
                item = item->next; // Use new next pointer
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

    // 1. Free children based on type
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

    // 2. Free siblings (linked list)
    if (node->next) {
        free_xon_ast(node->next);
    }

    // 3. Free current node
    free(node);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename.xon>\n", argv[0]);
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Failed to open file");
        return 1;
    }

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

       
        if (root) {
            printf("Parsing Successful! AST Structure:\n");
            print_ast(root, 0);
    
            // --- DEMO VISITOR PATTERN ---
            printf("\n--- Visitor Demo (Dynamic Traversal) ---\n");
            
            if (root->type == TYPE_OBJECT) {
                // The root object contains a linked list of pairs in 'value'
                DataNode* pair = root->data.aggregate.value;
                
                while (pair) {
                    // Each node in this list is a PAIR (which is a TYPE_OBJECT wrapper)
                    if (pair->data.aggregate.key) {
                        printf("Found Key: %-15s ", pair->data.aggregate.key->data.s_val);
                        
                        DataNode* val = pair->data.aggregate.value;
                        if (val) {
                            switch(val->type) {
                                case TYPE_STRING: printf("-> String: \"%s\"\n", val->data.s_val); break;
                                case TYPE_NUMBER: printf("-> Number: %g\n", val->data.n_val); break;
                                case TYPE_BOOL:   printf("-> Bool: %s\n", val->data.b_val ? "true" : "false"); break;
                                case TYPE_NULL:   printf("-> Null\n"); break;
                                case TYPE_LIST:   printf("-> [List]\n"); break;
                                case TYPE_OBJECT: printf("-> {Object}\n"); break;
                            }
                        }
                    }
                    // Move to the next pair in the object
                    pair = pair->next;
                }
            } else {
                printf("Root is not an object.\n");
            }
        } else {
            printf("Parsing Failed.\n");
        }
    
        xonParserFree(pParser, free);
        fclose(f);
        if (root) free_xon_ast(root);
        
        return 0;
    }