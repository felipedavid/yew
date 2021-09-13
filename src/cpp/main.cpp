#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

void run(std::string source) {
    Scanner scanner(source);
    vector<Token> tokens = scanner.scan_tokens();

    for (auto token : tokens) {
        std::cout << token.lexeme << std::endl;
    }
}

void run_file(char *file_name) {
    std::stringstream source;
    std::ifstream f;

    f.open(file_name);
    source << f.rdbuf();
        
    run(source.str());
}

void run_prompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        std::cin >> line;
        run(line);
    }
}

int main(char argc, char **argv) {
    if (argc > 2) {
        std::cout << "Usage: tsundere [script]" << std::endl;
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
}
