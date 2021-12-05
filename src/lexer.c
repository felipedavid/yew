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
    case '0' ... '9':
        long val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream++ - '0';
        }
        token.val = val;
        token.type = TOKEN_INT;
        break;
    case 'A' ... 'Z':
    case 'a' ... 'z':
        stream++;
        while (isalpha(*stream) || *stream == '_') {
            stream++;
        }
        token.type = TOKEN_NAME;
        token.name = str_intern_range(token.start, stream);
        break;
    default:
        token.type = *stream++;
        break;
    }
    token.end = stream;
}

void print_token(Token tk) {
    if (tk.type == TOKEN_INT) {
        printf("%d, \"%.*s\", %d\n", tk.type, tk.end - tk.start, tk.start, tk.val);
    } else {
        printf("%d, %.*s\n", tk.type, tk.end - tk.start, tk.start);
    }
}

void lex_test() {
    stream = "ABC+12345_haha_lumin";
    next_token();
    while (token.type) {
        print_token(token);
        next_token();
    }
}

const char *token_type_name(Token_Type type) {
    static const char buf[256];
    switch (type) {
    case TOKEN_NAME: sprintf(buf, "name"); break;
    case TOKEN_INT:  sprintf(buf, "int"); break;
    default: 
        if (type < 128 && isprint(type)) {
            sprintf(buf, "%c", type);
        } else {
            sprintf(buf, "<ASCII %d>", kind);
        }
        break;
    }
    return buf;
}

inline bool is_token(Token_Type type) {
    return token.type == type;
}

inline bool is_token_name(const char *name) {
    return is_token(TOKEN_NAME) && token.name == name;
}

inline bool match_token(Token_Type type) {
    if (is_token(type)) {
        next_token();
        return true;
    }
    return false;
}

inline void expect_token(Token_Type type) {
    if (is_token(type)) next_token();
    fatal("expected token %s, got %s", token_type_name(type), token_type_name(token.type));
}
