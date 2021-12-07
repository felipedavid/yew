#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

#include "common.h"

void fatal(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    printf("FATAL: ");
    vprintf(fmt, args);
    printf("\n");
    va_end(args);
    exit(1);
}

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
    if (!ptr) {
        perror("[!] xmalloc failed\n");
        exit(1);
    }
    return ptr;
}

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        perror("[!] xrealloc failed\n");
        exit(1);
    }
    return ptr;
}

void *buf__grow(void *buf, size_t min_cap, size_t elem_size) {
    size_t new_cap = MAX(min_cap, buf_len(buf) * 2 + 1);
    size_t new_size = offsetof(Buf_Hdr, buf) + (new_cap * elem_size);

    Buf_Hdr *new_buf = NULL;
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
    size_t len;
    const char *str;
} Intern_Str;

Intern_Str *interns;

const char *str_intern_range(const char *start, const char *end) {
    size_t len = end - start;
    for (int i = 0; i < buf_len(interns); i++) {
        if (interns[i].len == len && !strncmp(start, interns[i].str, len)) {
            return interns[i].str;
        }
    }
    char *str = xmalloc(len + 1);
    strncpy(str, start, len);
    str[len] = 0;

    buf_push(interns, ((Intern_Str){len, str}));
    return str;
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

    char z[] = "Hi there!";
    const char *pz = str_intern(z);
    assert(pz != px);
}
