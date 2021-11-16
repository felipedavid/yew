#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "scanner.h"

Token token;
const char *stream;

void next_token() {
    token.start = stream;
    switch (*stream) {
    case '0' ... '9': {
        uint64_t val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream++ - '0';
        }
        token.type = TOKEN_INT;
        token.val = val;
        break;
    }
    case 'A' ... 'Z': {
        stream++;
        while (isalpha(*stream) || *stream == '_') stream++;
        token.type = TOKEN_NAME;
        break;
    }
    default:
        token.type = *stream++;
        break;
    }
    token.end = stream;
}

void print_token(Token token) {
    switch (token.type) {
        case TOKEN_INT: 
            printf("INT, \"%.*s\", %d\n", (int)(token.end - token.start), token.start, token.val);
            break;
        case TOKEN_NAME:
            printf("NAME, \"%.*s\"\n", (int)(token.end - token.start), token.start);
            break;
        default:
            printf("WHATEVER, \"%.*s\"\n", (int)(token.end - token.start), token.start);
            break;
    }
}

void scanner_test() {
    stream = "+()_HELLO1,234+FOO!994";
    next_token();
    while (token.type) {
        print_token(token);
        next_token();
    }
}
