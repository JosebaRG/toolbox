typedef enum
{
	CIRCULAR,
	LIFO,
	FIFO
} QType;

typedef struct Queue_t Queue_t;
typedef struct QNode_t QNode_t;

struct QNode_t
{
	QNode_t * before;
	QNode_t * after;
	Queue_t * queue;
	void    * data;
};

struct Queue_t
{
	QNode_t * first;
	QNode_t * last;
	QType     type;
};

Queue_t * libqueue_create_queue (QType type);
Queue_t * libqueue_delete_queue (Queue_t * queue);

QNode_t * libqueue_add_node (Queue_t * queue, void * data);
QNode_t * libqueue_add_node_first (Queue_t * queue, void * data);
QNode_t * libqueue_add_node_last (Queue_t * queue, void * data);
QNode_t * libqueue_add_node_before (QNode_t * ref_node, void * data);
QNode_t * libqueue_add_node_after (QNode_t * ref_node, void * data);

QNode_t * libqueue_get_node (Queue_t * queue);
QNode_t * libqueue_get_node_first (Queue_t * queue);
QNode_t * libqueue_get_node_last (Queue_t * queue);
QNode_t * libqueue_get_node_before (QNode_t * ref_node);
QNode_t * libqueue_get_node_after (QNode_t * ref_node);

void * libqueue_remove_node (Queue_t * queue);
void * libqueue_remove_node_first (Queue_t * queue);
void * libqueue_remove_node_last (Queue_t * queue);
void * libqueue_remove_node_before (QNode_t * ref_node);
void * libqueue_remove_node_before (QNode_t * ref_node);
