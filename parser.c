#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"


typedef struct Node {
  char *type;
  Token *token;

  struct Node **children;
  int child_count;    // current number of children
  int child_capacity; // max capacity 
} Node;

Node *parse_expression(Token **current);


Node *create_node(char *type, Token *token) {
  Node *node = malloc(sizeof(Node));
  node->type = strdup(type);
  node->token = token;

  node->child_capacity = 3;
  node->child_count = 0;
  node->children = malloc(sizeof(Node *) * node->child_capacity);

  return node;
}

void add_child(Node *parent, Node *child) {
  if (parent->child_count == parent->child_capacity) {
    parent->child_capacity *= 2;
    parent->children =
        realloc(parent->children, sizeof(Node *) * parent->child_capacity);
  }
  parent->children[parent->child_count] = child;
  parent->child_count++;
}

void print_tree(Node *node, int indent) {
  for (int i = 0; i < indent; i++)
    printf("  ");
  printf("%s", node->type);
  if (node->token) {
    printf(" (%s)", node->token->value);
  }
  printf("\n");

  for (int i = 0; i < node->child_count; i++) {
    print_tree(node->children[i], indent + 1);
  }
}

void free_parse_tree(Node *node) {
  if (node == NULL) {
    return;
  }
  
  if (node->token) {
    free(node->token->value);
    node->token->value = NULL;

    free(node->token);
    node->token = NULL;

  }
  free(node->type);
  node->type = NULL;
  

  for (int i = 0; i < node->child_count; i++) {
    free_parse_tree(node->children[i]);
    node->children[i] = NULL;
  }
  free(node->children);
  node->children = NULL;

  free(node);

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

Node *parse_declaration(Token **current) {
  // eg int name = 0;

  if ((*current)->type != TYPE) {
    printf("ERROR PARSING DECLARATION. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "TYPE\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }

  Node *declaration = create_node("Declaration", *current);
  *current = (*current)->next;

  if ((*current)->type != IDENTIFIER) {
    printf("ERROR PARSING DECLARATION. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "IDENTIFIER\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }

  Node *identifier = create_node("Identifier", *current);
  add_child(declaration, identifier);
  *current = (*current)->next;

  if ((*current)->type != ASSIGNMENT_OPERATOR) {
    printf("ERROR PARSING DECLARATION. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "ASSIGNMENT OPERATOR\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  *current = (*current)->next;

  if ((*current)->type != INT && (*current)->type != FLOAT) {
    printf("ERROR PARSING DECLARATION. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "IDENTIFIER\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }

  Node *expression = parse_expression(current);
  add_child(declaration, expression);

  *current = (*current)->next;
  return declaration;
}

Node *parse_assignment(Token **current) {
  // eg name = 0;

  if ((*current)->type != IDENTIFIER) {
    printf("ERROR PARSING ASSIGNMENT. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "IDENTIFIER\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  Node *assignment = create_node("Identifier", *current);
  *current = (*current)->next;

  if ((*current)->type != ASSIGNMENT_OPERATOR) {
    printf("ERROR PARSING ASSIGNMENT. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "ASSIGNMENT OPERATOR\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  *current = (*current)->next;

  if ((*current)->type != INT && (*current)->type != FLOAT && (*current)->type != IDENTIFIER) {
    printf("ERROR PARSING ASSIGNMENT. UNEXPECTED TOKEN TYPE: %s %s \nEXPECTED "
           "LITERAL OR IDENTIFIER\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;

  }

  Node *expression = parse_expression(current);
  add_child(assignment, expression);

  *current = (*current)->next;

  return assignment;
}

Node *parse_crements(Token **current) {
  // eg id ++; or id --;
  if (((*current)->type == IDENTIFIER)) {
        Node *factor = create_node("Factor", *current);
        *current = (*current)->next;
        Node *crement = create_node("Crement", NULL);
        if ((*current)->type == INCREMENT ){
          crement = create_node("Increment", *current);
          add_child(crement, factor);
        } else{
          crement = create_node("Decrement", *current);
          add_child(crement, factor);
        }
    *current = (*current)->next;
    if ((*current)->type != SEPARATOR || strcmp((*current)->value, ";") != 0) {
      printf("EXPECTED SEPARATOR ; AFTER FACTOR. FOUND %s %s\n",
             token_type_to_string((*current)->type), (*current)->value);
      return NULL;
    }
    *current = (*current)->next;
    return crement;
  } 
  return NULL;
}

Node *parse_factor(Token **current) {
  // eg 0, 42

  if (((*current)->type == INT) || ((*current)->type == FLOAT) ||
      ((*current)->type == IDENTIFIER)) {
    Node *factor = create_node("Factor", *current);
    *current = (*current)->next;
    return factor;

  } else if (((*current)->type == SEPARATOR) && (strcmp((*current)->value,"(") == 0)) {
    *current = (*current)->next;
    Node *expression = parse_expression(current);
    if (expression == NULL) {
      printf("NULL EXPRESSION AT FACTOR\n");
      return NULL;
    }

    if ((*current)->type != SEPARATOR || strcmp((*current)->value, ")") != 0) {
      printf("EXPECTED SEPARATOR ) AFTER FACTOR. FOUND %s %s\n",
             token_type_to_string((*current)->type), (*current)->value);
      return NULL;
    }
    *current = (*current)->next;
    return expression;

  } else {
    printf("ERROR PARSING FACTOR. UNEXPECTED TOKEN TYPE: %s %s \n", token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
}

Node *parse_term(Token **current) {
  // eg 0 * 42, 42 / 0, 42 ^ 0, 42 // 0, 42 % 0

  Node *factor = parse_factor(current);

  if (factor == NULL) {
    printf("NULL FACTOR AT TERM\n");
    return NULL;
  }

  if ((*current)->type == MULTIPLICATION || (*current)->type == DIVISION ||
      (*current)->type == EXPONENT || (*current)->type == INTEGER_DIVISION ||
      (*current)->type == MODULUS) {

    Node *term = create_node("Term", *current);
    add_child(term, factor);

    *current = (*current)->next;


    Node *next_term = parse_term(current);

    if (next_term == NULL) {
      printf("NULL TERM\n");
      return NULL;
    }
    add_child(term, next_term);
    return term;
  } 
    else {
    // if not an operator, return the factor
    return factor;
  }
}

Node *parse_expression(Token **current) {
  // eg 42, 0 + 42

  Node *term = parse_term(current);


  if (term == NULL) {
    return NULL;
  }

  // check if next token is an operator
  if (operator_parsing((*current))) {
    // create the parent node to be the operator eg +
    Node *expression = create_node("Expression", *current);
    add_child(expression, term);

    *current = (*current)->next;

    if (((*current)->type == INT) || ((*current)->type == FLOAT) ||
        ((*current)->type == IDENTIFIER)) {
      add_child(expression, parse_expression(current));

      return expression;

    }
  
    else if (((*current)->type == SEPARATOR) && (strcmp((*current)->value, "(")==0)){
      Node *factor = parse_term(current);
      if (factor == NULL) {
        printf("NULL FACTOR AT EXPRESSION\n");
        return NULL;
      }
      add_child(expression, factor);
      return expression;
    }

    else {
      printf("ERROR PARSING EXPRESSION. UNEXPECTED TOKEN TYPE: %s %s \n",
             token_type_to_string((*current)->type), (*current)->value);
      return NULL;
    }
  }
  return term;
}

Node *parse_print(Token **current) {
  if (((*current)->type == KEYWORD) &&
      (strcmp((*current)->value, "print") != 0)) {
    printf("NOT A PRINT STATEMENT. UNEXPECTED TOKEN TYPE: %s %s \n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }

  Node *print = create_node("Print", *current);
  *current = (*current)->next;

  if (((*current)->type != SEPARATOR) && 
      (strcmp((*current)->value, "(") != 0)) {
    printf("EXPECTED SEPARATOR ( AFTER PRINT. FOUND %s %s\n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  *current = (*current)->next;

  Node *expression = parse_expression(current);
  if (expression == NULL) {
    printf("NULL EXPRESSION AT PRINT\n");
    return NULL;
  }
  add_child(print, expression);

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
  *current = (*current)->next;

  return print;
}

Node *parse_statement(Token **current) {
  // TO DO: account for separators after operators eg i++;(add else if)
  if (((*current)->type == KEYWORD) &&
      (strcmp((*current)->value, "print") == 0)) {
    Node *print = parse_print(current);
    if (print == NULL) {
      printf("NULL PRINT STATEMENT\n");
      return NULL;
    }

    return print;
  } else if (((*current)->type == TYPE) &&
             (((strcmp((*current)->value, "int") == 0)) ||
              ((strcmp((*current)->value, "float") == 0)))) {
    Node *declaration = parse_declaration(current);
    if (declaration == NULL) {
      printf("NULL DECLARATION\n");
      return NULL;
    }
    return declaration;
  } else if (((*current)->type) == IDENTIFIER) {
    if ((*current)->next->type == ASSIGNMENT_OPERATOR) {


    Node *assignment = parse_assignment(current);
    if (assignment == NULL) {
      printf("NULL ASSIGNMENT\n");
      return NULL;
    }
    return assignment;
  } else if((*current)->next->type == INCREMENT || (*current)->next->type == DECREMENT) {

    Node *crement = parse_crements(current);
    if (crement == NULL) {
      printf("NULL INCREMENT/DECREMENT\n");
      return NULL;
    }
    return crement;

  } else {
    printf("ERROR PARSING STATEMENT. UNEXPECTED TOKEN TYPE: %s %s \n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  } else if ((*current)->type == SEPARATOR && strcmp((*current)->value, ";") == 0) {
    *current = (*current)->next;
    return NULL;
  } else {
    printf("ERROR PARSING STATEMENT. UNEXPECTED TOKEN TYPE: %s %s \n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
}

Node *parse_program(Token **current, Node *program) {
  while ((*current)->type != END_OF_TOKENS) {
    if ((strcmp((*current)->value, "print") == 0) ||
        (((strcmp((*current)->value, "int") == 0)) ||
         ((strcmp((*current)->value, "float") == 0))) ||
        (*current)->type == IDENTIFIER) {

      Node *statement = parse_statement(current);
      if (statement == NULL) {
        printf("NULL STATEMENT\n");
        return NULL;
      }
      add_child(program, statement);

    } else {
      printf("ERROR PARSING PROGRAM. UNEXPECTED TOKEN TYPE: %s %s \n",
             token_type_to_string((*current)->type), (*current)->value);
      return NULL;
    }
  }
  if ((*current)->type == END_OF_TOKENS) {
    printf("PARSING COMPLETE\n");
  } else {
    printf("ERROR PARSING PROGRAM. UNEXPECTED TOKEN TYPE: %s %s \n",
           token_type_to_string((*current)->type), (*current)->value);
    return NULL;
  }
  return program;
}

Node *parse_tokens(Token *head) {
  Token *start = add_token(START, NULL, "start", 5, 0);
  Node *program = create_node("Program", start);
  program = parse_program(&head, program);
  return program;
}
