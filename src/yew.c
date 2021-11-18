#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "scanner.h"

int main(int argc, char **argv) {
    buf_test();
    scanner_test();
    str_intern_test();

    if (argc < 2) {
        printf("Usage: ./yew program.yew\n");
        exit(-1);
    }

    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        printf("[!] Error trying to open file '%s'\n", argv[1]);
        exit(-1);
    }

    // Load source code into memory
    char buffer[2048];
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    fread(buffer, 1, fsize, f);
    buffer[fsize] = '\0';

    printf("%s", buffer);

    fclose(f);
}
