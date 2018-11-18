#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "graph_parser.h"

char *stxerr = "Syntax Error: %s expected at position %d.\n";

void graph_parse(FILE *file, graph_t graph, void *(*Read_object)(FILE *file))
{
	adiacenze(file, graph, Read_object);
	if(!match(file, DOT))
		syntax_error("dot not found", file);
}

void adiacenze(FILE *file, graph_t graph, void *(*Read_object)(FILE *file))
{
	graph->V = get_new_vertex_set();
	graph->E = graph->op->Get_edge_struct();

	adiacenza_nodo(file, graph, Read_object);
	while(!feof(file) && !match(file, DOT))
	 	adiacenza_nodo(file, graph, Read_object);
}

void adiacenza_nodo(FILE *file, graph_t graph, void *(*Read_object)(FILE *file))
{
	 struct graph_node *node = nodo(file, graph->V, Read_object);
	 set_loc(graph->V, node->key, node);
	 if(match(file, ARROW))
	 {
	 	move(file, ARROW);
	 	lista_archi(file, graph, node->key);
	 }

	 if(!match(file, SEMICOLON))
	 	syntax_error("semicolon not found", file);
	 move(file, SEMICOLON);

}

struct graph_node *nodo(FILE *file, varr_t V, void *(*Read_object)(FILE *file))
{
	struct graph_node *node; 
	unsigned int key_node = key(file);

	if((node = get_loc(V, key_node)) == NULL)
		node = (struct graph_node *) malloc(sizeof(struct graph_node));
	if(node)
	{
		node->key = key_node;
		node->object = NULL;

		if(match(file, COMMA))
		{
			move(file, COMMA);
			node->object = Read_object(file);
		}
	}
	
	return node;
}

unsigned int key(FILE *file)
{
	return parse_unsigned_int(file);
}

unsigned int parse_unsigned_int(FILE *file)
{
	int c = 0, l = 0;
	unsigned int num = 0;
	
	while((c = fgetc(file)) != EOF && isdigit(c)) 
	{
		if (num <= ((UINT_MAX - (c - '0')) / 10)) /* Il numero che stai costruendo è più grande dell'intero massimo rappresentabile */
		{
			num = num*10 + (c-'0'); // Add the read digit to the number id
			l++;
   		}
	    else syntax_error("NODEID must be <= 4294967295", file); // Number too big: ERROR
	}

	if (l==0) 
		syntax_error("NODEID", file); 
	if(c != EOF)       // If EndOfFile not yet reached
		ungetc(c,file);      // Push the last character back to the file

	return num;
}

void lista_archi(FILE *file, graph_t graph, unsigned int key_source)
{
	struct edge *e = arco(file, graph);
	graph->E = graph->op->Set_edge(graph->E, key_source, e);

	while(match(file, COMMA))
	{
		move(file, COMMA);
		e = arco(file, graph);
		graph->E = graph->op->Set_edge(graph->E, key_source, e);
	}
}

struct edge *arco(FILE *file, graph_t graph) 
{
	double p = 1.0;

	struct edge *e = (struct edge *) malloc(sizeof(struct edge));
	if(e)
	{
		if(match(file, LPAR)) 
		{
			move(file, LPAR);
			p = peso(file); 

			if(!match(file, RPAR))
				syntax_error("')' not found", file); 
			move(file, RPAR);
		}
		
		e->peso = p;
		unsigned int key_target = key(file);
		struct graph_node *node_target = get_loc(graph->V, key_target);
		if(node_target == NULL) 
		{
			node_target = (struct graph_node *) malloc(sizeof(struct graph_node));
			if(node_target)
			{
				node_target->key = key_target;
				node_target->object = NULL;
			}

			set_loc(graph->V, node_target->key, node_target);
		}
		
		e->target = node_target;
	}

	return e;
}

double peso(FILE *file) 
{
	return parse_real(file);
}

double parse_real(FILE *file) 
{
	int sign = 1;

	char c;
	if((c = fgetc(file)) == '-')
		sign = -1;
	else
		ungetc(c, file); 

	double real = (double) parse_unsigned_int(file);
	if(match(file, DOT))
	{
		int i = 1;

		move(file, DOT);
		while((c = fgetc(file)) != EOF && isdigit(c)) 
		{
			double cifra = c - '0';

			i *= 10;
			real = real + (cifra / i);
		}

		if (i == 1) 
			syntax_error("parse_real error", file); 
		if(c != EOF)
			ungetc(c,file);   
	}

	return real * sign;
}

int match(FILE *file, enum terminal t)
{
  // [returns 1 if successful, 0 otherwise]
	char c;
	enum terminal rp;
	int res = 0;

	while(((c = fgetc(file)) == '\t') || (c== '\n') || (c== ' '))
		; // Skip intitial tabulation, newline and spaces
    switch(c) 
    {        // Determine the read symbol type
       case '(': rp = LPAR; 		break;
       case ')': rp = RPAR; 		break;
       case ',': rp = COMMA; 		break;
       case '.': rp = DOT; 			break;
       case ';': rp = SEMICOLON; 	break;
       case '-':
       {
       		char n_c;
       		if((n_c = fgetc(file)) == '>')
			{
				rp = ARROW;
				ungetc(n_c, file);
				break;
			}
       }
       default : rp = UNDEFINED; 	break;
    }

	ungetc(c,file);   // Push the characters read back to the file
    if (t == rp)        // The expexted type et and the read symbol type rp match
		res = 1;

  	return res;
}

void move(FILE *file, enum terminal tp)
{    
	int dim = 0;

	if(tp == ARROW)
		dim = 2;
	else
		dim = 1;

	fseek(file, dim, SEEK_CUR);
}

void syntax_error(char *err, FILE *file)
{  
	/* Print a syntax error string and abort */
 	 char str[80];

	 sprintf(str,stxerr,err,ftell(file)); // Build the syntax error string to print
	 catch_error(str); // Print the error string
}

void catch_error(char *errstr) 
{   
	/* Print message errstr on stderr and abort */
    fprintf(stderr, errstr);
    exit(1);
}

graph_t read_graph(const char *filename, struct operations *graph_op, void *(*Read_object)(FILE *file))
{
	graph_t graph = NULL;
	FILE *file = NULL;

	if(graph_op) 
	{
		file = fopen(filename, "r");
		if(!file)
			goto error;

		graph = get_new_graph(graph_op);
		if(!graph)
			goto error;

		graph_parse(file, graph, Read_object);
	}

	fclose(file);
	return graph;

	error:
		if(!graph)
			destroy_graph(graph, free);
		if(!file)
			fclose(file);
		return NULL;

}

void write_graph(const char *filename, graph_t graph, void (*Write_object)(FILE *file, void *obj))
{
	FILE *file = NULL;

	if(graph) 
	{
		file = fopen(filename, "w");
		if(!file)
			return;

		unsigned int len_V, i;
		for(i = 0, len_V = get_length(graph->V); i < len_V; i++)
		{
			struct graph_node *node;
			if((node = get_loc(graph->V, i)) != NULL)
			{
				fprintf(file, "%u", node->key);
				if(node->object)
				{
					fputc(',', file);
					Write_object(file, node->object);
				}

				void *list = graph->op->Adiac(graph->E, i);
				if(list)
				{
					struct edge *curr_e = graph->op->Get_curr(list);
					if(curr_e)
					{
						fprintf(file, "->");
						do
						{
							if(curr_e->peso != 1.0)
								fprintf(file, "(%lf)", curr_e->peso);
							fprintf(file, "%u", curr_e->target->key);

							list = graph->op->Next(list);
							curr_e = graph->op->Get_curr(list);

							if(curr_e)
								fputc(',', file);
						}while(curr_e);

						fputc(';', file);
					}

				}
			}
		}

		fputc('.', file);
		fclose(file);
	}

}

