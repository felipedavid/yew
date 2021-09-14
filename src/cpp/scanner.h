#include "token.h"

struct Scanner {
    std::string source;
    std::vector<Token> tokens;
    int start = 0; // points to the first character of the lexeme
    int current = 0; // points to the current character of the lexeme
    int line = 1;

    Scanner(std::string source);
    std::vector<Token> scan_tokens();
    void scan_token();
    void add_token(TokenType type);
    void add_token(TokenType type, Object literal);
};
