#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  struct Token *next;
} Token;

Token *add_token(TokenType type, Token *head, char *start, int length) {
  Token *new_token = (Token *)malloc(sizeof(Token));
  if (!new_token)
    return NULL;
  new_token->type = type;
  new_token->value = (char *)(malloc(length + 1));
  if (!new_token->value)
    return NULL;
  strncpy(new_token->value, start, length);
  new_token->value[length] = '\0';
  new_token->next = NULL;
  if (!head) {
    // return new token as head if head is empty
    return new_token;
  }

  Token *current = head;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = new_token;

  return head;
}

char *read_file_to_string(FILE *file) {
  if (file == NULL) {
    printf("Could not open file");
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  rewind(file);
  char *buffer = (char *)malloc(file_size + 1);
  if (buffer == NULL) {
    printf("allocation failed");
    fclose(file);
    return NULL;
  }
  fread(buffer, 1, file_size, file);
  buffer[file_size] = '\0';
  return buffer;
}

char *token_type_to_string(TokenType type) {
  switch (type) {
  case TYPE:
    return "TYPE";
  case INT:
    return "INT";
  case FLOAT:
    return "FLOAT";
  case KEYWORD:
    return "KEYWORD";
  case SEPARATOR:
    return "SEPARATOR";
  case ASSIGNMENT_OPERATOR:
    return "ASSIGNMENT_OPERATOR";
  case ADDITION:
    return "ADDITION";
  case SUBTRACTION:
    return "SUBTRACTION";
  case INCREMENT:
    return "INCREMENT";
  case DECREMENT:
    return "DECREMENT";
  case MULTIPLICATION:
    return "MULTIPLICATION";
  case DIVISION:
    return "DIVISION";
  case EXPONENT:
    return "EXPONENT";
  case INTEGER_DIVISION:
    return "INTEGER_DIVISION";
  case MODULUS:
    return "MODULUS";
  case OPERATOR:
    return "OPERATOR";
  case IDENTIFIER:
    return "IDENTIFIER";
  case STRING:
    return "STRING";
  case END_OF_TOKENS:
    return "END OF TOKENS";
  default:
    return "UNKNOWN";
  }
}

void print_tokens(Token *tokens) {
  int tokens_length = 0;
  while (tokens) {
    printf("%s %s\n", token_type_to_string(tokens->type), tokens->value);
    tokens = tokens->next;
    tokens_length++;
  }
  printf("Tokens length %d\n", tokens_length);
}

void free_tokens(Token *head) {
  Token *current = head;
  while (current != NULL) {
    Token *temp = current;
    current = temp->next;
    free(temp->value);
    free(temp);
  }
}

void lexer(char *file_contents, Token **tokens) {
  char *current = file_contents;

  while (*current != '\0') {
    // integer literals
    if (isdigit(*current)) {
      char *start = current;

      while (isdigit(*current))
        current++;

      int length = current - start;
      *tokens = add_token(INT, *tokens, start, length);

      continue;

    } else if (isalpha(*current)) {
      char *start = current;
      while (*current != ' ' && *current != '\0' && *current != '(' &&
             *current != ')')
        current++;
      int length = current - start;

      char word[length];
      memcpy(word, start, length);
      word[length] = '\0';

      // keywords
      if ((strcmp(word, "print") == 0) || (strcmp(word, "return") == 0) ||
          (strcmp(word, "exit") == 0) || (strcmp(word, "another") == 0) ||
          (strcmp(word, "if") == 0)) {
        *tokens = add_token(KEYWORD, *tokens, start, length);
      } else if ((strcmp(word, "int") == 0) || (strcmp(word, "float") == 0)) {
        *tokens = add_token(TYPE, *tokens, start, length);
      }
      // identifier
      else {
        *tokens = add_token(IDENTIFIER, *tokens, start, length);
      }

      continue;
    } else if ((*current == ';') || (*current == '(') || (*current == ')')) {
      char *start = current;
      *tokens = add_token(SEPARATOR, *tokens, start, 1);
    } else if (*current == '=') {
      char *start = current;
      while (!isdigit(*current) && !(isalpha(*current)) && (*current != ' ') &&
             (*current != '\0'))
        current++;
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(ASSIGNMENT_OPERATOR, *tokens, start, 1);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }
      continue;
    }
    // operators
    else if (*current == '+') {
      char *start = current;
      current++;
      while ((*current == '+') || (*current == '=')) {
        current++;
      }
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(ADDITION, *tokens, start, 1);
      } else if (length == 2) {
        // To do: add addition equals
        *tokens = add_token(INCREMENT, *tokens, start, 2);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }
      continue;
    } else if (*current == '-') {
      char *start = current;
      current++;
      while ((*current == '-') || (*current == '=')) {
        current++;
      }
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(SUBTRACTION, *tokens, start, 1);
      } else if (length == 2) {
        // To do: add subtraction equals
        *tokens = add_token(DECREMENT, *tokens, start, 2);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }
      continue;
    } else if (*current == '*') {
      char *start = current;
      current++;
      while ((*current == '*') || (*current == '=')) {
        current++;
      }
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(MULTIPLICATION, *tokens, start, 1);
      } else if (length == 2) {
        // To do: add multiplication equals
        *tokens = add_token(EXPONENT, *tokens, start, 2);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }
      continue;
    } else if (*current == '/') {
      char *start = current;
      current++;
      while ((*current == '/') || (*current == '=')) {
        current++;
      }
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(DIVISION, *tokens, start, 1);
      } else if (length == 2) {
        // To do: add division equals
        *tokens = add_token(INTEGER_DIVISION, *tokens, start, 2);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }
      continue;
    } else if (*current == '%') {
      char *start = current;
      current++;
      while (!isdigit(*current) && !(isalpha(*current)) && (*current != ' ') &&
             (*current != '\0')) {
        current++;
      }
      int length = current - start;
      if (length == 1) {
        *tokens = add_token(MODULUS, *tokens, start, 1);
      } else {
        *tokens = add_token(OPERATOR, *tokens, start, length);
      }

      continue;
    }
    current++;
  }
  *tokens = add_token(END_OF_TOKENS, *tokens, "/0", 2);
}