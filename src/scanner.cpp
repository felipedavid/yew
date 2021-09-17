#include "scanner.h"

Scanner::Scanner(std::string source) {
    this->source = source;
}

bool Scanner::is_at_end() { return current >= source.size(); }

std::vector<Token> Scanner::scan_tokens() {
    while (!is_at_end()) {
        start = current;
        scan_token();
    }

    tokens.push_back(Token(EOFF, "", line, 0));
    return tokens;
}

void Scanner::scan_token() {
    switch (source[current++]) {
        case '(': add_token(LEFT_PAREN); break;
        case ')': add_token(RIGHT_PAREN); break;
        case '{': add_token(LEFT_BRACE); break;
        case '}': add_token(RIGHT_BRACE); break;
        case ',': add_token(COMMA); break;
        case '.': add_token(DOT); break;
        case '-': add_token(MINUS); break;
        case '+': add_token(PLUS); break;
        case ';': add_token(SEMICOLON); break;
        case '*': add_token(STAR); break; 
        case '!': add_token(match('=') ? BANG_EQUAL : BANG); break;
        default:
            std::cout << line << " Unexpected Character" << std::endl;
            break;
    }
}

bool Scanner::match(char expected) {
    if (is_at_end()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

void Scanner::add_token(Token_Type type) {
    add_token(type, 0);
}

void Scanner::add_token(Token_Type type, uint64_t literal) {
    std::string lexeme = source.substr(start, current-start);
    tokens.push_back(Token(type, lexeme, line, literal));
}
