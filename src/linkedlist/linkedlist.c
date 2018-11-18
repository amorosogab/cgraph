#include <stdlib.h>

#include "linkedlist.h"

int search_in_list(linkedlist_t ll, void *e, struct node_list **prec, struct node_list **next, int (*Cmp)(struct node_list *nl, void *b));
int cmp_object(struct node_list *nl, void *b);
int cmp_node_list(struct node_list *nl, void *b);

linkedlist_t ll_insert(linkedlist_t ll, void *object)
{
	return ll_insert_node(ll, get_node_list(object));
}

struct node_list *get_node_list(void *object)
{
	struct node_list *nl = (struct node_list *) malloc(sizeof(struct node_list));
	if(nl)
		nl->object = object, nl->next = NULL;

	return nl;
}

linkedlist_t ll_insert_node(linkedlist_t ll, struct node_list *nl)
{
	linkedlist_t head = ll;

	if(ll && nl)
	{		
		struct node_list *prec = ll;
		while(ll = ll->next)
			prec = ll;
		prec->next = nl;
	}
	else if(nl)
		head = nl;

	return head;
}

linkedlist_t ll_insert_copy(linkedlist_t ll, void *object, void *(*Get_copy)(void *obj))
{
	void *obj = NULL;

	if(Get_copy != NULL)
	{
		obj = Get_copy(object);
		ll = ll_insert(ll, obj);
	}

	return ll;
}

linkedlist_t ll_extract_head(linkedlist_t ll)
{
	if(ll)
	{
		struct node_list *head = ll;
		ll = ll->next;
		free(head);
	}

	return ll;
}

linkedlist_t ll_extract_head_destroy(linkedlist_t ll, void (*Destroy_obj)(void *obj))
{
	if(Destroy_obj != NULL && ll)
	{
		Destroy_obj(ll->object);
		ll = ll_extract_head(ll);
	}
	
	return ll;
}

linkedlist_t ll_delete_node_object(linkedlist_t ll, void *object)
{
	if(object)
	{
		struct node_list *prec, *succ, *nl;
		if(search_object(ll, object, &prec, &succ))
		{
			if(prec)
				nl = prec->next, prec->next = succ;
			else
				nl = ll, ll = succ;

			free(nl);
		}
	}

	return ll;
}

linkedlist_t ll_delete_node_list(linkedlist_t ll, struct node_list *nl)
{
	if(nl)
	{
		struct node_list *prec, *succ;
		if(search_node_list(ll, nl, &prec, &succ))
		{
			if(prec)
				prec->next = succ;
			else
				ll = succ;

			free(nl);
		}
	}

	return ll;
}

int search_object(linkedlist_t ll, void *obj, struct node_list **prec, struct node_list **next)
{
	return search_in_list(ll, obj, prec, next, cmp_object);
}

int search_node_list(linkedlist_t ll, struct node_list *nl, struct node_list **prec, struct node_list **next)
{
	return search_in_list(ll, nl, prec, next, cmp_node_list);
}

int search_in_list(linkedlist_t ll, void *e, struct node_list **prec, struct node_list **next, int (*Cmp)(struct node_list *nl, void *b))
{
	if(ll && e)
	{
		if(prec)
			*prec = NULL;

		while(ll && !Cmp(ll, e))
		{
			if(prec)
				*prec = ll;
			ll = ll->next;
		}

		if(ll != NULL)
		{
			if(next)
				*next = ll->next;
			return 1;
		}
	}

	return 0;
} 

int cmp_object(struct node_list *nl, void *b)
{
	if(nl->object == b)
		return 1;
	else
		return 0;
}

int cmp_node_list(struct node_list *nl, void *b)
{
	if(nl == b)
		return 1;
	else
		return 0;
}

void ll_destroy(linkedlist_t ll, void (*Destroy_obj)(void *o))
{
	struct node_list *tmp;
	while(tmp = ll)
	{
		ll = ll->next;

		if(Destroy_obj)
			Destroy_obj(tmp->object);
		free(tmp);
	}
}