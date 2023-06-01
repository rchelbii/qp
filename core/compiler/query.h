#ifndef _QUERY_H
#define _QUERY_H

typedef struct {
	char* command;
	char* table;
	char* conditions;
} Query;

Query query_parser(Query*);


#endif