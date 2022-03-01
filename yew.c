#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <string.h>

typedef uint64_t u64;

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        fprintf(stderr, "xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        fprintf(stderr, "xrealloc failed\n");
        exit(1);
    }
    return ptr;
}

typedef struct {
    size_t len;
    size_t cap;
    char val[0];
} Buf_Hdr;

#define MAX(x, y) ((x > y) ? x : y)

#define buf__hdr(b) (b ? (Buf_Hdr *)((char *)b - offsetof(Buf_Hdr, val)) : NULL)
#define buf__fits(b, n) ((buf_len(b) + n) <= buf_cap(b))
#define buf__fit(b, n) (buf__fits(b, n) ? 0 : (b = buf__grow(b, buf_len(b) + n, sizeof(*b))))

#define buf_len(b) (b ? buf__hdr(b)->len : 0)
#define buf_cap(b) (b ? buf__hdr(b)->cap : 0)

#define buf_push(b, x) (buf__fit(b, 1), b[buf__hdr(b)->len++] = x)
#define buf_free(b) (b ? free(buf__hdr(b)), b = NULL : 0)

void *buf__grow(void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = MAX(new_len, 2*buf_cap(buf)+1); 
    assert(new_len <= new_cap);
    size_t new_size = offsetof(Buf_Hdr, val) + (new_cap * elem_size);

    Buf_Hdr *new_buf = NULL;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->val;
}

void buf_test() {
    int *buf = NULL;
    assert(buf_len(buf) == 0);

    enum { N = 1024 };
    for (int i = 0; i < N; i++) {
        buf_push(buf, i);
    }
    assert(buf_len(buf) == N);
    
    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }

    buf_free(buf);
    assert(buf_len(buf) == 0);
    assert(buf_cap(buf) == 0);
    assert(buf == NULL);
}

typedef struct {
    const char *str;
    size_t len;
} Intern_Str;

static Intern_Str *interns;

// TODO: Reimplement it with a hash table
const char *str_intern_range(const char *start, const char *end) {
    size_t len = end - start;
    for (int i = 0; i < buf_len(interns); i++) {
        const char *str = interns[i].str;
        if (len == interns[i].len && !strncmp(start, str, len)) {
            return str;
        }
    }

    char *new_str = xmalloc(len + 1);
    memcpy(new_str, start, len);
    new_str[len] = 0;

    buf_push(interns, ((Intern_Str){new_str, len}));
    return new_str;
}

const char *str_intern(const char *str) {
    return str_intern_range(str, str + strlen(str));
}

void str_intern_test() {
    char x[] = "Hi there";
    char y[] = "Hi there";
    const char *px = str_intern(x); 
    const char *py = str_intern(y); 
    assert(px == py);

    char z[] = "Hi there.";
    const char *pz = str_intern(z);
    assert(pz != px);
}

typedef enum {
    TP_INTEGER = 128,
    TP_NAME,
} Token_Type;

typedef struct {
    Token_Type type;
    const char *start;
    const char *end;
    union {
        u64 val;
        const char *val_str;
    };
} Token;

Token token;
const char *source;

void print_token() {
    switch (token.type) {
    case TP_INTEGER:
        printf("TOKEN INT: %lu\n", token.val);
        break;
    case TP_NAME:
        printf("TOKEN NAME: %s\n", token.val_str);
        break;
    default:
        printf("TOKEN: '%c'\n", token.type);
    }
}

void next_token() {
    switch (*source) {
    case '0' ... '9': {
        u64 val = 0;
        while (isdigit(*source)) {
            val *= 10;
            val += *source - '0';
            source++;
        }
        token.type = TP_INTEGER;
        token.val = val;
    } break;
    case 'a' ... 'z':
    case 'A' ... 'Z': 
    case '_': {
        token.start = source;
        while (isalnum(*source) || *source == '_') {
            source++;
        }
        token.end = source;
        token.type = TP_NAME;
        token.val_str = str_intern_range(token.start, token.end);
    } break;
    default:
        token.type = *source++;
    }
}

void lex_test() {
    source = "XY+(XY)_HELLO1,234+FOO!994";
    next_token();
    while (token.type) {
        print_token();
        next_token();
    }
}

int main(int argc, char **argv) {
    buf_test();
    str_intern_test();
    lex_test();
    return 0;
}
