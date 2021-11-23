#include <stdio.h>
#include <ctype.h>
#include <stdint.h>
#include <stdarg.h>

#include "scanner.h"
#include "common.h"

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
        token.name = str_intern_range(token.start, stream);
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
    stream = "XY+(XY)_HELLO1,234+FOO!994";
    next_token();
    while (token.type) {
        print_token(token);
        next_token();
    }
}

void fatal(const char *fmt, ...) {
}

const char *keyword_if;
const char *keyword_for;
const char *keyword_while;

void init_keywords() {
    keyword_if = str_intern("if");
    keyword_for = str_intern("for");
    keyword_while = str_intern("while");
}

inline bool is_token(Token_Type type) {
    return token.type == type;
}

inline bool is_token_name(const char *str) {
    return token.kind == TOKEN_NAME && token.name == str;
}

inline bool match_token(Token_Type type) {
    if (is_token(type)) {
        next_token(); 
        return true;
    }
    return false;
}

inline bool expect_token(Token_Type type) {
    if (is_token(kind)) {
        next_token();
        return true;
    } else {
        fatal("expected token %s, got %s", token_type_name(type), token_type_name(token.type));
    }
}

void parse_stmt() {
    if (is_token_name(keyword_if)) {
        next_token();
        parse_if_stmt();
    }
}
