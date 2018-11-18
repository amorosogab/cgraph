#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "queue/queue.h"
#include "graph.h"
#include "utils.h"


/*	DESCRIZIONE
	Scopo di questa funzione è quello di aggiungere un arco che parte dal nodo con chiave key_source 
	e va a finire nel nodo con chiave key_target.

	PARAMETRI DI INPUT
	graph: graph_t 				- Grafo.
	peso: double				- Peso dell'arco da aggiungere.
	key_source: unsigned int 	- Chiave nodo sorgente.
	key_target: unsigned int 	- chiave nodo target.

	PRE-CONDIZIONI
	graph != NULL,
	key_source riferisce a un nodo esistente,
	key_target riferisce a un nodo esistente

	POST-CONDIZIONI
	Il grafo passato in input e il grafo restituito in output, differiscono solo per l'aggiunta dell'arco che
	 va da 'key_source' a 'key_target' di peso 'peso'.

	VALORE DI RITORNO
	Restituisce un nuovo grafo contenente un arco che parte da key_source e va a finire in key_target. 
	Se uno dei due nodi sorgente o destinazione non esiste nel grafo, il nuovo arco non viene aggiunto. */
graph_t add_edge(graph_t graph, double peso, unsigned int key_source, unsigned int key_target)
{
	if(graph && key_source <= get_length(graph->V) && key_target <= get_length(graph->V))
	{
		struct edge *new_edge = (struct edge *) malloc(sizeof(struct edge));
		if(new_edge)
		{
			new_edge->peso = peso;
			new_edge->target = get_loc(graph->V, key_target);
			graph->E = graph->op->Set_edge(graph->E, key_source, new_edge);
		}
	}

	return graph;
}

/* 	DESCRIZIONE
	Scopo di questa funzione è quello di aggiungere un nuovo nodo al grafo, con oggetto object. 
	La chiave del vertice assume valore graph->n_ver + 1.

	PARAMETRI DI INPUT
	graph: graph_t 			- Grafo.
	object: void * 			- Dati satellite.

	PRE-CONDIZION1
	Graph != NULL

	POST-CONDIZIONI
	L'insieme dei vertici differisce dall'insieme dei vertici originario, per l'aggiunta 
	di un vertice contenente l'oggetto passato in input.

	VALORE DI RITORNO
	Se il grafo dato in ingresso è diverso da NULL, la funzione restituisce i grafo contenente un nuovo vertice. */
graph_t add_vertex(graph_t graph, void *object)
{
	if(graph)
	{
		if(graph->V == NULL)
			graph->V = get_new_vertex_set();

		if(is_initialized(graph->V))
		{
			struct graph_node *new_node = (struct graph_node *) malloc(sizeof(struct graph_node));
			if(new_node == NULL)
				return graph;

			unsigned int i, length_V = get_length(graph->V);
			for(i = 0; i < length_V && get_loc(graph->V, i) != NULL; i++)
				;

			if(i == length_V)
				new_node->key = length_V + 1;
			else
				new_node->key = i;
			new_node->object = object;
			set_loc(graph->V, new_node->key, new_node);
		}
	}

	return graph;
}

/* 	DESCRIZIONE
	La funzione crea e inizilizza un nuovo insieme di vertici vuoto.

	VALORE DI RITORNO 
	Restituisce un insieme di vertici vuoto. 		*/
varr_t get_new_vertex_set(void)
{
	varr_t V = get_varr();
	V = init_varr(V, DIMV, DIMV, INT_MAX, DIMV);

	return V;
}

/* 	DESCRIZIONE
	Cancella il vertice dal grafo, con chiave 'key'.

	PARAMETRI DI INPUT
	graph: graph_t		- Grafo.
	key: unsigned int	- Chiave corrispondente al vertice da eliminare.

	PRE-CONDIZION1
	Graph != NULL
	graph→V != NULL
	key >= 0
	key < n_vert

	VALORE DI RITORNO
	Restituisce il grafo dato in ingresso, senza il vertice la cui chiave è key. 
	Se key non esiste, non viene effettuata nessuna modifica. */
graph_t delete_vertex(graph_t graph, unsigned int key)
{
	if(graph && is_initialized(graph->V) && key >= 0 && key < get_length(graph->V))
	{
		struct graph_node *node = get_loc(graph->V, key);
		set_loc(graph->V, key, NULL);
		graph->E = graph->op->Delete_key_from_E(graph, key);
		free(node);
	}
	
	return graph;
}

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di calcolare il grafo trasposto del grafo dato in input.

	PARAMETRI DI INPUT
	graph: graph_t 	- Grafo.

	PRE-CONDIZION1
	Graph != NULL

	VALORE DI RITORNO
	Restituisce un nuovo grafo (trasposto), con gli stessi attributi di quello precedente. */
graph_t get_transposed(graph_t graph)
{
	graph_t GT = NULL;

	if(graph)
	{
		GT = get_new_graph(graph->op);
		if(GT == NULL)
			return NULL;

		GT->V = dup_varr(graph->V);
		unsigned int i, length_V = get_length(graph->V);
		for(i = 0; i < length_V; i++)
		{
			if(get_loc(graph->V, i) != NULL)
			{
				void *list = graph->op->Adiac(graph->E, i);
				struct edge *curr_e = graph->op->Get_curr(list);
				while(curr_e)
				{
					GT = add_edge(GT, curr_e->peso, curr_e->target->key, i);
					list = graph->op->Next(list);
					curr_e = graph->op->Get_curr(list);
				}
			}
		}
	}

	return GT;
}

/* 	DESCRIZIONE
	Effettua una visita in profondità dell'intero grafo.

	PARAMETRI DI INPUT
	graph: graph_t 	- Grafo.

	PARAMETRI DI OUTPUT
	out pred: struct graph_node **  - Array dei predecessori.

	POST-CONDIZIONI
	L'intero grafo è stato esplorato,
	In pred è possibile recuperare la foresta DF generata dalla visita in profondità.

	VALORE DI RITORNO
	La funzione restituisce in pred l'array dei predecessori. 
	E' compito del chiamante assicurarsi che pred abbia la giusta quantita' di memoria allocata e cioe' |V|. */
void DFS(graph_t graph, struct graph_node **pred)
{
	if(graph)
	{
		unsigned int length_V = get_length(graph->V);
		enum color *dfs_colors = (enum color *) malloc(sizeof(enum color) * length_V + 1);
		if(!dfs_colors)
			return;

		unsigned int i;
		for(i = 0; i < length_V; i++)
		{
			dfs_colors[i] = WHITE;
			if(pred != NULL) 
				pred[i] = NULL;
		}

		struct dfs_visit_data *data = (struct dfs_visit_data *) malloc(sizeof(struct dfs_visit_data));
		if(data == NULL)
			return;
		data->extra = pred;

		for(i = 0; i < length_V; i++)
			if(get_loc(graph->V, i) != NULL)
				DFS_visit(graph, i, dfs_colors, data, NULL, dfs_action, NULL);

		free(data);
		free(dfs_colors);
	}
}

/* 	DESCRIZIONE
	Effettua una visita in profondità del grafo, raggiungendo tutti i nodi raggiungibili da source_key.

	PARAMETRI DI INPUT
	graph: graph_t 	                - Grafo.
	source_key: int 	            - Chiave del nodo sorgente.
	dfs_colors[]: enum color     	- Array dei colori della visita.
	data: struct dfs_visit_data *	- 	Struct che racchiude i dati interni della dfs_visit per fare in modo 
										che Pre_inspect, Inspect e Post_inspect possano utilizzare i dati della 
										visita corrente.
	(*Pre_inspect)(visit_data: struct dfs_visit_data *): int - Questa funzione viene eseguita prima dell'ispezione
	 del nodo corrente. Se la funzione restituisce 0, il processo di ispezione non deve continuare.
	(*Action)(visit_data: struct dfs_visit_data *): int - Questa funzione viene eseguita per ogni arco uscente 
	dal nodo sorgente, prima della chiamata ricorsiva. Se la funzione restituisce 0, il processo di ispezione 
	non deve continuare.
	(*Post_inspect)(visit_data: struct dfs_visit_data *): int - Questa funzione viene eseguita al termine 
	dell'ispezione della lista di adiacenza del nodo corrente. Se la funzione restituisce 0, il processo di 
	ispezione non deve continuare.

	PRE-CONDIZION1
	Graph != NULL, 
	dfs_colors != NULL,
	source_key fa' riferimento a un nodo esistente.

	POST-CONDIZIONI
	Tutti gli archi uscenti dal nodo 'source_key', sono stati ispezionati.

	VALORE DI RITORNO
	Restituisce un intero maggiore di 0 se il processo di esplorazione del grafo deve continuare, 
	altimenti restituisce 0 se deve terminare. */
int DFS_visit(graph_t 				 graph,
			  unsigned int			 source_key,
			  enum color 			 dfs_colors[],
			  struct dfs_visit_data *data,
			  int 				     (*Pre_inspect)(struct dfs_visit_data *data),
			  int 					 (*Action)(struct dfs_visit_data *data),
			  int 					 (*Post_inspect)(struct dfs_visit_data *data))
{
	int c;
	void *list = NULL;
	struct edge *curr_e = NULL;

	c = 1;
	if(graph && dfs_colors)
	{
		dfs_colors[source_key] = GRAY;

		if(data)
		{
			data->graph = graph;
			data->source_key = source_key;
			data->list = &list;
			data->curr_e = &curr_e;
			data->dfs_colors = dfs_colors;
		}

		if(Pre_inspect)
			c = Pre_inspect(data);

		list = graph->op->Adiac(graph->E, source_key);
		if(c && list)
		{
		 	curr_e = graph->op->Get_curr(list);
			while(c && curr_e)
			{
				if(Action)
					c = Action(data);

				if(c && dfs_colors[curr_e->target->key] == WHITE) 
				{
					struct dfs_visit_data *new_data = (struct dfs_visit_data *) malloc(sizeof(struct dfs_visit_data));
					if(new_data == NULL)
						return 0;
					new_data->extra = data->extra;

					c = DFS_visit(graph, curr_e->target->key, dfs_colors, new_data, Pre_inspect, Action, Post_inspect);

					if(new_data != NULL)
						free(new_data);
				}

				list = graph->op->Next(list);
				curr_e = graph->op->Get_curr(list);
			}
		}

		dfs_colors[source_key] = BLACK;
		if(c && Post_inspect)
			c = Post_inspect(data);
	}

	return c;
}

/* 	DESCRIZIONE
	Azione da eseguire per ogni arco scoperto del grafo. Nel caso specifico l'azione da eseguire è 
	l'assegnamento del predecessore.

	PARAMETRI DI INPUT
	visit_data: struct dfs_visit_data * - Dati della dfs_visit

	PRE-CONDIZION1
	visit_data != NULL,
	visit_data→extra punta all'array dei predecessori.

	VALORE DI RITORNO
	Restituisce un valore maggiore di 0 se l'ispezione del grafo deve continuare, 0 altrimenti. */
int dfs_action(struct dfs_visit_data *visit_data)
{
	if(visit_data && visit_data->extra)
	{
		if(visit_data->dfs_colors[(*visit_data->curr_e)->target->key] == WHITE)
		{
			struct graph_node **pred = (struct graph_node **) visit_data->extra;
			pred[(*visit_data->curr_e)->target->key] = get_loc(visit_data->graph->V, visit_data->source_key);
		}

		return 1;
	}

	return 0;
}

/* 	DESCRIZIONE
	Effettua una visita in ampiezza dell'intero grafo, partendo dal nodo source_key.

	PARAMETRI DI INPUT
	graph: graph_t 				- Grafo.
	source_key: unsigned int 	- Chiave del nodo sorgente.

	PARAMETRI DI OUTPUT
	out pred: struct graph_node ** - Array dei predecessori.
	out dist: double *  	       - Array delle distanza.

	PRE-CONDIZION1
	Graph != NULL,
	source_key fa riferimento a un nodo esistente,
	Se pred != NULL allora pred punta a un'area di memoria di |V| locazioni,
	Se dist != NULL allora dist punta a un'area di memoria di |V| locazioni.

	POST-CONDIZIONI
	Sono stati esplorati tutti i vertici raggiungibili da source_key,
	Se pred era originariamente diverso da NULL, è possibile recuperare in esso l'albero BF della visita in ampiezza,
	Se dist era originariamente diverso da NULL, è possibile recuperare in esso le distanze minime dal vertice 
	sorgente source_key.

	VALORE DI RITORNO
	La funzione restituisce in pred l'array dei pedecessori e in dist l'array delle distanze dal nodo sorgente. 
	Se il chiamante non è interessato a uno dei due parametri di output, puo' passare NULL al parametro corrispondente. */
void BFS(graph_t graph, unsigned int source_key, struct graph_node **pred, double *dist)
{
	queue_t Q;

	unsigned int length_V = get_length(graph->V);
	enum color *bfs_colors = (enum color *) malloc(sizeof(enum color) * length_V + 1);

	if(graph && bfs_colors && source_key <= length_V)
	{
		unsigned int i;
		for(i = 0; i < length_V; i++)
		{
			bfs_colors[i] = WHITE;
			if(pred != NULL)
				pred[i] = NULL;
			if(dist != NULL)
				dist[i] = -1;
		}

		bfs_colors[source_key] = GRAY;
		if(dist)
			dist[source_key] = 0;

		Q = new_queue(1);
		if(Q)
			Q = enqueue_copy(Q, &source_key, int_copy);
		else
			return;

		while(!is_empty(Q))
		{
			unsigned int u = *((unsigned int *) top(Q));
			void *list = graph->op->Adiac(graph->E, u);

			if(list)
			{
				struct edge *u_edge = graph->op->Get_curr(list);
				while(u_edge)
				{
					if(bfs_colors[u_edge->target->key] == WHITE)
					{
						bfs_colors[u_edge->target->key] = GRAY;

						if(dist)
							dist[u_edge->target->key] = dist[u] + u_edge->peso;
						if(pred)
							pred[u_edge->target->key] = get_loc(graph->V, u);

						Q = enqueue_copy(Q, &u_edge->target->key, int_copy);	
					}

					list = graph->op->Next(list);
					u_edge = graph->op->Get_curr(list);
				}
			}

			Q = dequeue_destroy(Q, free);
		}
	}

	free(Q);
	free(bfs_colors);
}

/*	DESCRIZIONE
	Stampa un percorso che parte dal nodo 'from' e finisce nel nodo 'to', entrambi dati in ingresso. 
	Tale funzione si avvale dell'array dei predecessori per raggiungere il suo scopo.

	PARAMETRI DI INPUT
	graph: graph_t		   			- Grafo.
	from: struct graph_node *    	- Nodo sorgente.
	to: struct graph_node *	   		- Nodo di destinazione.
	pred: struct graph_node **  	- Array dei predecessori.
	(*Print)(obj: void *): void    	- Funzione per stampare;

	PRE-CONDIZION1
	Graph != NULL,
	Print != NULL,
	from è un nodo esistente di graph,
	to è un nodo esistente di graph,
	pred != NULL. 											*/
void print_path(graph_t graph, struct graph_node *from, struct graph_node *to, struct graph_node **pred, void (*Print)(void *obj))
{
	if(Print && graph && from && from->key <= get_length(graph->V) && to && to->key <= get_length(graph->V) && pred != NULL)
	{
		if(to == from)
		{
			printf("(key: %d; obj: ", from->key);
			Print(from->object);
			printf(")\n");
		}
		else if(pred[to->key] == NULL)
			printf("Non esistono percorsi tra i due nodi.\n"); 
		else
		{
			print_path(graph, from, pred[to->key], pred, Print);
			printf("(key: %d; obj: ", to->key);
			Print(to->object);
			printf(")\n");
		}
	}
}

/* DESCRIZIONE
	Lo scopo di questa funzione è quello di allocare e inizializzare opportunamente 
	un nuovo grafo e restituirlo in output.

	PARAMETRI DI INPUT
	op: struct operations * - Struttura delle operazioni.

	PRE-CONDIZION1
	Op != NULL

	VALORE DI RITORNO
	Restituisce un nuovo grafo con 0 vertici e V == E == NULL. 			*/
graph_t get_new_graph(struct operations *op)
{
	graph_t graph = NULL;

	if(op)
	{
		graph = (graph_t) malloc(sizeof(struct graph));
		if(graph)
		{
			graph->V = NULL;
			graph->E = NULL;
			graph->op = op;
		}
	}

	return graph;
}

/* 	DESCRIZIONE
	Genera l'insieme degli archi in maniera random.

	PARAMETRI DI INPUT
	V: struct graph_node **    - insime dei vertici
	n_vert                                - int.
	min_peso: double	- Peso minimo.
	max_peso: double	- Peso massimo.
	Op: struct operations *: struttura delle operazioni

	PRE-CONDIZION1
	V != NULL,
	L’insieme V ha esattamente n_vert vertici,
	Op != NULL,
	min_peso <= max_peso.

	POST-CONDIZIONI
	l'insieme E è costituito da un numero casuale di archi, ognuno con un peso che oscilla tra min_peso e max_peso, 
	estremi compresi.

	VALORE DI RITORNO
	Restituisce un nuovo insieme degli archi random. 					*/
void *random_E(varr_t V, double min_penso, double max_peso, struct operations *op)
{
	void *E = NULL;

	if(is_initialized(V) && op && min_penso < max_peso)
	{
		unsigned int i, j;

		E = op->Get_edge_struct();
		if(E)
		{
			unsigned int length_V = get_length(V);	
			for(i = 0; i < length_V; i++)
			{
				for(j = 0; j < length_V; j++)
				{
					if(get_loc(V, i) != NULL && get_loc(V, j) != NULL)
					{
						int is_edge = rand_num(0, 1);
						if(is_edge)
						{
							double peso = rand_num(min_penso, max_peso);
							struct edge *new_edge = (struct edge *) malloc(sizeof(struct edge));
							if(!new_edge)
								goto exit_error;
							else
							{
								new_edge->peso = peso;
								new_edge->target = get_loc(V, j);
								E = op->Set_edge(E, i, new_edge);
							}
						}
					}
				}
			}
		}
	}

	exit_error:
		return E;
}

/*	DESCRIZIONE
	Genera un grafo random.

	PARAMETRI DI INPUT
	Struct operations *op 	- Struttura delle operazioni.
	int min_vert: 			- Numero minimo di vertici.
	int max_vert			- Numero massimo di vertici. 
	double min_peso 		- Peso minimo.
	double max_peso 		- Peso massimo 
	void *(*Rand_obj)(void) - Funzione che restituisce un oggetto random.

	VALORE DI RITORNO 
	Restituisce un grafo random. 									*/
graph_t random_graph(struct operations *op, int min_vert, int max_vert, double min_peso, double max_peso, void *(*Rand_obj)(void))
{
	graph_t graph = NULL;


	if(op && Rand_obj)
	{

		graph = get_new_graph(op);
		if(graph)
		{
			int i, n = rand_num(min_vert, max_vert);
			for(i = 0; i < n; i++)
				graph = add_vertex(graph, Rand_obj());

			graph->E = random_E(graph->V, min_peso, max_peso, op);
		}
	}

	return graph;
}

/* 	DESCRIZIONE
	Cambia la rappresentazione interna del grafo passato in input.

	PARAMETRI DI INPUT
	graph: graph_t                  - Grafo.
	new_op: struct operations *     - Nuova struttura delle operazioni che opera sul nuovo grafo. 
									Ad esempio, se graph è un grafo a matrice di adiacenza e ne vogliamo 
									uno a liste di adiacenza, si deve passare una struttura delle operazioni 
									di un grafo a liste di adiacenza.

	PRE-CONDIZIONI
	Graph != NULL,
	new_op != NULL

	VALORE DI RITORNO
	Restituisce un nuovo grafo. 					*/
graph_t change_rappresentation(graph_t graph, struct operations *new_op)
{
	if(graph && new_op)
	{
		void *new_E = new_op->Get_edge_struct();
		if(new_E)
		{
			unsigned int i, length_V = get_length(graph->V);
			for(i = 0; i < length_V; i++)
			{
				if(get_loc(graph->V, i) != NULL)
				{
					void *list = graph->op->Adiac(graph->E, i);
					if(list)
					{
						struct edge *curr_e = graph->op->Get_curr(list);
						while(curr_e)
						{
							new_E = new_op->Set_edge(new_E, i, curr_e);

							list = graph->op->Next(list);
							curr_e = graph->op->Get_curr(list);
						}
					}
				}
			}

			graph->op->Free_E(graph->E, 0);
			graph->E = new_E;

			free(graph->op);
			graph->op = new_op;
		}
	}

	return graph;
}

/* 	DESCRIZIONE
	Verifica se un grafo è aciclico.

	PARAMETRI DI INPUT
	Graph: graph_t - Grafo

	VALORE DI RITORNO
	Se il grafo è aciclico, la funzione restituisce un valore maggiore di 0, altrimenti restituisce 0. 	*/
int is_acyclic(graph_t graph)
{
	int cont;

	if(graph)
	{
		unsigned int length_V = get_length(graph->V);
		enum color *dfs_colors = (enum color *) malloc(sizeof(enum color) * length_V + 1);
		if(!dfs_colors)
			return -1;

		unsigned int i;
		for(i = 0; i < length_V; i++)
			dfs_colors[i] = WHITE;

		struct dfs_visit_data *data = (struct dfs_visit_data *) malloc(sizeof(struct dfs_visit_data));
		if(data == NULL)
			return -1;

		for(i = 0, cont = 1; i < length_V && cont > 0; i++)
			if(get_loc(graph->V, i) != NULL)
				cont = DFS_visit(graph, i, dfs_colors, data, NULL, is_acyclic_action, NULL);

		free(data);
		free(dfs_colors);
	}

	return cont == 1;
}

/*	DESCRIZIONE
	Azione di verifica di un grafo acilcico. Usata come callback nella dfs_visit.

	PARAMETRI DI INPUT
	visit_data: struct dfs_visit_data * - data della visita.

	VALORE DI RITORNO
	Ritorna 0 se incontra un vertice colorato di grigio. 						*/
int is_acyclic_action(struct dfs_visit_data *visit_data)
{
	if(visit_data && visit_data->dfs_colors[(*visit_data->curr_e)->target->key] == GRAY)
		return 0;
	else
		return 1;
}

/* 	DESCRIZIONE
	Distrugge il grafo passato in input.

	PARAMETRI DI INPUT
	Graph: graph_t				– Grafo.
	(*Destroy_obj)(void *obj) 	– Funzione per distruggere l’oggetto. 		 */
void destroy_graph(graph_t graph, void (*Destroy_obj)(void *obj))
{
	struct graph_node *node;

	if(graph)
	{
		unsigned int i, length_V = get_length(graph->V);
		for(i = 0; i < length_V; i++)
		{
			if((node = get_loc(graph->V, i)) != NULL)
			{
				if(Destroy_obj)
					Destroy_obj(node->object);
				free(node);
			}
		}

		destroy_varr(graph->V);
		graph->op->Free_E(graph->E, 1);
		free(graph->op);
		free(graph);
	}
}