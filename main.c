#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[]) {
  if(argc<2){
    printf("Error: No file argument. Correct syntax: ./main <filename.inv>\n");
    exit(1);
  }
  FILE *file;
  file = fopen(argv[1], "r");

  char *file_contents = read_file_to_string(file);
  fclose(file);

  if (file_contents == NULL) {
    printf("No file contents");
    return 1;
  }

  Token *tokens = NULL;
  tokens = lexer(file_contents, &tokens);

  if (tokens == NULL) {
    printf("No tokens");
    return 1;
  }

  // print_tokens(tokens);

  // Parse tokens

  Node *program = parse_tokens(tokens);
  if (program == NULL) {
    printf("No program");
    return 1;
  }
  printf("Parsed tree:\n");
  print_tree(program, 0);

  
  free_tokens(tokens);
  free(file_contents);
  free_parse_tree(program);
  return 0;
}