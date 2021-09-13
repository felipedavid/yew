package tsundere;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static tsundere.TokenType.*;

class Scanner {
    private final String source;
    private final List<Token> tokens = new ArrayList<>();
    private int start = 0; // points to the first character of the lexeme
    private int current = 0; // points to the current character of the lexeme
    private int line = 1;

    Scanner(String source) {
        this.source = source;
    }

    List<Token> scanTokens() {
        while (current < source.length()) {
            // We are at the beginning of the next lexeme.
            start = current;
            scanToken();
        }

        tokens.add(new Token(EOF, "", null, line));
        return tokens;
    }

    private void scanToken() {
        char c = source.charAt(current++);
        switch (c) {
            case '(': addToken(LEFT_PAREN,  null); break;
            case ')': addToken(RIGHT_PAREN, null); break;
            case '{': addToken(LEFT_BRACE,  null); break;
            case '}': addToken(RIGHT_BRACE, null); break;
            case ',': addToken(COMMA,       null); break;
            case '.': addToken(DOT,         null); break;
            case '-': addToken(MINUS,       null); break;
            case '+': addToken(PLUS,        null); break;
            case ';': addToken(SEMICOLON,   null); break;
            case '*': addToken(STAR,        null); break;
            case '!': addToken(match('=') ? BANG_EQUAL    : BANG,    null); break;
            case '=': addToken(match('=') ? EQUAL_EQUAL   : EQUAL,   null); break;
            case '<': addToken(match('=') ? LESS_EQUAL    : LESS,    null); break;
            case '>': addToken(match('=') ? GREATER_EQUAL : GREATER, null); break;
            case '/':
                if (match('/')) {
                    // keep going until the end of the line
                    while (peek() != '\n' && current < source.length) current++;
                } else {
                    addToken(SLASH, null);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
              // Ignore whitespace.
              break;
            case '\n': line++;  break;
            case '"': 
                while (peek() != '"' && current < source.length) {
                    if (peek() == '\n') line++;
                    current++;
                }

                if (current >= source.length) {
                    Lox.Error("Unterminated string");
                    return;
                }

                // The closing ".
                current++;

                String value = source.substring(start + 1, current - 1);
                addToken(STRING, value);
                break;
            default:
                if (isDigit(c)) {
                    while (isDigit(peek())) current++;

                    // Look for a fractional part
                    if (peek() == '.' && isDigit(peekNext())) {
                        current++;
                        while (isDigit(peek())) current++;
                    }
                    addToken(TOKEN, Double.parseDouble(source.substring(start, current)));
                } else {
                    Lox.error(line, "Unexpected character.");
                }
                break;
        }
    }

    private boolean isDigit(char c) {
        return c >= '0' && c <= '9';
    }

    private boolean match(char expected) {
        if (current >= source.length) return false;
        if (source.charAt(current) != expected) return false;
        current++;
        return true;
    }

    private char peek() {
        if (current >= source.length) return '\0';
        return source.charAt(current);
    }

    private void addToken(TokenType type, Object literal) {
        String lexeme = source.substring(start, current);
        tokens.add(new Token(type, lexeme, literal, line));
    }
}
