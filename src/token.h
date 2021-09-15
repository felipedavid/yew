#include <iostream>
#include <string>

#include "token_type.h"
#include "literal_type.h"

struct Token {
    Token_Type type;
    std::string lexeme;
    Literal_Type literal;
    int line;

    Token(Token_type type, std::string lexeme, Literal_Type literal, int line);
    void print();
};
