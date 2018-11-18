typedef struct node_list * linkedlist_t;
struct node_list
{
	void			 *object;
	linkedlist_t      next;
};

linkedlist_t ll_insert(linkedlist_t ll, void *object);
linkedlist_t ll_insert_node(linkedlist_t ll, struct node_list *nl); 
linkedlist_t ll_insert_copy(linkedlist_t ll, void *object, void *(*Get_copy)(void *obj));
linkedlist_t ll_extract_head(linkedlist_t ll);
linkedlist_t ll_extract_head_destroy(linkedlist_t ll, void (*Destroy_obj)(void *obj));
linkedlist_t ll_delete_node_object(linkedlist_t ll, void *object);
linkedlist_t ll_delete_node_list(linkedlist_t ll, struct node_list *nl);

int search_node_list(linkedlist_t ll, struct node_list *nl, struct node_list **prec, struct node_list **next);
int search_object(linkedlist_t ll, void *obj, struct node_list **prec, struct node_list **next);

struct node_list *get_node_list(void *object);

void ll_destroy(linkedlist_t ll, void (*Destroy_obj)(void *o));