#ifndef GRAPH_H 
#define GRAPH_H 
	#include "../graph.h"
#endif

enum terminal
{
	LPAR,
	RPAR,
	COMMA,
	DOT,
	SEMICOLON,
	ARROW,
	UNDEFINED
};

struct parser_op
{
	void RW_object(FILE *file, void *obj);
	void RW_terminal(FILE *file, enum terminal terminal);
	void RW_key_node(FILE *file, unsigned int num);

	unsigned int RW_node(FILE *file, graph_t graph, void (*RW_object)(FILE *file, void *obj), unsigned int curr_node);

	void *RW_edge(FILE *file, graph_t graph, unsigned int key_source, void *curr_acienty);
};

struct parsing_attr
{
	graph_t graph;
	unsigned int key_curr_node;

	struct parser_op *parser_op;
};

void graph(FILE *file, graph_t graph, struct parsing_attr *parsing_attr);
void syntax_error(const char *err, FILE *file); 
void catch_error(char *errstr);
void adiacenze(FILE *file, graph_t graph, struct parsing_attr *parsing_attr);
void lista_archi(FILE *file, graph_t graph, unsigned int curr_node, unsigned int key_source, struct parsing_attr *parsing_attr);
void write_graph(const char *filename, graph_t graph, void (*Write_object)(FILE *file, void *obj));

unsigned int adiacenza_nodo(FILE *file, graph_t graph, unsigned int curr_node, struct parsing_attr *parsing_attr);
unsigned int nodo(FILE *file, graph_t graph, unsigned int curr_node, struct parsing_attr *parsing_attr);

void *arco(FILE *file, graph_t graph, void *curr_acienty, unsigned int key_source, struct parsing_attr *parsing_attr); 

double peso(FILE *file); 
double parse_real(FILE *file);

graph_t read_graph(const char *filename, struct operation *graph_op, void (*Read_object)(FILE *file, void *obj));
