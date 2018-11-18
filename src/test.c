#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph_list.h"
#include "graph_matrix.h"
#include "utils.h"
#include "parser/graph_parser.h"

/* 	COMMANDS 	<name_opitions>_<fun>	*/
#define RAND_2_3 		"RAND" 
#define MAN_2_3  		"MAN"
#define DEL_4_5  		"DEL"
#define MOD_4_5  		"MOD"
#define MATRIX_8_9_14 	"MAT"
#define LIST_8_9_14 	"LIS"

void print_graph_of_int_obj(graph_t g);
void print_edges_list_of_int_obj(void *E);
void print_edges_matrix_of_int_obj(void *E);

int main(void)
{
	char cmd[N_CMD][LEN_CMD];
	graph_t graph = NULL;
	int fun, no_exit = 1, n_args, rappresentazione = -1;

	do
	{
		printf("1. Crea Grafo\n"
			   "2. Inserisci Vertice\n"
			   "3. Insrisci Arco\n"
			   "4. Manipola Vertice\n"
			   "5. Manipola Arco\n"
			   "6. Trasponi Grafo\n"
			   "7. Stampa percorso tra nodi\n"
			   "8. Genera grafo\n"
			   "9. Cambia Rappresentazione\n"
			   "10. Controlla se è aciclico\n"
			   "11. Visita in ampiezza (BFS)\n"
			   "12. Visita in profondità (DFS)\n"
			   "13. Stampa grafo\n"
			   "14. Leggi grafo dal file\n"
			   "15. Scrivi grafo sul file\n"
			   "16. Exit\n"
			   "Inserisci comando: ");
		scanf("%d", &fun);
		if(graph == NULL && fun != 1 && fun != 8 && fun != 14 && fun != 16)
			continue;

		switch(fun)
		{
			case 1:
			{
				printf("Funzione selezionata 1 - Crea grafo\n"
					   "scegli rappresentazione (0 = matrice di adiacenza, 1 = lista di adiacenza): ");
				scanf("%d", &rappresentazione);

				if(rappresentazione)
					graph = get_new_graph(get_graph_list_operations());
				else
					graph = get_new_graph(get_graph_matrix_operations());
			}
			break;

			case 2:
			{
				int n_vert, i;

				printf("Funzione selezionata 2 - Inserisci Vertice\n"
					   "SINTASSI\n"
					   "::= <tipo_inserimento> <numero_vertici>\n"
					   "<tipo_inserimento> ::= (%s <intero_minimo> <intero_massimo>) | %s\n"
					   "> ", RAND_2_3, MAN_2_3);
				if((n_args = getcmds(cmd)) < 2)
					printf("Leggi la sintassi prima di scrivere\n");
				else if(strcmp(cmd[0], RAND_2_3) == 0 && n_args >= 4 && is_integer(cmd[1]) && is_integer(cmd[2]) && is_integer(cmd[3]))
				{
					int a, b;
					if(sscanf(cmd[1], "%d", &a) && sscanf(cmd[2], "%d", &b) && sscanf(cmd[3], "%d", &n_vert))
					{
						for(i = 0; i < n_vert; i++)
						{
							int *e = (int *) malloc(sizeof(int));
							*e = rand_num(a, b);
							printf("%d generato: %d\n", i, *e);
							graph = add_vertex(graph, e);
						}
					}
				}
				else if(strcmp(cmd[0], MAN_2_3) == 0 && is_integer(cmd[1]))
				{		
					if(sscanf(cmd[1], "%d", &n_vert))
					{
						for(i = 0; i < n_vert; i++)
						{
							int *e = (int *) malloc(sizeof(int));
							printf("inserisci il valore %d: ", i);
							scanf("%d", e);
							graph = add_vertex(graph, e);
						}
					}
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 3: 
			{
				int n_edge, i;

				printf("Funzione selezionata 3 - Inserisci Arco\n"
					   "SINTASSI\n"
					   "::= <tipo_inserimento> <numero_archi>\n"
					   "<tipo_inserimento> ::= (%s <peso_minimo> <peso_massimo>) | %s\n"
					   "> ", RAND_2_3, MAN_2_3);
				if((n_args = getcmds(cmd)) < 2)
					printf("Leggi la sintassi prima di scrivere\n");
				else if(strcmp(cmd[0], RAND_2_3) == 0 && n_args >= 4 && is_integer(cmd[1]) && is_integer(cmd[2]) && is_integer(cmd[3]))
				{
					int a, b;
					if(sscanf(cmd[1], "%d", &a) && sscanf(cmd[2], "%d", &b) && sscanf(cmd[3], "%d", &n_edge))
						graph->E = random_E(graph->V, (double) a, (double) b, graph->op);
				}
				else if(strcmp(cmd[0], MAN_2_3) == 0 && is_integer(cmd[1]))
				{		
					if(sscanf(cmd[1], "%d", &n_edge))
					{
						for(i = 0; i < n_edge; i++)
						{
							int s, t, p;
							printf("inserisci l'arco %d\n"
								   "source: ", i);
							scanf("%d", &s);
							printf("target: ");
							scanf("%d", &t);
							printf("peso: ");
							scanf("%d", &p);
							graph = add_edge(graph, (double) p, s, t);
						}
					}
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 4:
			{
				unsigned int id_vert;

				printf("Funzione selezionata 4 - Manipola Vertice\n"
					   "SINTASSI\n"
					   "::= <id_vertice> (<cancella_vertice> | <modifica_vertice>)\n"
					   "<cancella_vertice> ::= %s\n"
					   "<modifica_vertice> ::= %s <new_int>\n"
					   "> ", DEL_4_5, MOD_4_5);
				if((n_args = getcmds(cmd)) < 2)
					printf("Leggi la sintassi prima di scrivere\n");
				else if(is_integer(cmd[0]) && strcmp(cmd[1], DEL_4_5) == 0)
				{
					if(sscanf(cmd[0], "%u", &id_vert))
					{
						void *obj;
						if((obj = ((struct graph_node *) get_loc(graph->V, id_vert))->object) != NULL)
							free(obj);

						graph = delete_vertex(graph, id_vert);
					}
				}
				else if(is_integer(cmd[0]) && n_args >= 3 && strcmp(cmd[1], MOD_4_5) == 0 && is_integer(cmd[2]))
				{
					int new_int;
					if(sscanf(cmd[0], "%u", &id_vert) && sscanf(cmd[2], "%d", &new_int))
					{
						struct graph_node *node = get_loc(graph->V, id_vert);
						if(node)
							*((int *) node->object) = new_int;
					}			
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 5:
			{
				unsigned int source, target;

				printf("Funzione selezionata 5 - Manipola Arco\n"
					   "SINTASSI\n"
					   "::= <id_vertice_sorgente> <id_vertice_target> (<cancella_arco> | <modifica_arco>)\n"
					   "<cancella_vertice> ::= %s\n"
					   "<modifica_vertice> ::= %s <new_source> <new_target> <new_peso>\n"
					   "> ", DEL_4_5, MOD_4_5);
				if((n_args = getcmds(cmd)) < 3)
					printf("Leggi la sintassi prima di scrivere\n");
				else if (is_integer(cmd[0]) && is_integer(cmd[1]) && strcmp(cmd[2], DEL_4_5) == 0)
				{
					if(sscanf(cmd[0], "%u", &source) && sscanf(cmd[1], "%u", &target))
						graph->E = graph->op->Delete_edge(graph->E, source, target);
				}
				else if (is_integer(cmd[0]) && is_integer(cmd[1]) && n_args >= 6 && strcmp(cmd[2], MOD_4_5) == 0 && is_integer(cmd[3]) && is_integer(cmd[4]) && is_integer(cmd[5]))
				{
					unsigned int new_source, new_target, new_peso;
					if(sscanf(cmd[0], "%u", &source) && sscanf(cmd[1], "%u", &target) && sscanf(cmd[3], "%u", &new_source) && sscanf(cmd[4], "%u", &new_target) && sscanf(cmd[5], "%u", &new_peso))
					{
						graph->E = graph->op->Delete_edge(graph->E, source, target);
						graph = add_edge(graph, new_peso, new_source, new_target);
					}
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 6:
			{
				printf("Funzione selezionata 6 - Trasponi Grafo\n");
				graph_t GT = get_transposed(graph);
				destroy_varr(graph->V);
				graph->op->Free_E(graph->E, 1);
				free(graph);
				graph = GT;
			}
			break;

			case 7:
			{
				struct graph_node **pred = (struct graph_node **) malloc(sizeof(struct graph_node *) * get_length(graph->V));

				printf("Funzione selezionata 7 - Stampa percorso tra nodi\n"
					   "SINTASSI\n"
					   "::= <id_source> <id_target>\n"
					   "> ");
				if((n_args = getcmds(cmd)) < 2)
					printf("Leggi la sintassi prima di scrivere\n");
				else if(pred && is_integer(cmd[0]), is_integer(cmd[1]))
				{
					unsigned int source, target;

					if(sscanf(cmd[0], "%u", &source) && sscanf(cmd[1], "%u", &target)) 
					{
						BFS(graph, source, pred, NULL);
						print_path(graph, get_loc(graph->V, source), get_loc(graph->V, target), pred, print_int);
					}
				}

				free(pred);
			}
			break;

			case 8:
			{
				printf("Funzione selezionata 8 - Genera grafo\n"
					   "SINTASSI\n"
					   "::= <type> <min_vert> <max_vert> <min_peso> <max_peso>\n"
					   "<type> ::= %s (liste di adiacenza) | %s (matrice di adiacenza)\n"
					   "> ", MATRIX_8_9_14, LIST_8_9_14);
				if(getcmds(cmd) < 5)
					printf("Leggi la sintassi prima di scrivere\n");
				else if((strcmp(cmd[0], MATRIX_8_9_14) == 0 || strcmp(cmd[0], LIST_8_9_14) == 0) && is_integer(cmd[1]) && is_integer(cmd[2]) && is_integer(cmd[3]) && is_integer(cmd[4]))
				{
					int min_vert, max_vert, min_peso, max_peso;
					if(sscanf(cmd[1], "%d", &min_vert) && sscanf(cmd[2], "%d", &max_vert) && sscanf(cmd[3], "%d", &min_peso) && sscanf(cmd[4], "%d", &max_peso))
					{
						if(graph)
							destroy_graph(graph, free);

						if(strcmp(cmd[0], MATRIX_8_9_14) == 0)
							graph = random_graph(get_graph_matrix_operations(), min_vert, max_vert, min_peso, max_peso, rand_int), rappresentazione = 0;
						else
							graph = random_graph(get_graph_list_operations(), min_vert, max_vert, min_peso, max_peso, rand_int), rappresentazione = 1;
					}
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 9:
			{
				printf("Funzione selezionata 9 - Cambia Rappresentazione\n"
					   "::= %s (passa a matrice di adiacenza) | %s (passa a liste di adiacenza)\n"
					   "> ", MATRIX_8_9_14, LIST_8_9_14);
				if(getcmds(cmd) < 1)
					printf("Leggi la sintassi prima di scrivere\n");
				else if (strcmp(cmd[0], MATRIX_8_9_14) == 0)
					graph = change_rappresentation(graph, get_graph_matrix_operations()), rappresentazione = 0;
				else if(strcmp(cmd[0], LIST_8_9_14) == 0)
					graph = change_rappresentation(graph, get_graph_list_operations()), rappresentazione = 1;
				else
					printf("Non hai letto la sintassi prima di scrivere\n");
			}
			break;

			case 10:
			{
				printf("Funzione selezionata 10 - Controlla se è aciclico\n");
				if(is_acyclic(graph))
					printf("Il grafo è aciclico\n");
				else
					printf("Il grafo non è aciclico\n");		
			}
			break;

			case 11:
			{
				struct graph_node **pred = (struct graph_node **) malloc(sizeof(struct graph_node *) * get_length(graph->V));
				double *dist = (double *) malloc(sizeof(double) * get_length(graph->V));

				printf("Funzione selezionata 11 - Visita in ampiezza (BFS)\n"
					   "SINTASSI\n"
					   "::= <id_source>\n"
					   "> ");
				if(getcmds(cmd) < 1)
					printf("Leggi la sintassi prima di scrivere\n");
				else if(is_integer(cmd[0]))
				{
					unsigned int id_source, length_V = get_length(graph->V), i;
					if(sscanf(cmd[0], "%u", &id_source))
					{
						BFS(graph, id_source, pred, dist);
						printf("STAMPA ARRAY DEI PREDECESSORI\n");
						{
							for(i = 0; i < length_V; i++)
							{
								printf("%u: ", i);
								if(pred[i])
									printf("key: %u; obj: %d\n", pred[i]->key, *((int *) pred[i]->object));
								else
									printf("NULL\n");
							}
						}
						printf("STAMPA ARRAY DELLE DISTANZE\n");
						{
							for(i = 0; i < length_V; i++)
								printf("%u: dist %lf\n", i, dist[i]);
						}
					}
				}
				else
					printf("Non hai letto la sintassi prima di scrivere\n");

				free(pred);
				free(dist);
			}
			break;

			case 12:
			{
				unsigned int length_V = get_length(graph->V), i;
				struct graph_node **pred = (struct graph_node **) malloc(sizeof(struct graph_node *) * get_length(graph->V));

				printf("Funzione selezionata 12 - Visita in profondità (DFS)\n");
				DFS(graph, pred);
				for(i = 0; i < length_V; i++)
				{
					printf("%u: ", i);
					if(pred[i])
						printf("key: %u; obj: %d\n", pred[i]->key, *((int *) pred[i]->object));
					else
						printf("NULL\n");
				}

				free(pred);
			}
			break;

			case 13:
			{
				printf("Funzione selezionata 13 - Stampa grafo\n");

				print_graph_of_int_obj(graph);
				if(rappresentazione)
					print_edges_list_of_int_obj(graph->E);
				else
					print_edges_matrix_of_int_obj(graph->E);
			}
			break;

			case 14:
			{
				struct operations *graph_op;

				printf("Funzione selezionata 14 - Leggi grafo dal file\n");
				if(graph)
					destroy_graph(graph, free);
				printf("Scegli rappresentazione (%s, %s): ", LIST_8_9_14, MATRIX_8_9_14);
				if(getcmds(cmd) < 1)
					printf("Inserisci la reappresentazione da usare\n");
				else
				{
					if(strcmp(cmd[0], LIST_8_9_14) == 0)
						graph_op = get_graph_list_operations(), rappresentazione = 1;
					else if(strcmp(cmd[0], MATRIX_8_9_14) == 0)
						graph_op = get_graph_matrix_operations(), rappresentazione = 0;
					else
					{
						printf("Rappresentazione non riconosciuta\n");
						break;
					}

					graph = read_graph("file.txt", graph_op, read_int_object);
					printf("lettura eseguita con successo");
				}

			}
			break;

			case 15:
			{
				printf("Funzione selezionata 15 - Scrivi grafo sul file\n");
				write_graph("file.txt", graph, write_int_object);
				printf("scrittura eseguita con successo");
			}
			break;

			case 16: no_exit = 0;
			break;

			default: no_exit = 0;
		}

		printf("\n");
	}while(no_exit);

	destroy_graph(graph, free);
	return 0;
}

void print_graph_of_int_obj(graph_t g)
{
	if(g->V == NULL) 
	{
		printf("Il grafo è vuoto\n");
		return;
	}

	unsigned int length_V = get_length(g->V), i;

	printf("STAMPA DI %u VERTICI\n", length_V);
	for(i = 0; i < length_V; i++)
	{
		struct graph_node *gn;

		printf("%u: ", i);
		if(gn = get_loc(g->V, i))
			printf("key: %u \t object: %d\n", gn->key, *((int *) gn->object));
		else
			printf("NULL\n");
	}
}

void print_edges_list_of_int_obj(void *E)
{
	if(E)
	{
		printf("STAMPA LISTE DI ADIACENZA\n");

		unsigned int length_E = get_length(E), i;
		for(i = 0; i < length_E; i++)
		{
			printf("lista di adicenza %u: ", i);

			struct l_acienty *l_adiac = get_loc(E, i);
			if(l_adiac)
			{
				linkedlist_t ll = l_adiac->list;
				if(ll == NULL)
					printf("STO ESPLODENDO\n"), exit(1);
				while(ll)
				{
					if(ll->object)
						printf("\n\t(key: %u; peso: %lf; obj: %d)  ", ((struct edge *) ll->object)->target->key, ((struct edge *) ll->object)->peso, *((int *) ((struct edge *) ll->object)->target->object));
					else 
						printf("\n\tNULL  ");

					ll = ll->next;
				}

				printf("\n\t last: (key: %u; peso: %lf; obj: %d) \n", ((struct edge *) l_adiac->last->object)->target->key, ((struct edge *) l_adiac->last->object)->peso, *((int *) ((struct edge *) l_adiac->last->object)->target->object));
			}
			else
				printf("NULL\n");
		}
	}
	else
		printf("E = NULL\n");
}

void print_edges_matrix_of_int_obj(void *E)
{
	if(E)
	{
		struct varr *varr = (struct varr *) E;
		printf(" E = [n_blocks = \t %u\n"
			   "initial_dim = \t %u\n"
			   "block = \t %u\n"
			   "max_dim = \t %u\n"
			   "min_dim = \t %u\n"
			   "initialized = \t %d\n"
			   "length = \t %u]\n", varr->n_blocks, varr->initial_dim, varr->block, varr->max_dim, varr->min_dim, varr->initialized, get_length(varr));

		unsigned int i;
		printf("Valori:\n");
		short err_length;
		unsigned int length_i = get_length(varr);

		for(i = 0; i < length_i; i++)
		{
			struct varr *varr_i = get_loc(varr, i);
			if(varr_i) 
			{
				printf("\n\ncolonna %u:\n", i);
				printf("[n_blocks = \t %u\n"
					   "initial_dim = \t %u\n"
					   "block = \t %u\n"
					   "max_dim = \t %u\n"
					   "min_dim = \t %u\n"
					   "initialized = \t %d\n"
					   "length = \t %u]\n", varr_i->n_blocks, varr_i->initial_dim, varr_i->block, varr_i->max_dim, varr_i->min_dim, varr_i->initialized, get_length(varr_i));

				unsigned int j;
				printf("Valori:\n");
				unsigned int length_j = get_length(varr_i);

				for(j = 0; j < length_j; j++)
				{
					struct edge *e = get_loc(varr_i, j);
					if(e) 
						printf("riga %u: (key: %u; peso: %lf; obj: %d)", j, e->target->key, e->peso, *((int *) e->target->object));
					else
						printf("riga %u: NULL", j);

					printf("\n");
				}

			}
			else
				printf("colonna %u: NULL\n", i);
		}
	}
}