#ifndef _QUEUE_H
#define _QUEUE_H

/*
 * Type: queueElementT
 * -------------------
 * This is the type of objects held in the queue.
 */

typedef int queueElement;

/*
 * Type: queue
 * --------------
 * The actual implementation of a queue is completely
 * hidden.  Client will work with queueADT which is a
 * pointer to underlying queueCDT.
 */
typedef struct {
	queueElement *contents;
	int front;
	int count;
	int maxSize;
} queue;

/*
 * Function: QueueCreate
 * Usage: queue = QueueCreate();
 * -------------------------
 * A new empty queue is created and returned.
 */

void QueueInit(queue *queue, int maxSize);

/* Function: QueueDestroy
 * Usage: QueueDestroy(queue);
 * -----------------------
 * This function frees all memory associated with
 * the queue.  "queue" may not be used again unless
 * queue = QueueCreate() is called first.
 */

void QueueDestroy(queue *queue);

/*
 * Functions: QueueEnter, QueueDelete
 * Usage: QueueEnter(queue, element);
 *        element = QueueDelete(queue);
 * --------------------------------------------
 * These are the fundamental queue operations that enter
 * elements in and delete elements from the queue.  A call
 * to QueueDelete() on an empty queue or to QueueEnter()
 * on a full queue is an error.  Make use of QueueIsFull()
 * and QueueIsEmpty() (see below) to avoid these errors.
 */

void QueueEnter(queue *queue, queueElement element);
queueElement QueueDelete(queue *queue);

/*
 * Functions: QueueIsEmpty, QueueIsFull
 * Usage: if (QueueIsEmpty(queue)) ...
 * -----------------------------------
 * These return a true/false value based on whether
 * the queue is empty or full, respectively.
 */

int QueueIsEmpty(queue *queue);
int QueueIsFull(queue *queue);

#endif  /* not defined _QUEUE_H */
