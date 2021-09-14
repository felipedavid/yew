#include <iostream>
#include <string>
#include <fstream>

void run(std::string source) {
    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scan_tokens();

    for (auto token : tokens) {
        std::cout << token.lexeme << std::endl;
    }
}

void run_file(char *file_name) {
    std::ifstream f(file_name);
    std::string source((std::istreambuf_iterator<char>(f)),
                       (std::istreambuf_iterator<char>()));
    run(source);
}

void run_prompt() {
    std::string line;
    for (;;) {
        std::cout << "> ";
        std::getline(std::cin, line);
        if (line == "quit") break; 
        run(line);
    }
}

int main(int argc, char **argv) {
    if (argc > 2) {
        std::cout << "Usage: fei [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        run_file(argv[1]);
    } else {
        run_prompt();
    }
	return 0;
}
