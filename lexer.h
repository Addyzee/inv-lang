#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>


typedef enum {
    INT,
    FLOAT,
    KEYWORD,
    SEPARATOR,
    OPERATOR,
    IDENTIFIER,
    STRING,
    COMP,
    END_OF_TOKENS,
  } TokenType;
  
  typedef struct Token {
    TokenType type;
    char *value;
    struct Token *next;
  } Token;

Token *add_token(TokenType type, Token *head, char *start, int length);
char *read_file_to_string(FILE *file);
const char *token_type_to_string(TokenType type);
void print_tokens(Token *tokens);
void free_tokens(Token *head);
void lexer(char *file_contents, Token **tokens);


#endif