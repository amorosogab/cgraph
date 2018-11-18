#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#include "array.h"

/* Funzioni non visibili dall'esterno. */
short check_cohesion(struct varr *varr);
short is_overflow_mul(unsigned int a, unsigned int b, unsigned int *mul);
short is_overflow_sum(unsigned int a, unsigned int b, unsigned int *sum);
short is_underflow_diff(unsigned int a, unsigned int b, unsigned int *diff);

/* 	DESCRIZIONE
	Allora un nuovo array variabile e lo restituisce al chiamante.
	
	VALORE DI RITORNO
	In caso di successo restituisce un array variabile non inizializzato. In caso di fallimento
	restituisce NULL:															  				*/
struct varr *get_varr(void)
{
	struct varr *varr = (struct varr *) malloc(sizeof(struct varr));
	if(varr)
	{
		varr->A = NULL;
		varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Inizializza l'array variabile assicurandosi della coerenza dei parametri di input (vedi condizione if).

	PARAMETRI DI INPUT
	struct varr *varr:			Array variabile.
	unsigned int initial_dim:	Dimensione iniziale dell'array variabile.
	unsigned int block: 		Dimensione di un blocco dell'array variabile.
	unsigned int max_dim:		Dimensione massima dell'array variabile.
	unsigned int min_dim: 		Dimensione minima dell'array variabile.

	VALORE DI RITORNO 			
	Al termine di questa funzione, il chiamante dovrebbe controllare attraverso la funzione is_initialized, se
	l'inizializzazione è avvenuta con successo. 																*/
struct varr *init_varr(struct varr *varr, unsigned int initial_dim, unsigned int block, unsigned int max_dim, unsigned int min_dim)
{
	if(varr && !is_initialized(varr))
	{
		varr->length = 0;
		varr->n_blocks = ceil(initial_dim / block);
		varr->initial_dim = initial_dim;
		varr->block = block;
		varr->max_dim = max_dim;
		varr->min_dim = min_dim;

		varr->initialized = 0;
		if(check_cohesion(varr) > 0)
		{
			if(varr->A)
				free(varr->A);
			
			varr->A = (void **) calloc(initial_dim, sizeof(size_t));
			if(varr->A != NULL)
			{
				varr->initialized = 1;
				varr->length = initial_dim;
			}
		}
	
	}

	return varr;
}

/* 	DESCRIZIONE
	Imposta la variabile initial_dim al nuovo valore.

	PARAMETRI DI INPUT
	struct varr *varr: 			Array variabile.
	unsigned int initial_dim: 	Nuovo valore.

	VALORE DI RITORNO
	Restituisce la struttura con il nuovo valore impostato. 				*/
struct varr *set_initial_dim(struct varr *varr, unsigned int initial_dim)
{
	if(varr)
	{
		unsigned int old_blocks = varr->n_blocks;
		varr->n_blocks = (varr->n_blocks - ceil(varr->initial_dim / get_block(varr))) + ceil(initial_dim / get_block(varr));
		varr->initial_dim = initial_dim;
		if(varr->n_blocks > old_blocks)
			varr = add_blocks(varr, old_blocks - varr->n_blocks);

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Imposta la variabile block al nuovo valore e aggiorna il numero di blocchi.

	PARAMETRI DI INPUT
	struct varr *varr: 		Array variabile.
	unsigned int block: 	Nuovo valore.

	VALORE DI RITORNO
	Restituisce la struttura con il nuovo valore impostato. 				*/
struct varr *set_block(struct varr *varr, unsigned int block)
{
	unsigned int length = get_length(varr);
	if(varr && block > 0)
	{
		varr->block = block;
		varr->n_blocks = ceil(length / block);

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Imposta la variabile max_dim al nuovo valore.

	PARAMETRI DI INPUT
	struct varr *varr: 			Array variabile.
	unsigned int max_dim: 		Nuovo valore.

	VALORE DI RITORNO
	Restituisce la struttura con il nuovo valore impostato. 				*/
struct varr *set_max_dim(struct varr *varr, unsigned int max_dim)
{
	if(varr)
	{
		varr->max_dim = max_dim;

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Imposta la variabile min_dim al nuovo valore.

	PARAMETRI DI INPUT
	struct varr *varr: 			Array variabile.
	unsigned int min_dim: 		Nuovo valore.

	VALORE DI RITORNO
	Restituisce la struttura con il nuovo valore impostato. 				*/
struct varr *set_min_dim(struct varr *varr, unsigned int min_dim)
{
	if(varr)
	{
		varr->min_dim = min_dim;

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Verifica se gli attributi di varr sono coerenti. La coerenza degli attributi viene raggiunta
	se sono soddisfatte le seguenti condizioni:
	varr->n_blocks >= 0
	varr->n_blocks < varr->max_dim
	max_dim >= min_dim
	initial_dim <= max_dim
	initial_dim >= min_dim
	block <= max_dim
	block >= min_dim

	PARAMETRI DI INPUT
	struct varr *varr: Array variabile.

	VALORE DI RITORNO 
	Retituisce 1 se l'array variabile è coerente, 0 se non è coerente. Se varr è null,
	restituisce -1. 																			*/
short check_cohesion(struct varr *varr)
{
	if(varr)
	{
		if( varr->n_blocks >= 0														&&
			varr->n_blocks < get_max_dim(varr) 										&&
			get_max_dim(varr) >= get_min_dim(varr) 									&&
			get_initial_dim(varr) <= get_max_dim(varr) 								&&
			get_initial_dim(varr) >= get_min_dim(varr) 								&&
			get_block(varr) <= get_max_dim(varr) 									&&
			get_block(varr) >= get_min_dim(varr))
				return 1;
		else
				return 0;
	}
	else
		return -1;

}

/* 	DESCRIZIONE
	Recupera il valore initial_dim nella struttura e lo restituisce in output. 	*/
unsigned int get_initial_dim(struct varr *varr)
{
	return varr->initial_dim;
}

/* 	DESCRIZIONE
	Recupera il valore block nella struttura e lo restituisce in output. 	*/
unsigned int get_block(struct varr *varr)
{
	return varr->block;
}

/* 	DESCRIZIONE
	Recupera il valore max_dim nella struttura e lo restituisce in output. 	*/
unsigned int get_max_dim(struct varr *varr)
{
	return varr->max_dim;
}

/* 	DESCRIZIONE
	Recupera il valore min_dim nella struttura e lo restituisce in output. 	*/
unsigned int get_min_dim(struct varr *varr)
{
	return varr->min_dim;
}

/* 	PARAMETRI DI INPUT
	struct varr *varr: Array Variabile.

	VALORE DI RITORNO
	Restituisce la lunghezza dell'array.																					*/
unsigned int get_length(struct varr *varr)
{
	return varr->length;
}

/* 	DESCRIZIONE
	Recupera il valore initialized nella struttura e lo restituisce in output. 	*/
short is_initialized(struct varr *varr)
{
	return varr->initialized;
}

/* 	DESCRIZIONE
	Assegna alla locazione 'index' dell'array variabile, l'oggetto 'object',

	PARAMETRI DI INPUT
	struct varr *varr: 	Array Variabile.
	unsigned int index: Indice.
	void *object: 		Oggetto.

	VALORE DI RITORNO 
	Retituisce 1 in caso di successo, 0 altrimenti.							*/
short set_loc(struct varr *varr, unsigned int index, void *object)
{
	unsigned int length = get_length(varr);
	if(varr && index < get_max_dim(varr))
	{
		unsigned int new_block = 0;
		if(length == index)
			new_block = 1;
		else if(length < index)
			new_block = ceil((index - length) / get_block(varr)) + 1;

		/* Modifico il numero di blocchi prima di inserirli, per vedere se supera il controllo della coesione */
		varr->n_blocks += new_block; 	
		if(check_cohesion(varr))
		{
			/* Se supera il controllo della coesione allora sottrai il numero di blocchi aggiunto ed esegui l'estenzione 
			   effettiva dell'array. add_blocks provvede ad aggiornare gli attributi di varr					*/
			varr->n_blocks -= new_block; 	
			varr = add_blocks(varr, new_block);
			varr->A[index] = object;

			return 1;
		}
		else
		{
			/* Se non supera il controllo della coesione allora sottrai il numero di blocchi aggiunto e retituisce 0  */
			varr->n_blocks -= new_block;

			return 0;
		}
	}

	return 0;
}

/* 	DESCRIZIONE
	Recupera dall'array variabile, l'oggetto di indice 'index'.

	PARAMETRI DI INPUT
	struct varr *varr: 	Array Variabile.
	unsigned int index: Indice.

	VALORE DI RITORNO 
	In caso di successo restituisce l'oggetto, altrimenti restituisce NULL.				*/
void *get_loc(struct varr *varr, unsigned int index)
{
	if(varr && index < get_length(varr))
		return varr->A[index];

	return NULL;
}

/* 	DESCRIZIONE
	Distrugge l'array variabile passato in input.

	PARAMETRI DI INPUT
	struct varr *varr: Array Variabile. 			*/
void destroy_varr(struct varr *varr)
{
	if(varr)
	{
		if(varr->A)
			free(varr->A);
		free(varr);
	}
}

/* 	DESCRIZIONE
	Estende l'array variabile di 'blocks' un blocchi.

	PARAMETRI DI INPUT
	struct varr *varr: 	 Array Variabile.
	unsigned int blocks: Numero di blocchi.

	VALORE DI RITORNO				
	In caso do successo restituisce l'array variabile esteso. In caso contrario, non effettua nessuna
	modifica all'array passato in input.																  */
struct varr *add_blocks(struct varr *varr, unsigned int blocks)
{
	if(blocks > 0 && varr && is_initialized(varr))
	{
		unsigned int sum_length_block, mul_blocks_block, new_length, new_n_blocks;

		unsigned int length = get_length(varr);
		if(!is_overflow_mul(blocks, get_block(varr), &mul_blocks_block) && 
		   !is_overflow_sum(length, mul_blocks_block, &sum_length_block))
		{
			void **new_A;
			if(sum_length_block <= get_max_dim(varr))
			{
				if(sum_length_block > get_min_dim(varr))
				{
					new_length = sum_length_block;
					new_n_blocks = varr->n_blocks + blocks;
				}
				else
				{
					new_length = get_min_dim(varr);
					new_n_blocks = 0;
				}
			}
			else
			{
				new_length = get_max_dim(varr);
				new_n_blocks = floor(new_length / get_block(varr));
			}


			if(new_A = (void **) calloc(new_length, sizeof(size_t)))
			{
				if(memcpy(new_A, varr->A, sizeof(size_t) * length))
				{
					free(varr->A);
					varr->A = new_A;
					varr->n_blocks = new_n_blocks;
					varr->length = new_length;
				}
				else
					free(new_A);
			}
		}

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Riduce l'array variabile di 'blocks' blocchi.

	PARAMETRI DI INPUT
	struct varr *varr: 	 Array Variabile.
	unsigned int blocks: Numero di blocchi.

	VALORE DI RITORNO		
	In caso do successo restituisce l'array variabile ridotto. In caso contrario, non effettua nessuna
	modifica all'array passato in input.																 */
struct varr *remove_blocks(struct varr *varr, unsigned int blocks)
{
	if(blocks > 0 && varr && is_initialized(varr))
	{
		unsigned int diff_length_block, mul_blocks_block, new_length, new_n_blocks;

		unsigned int length = get_length(varr);
		if(!is_overflow_mul(blocks, get_block(varr), &mul_blocks_block))
		{
			void **new_A;
			if(!is_underflow_diff(length, mul_blocks_block, &diff_length_block) && diff_length_block > get_min_dim(varr))
			{
				if(diff_length_block <= get_max_dim(varr))
				{
					new_length = diff_length_block;
					new_n_blocks = varr->n_blocks - blocks;
				}
				else
				{
					new_length = get_max_dim(varr);
					new_n_blocks = floor(new_length / get_block(varr));
				}
			}
			else
			{
				new_length = get_min_dim(varr);
				new_n_blocks = 0;
			}

			if(new_A = (void **) calloc(new_length, sizeof(size_t)))
			{
				if (memcpy(new_A, varr->A, sizeof(size_t) * new_length))
				{
					free(varr->A);
					varr->A = new_A;
					varr->n_blocks = new_n_blocks;
					varr->length = new_length;
				}
				else
					free(new_A);
			}
			
		}

		if(check_cohesion(varr) > 0)
			varr->initialized = 1;
		else
			varr->initialized = 0;
	}

	return varr;
}

/* 	DESCRIZIONE
	Verifica se il risultato della moltiplicazione tra a e b (a * b) provoca un overflow del risultato.

	PARAMETRI DI INPUT
	unsigned int a: Primo termine.
	unsigned int b: Secondo termine.

	PARAMETRI DI OUTPUT
	unsigned int *mul:  Moltiplicazione a*b. Affidabile solo se la funzione restituisce un valore
						maggiore di 0.

	VALORE DI RITORNO 
	Restituisce 1 se l'operazione a*b provoca un overflow, -1 se mul == NULL. In caso di riuscita,
	restituisce 0.														 									*/
short is_overflow_mul(unsigned int a, unsigned int b, unsigned int *mul)
{
	if(!mul)
		return -1;

	*mul = a * b;
	if((((unsigned int)(log2((double) a))) + ((unsigned int)(log2((double) b))) + 1) > 
	   ((unsigned int) (log2(UINT_MAX) + 1)))
			return 1;
	else
			return 0;
}

/* 	DESCRIZIONE
	Verifica se il risultato della somma tra a e b (a + b) provoca un overflow del risultato.

	PARAMETRI DI INPUT
	unsigned int a: Primo termine.
	unsigned int b: Secondo termine.

	PARAMETRI DI OUTPUT
	unsigned int *sum:  Somma a+b. Affidabile solo se la funzione restituisce un valore
						maggiore di 0.

	VALORE DI RITORNO 
	Restituisce 1 se l'operazione a*b provoca un overflow, -1 se mul == NULL. In caso di riuscita,
	restituisce 0.														 									*/
short is_overflow_sum(unsigned int a, unsigned int b, unsigned int *sum)
{
	if(!sum)
		return -1;

	*sum = a + b;
	if(*sum < a || *sum < b)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Verifica se il risultato della differenza tra a e b (a - b) provoca un underflow del risultato.

	PARAMETRI DI INPUT
	unsigned int a: Primo termine.
	unsigned int b: Secondo termine.

	PARAMETRI DI OUTPUT
	unsigned int *diff:  Differenza a-b. Affidabile solo se la funzione restituisce un valore
						 maggiore di 0.

	VALORE DI RITORNO 
	Restituisce 1 se l'operazione a-b provoca un underflow, -1 se mul == NULL. In caso di riuscita,
	restituisce 0.														 									*/
short is_underflow_diff(unsigned int a, unsigned int b, unsigned int *diff)
{
	if(!diff)
		return -1;

	*diff = a - b;
	if(a < b)
		return 1;
	else
		return 0;
}

/* 	DESCRIZIONE
	Duplica un array variabile.

	PARAMETRI DI INPUT
	struct varr *varr: Array variabile da duplicare.

	VALORE DI RITORNO
	Restituisce l'array variabile duplicato. 			 */
varr_t dup_varr(struct varr *varr)
{
	struct varr *new_varr = NULL;
	if(varr && (new_varr = get_varr()))
	{
		new_varr->length = varr->length;
		new_varr->n_blocks = varr->n_blocks;
		new_varr->initial_dim = varr->initial_dim;
		new_varr->block = varr->block;
		new_varr->min_dim = varr->min_dim;
		new_varr->max_dim = varr->max_dim;
		new_varr->initialized = varr->initialized;

		if(new_varr->A = (void **) calloc(varr->length, sizeof(size_t)))
			memcpy(new_varr->A, varr->A, sizeof(size_t) * varr->length);
		else
			destroy_varr(new_varr);
	}

	return new_varr;
}