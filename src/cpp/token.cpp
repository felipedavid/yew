#include "token.h"

Token::Token(TokenType type, std::string lexeme, Object literal, int line) {
    this->type = type;
    this->lexeme = lexeme;
    this->literal = literal;
    this->line = line;
}

Token::to_string() {
    return lexeme;
}
