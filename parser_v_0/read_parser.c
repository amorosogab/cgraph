#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void read_terminal(FILE *file, enum terminal terminal)
{
	if(match(file, terminal))
		move(file, terminal);
	else
	{
		char buf[20];
		sprintf(buf, "expected '%c'", terminal);
		syntax_error(buf, file);
	}
}

void read_key_node(FILE *file, struct parsing_attr *parsing_attr)
{
	int c = 0, l = 0;
	unsigned int num;
	
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

	parsing_attr->key_curr_node = num;
}

unsigned int read_node(FILE *file, graph_t graph, void (*RW_object)(FILE *file, void *obj), unsigned int curr_node)
{

}

void *read_edge(FILE *file, graph_t graph, unsigned int key_source, void *curr_acienty)
{

}

struct parser_op *get_read_parser_op(void)
{
	struct parser_op *parser_op = (struct parser_op *) malloc(sizeof(struct parser_op *));
	if(parser_op) 
	{
		parser_op->RW_terminal = read_terminal;
		parser_op->RW_unsigned_num = read_unsigned_int;
		parser_op->RW_node = read_node;
		parser_op->RW_edge = read_edge;
	}

	return parser_op;
}

void move(FILE *file, enum terminal type) 
{

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
       		if(c = fgetc(file)) == '>')
			{
				rp = ARROW;
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