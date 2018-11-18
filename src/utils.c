#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <limits.h>

#include "utils.h"

void *int_copy(void *src)
{
	int *d = (int *) malloc(sizeof(int));
	*d = *((int *) src);

	return d;
}

int getcmds(char cmd[][LEN_CMD])
{
	int a, b;
	char c, prev_c;

	if(cmd)
	{
		/* clear input buffer */
		while((c = getchar()) != '\n' && c != EOF)
			;

		prev_c = EOF;
		a = b = 0;
		while(a < N_CMD && (c = getchar()) != '\n' && c != EOF)
		{
			if(c != ' ' && b < LEN_CMD)
				cmd[a][b++] = c;
			else
			{
				cmd[a][b] = 0;
				if(b != 0)
					a++;
				b = 0;
			}

			prev_c = c;
		}
	}

	if(a < N_CMD && b < LEN_CMD)
		cmd[a][b]=0;

	if(prev_c == ' ')
		return a;
	else
		return a+1;
}

int is_integer(char *str)
{
	int i = 0;
	while(str[i] != 0 && (isdigit(str[i]) || str[i] == '-' || str[i] == '+'))
		i++;

	if(str[i] == 0)
		return 1;
	else
		return 0;
}

void *read_int_object(FILE *file)
{
	int c = 0, l = 0;
	int num = 0;

	int sign = 1;
	if((c = fgetc(file)) == '-')
		sign = -1;
	else
		ungetc(c, file);

	while((c = fgetc(file)) != EOF && isdigit(c)) 
	{
		if(num <= ((INT_MAX - (c - '0')) / 10)) 
		{
			num = num*10 + (c-'0'); 
			l++;
   		}
	    else syntax_error("NODEID must be <= 4294967295", file); 
	}

	if (l==0) 
		syntax_error("int ID", file); 
	if(c != EOF)       
		ungetc(c,file);     

	int *ret = (int *) malloc(sizeof(int));
	*ret = num;
	*ret *= sign;
	return ret; 
}

void write_int_object(FILE *file, void *obj)
{
	if(obj)
	{
		int num = *((int *) obj);
		fprintf(file, "%d", num);
	}

}

double rand_num(double a, double b)
{
	/*return a + fmod(rand(), b - a + 1);*/
	return a + (rand() / (RAND_MAX / (b - a + 1)));
}

unsigned int rand_unsigned(unsigned int a, unsigned int b)
{
	return a + (rand() / (UINT_MAX / (b - a + 1)));
}

void *rand_int(void)
{
	int *r = (int *) malloc(sizeof(int));
	*r = rand_num(1, 100);

	return r;
}

void print_int(void *obj)
{
	int *o = (int *) obj;
	if(o)
		printf("%d", *o);
	else
		printf("NULL\n");
}