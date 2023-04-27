/**
 * @file libqueue.h
 *
 * @brief Library to work with queues.
 *
 * Queues are created as a structure with pointers to the next and previous nodes
 * and with void pointer to the undefined element contained by de the node.
 *
 * There is also a structure to contain the queue, that contains pointers to first
 * and last queue nodes, also contains an enum to define the queue type.
 * 
 * @author Joseba R.G.
 *         joseba.rg@protonmail.com
 */

#ifndef _LIBQUEUE_H
#define _LIBQUEUE_H

#include <stdio.h>

/*********************************************************************************
 *                                    STRUCTS
 *********************************************************************************/

typedef struct QNode_t
{
	QNode_t * before;
	QNode_t * after;
	void * data;
} QNode_t;

typedef struct Queue_t
{
	QNode_t * first;
	QNode_t * last;
	enum QType;
} Queue_t;

enum QType
{
	undefined,
	Circular,
	FiFo,
	FiLo,
	LiFo,
	LiLo
};

/*********************************************************************************
 *                                      API
 *********************************************************************************/

int libqueue_create_queue (Queue_t * queue, enum QType);
int libqueue_create_node (QNode_t * node, void * data);

int libqueue_add_node (Queue_t * queue, QNode_t * add_node);
int libqueue_add_node_first (Queue_t * queue, QNode_t * add_node);
int libqueue_add_node_last (Queue_t * queue, QNode_t * add_node);
int libqueue_add_node_next (Queue_t * queue, QNode_t * add_node, QNode_t * ref_node);
int libqueue_add_node_previous (Queue_t * queue, QNode_t * add_node, QNode_t * ref_node);

int libqueue_get_node_data (QNode_t * node, void * data);
int libqueue_set_node_data (QNode_t * node, void * data);

int libqueue_delete_queue (Queue_t * queue);

int libqueue_delete_node (Queue_t * queue, QNode_t * del_node);
int libqueue_delete_node_first (Queue_t * queue, QNode_t * del_node);
int libqueue_delete_node_last (Queue_t * queue, QNode_t * del_node);
int libqueue_delete_node_next (Queue_t * queue, QNode_t * del_node, QNode_t * ref_node);
int libqueue_delete_node_previous (Queue_t * queue, QNode_t * del_node, QNode_t * ref_node);

long libqueue_count_node (Queue_t * queue);

#endif //_LIBQUEUE_H
