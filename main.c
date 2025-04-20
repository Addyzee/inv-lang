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
  lexer(file_contents, &tokens);

  print_tokens(tokens);
  parse_tokens(tokens);


  free_tokens(tokens);

  free(file_contents);
}