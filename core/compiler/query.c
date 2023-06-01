#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "query.h"

Query new_query()
{
	Query *query = (Query *) malloc(sizeof(Query));
	if (query == NULL) {
		perror("ERROR: Cannot allocate query in memory \n");
		exit(EXIT_FAILURE);
	}

	query->command = NULL;
	query->table = NULL;
	query->conditions = NULL;

	return query;
}

Query query_parser(const char* sql_raw)
{
	Query* _query = (Query *) malloc(sizeof(Query));
	// TODO:
	return _query;
}