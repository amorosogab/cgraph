#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

#include "graph_matrix.h"

/* 	DESCRIZIONE
	Alloca e inizializza la struttura delle operazioni di un grafo rappresentato come matrice di
	adiacenza.

	VALORE DI RITORNO 
	Restituisce la struttua correttamente inizializzata. 			*/
struct operations *get_graph_matrix_operations(void)
{
	struct operations *op = (struct operations *) malloc(sizeof(struct operations));
	if(op)
	{
		op->Set_edge = set_edge_graph_matrix;
		op->Adiac = adiac_graph_matrix;
		op->Next = next_graph_matrix;
		op->Get_curr = get_curr_graph_matrix;
		op->Delete_edge = delete_edge_graph_matrix;
		op->Delete_key_from_E = delete_key_from_E_graph_matrix;
		op->Free_E = free_E_graph_matrix;
		op->Get_edge_struct = get_edge_struct_graph_matrix;
		op->Get_edge = get_edge_graph_matrix;
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
void *set_edge_graph_matrix(void *E, unsigned int key_source, struct edge *edge)
{
	if(!E)
		E = get_edge_struct_graph_matrix();

	if(is_initialized(E)) 
	{
		if(get_loc(E, key_source) == NULL)
		{
			varr_t v = get_varr();
			v = init_varr(v, get_length(E), DIMV, INT_MAX, DIMV);
			set_loc(E, key_source, v);
		}

		set_loc(get_loc(E, key_source), edge->target->key, edge);
	}

	return E;
}

/*	DESCRIZIONE
	Lo scopo di questa funzione è quello di restituire le adiacenze del nodo key.
	Nel caso specifico vinene adoperata una struttura che contiene la riga di riferimento
	della matrice di adiacenza e un contatore che indica l'arco corrente.

	PARAMETRI DI INPUT
	E: void *      		- Insieme degli archi.
	key: unsigned int   - Chiave del nodo sorgente.

	VALORE DI RITORNO
	La funzione restituisce la lista di adiacenza del nodo key.		 */
void *adiac_graph_matrix(void *E, unsigned int key)
{
	struct matrix_iterate *mi = (struct matrix_iterate *) malloc(sizeof(struct matrix_iterate));
	if(mi)
	{
		mi->A = get_loc(E, key);
		mi->i = 0;

		if(mi->A != NULL) 
		{
			unsigned int len = get_length(mi->A);
			while(mi->i < len && get_loc(mi->A, mi->i) == NULL)
				mi->i = mi->i + 1;

			if(mi->i >= len) 
			{
				free(mi);
				return NULL;
			}
		}
	}

	return mi;
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
void *next_graph_matrix(void *list)
{
	struct matrix_iterate *mi = (struct matrix_iterate *) list;
	mi->i = mi->i + 1;

	unsigned int len = get_length(mi->A);
	while(mi->i < len && get_loc(mi->A, mi->i) == NULL)
		mi->i = mi->i + 1;

	if(mi->i >= len) 
	{
		free(mi);
		return NULL;
	}

	return mi;
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
struct edge *get_curr_graph_matrix(void *list)
{
	struct matrix_iterate *mi = (struct matrix_iterate *) list;

	if(mi)
	{
		if(mi->i < get_length(mi->A))
			return get_loc(mi->A, mi->i);
		else
			return NULL;
	}
	else
		return NULL;
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
struct edge *get_edge_graph_matrix(void *E, unsigned int source, unsigned int target)
{
	return get_loc(get_loc(E, source), target);
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
void *delete_edge_graph_matrix(void *E, unsigned int key_source, unsigned int key_target)
{
	free(get_loc(get_loc(E, key_source), key_target));
	set_loc(get_loc(E, key_source), key_target, NULL);

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
void *delete_key_from_E_graph_matrix(graph_t graph, unsigned int key)
{
	if(graph->E && key < get_length(graph->E))
	{
		varr_t v_key = get_loc(graph->E, key);
		unsigned int len, i;

		for(i = 0, len = get_length(v_key); i < len; i++)
		{
			struct edge *e = NULL;
			if((e = get_loc(v_key, i)) != NULL) 
			{
				free(e);
				set_loc(v_key, i, NULL);
			}

			varr_t i_key = get_loc(graph->E, i);
			if(i_key != NULL && (e = get_loc(i_key, key)) != NULL)
			{
				free(e);
				set_loc(i_key, key, NULL);
			}	
		}
	}

	return graph->E;
}

/* 	DESCRIZIONE
	Dealloca la struttura E (non gli archi, ma la struttura che serve per rappresentare gli archi).

	PARAMETRI DI INPUT
	E: void *  - Insieme degli archi.

	PRE-CONDIZION1
	E != NULL

	POST-CONDIZIONI
	La struttura utilizzata per rappresentare gli archi è stata deallocata. 		*/
void free_E_graph_matrix(void *E, int edges)
{
	if(E)
	{
		unsigned int i, len_i;

		for (i = 0, len_i = get_length(E); i < len_i; ++i) {
			varr_t i_varr = get_loc(E, i);

			if(i_varr) 
			{
				struct edge *e;
				unsigned int j, len_j;

				if(edges)
				{
					for (j = 0, len_j = get_length(i_varr); j < len_j; ++j) 
					{
						if((e = get_loc(i_varr, j)) != NULL)
							free(e);
					}
				}

				destroy_varr(i_varr);
			}
		}

		destroy_varr(E);
	}
}

/* 	DESCRIZIONE
	Alloca e inizializza la struttura fisica dei archi.

	VALORE DI RITORNO
	Restituisce la strttura degli archi correttamente inizializzata. 	*/
void *get_edge_struct_graph_matrix(void)
{
	varr_t E = get_varr();
	E = init_varr(E, DIMV, DIMV, INT_MAX, DIMV);

	return E;
}