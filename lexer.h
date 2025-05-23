#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>


typedef enum {
  TYPE,
  INT,
  FLOAT,
  KEYWORD,
  SEPARATOR,
  ASSIGNMENT_OPERATOR,
  ADDITION,
  SUBTRACTION,
  MULTIPLICATION,
  DIVISION,
  INCREMENT,
  DECREMENT,
  EXPONENT,
  INTEGER_DIVISION,
  MODULUS,
  OPERATOR,
  IDENTIFIER,
  STRING,
  START,
  END_OF_TOKENS,
} TokenType;
  
  typedef struct Token {
    TokenType type;
    char *value;
    int line_number;
    struct Token *next;
  } Token;

Token *add_token(TokenType type, Token *head, char *start, int length, int line_number);
char *read_file_to_string(FILE *file);
const char *token_type_to_string(TokenType type);
void print_tokens(Token *tokens);
void free_tokens(Token *head);
Token *lexer(char *file_contents, Token **tokens);


#endif