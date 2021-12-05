
typedef enum {
    TOKEN_NAME = 128,
    TOKEN_INT,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    union {
        long val;
        const char *name;
    };
} Token;

void lex_test();
