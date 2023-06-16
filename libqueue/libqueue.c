/**
 * @file libqueue.c
 *
 * @brief Library to work with queues.
 *
 * Nodes are created as a structure with pointers to the next and previous nodes,
 * with a pointer to the queue and with void pointer to the undefined data element
 * contained by de the node.
 *
 * There is also a structure to contain the queue, that contains pointers to first
 * and last queue nodes, also contains an enum to define the queue type.
 * 
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#include "libqueue.h"
#include <stdlib.h>
#include <stddef.h>

/*********************************************************************************
 *                                      API
 *********************************************************************************/

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
	QNode_t * rm_node;

	while (queue->first != NULL)
	{
		rm_node = queue->first;
		queue->first = queue->first->after;
		libqueue_remove_node (rm_node);
	}
}

int32_t libqueue_count_nodes (Queue_t * queue)
{
	int32_t counter = 0;

	QNode_t * aux_node;
	aux_node = queue->first;

	while (aux_node != NULL)
	{
		counter++;
		aux_node = aux_node->after;
	}
	
	return counter;
}

QNode_t * libqueue_add_node (Queue_t * queue, void * data)
{
	return libqueue_add_node_last (queue, data);
}

QNode_t * libqueue_add_node_first (Queue_t * queue, void * data)
{
	QNode_t * qNode;
	qNode = (QNode_t *) malloc (sizeof (QNode_t));

	queue->first->before = qNode;
	qNode->after = queue->first;
	
	if (queue->type == CIRCULAR)
	{
		qNode->after = queue->first;
		qNode->before = queue->last;
		queue->last->after = qNode;
		queue->first-> = qNode;
	}
	else
		qNode->before = NULL;

	queue->first = qNode;

	qNode->queue = queue;
	qNode->data = data;

	return qNode;
}

QNode_t * libqueue_add_node_last (Queue_t * queue, void * data)
{
	QNode_t * qNode;
	qNode = (QNode_t *) malloc (sizeof (QNode_t));

	queue->last->after = qNode;
	qNode->before = queue->last;
	
	if (queue->type == CIRCULAR)
	{
		qNode->after = queue->first;
		qNode->before = queue->last;
		queue->first->before = qNode;
		queue->last-> = qNode;
	}
	else
		qNode->after = NULL;

	queue->last = qNode;

	qNode->queue = queue;
	qNode->data = data;

	return qNode;
}

QNode_t * libqueue_add_node_before (QNode_t * ref_node, void * data)
{
	QNode_t * qNode;
	qNode = (QNode_t *) malloc (sizeof (QNode_t));

	qNode->after = ref_node;
	qNode->before = ref_node->after;

	qNode->after->before = qNode;
	qNode->before->after = qNode;

	qNode->queue = ref_node->queue;
	qNode->data = data;

	return qNode;
}

QNode_t * libqueue_add_node_after (QNode_t * ref_node, void * data)
{
	QNode_t * qNode;
	qNode = (QNode_t *) malloc (sizeof (QNode_t));

	qNode->before = ref_node;
	qNode->after = ref_node->before;

	qNode->after->before = qNode;
	qNode->before->after = qNode;

	qNode->queue = ref_node->queue;
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
