#include "common.h"
#include "lexer.h"

inline bool is_token(Token_Type type) {
    return token.type == type;
}

inline bool is_token_name(const char *name) {
    return is_token(TOKEN_NAME) && token.name == name;
}

inline bool match_token(Token_Type type) {
    if (is_token(type)) {
        next_token();
        return true;
    }
    return false;
}

inline void expect_token(Token_Type type) {
    if (is_token(type)) next_token();
    fatal("expected token %s, got %s", token_type_name(type), token_type_name(token.type));
}

void parse_expr() {
    
}
