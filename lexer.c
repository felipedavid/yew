#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>

#include "lexer.h"
#include "common.h"

Token token;
const char *stream;

void next_token() {
    token.start = stream;
    switch (*stream) {
    case '0' ... '9': {
        long val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream++ - '0';
        }
        token.val = val;
        token.type = TOKEN_INT;
    } break;
    case 'A' ... 'Z':
    case 'a' ... 'z': {
        stream++;
        while (isalpha(*stream) || *stream == '_') {
            stream++;
        }
        token.type = TOKEN_NAME;
        token.name = str_intern_range(token.start, stream);
    } break;
    default:
        token.type = *stream++;
        break;
    }
    token.end = stream;
}

void print_token(Token tk) {
    printf("type: %s, lexeme: \"%.*s\"", token_type_name(tk.type), tk.end - tk.start, tk.start);
    if (tk.type == TOKEN_INT) {
        printf(", value: %ld\n", tk.val);
    } else {
        printf("\n");
    }
}

void lex_test() {
    stream = "ABC+12345_haha_lumin";
    printf("Stream: \"%s\"\n", stream);
    next_token();
    while (token.type) {
        print_token(token);
        next_token();
    }
}

const char *token_type_name(Token_Type type) {
    static char buf[256];
    switch (type) {
    case TOKEN_NAME: sprintf(buf, "name"); break;
    case TOKEN_INT:  sprintf(buf, "int");  break;
    default: 
        if (type < 128 && isprint(type)) {
            sprintf(buf, "%c", type);
        } else {
            sprintf(buf, "<ASCII %d>", type);
        }
        break;
    }
    return buf;
}
