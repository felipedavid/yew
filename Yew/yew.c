#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>

void *xrealloc(void *ptr, size_t n_bytes) {
    ptr = realloc(ptr, n_bytes);
    if (!ptr) {
        perror("[!] xrealloc failed!");
        exit(1);
    }
    return ptr;
}

void *xmalloc(size_t n_bytes) {
    void *ptr = malloc(n_bytes);
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

void *buf__grow(void *buf, size_t min_new_cap, size_t elem_size) {
    size_t new_cap = MAX((buf_cap(buf)+1) * 2, min_new_cap);
    size_t new_size = offsetof(Buf_Hdr, val) + new_cap * elem_size;

    Buf_Hdr *new_buf;
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

int main(int argc, char **argv) {
    buf_test();
    return 0;
}
