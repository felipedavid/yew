#include <stddef.h>

typedef struct {
    size_t len;
    size_t cap;
    char buf[0];
} Buf_Hdr;

#define MAX(x, y) ((x > y) ? x : y)

#define buf__hdr(b) (b ? (Buf_Hdr *) ((char *)b - offsetof(Buf_Hdr, buf)) : NULL)
#define buf__fit(b, n) (((buf_len(b) + n) <= buf_cap(b)) ? 0 : (b = buf__grow(b, buf_len(b)+n, sizeof(*b))))

#define buf_len(b) (b ? buf__hdr(b)->len : 0)
#define buf_cap(b) (b ? buf__hdr(b)->cap : 0)

#define buf_push(b, x) (buf__fit(b, 1), b[buf__hdr(b)->len++] = x)
#define buf_free(b) (b ? free(buf__hdr(b)), b = NULL : 0)

void buf_test();
void str_intern_test();
const char *str_intern_range(const char *start, const char *end);
void fatal(const char *fmt, ...);
