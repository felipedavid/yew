#ifndef STRETCHY_BUFFER_H
#define STRETCHY_BUFFER_H

#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    char buf[0];
} BufHdr;

#define MAX(x, y) (x >= y ? x : y)

#define buf__hdr(b) ((b) ? (BufHdr *)((char *)b - offsetof(BufHdr, buf)) : 0)
#define buf__fits(b, n) ((buf_len(b) + n) <= buf_cap(b))
#define buf__fit(b, n) (buf__fits(b, n) ? 0 : (b = buf__grow(b, buf_cap(b)+n, sizeof(*b))))

#define buf_len(b) ((b) ? buf__hdr(b)->len : 0)
#define buf_cap(b) ((b) ? buf__hdr(b)->cap : 0)

#define buf_push(b, x) (buf__fit(b, 1), b[buf_len(b)] = x, buf__hdr(b)->len++)

void *buf__grow(void *buf, size_t new_len, size_t elem_size);

#endif
