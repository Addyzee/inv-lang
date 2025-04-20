#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

typedef struct Node {
  char *type; // TO DO: use type for printing node
  Token *token;
  struct Node *left;
  struct Node *right;
} Node;

Node *init_node(Node *root, char *value) {
  Node *new_node = malloc(sizeof(Node));
  new_node->token = malloc(sizeof(Token));
  new_node->token->value = value;
  new_node->left = NULL;
  new_node->right = NULL;
  if (root == NULL) {
    return new_node;
  } else if (root->left == NULL) {
    root->left = new_node;
  } else if (root->right == NULL) {
    root->right = new_node;
  } else {
    init_node(root->left, value);
  }
  return root;
}

void print_indent(int level) {
  for (int i = 0; i < level; i++) {
    printf("  ");
  }
}

void print_tree(Node *root, int indent_level) {
  if (!root)
    return;

  print_indent(indent_level);
  printf("%s\n", root->token->value);

  if (root->left) {
    print_indent(indent_level);
    print_tree(root->left, indent_level + 1);
  }

  if (root->right) {
    print_indent(indent_level);
    print_tree(root->right, indent_level + 1);
  }
}

int operator_parsing(Token *token) {
  // check if the token is an operator
  switch (token->type) {
  case ADDITION:
    return 1;
  case SUBTRACTION:
    return 1;
  case MULTIPLICATION:
    return 1;
  case DIVISION:
    return 1;
  case EXPONENT:
    return 1;
  case INTEGER_DIVISION:
    return 1;
  case MODULUS:
    return 1;
  case INCREMENT:
    return 1;
  case DECREMENT:
    return 1;
  case OPERATOR:
    return 0;
  default:
    return 0;
  }
}

void parse_program() {}

void parse_statement() {}

void parse_declaration() {}

void parse_assignment() {}

Node *parse_factor(Token **current) {
  Node *factor = NULL;
  if (((*current)->type == INT) || ((*current)->type == FLOAT)) {
    factor = init_node(factor, (*current)->value);
    *current = (*current)->next;
    printf("current %s\n", (*current)->value);
  }
  if (factor == NULL) {
    printf(
        "ERROR PARSING FACTOR. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED FLOAT OR "
        "INT\n",
        token_type_to_string((*current)->type), (*current)->value);
  }
  return factor;
}

Node *parse_term(Token **current) {
  Node *term = NULL;

  Node *factor = parse_factor(current);
  if (factor == NULL) {
    printf("NULL FACTOR AT TERM\n");
    return NULL;
  }
  
  if((*current)->type == MULTIPLICATION || 
     (*current)->type == DIVISION ||
     (*current)->type == EXPONENT ||
     (*current)->type == INTEGER_DIVISION ||
     (*current)->type == MODULUS) {
    
    term = init_node(term, (*current)->value);

    term->left = factor;

    *current = (*current)->next;

    Node *next_term = parse_term(current);

    if (next_term == NULL) {
      printf("NULL TERM\n");
      return NULL;
    }
    term->right = next_term;
    print_tree(term, 0);
  } else{
    term = factor;
  } 


  return term;
}

Node *parse_expression(Token **current) {
  Node *expression = NULL;
  Node *term = parse_term(current);

  if (term == NULL) {
    return NULL;
  }

  // check if next token is an operator
  if (!operator_parsing((*current))) {
    // if not an operator, return the term
    expression = term;
  } else if (operator_parsing((*current))) {
    // create the parent node to be the operator eg +
    expression = init_node(expression, (*current)->value);
    expression->left = term;
    

    if (((*current)->next->type == INT) || ((*current)->next->type == FLOAT) ||
        ((*current)->next->type == IDENTIFIER)) {
      *current = (*current)->next;
      expression->right = parse_expression(current);

    } else {
      printf("ERROR PARSING EXPRESSION. UNEXPECTED TOKEN TYPE: %s %s \n",
             token_type_to_string((*current)->next->type),
             (*current)->next->value);
      return NULL;
    }
  }

  return expression;
}

Node *parse_print(Token **current) {
  if (((*current)->type == KEYWORD) &&
      (strcmp((*current)->value, "print") != 0)) {
    printf("Not a print statement");
    return NULL;
  }

  Node *print = NULL;
  print = init_node(print, (*current)->value);

  *current = (*current)->next;
  if ((*current)->type != SEPARATOR) {
    printf("EXPECTED SEPARATOR ( AFTER PRINT. FOUND %s %s\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  *current = (*current)->next;
  Node *expression = parse_expression(current);
  print_tree(expression, 0);
  if (expression == NULL) {
    printf("NULL EXPRESSION AT PRINT\n");
  }
  print->left = expression;
  printf("current %s\n", (*current)->value);

  // check correct syntax for print ending: );
  if (((*current)->type != SEPARATOR) ||
      (strcmp((*current)->value, ")") != 0)) {
    printf("EXPECTED SEPARATOR ) AFTER PRINT. FOUND %s %s\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  *current = (*current)->next;
  if ((*current)->type != SEPARATOR || strcmp((*current)->value, ";") != 0) {
    printf("EXPECTED SEPARATOR ; AFTER PRINT. FOUND %s %s\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }

  return print;
}

void parse_type() {}

void parse_tokens(Token *head) {
  Node *tree = parse_print(&head);
  print_tree(tree, 0);
}
