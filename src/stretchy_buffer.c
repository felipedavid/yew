#include <stdlib.h>
#include <assert.h>

#include "stretchy_buffer.h"

void *buf__grow(void *buf, size_t new_len, size_t elem_size) {
    size_t new_cap = MAX(buf_cap(buf) * 2 + 1, new_len);
    assert(new_len <= new_cap);
    size_t new_size = offsetof(BufHdr, buf) + (new_cap * elem_size);

    BufHdr *new_buf;
    if (buf) {
        new_buf = realloc(buf__hdr(buf), new_size);
    } else {
        new_buf = malloc(new_size);
        new_buf->len = 0;
    }
    new_buf->cap = new_cap;

    return new_buf->buf;
}
