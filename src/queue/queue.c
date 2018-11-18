#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di creare un nuova coda e restituirla in output.

	PARAMETRI DI INPUT
	int with_copy: 	Parametro che specifica se la coda deve prevedere la copia dell'oggetto
					oppure no.

	VALORE DI RITORNO
	Nuova coda. 																			*/
queue_t new_queue(int with_copy)
{
	queue_t queue = (queue_t) calloc(1, sizeof(struct queue));

	/* Operazioni necessaria per via dell'attributo with_copy costante */
	if(with_copy != 1)
		with_copy = 0;
	struct queue queue_tmp = {NULL, NULL, with_copy};
	memcpy(queue, &queue_tmp, sizeof(struct queue));
	
	return queue;
}

/* 	DESCRIZIONE
	Lo scopo di questa funzione è quello di restituire l'elemento che occupa la testa
	della coda, nella coda passata in input.

	PARAMETRI DI INPUT 
	queue_t queue - Coda.

	VALORE DI RITORNO
	Se la coda non è vuota, restituisce l'elemento di testa della coda. Altrimenti,
	restituisce NULL;
*/
void *top(queue_t queue)
{
	if(queue && !is_empty(queue))
		return queue->head->object;
	else
		return NULL;
}

/* 	DESCRIZIONE
	Restituisce un valore maggiore di 0 se la coda è vuota 
	
	PARAMETRI DI INPUT
	queue_t queue - Coda.
	
	VALORE DI RITORNO
	La funzione restituisce -1 se la coda non esiste. Se la coda esiste, ed è vuota, restituisce 1.
	Altrimenti, se la coda esiste e non è vuota, restituisce 0 										*/
int is_empty(queue_t queue)
{
	if(queue == NULL)
		return -1;
	else if(queue->head == NULL)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Questa funzione inserisce un oggetto 'object' nella coda. Tale elemento appena inserito, 
	occuparà la posizione testa della coda.

	PARAMETRI DI INPUT 
	queue_t queue - Coda.
	void *object  - Oggetto da inserire.
	
	VALORE DI RITORNO 
	Restituisce la coda passata in input, con 'object' in testa. Tale funzione restituisce NULL 
	solo se non è riuiscito ad allocare memoria, oppure se la coda passata in input è NULL. 	*/
queue_t enqueue(queue_t queue, void *object)
{
	if(queue == NULL)
		return NULL;

	struct queue_node *node = (struct queue_node *) calloc(1, sizeof(struct queue_node));
	if(node)
	{
		node->object = object;
		if(!is_empty(queue))
		{
			queue->tail->next = node;
			queue->tail = node;
		}
		else
			queue->head = queue->tail = node;		
	}

	return queue;
}

/* 	DESCRIZIONE
	Variante della funzione copy che effettua la copia dell'oggetto passato in input.

	PARAMETRI DI INPUT 
	queue_t queue 				- Coda.
	void *object  				- Oggetto da inserire.
	void *(*Copy)(void *object) - Funzione di copia dell'oggetto.
	
	VALORE DI RITORNO 
	Restituisce la coda passata in input, con 'object' in testa. Tale funzione restituisce NULL 
	solo se non è riuiscito ad allocare memoria, oppure se la coda passata in input è NULL. 	*/
queue_t enqueue_copy(queue_t queue, void *object, void *(*Copy)(void *object))
{
	if(queue && queue->with_copy && Copy)
	{
		void *new_object = Copy(object);
		return enqueue(queue, new_object);
	}

	return queue;
}

/* 	DESCRIZIONE
	Questa funzione estrae l'elemento testa dalla coda. 

	PARAMETRI DI INPUT 
	queue_t queue - Coda.
	
	VALORE DI RITORNO
	Restutuisce la nuova coda. 								*/
queue_t dequeue(queue_t queue)
{
	if(queue && !is_empty(queue))
	{
		struct queue_node *tmp = NULL;

		tmp = queue->head;
		queue->head = queue->head->next;
		if(queue->head == NULL)
			queue->tail = NULL;

		free(tmp);
	}

	return queue;
}

/* 	DESCRIZIONE
	Variante della funzione che estrae l'elemento testa dalla coda. Estenzione che distrugge
	l'oggetto estratto. 

	PARAMETRI DI INPUT 
	queue_t queue 				  - Coda.
	void (*Destroy)(void *object) - Funzione di distruzione.
	
	VALORE DI RITORNO
	Restutuisce la nuova coda. 								*/
queue_t dequeue_destroy(queue_t queue, void (*Destroy)(void *object))
{
	if(queue && !is_empty(queue) && queue->with_copy && Destroy)
	{
		void *object = queue->head->object;
		queue = dequeue(queue);
		Destroy(object);
	}

	return queue;
}

/* 	DESCRIZIONE
	Effettua la distruzione della coda data in ingresso. Il tipo della coda deve essere
	senza copia del dato.

	PARAMETRI DI INPUT 
	queue_t queue - Coda 																	*/
void queue_destroy(queue_t queue)
{
	if(queue && !queue->with_copy)
	{
		struct queue_node *head = queue->head;
		while(head)
			head = dequeue(queue)->head;

		free(queue);
	}
}

/* 	DESCRIZIONE
	Effettua la distruzione della coda data in ingresso. Il tipo della coda deve essere
	con copia del dato.

	PARAMETRI DI INPUT 
	queue_t queue - Coda 																	*/
void queue_destroy_copy(queue_t queue, void (*Destroy)(void *object))
{
	if(queue && queue->with_copy && Destroy)
	{
		struct queue_node *head = queue->head;
		while(head)
			head = dequeue_destroy(queue, Destroy)->head;

		free(queue);
	}
}
