#include <string>
#include <vector>

#include "token.h"

struct Scanner {
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    Scanner(std::string source);
    std::vector<Token> scan_tokens();
    void scan_token();
    void add_token(Token_Type type);
    void add_token(Token_Type type, Token_Literal literal);
};
