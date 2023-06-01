#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

typedef struct {
  char* buffer;
  size_t length;
  ssize_t buff_len; /* real buffer size readed from user */
} INPUT;

typedef enum {
  META_COMMAND_SUCCESS,
  META_COMMAND_UNRECONGIZED_COMMAND
} MetaCommandResult;

typedef enum {
  PREPARE_SUCCESS,
  PREPARE_UNRECOGNIZED_STATEMENT,
} PrepareResult;

typedef enum { 
  STATEMENT_INSERT,
  STATEMENT_SELECT 
} StatementType;

typedef struct {
  StatementType type;
} Statement;


INPUT* new_input(void) 
{
  INPUT* _input = (INPUT*) malloc(sizeof(INPUT));
  _input->buffer = NULL;
  _input->length = 0;
  _input->buff_len = 0;
  return _input;
}


void prompt(void) 
{
  printf("qp > ");
}

void read_input(INPUT* input) 
{
  ssize_t bytes_read = getline(&(input->buffer), &(input->length), stdin);
  if (bytes_read <= 0) {
    perror("ERROR: reading input failure \n");
    exit(EXIT_FAILURE);
  }

  input->length = bytes_read - 1;
  input->buffer[bytes_read - 1] = 0;
}

void free_input(INPUT* input)
{
  free(input->buffer);
  free(input);
}

MetaCommandResult do_meta_command(INPUT* input)
{
  if (strcmp(input->buffer, ".exit") == 0) {
    free_input(input);
    exit(EXIT_SUCCESS);
  }
  else {
    return META_COMMAND_UNRECONGIZED_COMMAND;
  }
}

PrepareResult prepare_statement(INPUT* input, Statement* statement)
{
  if (strncmp(input->buffer, "insert", 6) == 0) {
    statement->type = STATEMENT_INSERT;
    return PREPARE_SUCCESS;
  }

  if (strncmp(input->buffer, "select", 6) == 0) {
    statement->type = STATEMENT_SELECT;
    return PREPARE_SUCCESS;
  }

  return PREPARE_UNRECOGNIZED_STATEMENT;
}

void execute_statement(Statement statement)
{
  switch (statement.type) {
    case (STATEMENT_INSERT):
      printf("insert here .. \n");
      break;
    case (STATEMENT_SELECT):
      printf("select here .. \n");
      break;
  }
}


int main(int argc, char** argv)
{
  INPUT* input = new_input();
  Statement statement;

  while (true) {
    prompt();
    read_input(input);
    
    if (strncmp(input->buffer, ".", 1) == 0) {
      switch (do_meta_command(input)) {
        case (META_COMMAND_SUCCESS):
          continue;
        case (META_COMMAND_UNRECONGIZED_COMMAND):
          printf("Unrecognized command '%s' \n", input->buffer);
          continue;
      }
    } 

    switch (prepare_statement(input, &statement)) {
      case (PREPARE_SUCCESS):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start '%s' \n", input->buffer);
        continue;
      
    }

    execute_statement(statement);
    printf("Executed..\n");
  }
}














