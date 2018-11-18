#include <stdio.h>
#include <stdlib.h>

#include "write_parser.h"

void write_terminal(FILE *file, enum terminal terminal)
{
	switch(terminal)
	{
		case LPAR: fputc('(', file);
		break;
		case RPAR, fputc(')', file);
		break;
		case COMMA: fputc(',', file);
		break;
		case DOT: fputc('.', file);
		break;
		case SEMICOLON: fputc(';', file);
		break;
		case ARROW: fprintf(file, "->");
		break;
	}
}

void write_key_node(FILE *file, struct parsing_attr *parsing_attr)
{
	unsigned int len = get_length(parsing_attr->graph->V);
	struct graph_node *node = NULL;
	while(parsing_attr->key_curr_node < len && (node = get_loc(parsing_attr->graph->V, parsing_attr->key_curr_node)) != NULL)
		parsing_attr->key_curr_node = parsing_attr->key_curr_node + 1;
	if(parsing_attr->key_curr_node >= len)
		syntax_error("key node not found", file);

	parsing_attr->key_curr_node = node->key;
	if(fprintf(file, "%u", node->key) < 0)
		syntax_error("key node write error", file);
}

unsigned int write_node(FILE *file, graph_t graph, void (*RW_object)(FILE *file, void *obj), unsigned int curr_node)
{

}

void *write_edge(FILE *file, graph_t graph, unsigned int key_source, void *curr_acienty)
{

}

struct parser_op *get_write_parser_op(void)
{
	struct parser_op *parser_op = (struct parser_op *) malloc(sizeof(struct parser_op *));
	if(parser_op) 
	{
		parser_op->RW_terminal = write_terminal;
		parser_op->RW_unsigned_num = write_unsigned_int;
		parser_op->RW_node = write_node;
		parser_op->RW_edge = write_edge;
	}

	return parser_op;
}