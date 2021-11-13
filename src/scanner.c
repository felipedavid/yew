#include <stdio.h>
#include <ctype.h>
#include <stdint.h>

#include "scanner.h"

Token token;
const char *stream;

void next_token() {
    token.start = stream;
    switch (*stream) {
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9': {
        uint64_t val = 0;
        while (isdigit(*stream)) {
            val *= 10;
            val += *stream++ - '0';
        }
        token.end = stream;
        token.type = TOKEN_INT;
        token.val = val;
        break;
    }
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
    case 'H':
    case 'I':
    case 'J':
    case 'K':
    case 'L':
    case 'M':
    case 'N':
    case 'O':
    case 'P':
    case 'Q':
    case 'R':
    case 'S':
    case 'T':
    case 'U':
    case 'V':
    case 'W':
    case 'X':
    case 'Y':
    case 'Z':
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'h':
    case 'i':
    case 'j':
    case 'k':
    case 'l':
    case 'm':
    case 'n':
    case 'o':
    case 'p':
    case 'q':
    case 'r':
    case 's':
    case 't':
    case 'u':
    case 'v':
    case 'w':
    case 'x':
    case 'y':
    case 'z': {
        stream++;
        while (isalpha(*stream) || *stream == '_') stream++;
        token.type = TOKEN_NAME;
        token.end = stream;
        break;
    }
    default:
        token.type = *stream++;
        token.end = stream;
        break;
    }
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
