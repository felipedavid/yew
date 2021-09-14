#include "token_type.h"

struct Token {
    TokenType type;
    std::string lexeme; 
    Object literal;
    int line;

    Token(TokenType type, std::string lexeme, Object literal, int line);
    std::string to_string();
};
