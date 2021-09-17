#include <iostream>
#include <string>

#include "token_type.h"

struct Token {
    Token_Type type;
    std::string lexeme;
    int line;

    union {
        uint64_t integer_value;
        double float_value;
        char *string_value;
    };

    Token(Token_Type type, std::string lexeme, int line, uint64_t literal);
    void print();
};
