/* L'insieme E degli archi viene interpretato come un array variadico (varr_t) circolare, i cui nodi sono
   di tipo struct l_acienty *. 																			*/

#include <stdlib.h>
#include <limits.h>

#include "graph_list.h"

/* 	DESCRIZIONE
	Alloca e inizializza la struttura delle operazioni di un grafo rappresentato come liste di
	adiacenza.

	VALORE DI RITORNO 
	Restituisce la struttua correttamente inizializzata. 			*/
struct operations *get_graph_list_operations(void)
{
	struct operations *op = (struct operations *) malloc(sizeof(struct operations));
	if(op)
	{
		op->Set_edge = set_edge_graph_list;
		op->Adiac = adiac_graph_list;
		op->Next = next_graph_list;
		op->Get_curr = get_curr_graph_list;
		op->Delete_edge = delete_edge_graph_list;
		op->Delete_key_from_E = delete_key_from_E_graph_list;
		op->Free_E = free_E_graph_list;
		op->Get_edge_struct = get_edge_struct_graph_list;
		op->Get_edge = get_edge_graph_list;
	}

	return op;
}

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
void *set_edge_graph_list(void *E, unsigned int key_source, struct edge *edge)
{
	if(!E)
		E = get_edge_struct_graph_list();

	if(is_initialized(E))
	{
		struct l_acienty *l_adiac = get_loc(E, key_source);
		struct node_list *nl = get_node_list(edge);
		if(nl)
		{
			if(!l_adiac)
			{
				l_adiac = (struct l_acienty *) malloc(sizeof(struct l_acienty));
				if(!l_adiac)
					goto malloc_error;
				l_adiac->list = l_adiac->last = NULL;
			}
			
			l_adiac->list = ll_insert_node(l_adiac->list, nl);
			l_adiac->last = nl;
			set_loc(E, key_source, l_adiac);
		}
	}

	malloc_error:
		return E;
}

/*	DESCRIZIONE
	Lo scopo di questa funzione è quello di restituire la lista di adiacenza del nodo key.

	PARAMETRI DI INPUT
	E: void *      		- Insieme degli archi.
	key: unsigned int   - Chiave del nodo sorgente.

	VALORE DI RITORNO
	La funzione restituisce la lista di adiacenza del nodo key.  		 */
void *adiac_graph_list(void *E, unsigned int key)
{
	struct l_acienty * l_adiac;
	if((l_adiac = get_loc(E, key)) != NULL)
		return l_adiac->list;
	else
		return NULL;
}

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
void *next_graph_list(void *list)
{
	linkedlist_t ll = (linkedlist_t) list;
	if(ll)
		ll = ll->next;

	return ll;
}

/* 	DESCRIZIONE
	Estrae il nodo corrente dalla lista passata in input. 
	Va usato con next per scorrere la lista di adiacenza fornita da adiac. 
	Per nodo corrente si intende il nodo alla prima posizione della lista.

	PARAMETRI DI INPUT
	list: void * - lista.

	PRE-CONDIZIONI
	List != NULL

	VALORE DI RITORNO
	Arco presente alla prima posizione della lista. 			*/
struct edge *get_curr_graph_list(void *list)
{
	struct edge *e = NULL;

	linkedlist_t ll = (linkedlist_t) list;
	if(ll)
		e = (struct edge *) ll->object;

	return e;
}

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di restituire l'arco sorgente in 'source' e che ha
	come target 'target'.

	PARAMETRI DI INPUT
	void *E 			- Insieme degli archi. 
	unsigned int source - Nodo sorgente.
	unsigned int target - Nodo target.

	VALORE DI RITORNO
	Restituisce l'arco che parte da source e va a finire in target (se esisnte. Altrimenti restituisce null.) */
struct edge *get_edge_graph_list(void *E, unsigned int source, unsigned int target)
{
	struct edge *e = NULL;
	struct l_acienty *list_a = get_loc(E, source);
	if(list_a && list_a->list)
	{
		if(list_a->last && ((struct edge *) list_a->last->object)->target->key == target)
			e = list_a->last->object;
		else
		{
			struct edge *curr_e = get_curr_graph_list(list_a->list);
			while(curr_e && curr_e->target->key != target)
			{
				list_a->list = next_graph_list(list_a->list);
				curr_e = get_curr_graph_list(list_a->list);
			}

			if(curr_e)
				e = curr_e;
		}
	}

	return e;
}

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
void *delete_edge_graph_list(void *E, unsigned int key_source, unsigned int key_target)
{
	struct l_acienty *l_adiac = get_loc(E, key_source);
	if(l_adiac)
	{
		struct node_list *prec = NULL;

		linkedlist_t list = l_adiac->list;
		while(list && ((struct edge *) list->object)->target->key != key_target)
			prec = list, list = list->next;

		if(list)
		{
			if(l_adiac->list == l_adiac->last)
			{
				set_loc(E, key_source, NULL);
				free(l_adiac);
				l_adiac = NULL;
			}
			else if(prec)
				prec->next = list->next;
			else
				l_adiac->list = l_adiac->list->next;

			free(list->object);
			if(l_adiac && l_adiac->last == list)
				l_adiac->last = prec;
			free(list); 
		}
	}

	return E;
}

/* 	DESCRIZIONE
	Lo scopo di questa funzione è: data la chiave key e l'insieme E degli archi, elimina da E 
	tutti i riferimenti alla chiave key (che sia esso un arco entrante, oppure un arco uscente). 

	PARAMETRI DI INPUT
	graph: graph_t 		- Grafo.
	key: unsigned int	- Chiave da eliminare da E.

	VALORE DI RITORNO
	Restituisce l'insieme degli archi E 					*/
void *delete_key_from_E_graph_list(graph_t graph, unsigned int key)
{
	if(graph && graph->E)
	{
		unsigned int n_node = get_length(graph->V);
		unsigned int i;

		for(i = 0; i < n_node; i++)
			if(get_loc(graph->E, i) != NULL)
				graph->E = delete_edge_graph_list(graph->E, i, key);

		struct l_acienty *l_adiac = get_loc(graph->E, key);
		if(l_adiac)
		{
			ll_destroy(l_adiac->list, free);
			free(l_adiac);
			set_loc(graph->E, key, NULL);
		}
	}

	if(graph != NULL)
		return graph->E;
	else
		return NULL;
}

/* 	DESCRIZIONE
	Dealloca la struttura E (non gli archi, ma la struttura che serve per rappresentare gli archi).

	PARAMETRI DI INPUT
	E: void *  - Insieme degli archi.

	PRE-CONDIZION1
	E != NULL

	POST-CONDIZIONI
	La struttura utilizzata per rappresentare gli archi è stata deallocata. 		*/
void free_E_graph_list(void *E, int edges)
{
	if(E)
	{
		unsigned int length_E = get_length(E), i;
		for(i = 0; i < length_E; i++)
		{
			struct l_acienty *l_adiac = get_loc(E, i);
			if(l_adiac)
			{
				if(edges)
					ll_destroy(l_adiac->list, free);
				else
					ll_destroy(l_adiac->list, NULL);

				free(l_adiac);
			}
		}

		destroy_varr(E);
	}
}

/* 	DESCRIZIONE
	Alloca e inizializza la struttura fisica dei archi.

	VALORE DI RITORNO
	Restituisce la strttura degli archi correttamente inizializzata. 	*/
void *get_edge_struct_graph_list(void)
{
	varr_t E = get_varr();
	E = init_varr(E, DIMV, DIMV, INT_MAX, DIMV);

	return E;
}