#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void QueueInit(queue *queue, int maxSize) {
	queueElement *newContents;

	newContents = (queueElement *) malloc(sizeof(queueElement) * maxSize);

	if (newContents == NULL) {
		fprintf(stderr, "Insufficient memory to initialize queue.\n");
		exit(1);
	}

	queue->contents = newContents;
	queue->front = 0;
	queue->count = 0;
	queue->maxSize = maxSize;
}

void QueueDestroy(queue *queue) {
	free(queue->contents);
}

void QueueEnter(queue *Q, queueElement element) {
	int newElementIndex;

	if (Q->count >= Q->maxSize) {
		fprintf(stderr, "QueueEnter on Full Queue.\n");
		exit(1); /* Exit program, returning error code. */
	}

	/*
	 * Calculate index at which to put
	 * next element.
	 */
	newElementIndex = (Q->front + Q->count) % Q->maxSize;
	Q->contents[newElementIndex] = element;

	Q->count++;
}

queueElement QueueDelete(queue *Q) {
	queueElement oldElement;

	if (Q->count <= 0) {
		fprintf(stderr, "QueueDelete on Empty Queue.\n");
		exit(1); /* Exit program, returning error code. */
	}

	/* Save the element so we can return it. */
	oldElement = Q->contents[Q->front];

	/*
	 * Advance the index of the front,
	 * making sure it wraps around the
	 * array properly.
	 */
	Q->front++;
	Q->front %= Q->maxSize;

	Q->count--;

	return oldElement;
}

int QueueIsEmpty(queue *queue) {
	return queue->count <= 0;
}

int QueueIsFull(queue *queue) {
	return queue->count >= queue->maxSize;
}
