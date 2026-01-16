#ifndef XON_LEXER_H
#define XON_LEXER_H

#include <stdio.h>

typedef union {
    char *sVal;
    double nVal;
} XonTokenData;

// NEW: Added int *pLine to track line numbers
int xon_get_token(FILE *pFile, XonTokenData *pData, char **ppzErrMsg, int *pLine);

#endif // XON_LEXER_H