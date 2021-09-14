#include <stdlib.h>
#include <string>
#include <vector>

#include "scanner.h"
#include "token.h"

Scanner::Scanner(std::string source) {
    this->source = source;
}

std::vector<Token> Scanner::scan_tokens() {
    while (current < source.size()) {
        start = current;
        scan_token();
    }
    
    Token token(EOF, "", NULL, line);
    tokens.push_back(token);
    return tokens;
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

void Scanner::scan_token() {
    switch (source[current++]) {
        case '(': add_token(LEFT_PAREN);  break;
        case ')': add_token(RIGHT_PAREN); break;
        case '{': add_token(LEFT_BRACE);  break;
        case '}': add_token(RIGHT_BRACE); break;
        case ',': add_token(COMMA);       break;
        case '.': add_token(DOT);         break;
        case '-': add_token(MINUS);       break;
        case '+': add_token(PLUS);        break;
        case ';': add_token(SEMICOLON);   break;
        case '*': add_token(STAR);        break;
        case '!': add_token(match('=') ? BANG_EQUAL : BANG);       break;
        case '=': add_token(match('=') ? EQUAL_EQUAL : EQUAL);     break;
        case '<': add_token(match('=') ? LESS_EQUAL : LESS);       break;
        case '>': add_token(match('=') ? GREATER_EQUAL : GREATER); break;
        case '/':
            if (match('/')) {
                while (peek() != '\n' && current < source.size()) current++;
            } else {
                add_token(SLASH);
            }
            break;
        // Ignore whitespace.
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n': line++; break;
        case '"':
            while (peek() != '"' && current < source.size()) current++;
            if (current >= source.size()) {
                std::cout << "Unterminated String" << std::endl;
                exit(1);
            }

            current++; // Jump the closing ".
            std::string = source.substr(start + 1, current - 1);
            add_token(STRING, value);
            break;
        default:
            if (is_digit(c)) {
                while (is_digit(peek())) current++;

                if (peek() == '.' && is_digit(peek_next())) {
                    current++;
                    while (is_digit(peek())) current++;
                }
                addToken(NUMBER, atof(source.substr(start, current).c_str()));
            } else {
                std::cout << "Unexpected character" << std::endl;
                exit(1);
            }
            break;
    }
}

void Scanner::add_token(TokenType type) {
    add_token(type, NULL);
}

void Scanner::add_token(TokenType type, Object literal) {
    std::string lexeme = source.substr(start, current);
    Token token(type, lexeme, literal, line);
    tokens.push_back();
}
