/**
 * @file libqueue.h
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

#ifndef _LIBQUEUE_H
#define _LIBQUEUE_H

#include "libtypes.h"

/*********************************************************************************
 *    Graphical representation of the nodes in the library.
 *********************************************************************************
 *
 *
 *              last       ┌───────┐     first
 *      ┌──────────────────┤ QUEUE ├─────────────────┐
 *      │                  └───────┘                 │
 *      ▼                                            ▼
 *   ┌──────┐before ┌──────┐before ┌──────┐before ┌──────┐
 *   │      ├──────►│      ├──────►│      ├──────►│      │
 *   │ NODE │ after │ NODE │ after │ NODE │ after │ NODE │
 *   │      │◄──────┤      │◄──────┤      │◄──────┤      │
 *   └──┬─┬─┘       └──────┘       └──────┘       └┬───┬─┘
 *    ▲ │ │                                        │ ▲ │
 *    │ │ │             If FIFO OR LIFO            │ │ │
 *    │ │ │      after      ┌────┐      before     │ │ │
 *    │ │ └────────────────►│NULL│◄────────────────┘ │ │
 *    │ │                   └────┘                   │ │
 *    │ │        after                               │ │
 *    │ └────────────────────────────────────────────┘ │
 *    │                  IF CIRCULAR    before         │
 *    └────────────────────────────────────────────────┘
 */

/*********************************************************************************
 *                                    STRUCTS
 *********************************************************************************/
typedef struct Queue_t Queue_t;
typedef struct QNode_t QNode_t;

typedef enum
{
	CIRCULAR,
	LIFO,
	FIFO
} QType;

struct QNode_t
{
	QNode_t * before; /**< pointer to the older element in the queue */
	QNode_t * after;  /**< pointer to the newer element in the queue */
	Queue_t * queue;  /**< ponter to the queue that contains this node */
	void    * data;   /**< generic pointer to the data contained on the node */
};

struct Queue_t
{
	QNode_t * first; /**< pointer to the oldest element in the queue */
	QNode_t * last;  /**< pointer to the newest element in the queue */
	QType     type;  /**< queue type definition*/
};

/*********************************************************************************
 *                                      API
 *********************************************************************************/

Queue_t * libqueue_create_queue (QType type);
uint32_t libqueue_delete_queue (Queue_t * queue);
uint32_t libqueue_count_nodes (Queue_t * queue);

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

void * libqueue_get_data (Queue_t * queue);
void * libqueue_get_data_first (Queue_t * queue);
void * libqueue_get_data_last (Queue_t * queue);
void * libqueue_get_data_before (QNode_t * ref_node);
void * libqueue_get_data_after (QNode_t * ref_node);

void * libqueue_remove_node (Queue_t * queue);
void * libqueue_remove_node_first (Queue_t * queue);
void * libqueue_remove_node_last (Queue_t * queue);
void * libqueue_remove_node_before (QNode_t * ref_node);
void * libqueue_remove_node_after (QNode_t * ref_node);

void libqueue_swap_node (QNode_t * ref_node_a, QNode_t * ref_node_b);

#endif //_LIBQUEUE_H
