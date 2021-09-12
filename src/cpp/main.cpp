#include <stdio.h>

void runFile(char *file_name) {
    auto 
}

int main(char argc, char **argv) {
    if (argc > 2) {
        printf("Usage: tsundere [script]");
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }
}
