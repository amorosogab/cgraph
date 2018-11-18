#define N_CMD	15
#define LEN_CMD	50

void *int_copy(void *src);
int getcmds(char cmd[][LEN_CMD]);
int is_integer(char *str);
double rand_num(double a, double b);
void *rand_int(void);
void print_int(void *obj);
void *read_int_object(FILE *file);
void write_int_object(FILE *file, void *obj);