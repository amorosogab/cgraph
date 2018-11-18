

void write_terminal(FILE *file, enum terminal terminal);
void write_key_node(FILE *file, struct parsing_attr *parsing_attr);

unsigned int write_node(FILE *file, graph_t graph, void (*RW_object)(FILE *file, void *obj), unsigned int curr_node);

void *write_edge(FILE *file, graph_t graph, unsigned int key_source, void *curr_acienty);

struct parser_op *get_write_parser_op(void);