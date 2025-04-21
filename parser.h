#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include "lexer.h"

typedef struct Node {
    char *type;
    Token *token;
  
    struct Node **children;
    int child_count;    
    int child_capacity; 
  } Node;

Node *parse_tokens(Token *head);
void print_tree(Node *node, int indent);
void free_parse_tree(Node *node);


#endif