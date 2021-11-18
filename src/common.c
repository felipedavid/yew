#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

void *buf__grow(void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = MAX(buf_cap(buf) * 2 + 1, new_len);
    assert(new_len <= new_cap);
    size_t new_size = offsetof(BufHdr, buf) + (new_cap * elem_size);

    BufHdr *new_buf;
    if (buf) {
        new_buf = xrealloc(buf__hdr(buf), new_size);
    } else {
        new_buf = xmalloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->buf;
}

void buf_test() {
    int *buf = NULL;
    enum { N = 1024 };
    for (int i = 0; i < N; i++) {
        buf_push(buf, i);
    }

    assert(buf_len(buf) == N);
    for (int i = 0; i < N; i++) {
        assert(buf[i] == i);
    }
    buf_free(buf);
    
    assert(buf_cap(buf) == 0);
    assert(buf_len(buf) == 0);
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        perror("xrealloc failed");
        exit(1);
    }
    return ptr;
}

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        perror("xmalloc failed");
        exit(1);
    }
    return ptr;
}

typedef struct {
    size_t len;
    const char *str;
} Intern_Str;

static Intern_Str *interns;

const char *str_intern_range(const char *start, const char *end) {
    size_t len = end - start;
    for (size_t i = 0; i < buf_len(interns); i++) {
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
    return str_intern_range(str, str + strlen(str));
}

void str_intern_test() {
    char x[] = "hello";
    char y[] = "hello";
    assert(x != y);
    const char *px = str_intern(x);
    const char *py = str_intern(y);
    assert(px == py);
    char z[] = "hello!";
    assert(str_intern(z) != str_intern(x));
}
