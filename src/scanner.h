#ifndef SCANNER_H
#define SCANNER

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TOKEN_INT = 128, 
    TOKEN_NAME,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    union {
        uint64_t val;
        const char *name;
    };
} Token;

void scanner_test();

#endif
