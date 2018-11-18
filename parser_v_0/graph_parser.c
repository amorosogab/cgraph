#include <stdio.h>
#include <stdlib.h>

#include "graph_parser.h"

char *stxerr = "Syntax Error: %s expected at position %d.\n";

void graph(FILE *file, struct parsing_attr *parsing_attr)
{
	adiacenze(file, parsing_attr);
	parsing_attr->parser_op->RW_terminal(file, DOT);
}

void syntax_error(char *err, FILE *file)
{  
	/* Print a syntax error string and abort */
 	 char str[80];

	 sprintf(str,stxerr,err,ftell(file)); // Build the syntax error string to print
	 catch_error(str); // Print the error string
}

void catch_error(char *errstr) 
{   
	/* Print message errstr on stderr and abort */
    fprintf(stderr, errstr);
    exit(1);
}

void adiacenze(FILE *file, struct parsing_attr *parsing_attr)
{
	adiacenza_nodo(file, parsing_attr);
}

void adiacenza_nodo(FILE *file, struct parsing_attr *parsing_attr)
{
	nodo(file, parsing_attr);
}

void nodo(FILE *file, struct parsing_attr *parsing_attr)
{
	parsing_attr->parser_op->RW_key_node(file, parsing_attr);
	parsing_attr->parser_op->RW_terminal(file, COMMA);
	curr_node = parser_op->RW_node(file, graph, parser_op->RW_object, curr_node);
}

void lista_archi(FILE *file, struct parsing_attr *parsing_attr)
{

}

void *arco(FILE *file, graph_t graph, void *curr_acienty, unsigned int key_source, struct parsing_attr *parsing_attr) 
{

}

double peso(FILE *file) 
{

}

double parse_real(FILE *file) 
{

}

graph_t read_graph(const char *filename, struct operation *graph_op, void (*Read_object)(FILE *file, void *obj))
{
	graph_t graph = NULL;
	struct parser_op *parser_op = NULL;
	struct parsing_attr *parsing_attr = NULL;
	FILE *file = NULL;

	if(graph_op) 
	{
		file = fopen(filename, "r");
		if(!file)
			goto error;

		parser_op = get_read_parser_op();
		if(!parser_op)
			goto error;
		parsing_attr = get_parsing_attr(parser_op)
		if(!parsing_attr) 
			goto error;

		graph = get_new_graph(graph_op);
		if(!graph)
			goto error;

		parsing_attr->graph = graph;
		graph(file, parsing_attr);
	}

	free(parsing_attr);
	free(parser_op);
	fclose(file);
	return graph;

	error:
		if(!graph)
			destroy_graph(graph);
		if(!parser_op)
			free(parser_op);
		if(!parsing_attr)
			free(parsing_attr);
		if(!file)
			fclose(file);
		return NULL;

}

struct parsing_attr *get_parsing_attr(struct parser_op *parser_op)
{
	struct parsing_attr *parsing_attr = (struct parsing_attr *) malloc(sizeof(struct parsing_attr *));
	if(!parsing_attr)
	{
		parsing_attr->key_curr_node = 0;
		parsing_attr->graph = NULL;

		parsing_attr->parser_op = parser_op;
	}

	return parsing_attr;
}

void write_graph(const char *filename, graph_t graph, void (*Write_object)(FILE *file, void *obj))
{
	struct parser_op *parser_op = NULL;
	struct parsing_attr *parsing_attr = NULL;
	FILE *file = NULL;

	if(graph) 
	{
		file = fopen(filename, "w");
		if(!file)
			goto error;

		parser_op = get_write_parser_op();
		if(!parser_op)
			goto error;
		parsing_attr = get_parsing_attr(parser_op)
		if(!parsing_attr)
			goto error;

		parsing_attr->graph = graph;
		graph(file, parsing_attr);

		return;
	}
	else
		goto error;


	error:
		if(!parser_op)
			free(parser_op);
		if(!parsing_attr)
			free(parsing_attr);
		if(!file)
			fclose(file);
		return;
}