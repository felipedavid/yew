#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>

typedef uint64_t u64;

void* xrealloc(void* ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        perror("[!] xrealloc failed!");
        exit(1);
    }
    return ptr;
}

void* xmalloc(size_t n_bytes) {
    void* ptr = malloc(n_bytes);
    if (!ptr) {
        perror("[!] xmalloc failed!");
        exit(1);
    }
    return ptr;
}

#define MAX(x, y) ((x > y) ? x : y)

typedef struct {
    size_t len;
    size_t cap;
    char val[0];
} Buf_Hdr;

#define buf__hdr(b) ((b) ? (Buf_Hdr *)((char*)b - offsetof(Buf_Hdr, val)) : NULL)
#define buf__fit(b, n) (((buf_len(b) + n) <= buf_cap(b)) ? 0 : (b = buf__grow(b, buf_len(b)+n, sizeof(*b))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)

#define buf_push(b, v) (buf__fit(b, 1), b[buf__hdr(b)->len++] = v)

void* buf__grow(void* buf, size_t min_new_cap, size_t elem_size) {
    size_t new_cap = MAX((buf_cap(buf) + 1) * 2, min_new_cap);
    size_t new_size = offsetof(Buf_Hdr, val) + new_cap * elem_size;

    Buf_Hdr* new_buf;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    }
    else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->val;
}

void buf_test() {
    int* buf = NULL;
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);

    enum { N = 1024 };
    for (int i = 0; i < N; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == N);

    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }
}

typedef struct {
    const char *str;
    size_t len;
} Intern_Str;

Intern_Str *interned_strings;

const char *intern_str_range(const char *start, const char *end) {
    size_t len = end - start;
    for (int i = 0; i < buf_len(interned_strings); i++) {
        if ((len == interned_strings[i].len) && !strncmp(start, interned_strings[i].str, len)) {
            return interned_strings[i].str;
        }
    }

    char *str = xmalloc(len + 1);
    strncpy_s(str, len+1, start, len);
    str[len] = 0;

    buf_push(interned_strings, ((Intern_Str){ str, len }) );

    return str;
}

const char *intern_str(const char *str) {
    return intern_str_range(str, str + strlen(str));
}

void intern_str_test() {
    char x[] = "Hi there!";
    char y[] = "Hi there!";
    const char *px = intern_str(x);
    const char *py = intern_str(y);
    assert(px == py);

    char z[] = "Hi there?";
    const char *pz = intern_str(z);
    assert(px != pz);
}

const char *stream;

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
        const char *name;
    };
} Token;

Token token;

void next_token() {
    token.start = stream;
    switch (*stream++) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9': {
            u64 val = 0;
            while (isdigit(*stream)) {
                val *= 10;
                val *= *stream++ - '0';
            }
            token.value = val;
            token.type = TOKEN_INT;
        } break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
        case 'g':
        case 'h':
        case 'i':
        case 'j':
        case 'k':
        case 'l':
        case 'm':
        case 'n':
        case 'o':
        case 'p':
        case 'q':
        case 'r':
        case 's':
        case 't':
        case 'u':
        case 'v':
        case 'w':
        case 'x':
        case 'y':
        case 'z':
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'H':
        case 'I':
        case 'J':
        case 'K':
        case 'L':
        case 'M':
        case 'N':
        case 'O':
        case 'P':
        case 'Q':
        case 'R':
        case 'S':
        case 'T':
        case 'U':
        case 'V':
        case 'W':
        case 'X':
        case 'Y':
        case 'Z': {
            while (isalpha(*stream) || *stream == '_') {
                stream++;
            }
            token.name = intern_str_range(token.start, stream);
            token.type = TOKEN_NAME;
        } break;
        default: {
            token.type = *(stream - 1);
        } break;
    }
    token.end = stream;
}

void lex_test() {
    stream = "A+word_hh!2222";
    next_token();
    while (token.type) {
        printf("Token: %d\n", token.type);
        next_token();
    }
}

int main(int argc, char** argv) {
    buf_test();
    intern_str_test();
    lex_test();
    return 0;
}
