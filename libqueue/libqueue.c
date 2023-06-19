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

uint32_t libqueue_delete_queue (Queue_t * queue)
{
	QNode_t * rm_node;

	uint32_t counter = 0;

	while (queue->first != NULL)
	{
		rm_node = queue->first;
		queue->first = queue->first->after;
		libqueue_remove_node (rm_node);
		counter++;
	}

	return counter;
}

uint32_t libqueue_count_nodes (Queue_t * queue)
{
	uint32_t counter = 0;

	QNode_t * aux_node;
	aux_node = queue->first;

	if (aux_node != NULL)
		return 0;

	do
	{
		aux_node = aux_node->after;
		counter++;
	} while ((aux_node != NULL) || ((queue->type == CIRCULAR) && (aux_node != queue->first)));

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
			return NULL;
			break;
    }
}

QNode_t * libqueue_get_node_first (Queue_t * queue)
{
	return queue->first;

}

QNode_t * libqueue_get_node_last (Queue_t * queue)
{
	return queue->last;
}

QNode_t * libqueue_get_node_before (QNode_t * ref_node);
{
	return ref_node->before;
}

QNode_t * libqueue_get_node_after (QNode_t * ref_node);
{
	return ref_node->after;
}

void * libqueue_get_data (Queue_t * queue)
{
	switch (queue->type)
	{
		case CIRCULAR:
			return queue->first->data;
			break;
		case LIFO:
			return queue->last->data;
			break;
		case FIFO:
			return queue->first->data;
			break;
		default:
			return NULL;
			break;
}

void * libqueue_get_data_first (Queue_t * queue)
{
	return queue->first->data;
}

void * libqueue_get_data_last (Queue_t * queue)
{
	return queue->first->data;
}

void * libqueue_get_data_before (QNode_t * ref_node);
{
	return ref_node->before->data;
}

void * libqueue_get_data_after (QNode_t * ref_node);
{
	return ref_node->after->data;
}

void * libqueue_remove_node (Queue_t * queue)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (queue->first == NULL)
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

void * libqueue_remove_node_first (Queue_t * queue)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (queue->first == NULL)
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
		default:
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
	}

	data = rm_node->data;

	free (rm_node);
	
	return data;
}

void * libqueue_remove_node_last (Queue_t * queue)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (queue->last == NULL)
		return data;

	switch (queue->type)
	{
		case CIRCULAR:
			rm_node = queue->last;
			if (queue->first == queue->last)
			{
				queue->first = NULL;
				queue->last  = NULL;
			}
			else
			{			
				queue->last->before->after = queue->last->after;
				queue->last->after->before = queue->last->before;
			}
			break;
		default:
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
	}

	data = rm_node->data;

	free (rm_node);
	
	return data;
}

void * libqueue_remove_node_before (QNode_t * ref_node)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (ref_node == NULL)
		return data;
	
	if (ref_node->before == NULL)
		return data;

	rm_node = ref_node->before;

	rm_node->before->after = rm_node->after;
	rm_node->after->before = rm_node->before;

	data = rm_node->data;

	free (rm_node);
	
	return data;
}

void * libqueue_remove_node_after (QNode_t * ref_node)
{
	QNode_t * rm_node;
	void * data = NULL;

	if (ref_node == NULL)
		return data;
	
	if (ref_node->before == NULL)
		return data;

	rm_node = ref_node->before;

	rm_node->before->after = rm_node->after;
	rm_node->after->before = rm_node->before;

	data = rm_node->data;

	free (rm_node);
	
	return data;
}

