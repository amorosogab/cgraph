#include "varr/array.h"

#define DIMV 20

/* Struttura che rappresenta il nodo di un grafo. */
struct graph_node
{
	unsigned int key; 		/* Chiave usata solo per la gestione interna. 	*/
	void 		 *object; 	/* Dati satellite. 								*/
};

/* Struttura che rappresenta un arco di un grafo. */
struct edge
{
	double 			   peso; 	/* Peso dell'arco. 	*/
	struct graph_node *target; 	/* Nodo target. 	*/
};

/* Struttura che rappresenta un grafo generico. */
typedef struct graph * graph_t;
struct graph
{
	varr_t  		   V; 	/* Array variadico circolare.	*/
	void 			  *E;	/* Insieme degli archi. 		*/
	struct operations *op;	/* Struttura delle operazioni.	*/
};

/* Enumerazione usata per scorrere il grafo. */
enum color
{
	WHITE, 
	GRAY, 
	BLACK
};

struct operations
{
	/* 	DESCRIZIONE
		Lo scopo di questa funzione è quello di aggiungere l'arco edge al nodo sorgente con chiave key_source.

		PARAMETRI DI INPUT
		E: void *      	    		- Insieme degli archi.
		key_source: unsigned int    - Chiave del nodo sorgente.
		edge: struct edge *  		- Arco da aggiungere.

		PRE-CONDIZIONI
		E != NULL,
		key_source fa riferimento a un nodo esistente,
		edge != NULL

		POST-CONDIZIONI
		L'insieme degli archi E restituito e quello dato in input, differiscono solo per 
		l'aggiunta dell'arco 'edge' che ha come sorgente 'key_source'.

		VALORE DI RITORNO
		Restituisce l'insieme degli archi con un arco che parte dal nodo sorgente la cui chiave è key 
		(se tale nodo esiste.). 							*/
	void * 		 (*Set_edge)(void *E, unsigned int key_source, struct edge *edge);

	/*	DESCRIZIONE
		Lo scopo di questa funzione è quello di restituire l'insieme di archi che partono dal nodo con chiave key.

		PARAMETRI DI INPUT
		E: void *      		- Insieme degli archi.
		key: unsigned int   - Chiave del nodo sorgente.

		VALORE DI RITORNO
		La funzione restituisce la lista di adiacenza del nodo key.  		 */
	void * 		 (*Adiac)(void *E, unsigned int key);

	/* 	DESCRIZIONE
		Questa funzione permette di iterare (scorrere) una lista di adiacenza. 
		Lo scopo di questa funzione è solo quello di far puntare list all'elemento 
		successivo della lista.

		PARAMETRI DI INPUT
		list: void * - Lista di adiacenza.

		PRE-CONDIZION1
		List != NULL

		VALORE DI RITORNO
		Restituisce la lista che punta all'elemento successivo. 			*/
	void * 		 (*Next)(void *list);

	/* 	DESCRIZIONE
		Estrae il nodo corrente dalla lista passata in input. 
		Va usato con next per scorrere la lista di adiacenza fornita da adiac. 
		Per nodo corrente si intende il nodo alla prima posizione della lista.

		PARAMETRI DI INPUT
		list: void * - lista.

		PRE-CONDIZIONI
		List != NULL

		VALORE DI RITORNO
		Arco presente alla prima posizione della lista. */
	struct edge *(*Get_curr)(void *list);

	/* 	DESCRIZIONE
		Lo scopo di questa funzione è quello di rimuovere un arco il cui nodo sorgente è
		key_source e nodo destinatario key_target.

		PARAMETRI DI INPUT
		E: void *      	    		- Insieme degli archi.
		key_source: unsigned int	- Chiave del nodo sorgente.
		key_target: unsigned int 	- Chiave del nodo destinatario.

		VALORE DI RITORNO
		Restituisce il grafo passato in input, senza l'arco che parte da key_source 
		e va a finire in key_target. 												*/
	void *	 	 (*Delete_edge)(void *E, unsigned int key_source, unsigned int key_target);

	/* 	DESCRIZIONE
		Lo scopo di questa funzione è: data la chiave key e l'insieme E degli archi, elimina da E 
		tutti i riferimenti alla chiave key (che sia esso un arco entrante, oppure un arco uscente). 
		Per esempio, nel caso di un grafo a matrice di adiacenza, basta eliminare la riga e la colonna 
		corrispondenti alla chiave key (perché la riga indica gli archi uscenti e la colonna gli archi entranti), 
		mentre in un grafo a lista di adiacenza, si deve eliminare la lista di adiacenza di key e 
		bisogna esplorare l'intero insieme E per eliminare qualsiasi riferimento a key.

		PARAMETRI DI INPUT
		graph: graph_t 		- Grafo.
		key: unsigned int	- Chiave da eliminare da E.

		VALORE DI RITORNO
		Restituisce l'insieme degli archi E 					*/
	void *  	 (*Delete_key_from_E)(graph_t graph, unsigned int key);

	/* 	DESCRIZIONE
		Dealloca la struttura E (non gli archi, ma la struttura che serve per rappresentare gli archi).

		PARAMETRI DI INPUT
		E: void *  - Insieme degli archi.

		PRE-CONDIZION1
		E != NULL

		POST-CONDIZIONI
		La struttura utilizzata per rappresentare gli archi è stata deallocata. 		*/
	void 		 (*Free_E)(void *E, int edges);

	/* 	DESCRIZIONE
		Alloca e inizializza la struttura fisica dei archi.

		VALORE DI RITORNO
		Restituisce la strttura degli archi correttamente inizializzata. 	*/
	void *		 (*Get_edge_struct)(void);

	/* 	DESCRIZIONE
		Lo scopo di questa funzione è quello di restituire l'arco sorgente in 'source' e che ha
		come target 'target'.

		PARAMETRI DI INPUT
		void *E 			- Insieme degli archi. 
		unsigned int source - Nodo sorgente.
		unsigned int target - Nodo target.

		VALORE DI RITORNO
		Restituisce l'arco che parte da source e va a finire in target (se esisnte. Altrimenti restituisce null.) */
	struct edge *(*Get_edge)(void *E, unsigned int source, unsigned int target);
};

struct dfs_visit_data
{
	graph_t 	  graph; 		/* Grafo che si sta scorrendo. 			*/
	unsigned int  source_key;	/* Nodo sorgente 						*/
	void 		 *list; 		/* Lista di adiacenza di source_key. 	*/
	struct edge **curr_e; 		/* Arco corrente. 						*/
	enum color 	 *dfs_colors; 	/* Colori dei nodi della dfs. 			*/
	void 		 *extra; 		/* Dati extra. 							*/
};



graph_t add_edge(graph_t graph, double peso, unsigned int key_source, unsigned int key_target);
graph_t add_vertex(graph_t graph, void *object);
graph_t delete_vertex(graph_t graph, unsigned int key);
graph_t get_transposed(graph_t graph);
graph_t get_new_graph(struct operations *op);
graph_t change_rappresentation(graph_t graph, struct operations *new_op);
graph_t random_graph(struct operations *op, int min_vert, int max_vert, double min_peso, double max_peso, void *(*Rand_obj)(void));

void DFS(graph_t graph, struct graph_node **pred);
void BFS(graph_t graph, unsigned int source_key, struct graph_node **pred, double *dist);
void print_path(graph_t graph, struct graph_node *from, struct graph_node *to, struct graph_node **pred, void (*Print)(void *obj));
void destroy_graph(graph_t graph, void (*Destroy_obj)(void *obj));

void *random_E(varr_t V, double min_penso, double max_peso, struct operations *op);
void *random_n_edge(graph_t graph, unsigned int n, double min_penso, double max_peso);

int DFS_visit(graph_t 				 graph,
			  unsigned int			 source_key,
			  enum color 			*dfs_colors,
			  struct dfs_visit_data *data,
			  int 				     (*Pre_inspect)(struct dfs_visit_data *data),
			  int 					 (*Action)(struct dfs_visit_data *data),
			  int 					 (*Post_inspect)(struct dfs_visit_data *data));
int dfs_action(struct dfs_visit_data *visit_data);
int is_acyclic(graph_t graph);
int is_acyclic_action(struct dfs_visit_data *visit_data);

varr_t get_new_vertex_set(void);