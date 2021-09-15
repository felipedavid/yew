#include "scanner.h"

Scanner::Scanner(std::string source) {
    this->source = source;
}

bool is_at_end() { return current >= source.size(); }

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }

    tokens.push_back(Token(EOFF, "", NULL, line));
    return tokens;
}

void Scanner::scan_token() {
    switch (source[current++]) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break; 
        default:
            std::cout << line << " Unexpected Character" << std::endl;
            break;
    }
}

void Scanner::add_token(Token_Type type) {
    add_token(type, NULL);
}

void Scanner::add_token(Token_Type type, Token_Literal literal) {
    std::string lexeme = source.substr(start, current);
    tokens.add(Token(type, lexeme, literal, line));
}
