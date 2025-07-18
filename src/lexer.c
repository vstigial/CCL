#include "lexer.h"

#include "cstd/string.h"
#include "cstd/stdbool.h"
#include "cstd/ctype.h"

char *text;
int line;
int column;
int idx;

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

Token singleCharToken(Token_Type type, char *value) {
    int lineS = line, columnS = column;
    advance();
    return (Token){type, value, lineS, columnS};
}

Token handleNumber() {
    int lineS = line, columnS = column;
    size_t start = idx;
    while (isdigit(currentChar())) {
        advance();
    }

    size_t length = idx - start;
    char *value = substr(text, start, length); // TODO: Remember to free these mallocs when they are finished!

    return (Token){TokenType_Number, value, lineS, columnS};
}

Token handleIdentifier() {
    int lineS = line, columnS = column;
    size_t start = idx;

    while (isalnum(currentChar()) || currentChar() == '_')
        advance();

    size_t length = idx - start;
    char *value = substr(text, start, length);

    if (streq(value, "fn"))
        return (Token){TokenType_Fn, value, lineS, columnS};
    if (streq(value, "return"))
        return (Token){TokenType_Return, value, lineS, columnS};
    if (streq(value, "let"))
        return (Token){TokenType_Let, value, lineS, columnS};
    if (streq(value, "if"))
        return (Token){TokenType_If, value, lineS, columnS};

    return (Token){TokenType_Identifier, value, lineS, columnS};
}

Token handleStringLit() {
    int lineS = line, columnS = column;
    advance(); // Skip opening '"'

    size_t start = idx;

    while (currentChar() != '"' && currentChar() != '\0')
        advance();

    size_t length = idx - start;
    char *value = substr(text, start, length);

    if (currentChar() == '"')
        advance();

    return (Token){TokenType_String, value, lineS, columnS};
}

Token getNextToken() {
    while (true) {
        skipWhitespace();

        if (!isspace(currentChar())) {
            break;
        }
    }

    if (idx >= strlen(text)) {
        return (Token){TokenType_EndOfFile, "", line, column};
    }

    char character = currentChar();

    if (isalpha(character)) {
        return handleIdentifier();
    }

    if (isdigit(character)) {
        return handleNumber();
    }

    if (character == '"') {
        return handleStringLit();
    }

    if (character == '+')
        return singleCharToken(TokenType_Plus, "+");
    if (character == '-')
        return singleCharToken(TokenType_Minus, "-");
    if (character == '/')
        return singleCharToken(TokenType_Slash, "/");
    if (character == '*')
        return singleCharToken(TokenType_Star, "*");
    if (character == ',')
        return singleCharToken(TokenType_Comma, ",");
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

    size_t mallocSize = 128;
    Token *TokenArray = malloc(mallocSize * sizeof(Token));
    size_t TokenIdx   = 0;

    while (true) {
        if (TokenIdx == mallocSize) {
            mallocSize *= 2;
            TokenArray = realloc(TokenArray, mallocSize * sizeof(Token));
        }

        Token token = getNextToken();
        if (token.type == TokenType_EndOfFile)
            break;

        TokenArray[TokenIdx++] = token;
    }

    TokenArray[TokenIdx] = (Token){TokenType_EndOfFile, "", line, column};
    return TokenArray;
}