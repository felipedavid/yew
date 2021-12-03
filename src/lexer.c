#include <ctype.h>
#include <stdio.h>

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
        token.str = str_intern_range(token.start, stream);
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
