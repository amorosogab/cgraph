typedef void * varr_t;
struct varr
{
	void **A; 						/* Array vero e proprio. */
	unsigned int length; 			/* Lunghezza attuale dell'array variabile. */
	unsigned int n_blocks; 			/* Numero di blocchi di dimensione 'block' di cui è composto l'array. */
	unsigned int initial_dim; 		/* Dimensione iniziale dell'array, appena si inserisce il primo elemento. */
	unsigned int block; 			/* Numero di locazioni di cui è composto un blocco. */
	unsigned int max_dim;			/* Dimensione massima dell'array variabile. */
	unsigned int min_dim; 			/* Dimensione minimale dell'array variabile. */
	short initialized; 				/* Variabile che ci dice se l'array variabile è inizializzato oppure no. */
};

struct varr *get_varr(void);
struct varr *init_varr(struct varr *varr, unsigned int initial_dim, unsigned int block, unsigned int max_dim, unsigned int min_dim);
struct varr *set_initial_dim(struct varr *varr, unsigned int initial_dim);
struct varr *set_block(struct varr *varr, unsigned int block);
struct varr *set_max_dim(struct varr *varr, unsigned int max_dim);
struct varr *set_min_dim(struct varr *varr, unsigned int min_dim);
struct varr *add_blocks(struct varr *varr, unsigned int blocks);
struct varr *remove_blocks(struct varr *varr, unsigned int blocks);

unsigned int get_initial_dim(struct varr *varr);
unsigned int get_block(struct varr *varr);
unsigned int get_max_dim(struct varr *varr);
unsigned int get_min_dim(struct varr *varr);
unsigned int get_length(struct varr *varr);

short is_initialized(struct varr *varr);
short set_loc(struct varr *varr, unsigned int index, void *object);

void *get_loc(struct varr *varr, unsigned int index);

void destroy_varr(struct varr *varr);

varr_t dup_varr(struct varr *varr);