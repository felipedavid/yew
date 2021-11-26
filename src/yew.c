#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <stdint.h>
#include <string.h>

#define MAX(x, y) (x > y ? x : y)
typedef uint64_t u64;

typedef struct {
    size_t len;
    size_t cap;
    char buf[0];
} Buf_Hdr;

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (ptr == NULL) {
        perror("malloc failed.\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (ptr == NULL) {
        perror("malloc failed.\n");
        exit(1);
    }
    return ptr;
}

#define buf__hdr(b) (b ? (Buf_Hdr *)((char *)b - offsetof(Buf_Hdr, buf)) : 0)
#define buf__fit(b, x) (((buf_len(b) + x) <= buf_cap(b)) ? 0 : (b = buf__grow(b, buf_cap(b)+x, sizeof(*b))))

#define buf_len(b) (b ? buf__hdr(b)->len : 0)
#define buf_cap(b) (b ? buf__hdr(b)->cap : 0)

#define buf_push(b, x) (buf__fit(b, 1), b[buf__hdr(b)->len++] = x)
#define buf_free(b) (b ? free(buf__hdr(b)), b = NULL : 0)

void *buf__grow(void *buf, size_t min_cap, size_t elem_size) {
    // Try to always double the capacity of the buffer. But when
    // required more than the double in "min_cap", allocate space for min_cap.
    int new_cap = MAX(buf_cap(buf)*2 + 1, min_cap);
    size_t new_size = offsetof(Buf_Hdr, buf) + (new_cap * elem_size);

    Buf_Hdr *new_buf;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->buf;
}


typedef struct {
    size_t len;
    const char *str;
} Intern_Str;

Intern_Str *interns;

const char *str_intern_range(const char *start, const char *end) {
    size_t len = end - start;
    for (int i = 0; i < buf_len(interns); i++) {
        if (interns[i].len == len && !strncmp(interns[i].str, start, len)) {
            return interns[i].str;
        }
    }

    char *str = xmalloc(len + 1);
    memcpy(str, start, len);
    str[len] = 0;

    buf_push(interns, ((Intern_Str){len, str}));

    return str;
}

const char *str_intern(const char *str) {
    return str_intern_range(str, str+strlen(str));
}

typedef enum {
    TOKEN_INT = 128,
    TOKEN_NAME,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    union {
        u64 value;
        const char *str;
    };
} Token;

Token token;
const char *stream;

void next_token() {
    token.start = stream;
    switch(*stream) {
    case 'a' ... 'z':
    case 'A' ... 'Z': {
        stream++;
        while (isalpha(*stream) || *stream == '_') {
            stream++;
        }
        token.type = TOKEN_NAME;
        token.str = str_intern_range(token.start, stream);
    } break;
    case '0' ... '9': {
        stream++;
        u64 value = 0;
        while (isdigit(*stream)) {
            value *= 10;
            value += *stream++ - '0';
        }
        token.type = TOKEN_INT;
        token.value = value;
    } break;
    default:
        token.type = *stream++;
        break;
    }
    token.end = stream;
}

void print_token(Token token) {
    switch (token.type) {
    case TOKEN_INT: printf("TOKEN_INT, value: %d", token.value); break;
    case TOKEN_NAME: printf("TOKEN_NAME, value: %s", token.str); break;
    default: printf("OTHER"); break;
    }
    // Print lexeme
    printf(", lexeme: \"%.*s\"\n", (token.end - token.start), token.start, token.type);
}

// Test lexer
void lex_test() {
    stream = "abc123+34"; 
    next_token();
    while (token.type) {
        print_token(token);
        next_token();
    }
}

// Stretchy buffers test
void buf_test() {
    enum { N = 1024 };
    int *buf = NULL;
    for (int i = 0 ; i < 1024; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == N);

    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }

    buf_free(buf);
    assert(buf == NULL);
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);
}

void str_intern_test() {
    const char *x = str_intern("hello");
    const char *y = str_intern("hello");
    assert(x == y);

    const char *z = str_intern("hello!");
    assert(z != x);
}

int main() {
    // Run tests
    buf_test();
    lex_test();
    str_intern_test();
}
