#include "token.h"

Token::Token(Token_Type type, std::string lexeme, int line, uint64_t literal) {
    this->type = type;
    this->lexeme = lexeme;
    this->integer_value = literal;
    this->line = line;
}

void Token::print() {
    std::cout << lexeme << std::endl;
}
