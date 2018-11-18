

void read_terminal(FILE *file, enum terminal terminal);
void read_key_node(FILE *file, struct parsing_attr *parsing_attr);
void move(FILE *file, enum terminal tp)

unsigned int read_node(FILE *file, graph_t graph, void (*RW_object)(FILE *file, void *obj), unsigned int curr_node);
void *read_edge(FILE *file, graph_t graph, unsigned int key_source, void *curr_acienty);

struct parser_op *get_read_parser_op(void);

int match(FILE *file, enum terminal t);