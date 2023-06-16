#include "libqueue.h"
#include <stdlib.h>
#include <stddef.h>

Queue_t * libqueue_create_queue (QType type)
{
	Queue_t * queue;
	queue = (Queue_t *) malloc (sizeof (Queue_t));

	queue->first = NULL;
	queue->last = NULL;
	queue->type = type;

	return queue;
}

Queue_t * libqueue_delete_queue (Queue_t * queue)
{
	while (queue->first != NULL)
	{
		libqueue_remove_node (queue);
	}
}

QNode_t * libqueue_add_node (Queue_t * queue, void * data)
{
	QNode_t * qNode;
	qNode = (QNode_t *) malloc (sizeof (QNode_t));

	queue->last->after = qNode;
	qNode->before = queue->last;
	
	if (queue->type == CIRCULAR)
	{
		qNode->after = queue->first;
		queue->first->before = qNode;
	}
	else
		qNode->after = NULL;

	queue->last = qNode;

	qNode->queue = queue;
	qNode->data = data;

	return qNode;
}

QNode_t * libqueue_get_node (Queue_t * queue)
{
	switch (queue->type)
	{
		case CIRCULAR:
			return queue->first;
			break;
		case LIFO:
			return queue->last;
			break;
		case FIFO:
			return queue->first;
			break;
		default:
			break;
    }
}

void * libqueue_remove_node (Queue_t * queue)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (queue->first != NULL)
		return data;

	switch (queue->type)
	{
		case CIRCULAR:
			rm_node = queue->first;
			if (queue->first == queue->last)
			{
				queue->first = NULL;
				queue->last  = NULL;
			}
			else
			{			
				queue->first->before->after = queue->first->after;
				queue->first->after->before = queue->first->before;
			}
			break;
		case LIFO:
			rm_node = queue->last;
			if (queue->first == queue->last)
			{
				queue->first = NULL;
				queue->last  = NULL;
			}
			else
			{
				queue->last->before->after = NULL;
				queue->last = queue->last->before;
			}
			break;
		case FIFO:
			rm_node = queue->first;
			if (queue->first == queue->last)
			{
				queue->first = NULL;
				queue->last  = NULL;
			}
			else
			{
				queue->first->after->before = NULL;
				queue->first = queue->first->after;
			}
			break;
		default:
			rm_node = NULL;
			break;
    }

	data = rm_node->data;

	free (rm_node);
	
	return data;
}
