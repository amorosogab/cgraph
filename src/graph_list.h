#ifndef GRAPH_H 
#define GRAPH_H 
	#include "graph.h"
#endif

#include "linkedlist/linkedlist.h"

struct l_acienty
{
	linkedlist_t list; 			/*  Lista di adiacenza. 								*/
	struct node_list *last; 	/* 	Puntatore all'elemento finale della lista. 
									Permette di fare gli inserimenti in tempo costante. */
};

struct operations *get_graph_list_operations(void);

void *adiac_graph_list(void *E, unsigned int key);
void *next_graph_list(void *list);
void *delete_key_from_E_graph_list(graph_t graph, unsigned int key);
void *get_edge_struct_graph_list(void);
void *delete_edge_graph_list(void *E, unsigned int key_source, unsigned int key_target);
void *set_edge_graph_list(void *E, unsigned int key_source, struct edge *edge);

struct edge *get_curr_graph_list(void *list);
struct edge *get_edge_graph_list(void *E, unsigned int source, unsigned int target);

void free_E_graph_list(void *E, int edges);
