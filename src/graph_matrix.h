#ifndef GRAPH_H 
#define GRAPH_H 
	#include "graph.h"
#endif

/* Struttura usata per iterare le adiacenze di un nodo. */
struct matrix_iterate
{
	varr_t 		 A; 	
	unsigned int i;
};

struct operations *get_graph_matrix_operations(void);

void *adiac_graph_matrix(void *E, unsigned int key);
void *next_graph_matrix(void *list);
void *delete_key_from_E_graph_matrix(graph_t graph, unsigned int key);
void *get_edge_struct_graph_matrix(void);
void *set_edge_graph_matrix(void *E, unsigned int key_source, struct edge *edge);
void *delete_edge_graph_matrix(void *E, unsigned int key_source, unsigned int key_target);

struct edge *get_curr_graph_matrix(void *list);
struct edge *get_edge_graph_matrix(void *E, unsigned int source, unsigned int target);

void free_E_graph_matrix(void *E, int edges);
