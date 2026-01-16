#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "xon.h" // Required for token IDs

int xon_get_token(FILE *pFile, XonTokenData *pData, char **ppzErrMsg, int *pLine) {
    int c;

    // 1. Skip Whitespace and Comments
    while ((c = fgetc(pFile)) != EOF) {
        if (c == '\n') {
            (*pLine)++;
        }
        else if (isspace(c)) {
            continue;
        }
        else if (c == '/') {
            int next = fgetc(pFile);
            if (next == '/') {
                while ((c = fgetc(pFile)) != EOF && c != '\n');
                if (c == '\n') (*pLine)++;
                continue;
            } else {
                ungetc(next, pFile);
                break; 
            }
        }
        else {
            break;
        }
    }

    if (c == EOF) return 0; 

    // 2. Handle Structural Tokens
    switch (c) {
        case '{': return LBRACE;
        case '}': return RBRACE;
        case '[': return LBRACKET;
        case ']': return RBRACKET;
        case ':': return COLON;
        case ',': return COMMA;
    }

    // 3. Handle Strings
    if (c == '"') {
        char buffer[1024];
        int i = 0;
        while ((c = fgetc(pFile)) != EOF && i < 1023) {
            if (c == '"') break;
            if (c == '\\') {
                c = fgetc(pFile);
                if (c == EOF) break;
                switch(c) {
                    case 'n': buffer[i++] = '\n'; break;
                    case 't': buffer[i++] = '\t'; break;
                    case 'r': buffer[i++] = '\r'; break;
                    case '"': buffer[i++] = '"'; break;
                    case '\\': buffer[i++] = '\\'; break;
                    default: buffer[i++] = (char)c; break;
                }
            } else {
                buffer[i++] = (char)c;
            }
        }
        buffer[i] = '\0';
        pData->sVal = strdup(buffer); 
        return STRING;
    }

    // 4. Handle Numbers
    // 4. Handle Numbers (Decimal and Hex)
    if (isdigit(c) || c == '-') {
        char buffer[64];
        int i = 0;
        
        // Check for Hex (0x...)
        if (c == '0') {
            buffer[i++] = (char)c;
            int next = fgetc(pFile);
            if (next == 'x' || next == 'X') {
                buffer[i++] = (char)next;
                while ((c = fgetc(pFile)) != EOF && (isxdigit(c))) {
                    if (i < 63) buffer[i++] = (char)c;
                }
                buffer[i] = '\0';
                if (c != EOF) ungetc(c, pFile);
                
                // Convert Hex string to double/long
                pData->nVal = (double)strtol(buffer, NULL, 16);
                return NUMBER;
            }
            // Not hex, put back the char and continue as decimal
            ungetc(next, pFile);
        } else {
            buffer[i++] = (char)c;
        }
        
        // Standard Decimal parsing
        while ((c = fgetc(pFile)) != EOF && (isdigit(c) || c == '.')) {
            if (i < 63) buffer[i++] = (char)c;
        }
        buffer[i] = '\0';
        if (c != EOF) ungetc(c, pFile);

        pData->nVal = strtod(buffer, NULL);
        return NUMBER;
    }


    // 5. Handle Keywords (True, False, Null)
    // 5. Handle Identifiers and Keywords
    if (isalpha(c) || c == '_') { // Allow underscores
        char buffer[64];
        int i = 0;
        buffer[i++] = (char)c;
        while ((c = fgetc(pFile)) != EOF && (isalnum(c) || c == '_') && i < 63) {
            buffer[i++] = (char)c;
        }
        buffer[i] = '\0';
        if (c != EOF) ungetc(c, pFile);

        if (strcmp(buffer, "true") == 0) return TRUE;
        if (strcmp(buffer, "false") == 0) return FALSE;
        if (strcmp(buffer, "null") == 0) return NULL_VAL;
        
        // It's not a keyword, so it's an Identifier (Unquoted Key)
        pData->sVal = strdup(buffer);
        return STRING; // We can reuse STRING token ID for simplicity in the parser!
    }

    *ppzErrMsg = strdup("Unexpected character found");
    return -1;
}
