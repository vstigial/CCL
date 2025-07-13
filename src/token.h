#pragma once

typedef enum {
  TokenType_Fn,         // "fn"
  TokenType_Return,     // "return"
  TokenType_Let,        // "let"
  TokenType_Identifier, // variable and function names, e.g., "std", "io", "main"
  TokenType_Number,     // numeric literals, e.g., "0"
  TokenType_String,     // string literals
  TokenType_Colon,      // ":"
  TokenType_LParen,     // "("
  TokenType_RParen,     // ")"
  TokenType_LBrace,     // "{"
  TokenType_RBrace,     // "}"
  TokenType_EndOfFile,  // end of input
  TokenType_Unknown,    // any unknown token

  TokenType_If,         // "if"
  TokenType_Equal,      // '='

  TokenType_Plus,       // '+'
  TokenType_Minus,      // '-'
  TokenType_Star,       // '*'
  TokenType_Slash,      // '/'
} Token_Type; // "TokenType" definition conflicts with winnt.h header

typedef struct {
  Token_Type type;
  char* value;
  int line;
  int col;
} Token;