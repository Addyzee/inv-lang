#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

typedef struct Node {
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
    if(root == NULL){
        return new_node;
    } else if(root->left == NULL){
        root->left = new_node;
    } else if(root->right == NULL){
        root->right = new_node;
    }else{
        init_node(root->left,value);
    }
    return root;
 
}

void print_tree(Node *root) {
  printf("%s\n", root->token->value);
  if (root->left) {
    printf("left:\n");
    print_tree(root->left);
  }
  if (root->right) {
    printf("right\n");
    print_tree(root->right);
  }
}

void parse_tokens(Token *head) {
  Token *current = head;

  while (current) {
    printf("showing token: %s\n", current->value);
    current = current->next;
  }
}

void tree() {
  printf("tree\n");
  Node *root = NULL;
  root = init_node(root, "+");
  root = init_node(root, "*");
  root = init_node(root, "5");
  print_tree(root);
  free(root->left);
  free(root->right);
  free(root);
}