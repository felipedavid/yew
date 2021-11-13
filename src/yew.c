#include <stdio.h>
#include <stdlib.h>

#include "stretchy_buffer.h"

int main(int argc, char **argv) {
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

    printf("%s\n", buffer);
    
    // Scan tokens
    // Token *tokens = scan(buffer);

    fclose(f);
}
