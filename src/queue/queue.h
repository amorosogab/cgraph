typedef struct queue * queue_t;
struct queue
{
	struct queue_node *head;
	struct queue_node *tail;
	const int with_copy;
};

struct queue_node
{
	void *object;
	struct queue_node *next;
};

queue_t new_queue(int with_copy);
queue_t enqueue(queue_t queue, void *object);
queue_t dequeue(queue_t queue);
queue_t enqueue_copy(queue_t queue, void *object, void *(*Copy)(void *object));
queue_t dequeue_destroy(queue_t queue, void (*Destroy)(void *object));

void *top(queue_t queue);
void queue_destroy(queue_t queue);
void queue_destroy_copy(queue_t queue, void (*Destroy)(void *object));

int is_empty(queue_t queue);
