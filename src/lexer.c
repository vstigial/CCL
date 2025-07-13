#include "lexer.h"

#include "cstd/string.h"
#include "cstd/stdbool.h"
#include "cstd/ctype.h"

char *text;
int line;
int column;
int idx;
Token *TokenArray;

char currentChar() {
    return (idx < strlen(text)) ? text[idx] : '\0';
}

void advance() {
    if (currentChar() == '\n') {
        line++;
        column = 0;
    } else {
        column++;
    }

    idx++;
}

void skipWhitespace() {
    while (isspace(currentChar())) {
        advance();
    }
}

Token singleCharToken(TokenType type, char *value) {
    int lineS = line, columnS = column;
    advance();
    return (Token){type, value, lineS, columnS};
}

Token getNextToken() {
    while (true) {
        skipWhitespace();

        if (!isspace(currentChar())) {
            break;
        }
    }

    if (idx > strlen(text)) {
        return (Token){TokenType_EndOfFile, "", line, column};
    }

    char character = currentChar();

    if (isalpha(character)) {
        // todo
    }

    if (isdigit(character)) {
        // todo
    }

    if (character == '"') {
        // todo
    }

    if (character == '+')
        return singleCharToken(TokenType_Plus, "+");
    if (character == '-')
        return singleCharToken(TokenType_Minus, "-");
    if (character == '/')
        return singleCharToken(TokenType_Slash, "/");
    if (character == '*')
        return singleCharToken(TokenType_Star, "*");
    if (character == ':')
        return singleCharToken(TokenType_Colon, ":");
    if (character == '=')
        return singleCharToken(TokenType_Equal, "=");
    if (character == '(')
        return singleCharToken(TokenType_LParen, "(");
    if (character == ')')
        return singleCharToken(TokenType_RParen, ")");
    if (character == '{')
        return singleCharToken(TokenType_LBrace, "{");
    if (character == '}')
        return singleCharToken(TokenType_RBrace, "}");

}

Token* tokenize(char* input) {
    text   = input;
    idx    = 0;
    line   = 1;
    column = 1;

}