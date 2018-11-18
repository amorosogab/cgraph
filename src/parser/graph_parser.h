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

void graph_parse(FILE *file, graph_t graph, void *(*Read_object)(FILE *file));
void adiacenze(FILE *file, graph_t graph, void *(*Read_object)(FILE *file));
void adiacenza_nodo(FILE *file, graph_t graph, void *(*Read_object)(FILE *file));
struct graph_node *nodo(FILE *file, varr_t V, void *(*Read_object)(FILE *file));
unsigned int key(FILE *file);
unsigned int parse_unsigned_int(FILE *file);
void lista_archi(FILE *file, graph_t graph, unsigned int key_source);
struct edge *arco(FILE *file, graph_t graph);
double peso(FILE *file);
double parse_real(FILE *file); 
int match(FILE *file, enum terminal t);
void move(FILE *file, enum terminal tp);
void syntax_error(char *err, FILE *file);
void catch_error(char *errstr);
graph_t read_graph(const char *filename, struct operations *graph_op, void *(*Read_object)(FILE *file));
void write_graph(const char *filename, graph_t graph, void (*Write_object)(FILE *file, void *obj));