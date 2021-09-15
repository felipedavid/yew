#include "token.h"

Token::Token(Token_type type, std::string lexeme, Literal_Type literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

void Token::print() {
    std::cout << lexeme << std::endl;
}
