#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

int main() {
  FILE *file;
  file = fopen("test.inv", "r");

  char *file_contents = read_file_to_string(file);
  fclose(file);

  if (file_contents == NULL) {
    printf("No file contents");
    return 1;
  }

  Token *tokens = NULL;
  lexer(file_contents, &tokens);

  print_tokens(tokens);
  free_tokens(tokens);

  free(file_contents);
}