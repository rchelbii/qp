#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#define sizeofattr(Struct, Attr) sizeof(((Struct*)0)->Attr)

#define PAGE_SIZE 4096
#define TABLE_MAX_PAGES 100

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / TABLE_MAX_PAGES;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

typedef struct {
  uint32_t num_rows;
  void* pages[TABLE_MAX_PAGES];
} Table;

/*
 ** support two operations: inserting a row and printing all rows
 ** reside only in memory (no persistence to disk)
 ** support a single, hard-coded table

   id: integer
   usename: varchar(32)
   email: varchar(255)
*/

typedef struct {
  int id;
  char username[32];
  char email[255];
} Row;

const uint32_t ID_SIZE = sizeofattr(Row, id);
const uint32_t USERNAME_SIZE = sizeofattr(Row, username);
const uint32_t EMAIL_SIZE = sizeofattr(Row, email);

/* the layout of a serialized row */
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_SIZE + USERNAME_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

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
  PREPARE_SYNTAX_ERROR,
  PREPARE_UNRECOGNIZED_STATEMENT,
} PrepareResult;

typedef enum { 
  STATEMENT_INSERT,
  STATEMENT_SELECT 
} StatementType;

typedef struct {
  StatementType type;
  Row row_to_insert;
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
    int args_assigned = sscanf(input->buffer,
                               "%d %s %s",
                               &(statement->row_to_insert.id), 
                               statement->row_to_insert.username, 
                               statement->row_to_insert.email);
    if (args_assigned < 3) return PREPARE_SYNTAX_ERROR;
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


void serialize_row(Row* source, void* dest) {
  memcpy(dest + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(dest + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(dest + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
}

void deserialize_row(void* source, Row* dest)
{
  memcpy(&(dest->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(dest->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(dest->email), source + EMAIL_OFFSET, EMAIL_SIZE);
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
      case (PREPARE_SYNTAX_ERROR):
        break;
      case (PREPARE_UNRECOGNIZED_STATEMENT):
        printf("Unrecognized keyword at start '%s' \n", input->buffer);
        continue;
      
    }

    execute_statement(statement);
    printf("Executed..\n");
  }
}














